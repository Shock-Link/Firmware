#include "CaptivePortalInstance.h"

#include "CommandHandler.h"
#include "event_handlers/WebSocket.h"
#include "GatewayConnectionManager.h"
#include "Logging.h"
#include "serialization/WSLocal.h"
#include "util/HexUtils.h"
#include "util/TaskUtils.h"
#include "wifi/WiFiManager.h"

#include "serialization/_fbs/DeviceToLocalMessage_generated.h"

#include <LittleFS.h>
#include <WiFi.h>

static const char* TAG = "CaptivePortalInstance";

constexpr std::uint16_t HTTP_PORT                 = 80;
constexpr std::uint16_t WEBSOCKET_PORT            = 81;
constexpr std::uint16_t DNS_PORT                  = 53;
constexpr std::uint32_t WEBSOCKET_PING_INTERVAL   = 10'000;
constexpr std::uint32_t WEBSOCKET_PING_TIMEOUT    = 1000;
constexpr std::uint8_t WEBSOCKET_PING_RETRIES     = 3;
constexpr std::uint32_t WEBSOCKET_UPDATE_INTERVAL = 10;  // 10ms / 100Hz

using namespace OpenShock;

const esp_partition_t* _getStaticPartition() {
  const esp_partition_t* partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, "static0");
  if (partition != nullptr) {
    return partition;
  }

  partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, "static1");
  if (partition != nullptr) {
    return partition;
  }

  return nullptr;
}

bool _tryGetPartitionHash(char (&buffer)[65]) {
  static bool initialized              = false;
  static std::uint8_t staticSha256[32] = {0};

  if (!initialized) {
    initialized = true;

    ESP_LOGD(TAG, "Looking for static partition");

    // Get the static partition
    const esp_partition_t* partition = _getStaticPartition();
    if (partition == nullptr) {
      ESP_LOGE(TAG, "Failed to find static partition");
      return false;
    }

    ESP_LOGD(TAG, "Found static partition, getting hash...");

    // Get the hash of the partition
    esp_err_t err = esp_partition_get_sha256(partition, staticSha256);
    if (err != ESP_OK) {
      ESP_LOGE(TAG, "Failed to get partition hash: %s", esp_err_to_name(err));
      return false;
    }

    ESP_LOGD(TAG, "Got partition hash");
  }

  // Copy the hash to the output buffer
  HexUtils::ToHex<32>(staticSha256, buffer, false);

  return true;
}

CaptivePortalInstance::CaptivePortalInstance()
  : m_webServer(HTTP_PORT)
  , m_socketServer(WEBSOCKET_PORT, "/ws", "json")
  , m_socketDeFragger(std::bind(&CaptivePortalInstance::handleWebSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))
  , m_dnsServer()
  , m_taskHandle(nullptr) {
  m_socketServer.onEvent(std::bind(&WebSocketDeFragger::handler, &m_socketDeFragger, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
  m_socketServer.begin();
  m_socketServer.enableHeartbeat(WEBSOCKET_PING_INTERVAL, WEBSOCKET_PING_TIMEOUT, WEBSOCKET_PING_RETRIES);

  ESP_LOGI(TAG, "Setting up DNS server");
  m_dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Check if the www folder exists and is populated
  bool indexExists = LittleFS.exists("/www/index.html.gz");

  // Get the hash of the filesystem
  char fsHash[65];
  bool gotFsHash = _tryGetPartitionHash(fsHash);

  bool fsOk = indexExists && gotFsHash;
  if (fsOk) {
    ESP_LOGI(TAG, "Serving files from LittleFS");
    ESP_LOGI(TAG, "Filesystem hash: %s", fsHash);

    char softAPURL[64];
    snprintf(softAPURL, sizeof(softAPURL), "http://%s", WiFi.softAPIP().toString().c_str());

    // Serving the captive portal files from LittleFS
    m_webServer.serveStatic("/", LittleFS, "/www/", "max-age=3600").setDefaultFile("index.html").setSharedEtag(fsHash);

    // Redirecting connection tests to the captive portal, triggering the "login to network" prompt
    m_webServer.onNotFound([softAPURL](AsyncWebServerRequest* request) { request->redirect(softAPURL); });
  } else {
    ESP_LOGE(TAG, "/www/index.html or hash files not found, serving error page");

    m_webServer.onNotFound([](AsyncWebServerRequest* request) {
      request->send(
        200,
        "text/plain",
        // Raw string literal (1+ to remove the first newline)
        1 + R"(
You probably forgot to upload the Filesystem with PlatformIO!
Go to PlatformIO -> Platform -> Upload Filesystem Image!
If this happened with a file we provided or you just need help, come to the Discord!

discord.gg/OpenShock
)"
      );
    });
  }

  m_webServer.begin();

  if (fsOk) {
    if (TaskUtils::TaskCreateExpensive(CaptivePortalInstance::task, TAG, 8192, this, 1, &m_taskHandle) != pdPASS) {
      ESP_LOGE(TAG, "Failed to create task");
    }
  }
}

CaptivePortalInstance::~CaptivePortalInstance() {
  if (m_taskHandle != nullptr) {
    vTaskDelete(m_taskHandle);
    m_taskHandle = nullptr;
  }
  m_webServer.end();
  m_socketServer.close();
  m_dnsServer.stop();
}

void CaptivePortalInstance::task(void* arg) {
  CaptivePortalInstance* instance = reinterpret_cast<CaptivePortalInstance*>(arg);

  while (true) {
    instance->m_socketServer.loop();
    instance->m_dnsServer.processNextRequest();
    vTaskDelay(pdMS_TO_TICKS(WEBSOCKET_UPDATE_INTERVAL));
  }
}

void CaptivePortalInstance::handleWebSocketClientConnected(std::uint8_t socketId) {
  ESP_LOGD(TAG, "WebSocket client #%u connected from %s", socketId, m_socketServer.remoteIP(socketId).toString().c_str());

  WiFiNetwork connectedNetwork;
  WiFiNetwork* connectedNetworkPtr = nullptr;
  if (WiFiManager::GetConnectedNetwork(connectedNetwork)) {
    connectedNetworkPtr = &connectedNetwork;
  }

  Serialization::Local::SerializeReadyMessage(connectedNetworkPtr, GatewayConnectionManager::IsLinked(), std::bind(&CaptivePortalInstance::sendMessageBIN, this, socketId, std::placeholders::_1, std::placeholders::_2));
}

void CaptivePortalInstance::handleWebSocketClientDisconnected(std::uint8_t socketId) {
  ESP_LOGD(TAG, "WebSocket client #%u disconnected", socketId);
}

void CaptivePortalInstance::handleWebSocketClientError(std::uint8_t socketId, std::uint16_t code, const char* message) {
  ESP_LOGE(TAG, "WebSocket client #%u error %u: %s", socketId, code, message);
}

void CaptivePortalInstance::handleWebSocketEvent(std::uint8_t socketId, WebSocketMessageType type, const std::uint8_t* payload, std::size_t length) {
  switch (type) {
    case WebSocketMessageType::Connected:
      handleWebSocketClientConnected(socketId);
      break;
    case WebSocketMessageType::Disconnected:
      handleWebSocketClientDisconnected(socketId);
      break;
    case WebSocketMessageType::Text:
      ESP_LOGE(TAG, "Message type is not supported");
      break;
    case WebSocketMessageType::Binary:
      EventHandlers::WebSocket::HandleLocalBinary(socketId, payload, length);
      break;
    case WebSocketMessageType::Error:
      handleWebSocketClientError(socketId, length, reinterpret_cast<const char*>(payload));
      break;
    case WebSocketMessageType::Ping:
    case WebSocketMessageType::Pong:
      // Do nothing
      break;
    default:
      m_socketDeFragger.clear();
      ESP_LOGE(TAG, "Unknown WebSocket event type: %u", type);
      break;
  }
}
