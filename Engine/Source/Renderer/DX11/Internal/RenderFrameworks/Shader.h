#pragma once
#include "../Common.h"

struct VertexShader
{
  ComPtr<ID3D11VertexShader> shader;
  ComPtr<ID3D11InputLayout> layout;
};

struct PixelShader
{
  ComPtr<ID3D11PixelShader> shader;
};