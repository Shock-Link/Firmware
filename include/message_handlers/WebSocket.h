#pragma once

#include <cstdint>

namespace OpenShock::MessageHandlers::WebSocket {
  void HandleGatewayBinary(const uint8_t* data, std::size_t len);
  void HandleLocalBinary(uint8_t socketId, const uint8_t* data, std::size_t len);
}
