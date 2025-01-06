#pragma once

#include <string>

namespace ns
{

struct UUID
{
  static constexpr size_t kNumBytes{16};
  uint8_t byte[kNumBytes];

  UUID() : byte{} {}
  UUID(const uint8_t* data);
  UUID(const std::string& str);

  bool IsNil();
  std::string ToString(bool separated = false);

  bool operator==(const UUID& other) const;

  static bool IsValidUUIDString(const std::string& uuid);
};

UUID GenerateRandomUUID();
UUID GenerateUUIDFromName(const std::string& name);

} // namespace ns

namespace std
{
template <>
struct hash<ns::UUID>
{
  std::size_t operator()(const ns::UUID& uuid) const noexcept
  {
    std::size_t hash = 2166136261u; // FNV-1a 32-bit hash offset
    for (int i = 0; i < 16; ++i)
    {
      hash ^= uuid.byte[i];
      hash *= 16777619u; // FNV-1a prime
    }
    return hash;
  }
};
} // namespace std
