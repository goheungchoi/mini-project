#pragma once
#include "../Common.h"

//�켱�� �� pipe line�� �ϳ��� ���̴��� ����غ���...
//class PipelineState
//{
//public:
//  PipelineState() = default;
//  ~PipelineState() = default;
//
//public:
//  void CreateViewPort(FLOAT TopLeftX, FLOAT TopLeftY,FLOAT Width, FLOAT Height,
//                      FLOAT MinDepth, FLOAT MaxDepth) 
//  {
//    _viewPort = {
//        TopLeftX, TopLeftY, Width, Height, MinDepth, MaxDepth
//    };
//  }
//
//  
//public:
//  ComPtr<ID3D11RasterizerState> _rasterizeState = nullptr;
//  ComPtr<ID3D11DepthStencilState> _depthStencilState = nullptr;
//  ComPtr<ID3D11BlendState> _blendState = nullptr;
//  D3D11_VIEWPORT _viewPort;
//};