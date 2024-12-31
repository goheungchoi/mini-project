#pragma once
#include "Common.h"
#include "Core/Common.h"
#include "Core/Handle/ResourceHandle.h"
#include "RenderFrameworks/Shader.h"
struct MeshBuffer;
struct Sampler;

struct ResourceStorage
{
  std::unordered_map<MeshHandle, MeshBuffer*> meshMap;
  std::unordered_map<ShaderHandle, VertexShader> vertexShaderMap;
  std::unordered_map<ShaderHandle, ComPtr<ID3D11PixelShader>> pixelShaderMap;
};