#pragma once

#include "Core/Common.h"

struct xUUID {
  static constexpr size_t kNumBytes{16};
  uint8_t byte[kNumBytes];

	xUUID() : byte{} {}
  xUUID(const uint8_t* data);
  xUUID(const std::string& str);

	bool IsNil();
  std::string ToString(bool separated = false);

	bool operator==(const xUUID& other) const;

	static bool IsValidUUIDString(const std::string& uuid);
};

namespace std {
template <>
struct hash<xUUID> {
  std::size_t operator()(const xUUID& uuid) const noexcept {
    std::size_t hash = 2166136261u;  // FNV-1a 32-bit hash offset
    for (int i = 0; i < 16; ++i) {
      hash ^= uuid.byte[i];
      hash *= 16777619u;  // FNV-1a prime
    }
    return hash;
  }
};
}  // namespace std

xUUID GenerateRandomUUID();
xUUID GenerateUUIDFromName(const std::string& name);
