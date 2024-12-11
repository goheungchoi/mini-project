#pragma once

#include "Core/Common.h"

enum class ShaderType : uint8_t
{
  kUnknown = 0,

  kVertex,
  kGeometry,
  kHull,
  kTessellator,
  kDomain,
	kPixel,

  kCompute,

  kRayGen,
  kRayIntersect,
  kRayAnyHit,
  kRayClosestHit,
  kRayMiss,
  kRayCallable,

  kMesh,
  kTask,

  kInvalid = std::numeric_limits<uint8_t>::max()
};

enum class ShaderDefineValueType : uint8_t
{
  kNone,    // 0 bytes
  kBool,    // 1 bytes
  kInteger, // 4 bytes
  kFloat,   // 4 bytes
  kString,  // any bytes
};

template<typename T>
concept CShaderDefineValueType =
    (std::same_as<T, nullptr_t> || std::same_as<T, bool> ||
     std::same_as<T, int> || std::same_as<T, float> || std::same_as<T, char*> ||
     std::same_as<T, const char*> || std::same_as<T, std::string>);

struct ShaderDefine
{
  ShaderDefineValueType type;
  std::string define;
  std::vector<char> value;

	ShaderDefine() = default;

	template <CShaderDefineValueType T>
  ShaderDefine(const std::string& define, T value) : define(define)
  {
    SetValue<T>(value);
	}

	template <CShaderDefineValueType T>
  ShaderDefine(std::string&& define, T value) : define(std::move(define))
  {
    SetValue<T>(value);
  }

	template <CShaderDefineValueType T>
  void SetValue(T value)
  {
    if constexpr (std::is_same_v<T, nullptr_t>)
    {
      type = ShaderDefineValueType::kNone;
      this->value.clear();
      return;
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
      type = ShaderDefineValueType::kString;
      this->value.resize(value.size() + 1);
      memcpy(this->value.data(), value.c_str(), this->value.size());
    }
    else if constexpr (std::is_same_v<T, char*> ||
                       std::is_same_v<T, const char*>)
    {
      type = ShaderDefineValueType::kString;
      this->value.resize(strlen(value));
      memcpy(this->value.data(), value, this->value.size());
    }
    else
    {
      if constexpr (std::is_same_v<T, bool>)
				type = ShaderDefineValueType::kBool;
      else if constexpr (std::is_same_v<T, int>)
        type = ShaderDefineValueType::kInteger;
      else
        type = ShaderDefineValueType::kFloat;

      this->value.resize(sizeof(T));
      T* p = reinterpret_cast<T*>(this->value.data());
      *p = value;
    }
	}

	template <CShaderDefineValueType T>
  T GetValue()
  {
    if constexpr (std::is_same_v<T, nullptr_t>)
    {
      if (type != ShaderDefineValueType::kNone)
        throw std::exception("Shader define type mismatch detected!");
      return nullptr;
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
      if (type != ShaderDefineValueType::kString)
        throw std::exception("Shader define type mismatch detected!");
      std::string str(value.data());
      return str;
    }
    else if constexpr (std::is_same_v<T, char*> ||
                       std::is_same_v<T, const char*>)
    {
      if (type != ShaderDefineValueType::kString)
        throw std::exception("Shader define type mismatch detected!");
      return value.data();
    }
    else
    {
      if constexpr (std::is_same_v<T, bool>)
      {
        if (type != ShaderDefineValueType::kBool)
          throw std::exception("Shader define type mismatch detected!");
      }
      else if constexpr (std::is_same_v<T, int>)
      {
        if (type != ShaderDefineValueType::kInteger)
          throw std::exception("Shader define type mismatch detected!");
      }
      else
      {
        if (type != ShaderDefineValueType::kFloat)
          throw std::exception("Shader define type mismatch detected!");
      }

      T* p = reinterpret_cast<T*>(this->value.data());
      return *p;
    }
	}
};

template void ShaderDefine::SetValue<nullptr_t>(nullptr_t);
template void ShaderDefine::SetValue<bool>(bool);
template void ShaderDefine::SetValue<int>(int);
template void ShaderDefine::SetValue<float>(float);
template void ShaderDefine::SetValue<char*>(char*);
template void ShaderDefine::SetValue<const char*>(const char*);
template void ShaderDefine::SetValue<std::string>(std::string);

template nullptr_t ShaderDefine::GetValue<nullptr_t>();
template bool ShaderDefine::GetValue<bool>();
template int ShaderDefine::GetValue<int>();
template float ShaderDefine::GetValue<float>();
template char* ShaderDefine::GetValue<char*>();
template const char* ShaderDefine::GetValue<const char*>();
template std::string ShaderDefine::GetValue<std::string>();

struct ShaderData {
  std::string name;
  std::string path;
  std::vector<ShaderDefine> defines;
	ShaderType type;

	std::vector<uint8_t> data;
};
