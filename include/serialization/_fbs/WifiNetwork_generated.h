// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_WIFINETWORK_OPENSHOCK_SERIALIZATION_TYPES_H_
#define FLATBUFFERS_GENERATED_WIFINETWORK_OPENSHOCK_SERIALIZATION_TYPES_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 25 &&
              FLATBUFFERS_VERSION_MINOR == 2 &&
              FLATBUFFERS_VERSION_REVISION == 10,
             "Non-compatible flatbuffers version included");

#include "WifiAuthMode_generated.h"

namespace OpenShock {
namespace Serialization {
namespace Types {

struct WifiNetwork;
struct WifiNetworkBuilder;

struct WifiNetwork FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef WifiNetworkBuilder Builder;
  struct Traits;
  static FLATBUFFERS_CONSTEXPR_CPP11 const char *GetFullyQualifiedName() {
    return "OpenShock.Serialization.Types.WifiNetwork";
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_SSID = 4,
    VT_BSSID = 6,
    VT_CHANNEL = 8,
    VT_RSSI = 10,
    VT_AUTH_MODE = 12,
    VT_SAVED = 14
  };
  const ::flatbuffers::String *ssid() const {
    return GetPointer<const ::flatbuffers::String *>(VT_SSID);
  }
  const ::flatbuffers::String *bssid() const {
    return GetPointer<const ::flatbuffers::String *>(VT_BSSID);
  }
  uint8_t channel() const {
    return GetField<uint8_t>(VT_CHANNEL, 0);
  }
  int8_t rssi() const {
    return GetField<int8_t>(VT_RSSI, 0);
  }
  OpenShock::Serialization::Types::WifiAuthMode auth_mode() const {
    return static_cast<OpenShock::Serialization::Types::WifiAuthMode>(GetField<uint8_t>(VT_AUTH_MODE, 0));
  }
  bool saved() const {
    return GetField<uint8_t>(VT_SAVED, 0) != 0;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_SSID) &&
           verifier.VerifyString(ssid()) &&
           VerifyOffset(verifier, VT_BSSID) &&
           verifier.VerifyString(bssid()) &&
           VerifyField<uint8_t>(verifier, VT_CHANNEL, 1) &&
           VerifyField<int8_t>(verifier, VT_RSSI, 1) &&
           VerifyField<uint8_t>(verifier, VT_AUTH_MODE, 1) &&
           VerifyField<uint8_t>(verifier, VT_SAVED, 1) &&
           verifier.EndTable();
  }
};

struct WifiNetworkBuilder {
  typedef WifiNetwork Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_ssid(::flatbuffers::Offset<::flatbuffers::String> ssid) {
    fbb_.AddOffset(WifiNetwork::VT_SSID, ssid);
  }
  void add_bssid(::flatbuffers::Offset<::flatbuffers::String> bssid) {
    fbb_.AddOffset(WifiNetwork::VT_BSSID, bssid);
  }
  void add_channel(uint8_t channel) {
    fbb_.AddElement<uint8_t>(WifiNetwork::VT_CHANNEL, channel, 0);
  }
  void add_rssi(int8_t rssi) {
    fbb_.AddElement<int8_t>(WifiNetwork::VT_RSSI, rssi, 0);
  }
  void add_auth_mode(OpenShock::Serialization::Types::WifiAuthMode auth_mode) {
    fbb_.AddElement<uint8_t>(WifiNetwork::VT_AUTH_MODE, static_cast<uint8_t>(auth_mode), 0);
  }
  void add_saved(bool saved) {
    fbb_.AddElement<uint8_t>(WifiNetwork::VT_SAVED, static_cast<uint8_t>(saved), 0);
  }
  explicit WifiNetworkBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<WifiNetwork> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<WifiNetwork>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<WifiNetwork> CreateWifiNetwork(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> ssid = 0,
    ::flatbuffers::Offset<::flatbuffers::String> bssid = 0,
    uint8_t channel = 0,
    int8_t rssi = 0,
    OpenShock::Serialization::Types::WifiAuthMode auth_mode = OpenShock::Serialization::Types::WifiAuthMode::Open,
    bool saved = false) {
  WifiNetworkBuilder builder_(_fbb);
  builder_.add_bssid(bssid);
  builder_.add_ssid(ssid);
  builder_.add_saved(saved);
  builder_.add_auth_mode(auth_mode);
  builder_.add_rssi(rssi);
  builder_.add_channel(channel);
  return builder_.Finish();
}

struct WifiNetwork::Traits {
  using type = WifiNetwork;
  static auto constexpr Create = CreateWifiNetwork;
};

inline ::flatbuffers::Offset<WifiNetwork> CreateWifiNetworkDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *ssid = nullptr,
    const char *bssid = nullptr,
    uint8_t channel = 0,
    int8_t rssi = 0,
    OpenShock::Serialization::Types::WifiAuthMode auth_mode = OpenShock::Serialization::Types::WifiAuthMode::Open,
    bool saved = false) {
  auto ssid__ = ssid ? _fbb.CreateString(ssid) : 0;
  auto bssid__ = bssid ? _fbb.CreateString(bssid) : 0;
  return OpenShock::Serialization::Types::CreateWifiNetwork(
      _fbb,
      ssid__,
      bssid__,
      channel,
      rssi,
      auth_mode,
      saved);
}

}  // namespace Types
}  // namespace Serialization
}  // namespace OpenShock

#endif  // FLATBUFFERS_GENERATED_WIFINETWORK_OPENSHOCK_SERIALIZATION_TYPES_H_
