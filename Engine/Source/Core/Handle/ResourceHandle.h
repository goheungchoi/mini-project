#pragma once

#include "Handle.h"

enum class ResourceType : uint8_t
{
  kUnknown,
  kShader,
  kMesh,
  kMaterial,
  kTexture,
	kModel,
  kAnimation,
  kAnimator,
  kAudio,
};

inline void SetHandleResourceType(Handle* handle, ResourceType type)
{
  handle->SetDesc(static_cast<size_t>(type) << 8);
}

inline ResourceType GetHandleResourceType(const Handle* handle)
{
  return static_cast<ResourceType>((uint8_t)(handle->desc >> 8));
}

struct ShaderHandle : public Handle
{
  ShaderHandle() : Handle{} {}
  ShaderHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kShader);
  }
  ShaderHandle(uint64_t index, uint64_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kShader);
  }
};

struct MeshHandle : public Handle
{
  MeshHandle() : Handle{} {}
  MeshHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kMesh);
  }
  MeshHandle(uint64_t index, uint64_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kMesh);
  }
};

struct MaterialHandle : public Handle
{
  MaterialHandle() : Handle{} {}
  MaterialHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kMaterial);
  }
  MaterialHandle(uint64_t index, uint64_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kMaterial);
  }
};

struct TextureHandle : public Handle
{
  TextureHandle() : Handle{} {}
  TextureHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kTexture);
  }
  TextureHandle(uint64_t index, uint64_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kTexture);
  }
};

struct ModelHandle : public Handle
{
  ModelHandle() : Handle{} {}
  ModelHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kModel);
  }
  ModelHandle(uint64_t index, uint64_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kModel);
  }
};

struct AnimationHandle : public Handle
{
  AnimationHandle() : Handle{} {}
  AnimationHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kAnimation);
  }
  AnimationHandle(uint64_t index, uint64_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kAnimation);
  }
};

struct AnimatorHandle : public Handle
{
  AnimatorHandle() : Handle{} {}
  AnimatorHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kAnimator);
  }
  AnimatorHandle(uint64_t index, uint64_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kAnimator);
  }
};

struct AudioHandle : public Handle
{
  AudioHandle() : Handle{} {}
  AudioHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kAudio);
  }
  AudioHandle(uint64_t index, uint64_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kAudio);
  }
};


namespace std
{
template <>
struct hash<ShaderHandle>
{
  std::size_t operator()(const ShaderHandle& meshHandle) const noexcept
  {
    return meshHandle._value;
  }
};

template <>
struct hash<MeshHandle>
{
  std::size_t operator()(const MeshHandle& meshHandle) const noexcept
  {
    return meshHandle._value;
  }
};

template <>
struct hash<MaterialHandle>
{
  std::size_t operator()(const MaterialHandle& meshHandle) const noexcept
  {
    return meshHandle._value;
  }
};

template <>
struct hash<TextureHandle>
{
  std::size_t operator()(const TextureHandle& meshHandle) const noexcept
  {
    return meshHandle._value;
  }
};

template <>
struct hash<ModelHandle>
{
  std::size_t operator()(const ModelHandle& meshHandle) const noexcept
  {
    return meshHandle._value;
  }
};

template <>
struct hash<AnimationHandle>
{
  std::size_t operator()(const AnimationHandle& meshHandle) const noexcept
  {
    return meshHandle._value;
  }
};

template <>
struct hash<AnimatorHandle>
{
  std::size_t operator()(const AnimatorHandle& meshHandle) const noexcept
  {
    return meshHandle._value;
  }
};

template <>
struct hash<AudioHandle>
{
  std::size_t operator()(const AudioHandle& meshHandle) const noexcept
  {
    return meshHandle._value;
  }
};
} // namespace std
