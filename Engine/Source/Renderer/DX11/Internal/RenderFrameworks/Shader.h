#pragma once
#include "../Common.h"
#include "../Device.h"
struct VertexShader
{
  ComPtr<ID3D11VertexShader> shader;
  ComPtr<ID3D11InputLayout> layout;
};

struct PixelShader
{
  ComPtr<ID3D11PixelShader> shader;
};

class ShaderCompiler
{
private:
  Device* _device;

public:
  ShaderCompiler(Device* device) : _device{device} {}
  ~ShaderCompiler() {}

public:
  VertexShader* CompileVertexShader(const std::vector<D3D_SHADER_MACRO>& macros)
  {
    VertexShader* pShader = new VertexShader();
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the
    // shaders. Setting this flag improves the shader debugging experience,
    // but still allows the shaders to be optimized and to run exactly the
    // way they will run in the release configuration of this program.
    shaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HR_T(D3DCompileFromFile(L"../Engine/Source/Renderer/DX11/Shader/DX11Shader.hlsl", macros.data(),
                            D3D_COMPILE_STANDARD_FILE_INCLUDE, "vs_main",
                            "vs_5_0", shaderFlags, 0, &shaderBlob, &errorBlob));
    HR_T(_device->GetDevice()->CreateVertexShader(
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr,
        pShader->shader.GetAddressOf()));
    auto inputLayoutDesc = CreateInputLayoutDesc(
        std::vector<uint8_t>((uint8_t*)shaderBlob->GetBufferPointer(),
                             (uint8_t*)shaderBlob->GetBufferPointer() +
                                 shaderBlob->GetBufferSize()),
        shaderBlob->GetBufferSize());

    HR_T(_device->GetDevice()->CreateInputLayout(
        inputLayoutDesc.data(), static_cast<UINT>(inputLayoutDesc.size()),
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
        pShader->layout.GetAddressOf()));

    shaderBlob->Release();
    return pShader;
  }
  PixelShader* CompilePixelShader(const std::vector<D3D_SHADER_MACRO>& macros)
  {
    PixelShader* pShader = new PixelShader();
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the
    // shaders. Setting this flag improves the shader debugging experience,
    // but still allows the shaders to be optimized and to run exactly the
    // way they will run in the release configuration of this program.
    shaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HR_T(D3DCompileFromFile(
        L"../Engine/Source/Renderer/DX11/Shader/DX11Shader.hlsl",
        macros.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE, "ps_main", "ps_5_0",
        shaderFlags, 0, &shaderBlob, &errorBlob));
    HR_T(_device->GetDevice()->CreatePixelShader(
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr,
        pShader->shader.GetAddressOf()));


    shaderBlob->Release();
    return pShader;
  }
};