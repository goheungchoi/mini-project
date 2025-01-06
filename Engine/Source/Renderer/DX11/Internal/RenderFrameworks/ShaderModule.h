#pragma once
#include "../Common.h"
#include "../Descriptions.h"
using namespace Microsoft::WRL;

// �ʼ� ���� ���������ο� �ʿ��� ���̴����� ���� ����ü
// SWTODO : �ʿ��Ѱ�?
struct ShaderModule
{
  ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
  ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
  ComPtr<ID3D11InputLayout> _pinputLayout = nullptr;
};
