#pragma once
#include "../Common.h"
#include "../Device.h"
#include "Core/Common.h"
#include "Shared/Config/Config.h"

#include <fstream>

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
//  VertexShader* CompileVertexShader(const std::vector<D3D_SHADER_MACRO>& macros,
//                                    LPCSTR entryPoint)
//  {
//    VertexShader* pShader = new VertexShader();
//    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#ifdef _DEBUG
//    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the
//    // shaders. Setting this flag improves the shader debugging experience,
//    // but still allows the shaders to be optimized and to run exactly the
//    // way they will run in the release configuration of this program.
//    shaderFlags |= D3DCOMPILE_DEBUG;
//
//    // Disable optimizations to further improve shader debugging
//    shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
//#endif
//    ID3DBlob* shaderBlob = nullptr;
//    ID3DBlob* errorBlob = nullptr;
//    std::wstring filename = L"/DX11Shader.hlsl";
//    std::wstring shaderPath =
//        Utility::convertToUTF16(ns::kShaderDir) + filename;
//
//    try
//    {
//      HR_T(D3DCompileFromFile(
//          shaderPath.c_str(), macros.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE,
//          entryPoint, "vs_5_0", shaderFlags, 0, &shaderBlob, &errorBlob));
//      
//    }
//    catch (const std::exception& e)
//    {
//      std::ofstream o("exception_info.txt");
//      o << e.what() << std::endl;
//      exit(-1);
//    }
//    
//    try
//    {
//      HR_T(_device->GetDevice()->CreateVertexShader(
//          shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr,
//          pShader->shader.GetAddressOf()));
//    }
//    catch (const std::exception& e)
//    {
//      std::ofstream o("exception_info.txt");
//      o << e.what() << std::endl;
//      exit(-1);
//    }
  VertexShader* CompileVertexShader(const std::vector<D3D_SHADER_MACRO>& macros,
                                    LPCSTR entryPoint)
  {
    VertexShader* pShader = new VertexShader();
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
    shaderFlags |= D3DCOMPILE_DEBUG;
    shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    std::wstring filename = L"/DX11Shader.hlsl";
    std::wstring shaderPath = L"../Library/Shader/DX11Shader.hlsl";
        //Utility::convertToUTF16(ns::kShaderDir) + filename;

    // **Shader Compile**
    HRESULT hr = D3DCompileFromFile(
        shaderPath.c_str(), macros.data(), D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint, "vs_5_0", shaderFlags, 0, &shaderBlob, &errorBlob);

    if (FAILED(hr))
    {
      std::ofstream o("exception_info.txt");
      
      // 에러 메시지가 있으면 기록
      if (errorBlob)
      {
        o << "Shader Compilation Failed: "
          << static_cast<const char*>(errorBlob->GetBufferPointer())
          << std::endl;
        errorBlob->Release();
      }
      else
      {
        o << "Shader Compilation Failed with HRESULT: 0x" << std::hex << hr
          << std::endl;
      }
      throw std::runtime_error("D3DCompileFromFile failed");
    }

    // **Create Vertex Shader**
    hr = _device->GetDevice()->CreateVertexShader(
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr,
        pShader->shader.GetAddressOf());

    if (FAILED(hr))
    {
      std::ofstream o("exception_info.txt");
      o << "CreateVertexShader Failed with HRESULT: 0x" << std::hex << hr
        << std::endl;
      throw std::runtime_error("CreateVertexShader failed");
    }
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

    CreateInputLayoutDesc(
        inputLayoutDesc,
        std::vector<uint8_t>((uint8_t*)shaderBlob->GetBufferPointer(),
                             (uint8_t*)shaderBlob->GetBufferPointer() +
                                 shaderBlob->GetBufferSize()),
        shaderBlob->GetBufferSize());

    HR_T(_device->GetDevice()->CreateInputLayout(
        inputLayoutDesc.data(), static_cast<UINT>(inputLayoutDesc.size()),
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
        pShader->layout.GetAddressOf()));
    std::ranges::for_each(inputLayoutDesc, [](D3D11_INPUT_ELEMENT_DESC& desc) {
      SAFE_RELEASE(desc.SemanticName);
    });
    shaderBlob->Release();
    return pShader;
  }
  PixelShader* CompilePixelShader(const std::vector<D3D_SHADER_MACRO>& macros,
                                  LPCSTR entryPoint)
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
    std::wstring filename = L"/DX11Shader.hlsl";
    std::wstring shaderPath = L"../Library/Shader/DX11Shader.hlsl";
        //Utility::convertToUTF16(ns::kShaderDir) + filename;
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HR_T(D3DCompileFromFile(shaderPath.c_str(), macros.data(),
        D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, "ps_5_0", shaderFlags, 0,
        &shaderBlob, &errorBlob));
    HR_T(_device->GetDevice()->CreatePixelShader(
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr,
        pShader->shader.GetAddressOf()));

    shaderBlob->Release();
    return pShader;
  }
};