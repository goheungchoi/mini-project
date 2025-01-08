#pragma once
//use d3d
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <directxtk/DDSTextureLoader.h>
//use comptr
#include <wrl.h>
#include <stdexcept>
#include <vector>
#include <unordered_map>
class com_exception : public std::exception
{
public:
  com_exception(HRESULT hr) : result(hr) {}

  const char* what() const noexcept override
  {
    static char s_str[64] = {};
    sprintf_s(s_str, "Failure with HRESULT of %08X",
              static_cast<unsigned int>(result));
    return s_str;
  }

private:
  HRESULT result;
};

inline void HR_T(HRESULT hr)
{
  if (FAILED(hr))
  {
    throw com_exception(hr);
  }
}

template<typename T>
void SAFE_RELEASE(T& p)
{
  if (p)
  {
    delete p;
    p = nullptr;
  }
}