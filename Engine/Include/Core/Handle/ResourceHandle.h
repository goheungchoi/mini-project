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
  ShaderHandle(Handle& handle) : Handle(handle)
  {
    if (ResourceType::kShader != GetHandleResourceType(this))
    {
      throw std::exception("Handle's resource type mismatch!");
		}
	}
  ShaderHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kShader);
  }
  ShaderHandle(uint32_t index, uint16_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kShader);
  }
};

struct MeshHandle : public Handle
{
  MeshHandle() : Handle{} {}
  MeshHandle(Handle& handle) : Handle(handle)
  {
    if (ResourceType::kMesh != GetHandleResourceType(this))
    {
      throw std::exception("Handle's resource type mismatch!");
    }
  }
  MeshHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kMesh);
  }
  MeshHandle(uint32_t index, uint16_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kMesh);
  }
};

struct MaterialHandle : public Handle
{
  MaterialHandle() : Handle{} {}
  MaterialHandle(Handle& handle) : Handle(handle)
  {
    if (ResourceType::kMaterial != GetHandleResourceType(this))
    {
      throw std::exception("Handle's resource type mismatch!");
    }
  }
  MaterialHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kMaterial);
  }
  MaterialHandle(uint32_t index, uint16_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kMaterial);
  }
};

struct TextureHandle : public Handle
{
  TextureHandle() : Handle{} {}
  TextureHandle(Handle& handle) : Handle(handle)
  {
    if (ResourceType::kTexture != GetHandleResourceType(this))
    {
      throw std::exception("Handle's resource type mismatch!");
    }
  }
  TextureHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kTexture);
  }
  TextureHandle(uint32_t index, uint16_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kTexture);
  }
};

struct ModelHandle : public Handle
{
  ModelHandle() : Handle{} {}
  ModelHandle(Handle& handle) : Handle(handle)
  {
    if (ResourceType::kModel != GetHandleResourceType(this))
    {
      throw std::exception("Handle's resource type mismatch!");
    }
  }
  ModelHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kModel);
  }
  ModelHandle(uint32_t index, uint16_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kModel);
  }
};

struct AnimationHandle : public Handle
{
  AnimationHandle() : Handle{} {}
  AnimationHandle(Handle& handle) : Handle(handle)
  {
    if (ResourceType::kAnimation != GetHandleResourceType(this))
    {
      throw std::exception("Handle's resource type mismatch!");
    }
  }
  AnimationHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kAnimation);
  }
  AnimationHandle(uint32_t index, uint16_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kAnimation);
  }
};

struct AnimatorHandle : public Handle
{
  AnimatorHandle() : Handle{} {}
  AnimatorHandle(Handle& handle) : Handle(handle)
  {
    if (ResourceType::kAnimator != GetHandleResourceType(this))
    {
      throw std::exception("Handle's resource type mismatch!");
    }
  }
  AnimatorHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kAnimator);
  }
  AnimatorHandle(uint32_t index, uint16_t version) : Handle{index, version}
  {
    SetHandleResourceType(this, ResourceType::kAnimator);
  }
};

struct AudioHandle : public Handle
{
  AudioHandle() : Handle{} {}
  AudioHandle(Handle& handle) : Handle(handle)
  {
    if (ResourceType::kAudio != GetHandleResourceType(this))
    {
      throw std::exception("Handle's resource type mismatch!");
    }
  }
  AudioHandle(uint64_t value) : Handle{value}
  {
    SetHandleResourceType(this, ResourceType::kAudio);
  }
  AudioHandle(uint32_t index, uint16_t version) : Handle{index, version}
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
