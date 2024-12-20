#pragma once

#include "Core/Common.h"

struct Handle {
  static constexpr size_t kNumIndexBits{32};
  static constexpr size_t kNumVersionBits{16};
  static constexpr size_t kNumDescBits{16};

  static constexpr uint64_t kMinVersion{0};
  static constexpr uint64_t kMaxVersion{(1ULL << kNumVersionBits) - 2U};

  static constexpr uint64_t kMaxIndex{(1ULL << kNumIndexBits) - 2U};

  static constexpr uint64_t kInvalidHandle{kUInt64Max};

	union {
    uint64_t _value;

		struct
    {
      uint64_t index : kNumIndexBits;
      uint64_t version : kNumVersionBits;
      uint64_t desc : kNumDescBits;
    };
	};

  Handle() : _value{kInvalidHandle} {}
  Handle(uint64_t value) : _value{value} {}
  Handle(uint64_t index, uint64_t version)
      : index{index}, version{version}, desc{} {}
  Handle(uint64_t index, uint64_t version, uint64_t desc)
      : index{index}, version{version}, desc{desc} {}

	Handle(const Handle&) = default;
	Handle& operator=(const Handle&) = default;

  Handle(Handle&& other) noexcept {
    _value = other._value;
    other._value = kInvalidHandle;
  }
  Handle& operator=(Handle&& other) noexcept {
    _value = other._value;
    other._value = kInvalidHandle;
    return *this;
  }

	bool IsInvalid() { return _value == kInvalidHandle; }

  void SetDesc(uint64_t description) { desc = description; }

  inline operator uint64_t() const { return _value; }
};

namespace std
{
template <>
struct hash<Handle>
{
  std::size_t operator()(const Handle& meshHandle) const noexcept
  {
    return meshHandle._value;
  }
};
}

