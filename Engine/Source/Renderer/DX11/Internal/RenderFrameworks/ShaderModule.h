#pragma once
#include "../Common.h"
#include "../Descriptions.h"
using namespace Microsoft::WRL;

// 필수 렌더 파이프라인에 필요한 셰이더들을 묶은 구조체
// SWTODO : 필요한가?
struct ShaderModule
{
  ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
  ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
  ComPtr<ID3D11InputLayout> _pinputLayout = nullptr;
};
