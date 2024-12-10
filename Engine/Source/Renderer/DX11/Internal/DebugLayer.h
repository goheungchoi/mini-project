#pragma once
#include "Common.h"
#include "Descriptions.h"
using namespace Microsoft::WRL;

class DebugLayer
{
public:
  DebugLayer() = default;
  ~DebugLayer() = default;
  DebugLayer(const DebugLayer&) = delete;
  DebugLayer& operator=(const DebugLayer&) = delete;
  DebugLayer(DebugLayer&&) = delete;
  DebugLayer& operator=(DebugLayer&&) = delete;

public:
private:
  Comptr<ID3D11Debug> _debugLayer = nullptr;
};