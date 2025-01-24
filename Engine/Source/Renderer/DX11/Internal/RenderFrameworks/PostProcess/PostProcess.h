#pragma once
#include "Core/Common.h"
#include "../../Common.h"
#include "../../Device.h"
class PostProcess
{
private:
  Device* _device;

protected:
  ComPtr<ID3D11RenderTargetView> _rtv;
  ComPtr<ID3D11Texture2D> _texture;
  ComPtr<ID3D11ShaderResourceView> _srv;
  

public:
  PostProcess(Device* device) : _device{device} 
  {
    
  };
  
};