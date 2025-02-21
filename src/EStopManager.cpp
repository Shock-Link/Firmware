#include <freertos/FreeRTOS.h>

#include "estop/EStopManager.h"

const char* const TAG = "EStopManager";

#include "Chipset.h"
#include "config/Config.h"
#include "events/Events.h"
#include "Logging.h"
#include "SimpleMutex.h"
#include "Time.h"
#include "util/TaskUtils.h"

#include <driver/gpio.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <freertos/timers.h>

#include <cstdint>

using namespace OpenShock;

const uint32_t k_estopHoldToClearTime = 5000;
const uint32_t k_estopUpdateRate      = 5;   // 200 Hz
const uint32_t k_estopCheckCount      = 13;  // 65 ms at 200 Hz
const uint16_t k_estopCheckMask       = 0xFFFF >> ((sizeof(uint16_t) * 8) - k_estopCheckCount);

static OpenShock::SimpleMutex s_estopMutex = {};
static gpio_num_t s_estopPin               = GPIO_NUM_NC;
static TaskHandle_t s_estopTask;

static EStopState s_estopState    = EStopState::Idle;
static bool s_estopActive         = false;
static int64_t s_estopActivatedAt = 0;

static volatile bool s_externallyTriggered = false;

static void _estopUpdateExternals(bool isActive, bool isAwaitingRelease)
{
  // Post an event
  ESP_ERROR_CHECK(esp_event_post(OPENSHOCK_EVENTS, OPENSHOCK_EVENT_ESTOP_STATE_CHANGED, &s_estopState, sizeof(s_estopState), portMAX_DELAY));
}

// Samples the estop at a fixed rate and sends messages to the estop event handler task
static void _estopCheckerTask(void* pvParameters)
{
  uint16_t history = 0xFFFF;  // Bit history of samples, 0 is pressed

  EStopState state      = EStopState::Idle;
  int64_t deactivatesAt = 0;

  bool lastBtnState = false;

  for (;;) {
    // Sleep for the update rate
    vTaskDelay(pdMS_TO_TICKS(k_estopUpdateRate));

    // Get current time
    int64_t now = OpenShock::millis();

    bool btnState;
    if (s_externallyTriggered) {
      s_externallyTriggered = false;

      // Emulate an EStop activation
      history       = 0xFFFF;
      state         = EStopState::Active;
      deactivatesAt = 0;
      lastBtnState  = false;
      btnState      = false;
    } else {
      // Sample the EStop
      history = (history << 1) | gpio_get_level(s_estopPin);

      // Check if the EStop is released (not all bits are 1)
      btnState = (history & k_estopCheckMask) != k_estopCheckMask;
      if (btnState == lastBtnState) {
        // If the state hasn't changed, handle timing transitions
        if (state == EStopState::ActiveClearing && now > deactivatesAt) {
          state = EStopState::AwaitingRelease;
          _estopUpdateExternals(s_estopActive, true);
        }
        continue;
      }
      lastBtnState = btnState;
    }

    switch (state) {
      case EStopState::Idle:
        if (btnState) {
          state              = EStopState::Active;
          s_estopActive      = true;
          s_estopActivatedAt = now;
        }
        break;
      case EStopState::Active:
        if (btnState) {
          state         = EStopState::ActiveClearing;
          deactivatesAt = now + k_estopHoldToClearTime;
        }
        break;
      case EStopState::ActiveClearing:
        if (!btnState) {
          state = EStopState::Active;
        } else if (now > deactivatesAt) {
          state = EStopState::AwaitingRelease;
        }
        break;
      case EStopState::AwaitingRelease:
        if (!btnState) {
          state         = EStopState::Idle;
          s_estopActive = false;
        }
        break;
      default:
        continue;
    }

    _estopUpdateExternals(s_estopActive, state == EStopState::AwaitingRelease);
  }
}

static bool _setEStopEnabledImpl(bool enabled)
{
  if (enabled) {
    if (s_estopTask == nullptr) {
      if (TaskUtils::TaskCreateUniversal(_estopCheckerTask, TAG, 4096, nullptr, 5, &s_estopTask, 1) != pdPASS) {  // TODO: Profile stack size and set priority
        OS_LOGE(TAG, "Failed to create EStop event handler task");
        return false;
      }
    }
  } else {
    if (s_estopTask != nullptr) {
      vTaskDelete(s_estopTask);
      s_estopTask = nullptr;
    }
  }

  return true;
}

static bool _setEStopPinImpl(gpio_num_t pin)
{
  esp_err_t err;

  if (s_estopPin == pin) {
    return true;
  }

  if (!OpenShock::IsValidInputPin(pin)) {
    OS_LOGE(TAG, "Invalid EStop pin: %hhi", static_cast<int8_t>(pin));
    return false;
  }

  bool wasRunning = s_estopTask != nullptr;
  if (wasRunning) {
    if (!_setEStopEnabledImpl(false)) {
      OS_LOGE(TAG, "Failed to disable EStop event handler task");
      return false;
    }
  }

  // Configure the new pin
  gpio_config_t io_conf = {
    .pin_bit_mask = 1ULL << pin,
    .mode         = GPIO_MODE_INPUT,
    .pull_up_en   = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type    = GPIO_INTR_DISABLE,
  };

  err = gpio_config(&io_conf);
  if (err != ESP_OK) {
    OS_LOGE(TAG, "Failed to configure EStop pin");
    return false;
  }

  gpio_num_t oldPin = s_estopPin;

  // Set the new pin
  s_estopPin = pin;

  if (oldPin != GPIO_NUM_NC) {
    // Reset the old pin
    err = gpio_reset_pin(oldPin);
    if (err != ESP_OK) {
      OS_LOGE(TAG, "Failed to reset old EStop pin");
      return false;
    }
  }

  if (wasRunning) {
    if (!_setEStopEnabledImpl(true)) {
      OS_LOGE(TAG, "Failed to re-enable EStop event handler task");
      return false;
    }
  }

  return true;
}

static bool s_estopInitialized = false;
bool EStopManager::Init()
{
  if (s_estopInitialized) {
    return true;
  }
  s_estopInitialized = true;

  Config::EStopConfig cfg;
  if (!OpenShock::Config::GetEStop(cfg)) {
    OS_LOGE(TAG, "Failed to get EStop pin from config");
    return false;
  }

  OpenShock::ScopedLock lock__(&s_estopMutex);

  if (!_setEStopPinImpl(cfg.gpioPin)) {
    OS_LOGE(TAG, "Failed to set EStop pin");
    return false;
  }

  if (!_setEStopEnabledImpl(cfg.enabled)) {
    OS_LOGE(TAG, "Failed to create EStop event handler task");
    return false;
  }

  return true;
}

bool EStopManager::SetEStopEnabled(bool enabled)
{
  OpenShock::ScopedLock lock__(&s_estopMutex);

  if (s_estopPin == GPIO_NUM_NC) {
    gpio_num_t pin;
    if (!OpenShock::Config::GetEStopGpioPin(pin)) {
      OS_LOGE(TAG, "Failed to get EStop pin from config");
      return false;
    }
    if (!_setEStopPinImpl(pin)) {
      OS_LOGE(TAG, "Failed to set EStop pin");
      return false;
    }
  }

  bool success = _setEStopEnabledImpl(enabled);

  return success;
}

bool EStopManager::SetEStopPin(gpio_num_t pin)
{
  OpenShock::ScopedLock lock__(&s_estopMutex);

  return _setEStopPinImpl(pin);
}

bool EStopManager::IsEStopped()
{
  return s_estopActive;
}

int64_t EStopManager::LastEStopped()
{
  return s_estopActivatedAt;
}

void EStopManager::Trigger()
{
  s_externallyTriggered = true;
}
