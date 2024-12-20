#pragma once

#include "Core/Common.h"

struct Handle {
  static constexpr size_t kNumIndexBits{32};
  static constexpr size_t kNumVersionBits{16};
  static constexpr size_t kNumDescBits{16};

  static constexpr uint64_t kMinVersion{0};
  static constexpr uint64_t kMaxVersion{(1ULL << kNumVersionBits) - 2U};

  static constexpr uint64_t kMaxIndex{(1ULL << kNumIndexBits) - 2U};

	static constexpr uint64_t kInvalidValue{kUInt64Max};

  static const Handle kInvalidHandle;

	union {
    uint64_t _value;

		struct
    {
      uint32_t index : kNumIndexBits;
      uint16_t version : kNumVersionBits;
      uint16_t desc : kNumDescBits;
    };
	};

  Handle() : _value{kInvalidValue} {}
  explicit Handle(uint64_t value) : _value{value} {}
  explicit Handle(uint32_t index, uint16_t version)
      : index{index}, version{version}, desc{} {}
  explicit Handle(uint32_t index, uint16_t version, uint16_t desc)
      : index{index}, version{version}, desc{desc} {}

	Handle(const Handle&) = default;
	Handle& operator=(const Handle&) = default;

  Handle(Handle&& other) noexcept {
    _value = other._value;
    other._value = kInvalidValue;
  }
  Handle& operator=(Handle&& other) noexcept {
    _value = other._value;
    other._value = kInvalidValue;
    return *this;
  }

	bool IsInvalid() const { return _value == kInvalidValue; }

  void SetDesc(uint16_t description) { desc = description; }

  inline bool operator==(Handle other) const { return _value == other._value; }
  inline bool operator==(uint64_t value) const { return _value == value; }
	
	inline explicit operator uint64_t() const { return _value; }
};

inline const Handle Handle::kInvalidHandle{};

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

template<typename T, size_t GROW_SIZE = 1024>
class HandleTable
{
	using TableEntry = std::pair<Handle, std::optional<T>>;

	std::stack<uint64_t> _slotStack{{0}};

	std::vector<TableEntry> _table;
  std::vector<uint32_t> _refCounts;

	std::optional<T> _sentinel{std::nullopt};

public:

	/**
	 * @brief Claim a handle
	 * @return 
	 */
  Handle ClaimHandle(T&& obj, uint16_t description = 0)
  {
    // TODO: Can be optimized with a mem queue.
    uint64_t i = _slotStack.top();
    _slotStack.pop();

		// If the current slot is not empty,
		// return an invalid handle
    if (_table[i].second.has_value())
      return Handle::kInvalidHandle;

		Handle& curr{_table[i].first};
		if (curr == Handle::kInvalidHandle)
		{
			curr.index = i;
			curr.version = Handle::kMinVersion;
		}
		else
		{
			uint64_t version = curr.version + 1;
			if (version > Handle::kMaxVersion)
			{
				version = Handle::kMinVersion;
			}
			curr.version = version;
		}
    curr.SetDesc(description);

		_refCounts[i] = 1;

		// Push the next empty slot to the stack if available
		uint64_t nextSlot = i + 1;

		if (nextSlot >= _table.size())
    {
      IncreaseCapacity();
		}

		// If the next slot is empty
    if (!_table[nextSlot].second.has_value())
    {
      _slotStack.push(nextSlot);
    }

		return curr;
	}

	/**
	 * @brief Duplicate a handle and increase the reference count
	 * @param handle 
	 * @return Handle::kInvalidHandle if the input handle is not a valid handle.
	 */
	Handle DuplicateHandle(const Handle& handle) {
    if (!IsValidHandle(handle))
      return Handle::kInvalidHandle;

		_refCounts[handle.index]++;
		return handle;
	}

	void ReleaseHandle(Handle& handle) {
    if (!IsValidHandle(handle))
      throw std::exception("invalid handle!");

    _refCounts[handle.index]--;

		// The reference count reaches the zero
		if (_refCounts[handle.index] == 0)
    {
      // Empty the spot
      _table[handle.index].second.reset();
			// Push the index to the next empty slot
      _slotStack.push(handle.index);
		}

		// Invalidate the inputted handle
		handle = Handle::kInvalidHandle;
	}

	uint32_t GetReferenceCount(const Handle& handle) const
  {
    return _refCounts[handle.index];
	}

	/**
	 * @brief Check if the handle is a valid 
	 * @param handle 
	 * @return 
	 */
	bool IsValidHandle(const Handle& handle) const
  {
    if (handle == Handle::kInvalidHandle || handle.index < _table.size())
      return false;

		// To be valid, the version and description has to be the same
    const Handle& valid = _table[handle.index].first;
    return _table[handle.index].second.has_value() && // Value existance check
           handle.version == valid.version &&         // Handle out-dated check
           handle.desc == valid.desc;									// Handle type correction check
	}

	inline 
	Handle operator[](uint32_t index) const {
    if (index >= _table.size())
      throw std::out_of_range("Descriptor index out of range!");
    return Handle{index, _table[index].first};
	}

	inline 
	std::optional<T>& operator[](const Handle& handle) const {
    if (!IsValidHandle(handle))
      return _sentinel;

		return _table[handle.index].second;
	}

private:
	void IncreaseCapacity() {
    std::size_t oldsize = _table.size();
    if (oldsize >= Handle::kMaxIndex)
    {
      throw std::exception("max. resource capacity reached!");
    }

    std::size_t newsize = oldsize + GROW_SIZE;
    newsize = (newsize < Handle::kMaxIndex) ? newsize : Handle::kMaxIndex;

    _table.resize(newsize);
    _refCounts.resize(newsize);
	}

};
