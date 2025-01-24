#pragma once
#include "Core/Common.h"
#include "../../Common.h"
#include "../../Device.h"
#include "../../Resources/QuadFrame.h"
class PostProcess
{
private:
  Device* _device;

protected:
  ComPtr<ID3D11RenderTargetView> _rtv;
  ComPtr<ID3D11Texture2D> _texture;
  ComPtr<ID3D11ShaderResourceView> _srv;
  
  Quad::QuadFrame* _frame;

public:
  PostProcess(Device* device) : _device{device} 
  {
    _frame = new Quad::QuadFrame(device);
  };
  ~PostProcess() {}
  
};