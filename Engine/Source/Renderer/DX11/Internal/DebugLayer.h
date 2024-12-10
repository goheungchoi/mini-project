#pragma once
#include "Common.h"
#include "Descriptions.h"
using namespace Microsoft::WRL;

//using with #ifdef _DEBUG
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
  void CreateDebugLayer(ID3D11Device* pDevice)
  {
    HR_T(pDevice->QueryInterface(
        __uuidof(ID3D11Debug),
        reinterpret_cast<void**>(_debug.GetAddressOf())));
  }
  //check memory leaks 
  void ReportLiveObjects()
  { 
    _debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
  }
  /*
  CHOOSE ONE
  typedef enum D3D11_MESSAGE_SEVERITY {
  D3D11_MESSAGE_SEVERITY_CORRUPTION = 0,->�߻��� �Ϻ� ������ �ջ��� �����մϴ�.
  D3D11_MESSAGE_SEVERITY_ERROR,->���� �޽����� �����մϴ�.
  D3D11_MESSAGE_SEVERITY_WARNING,->��� �޽����� �����մϴ�.
  D3D11_MESSAGE_SEVERITY_INFO,->���� �޽����� �����մϴ�.
  D3D11_MESSAGE_SEVERITY_MESSAGE->�ջ�, ����, ��� �Ǵ� ���� �̿��� �޽����� �����մϴ�.
  } ;
  */
  void SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY severity, bool enable)
  {
    ComPtr<ID3D11InfoQueue> infoQueue;
    if (SUCCEEDED(_debug->QueryInterface(
            __uuidof(ID3D11InfoQueue),
            reinterpret_cast<void**>(infoQueue.GetAddressOf()))))
    {
      if (enable)
      {
        infoQueue->SetBreakOnSeverity(severity, TRUE);
      }
      else
      {
        infoQueue->SetBreakOnSeverity(severity, FALSE);
      }
    }
  }

private:
  ComPtr<ID3D11Debug> _debug = nullptr;
};