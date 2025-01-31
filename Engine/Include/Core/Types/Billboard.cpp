#include "Billboard.h"
#include "Renderer/DX11/Internal/Resources/BillboardQuad.h"
#include "Renderer/DX11/Internal/Device.h"
Billboard::Billboard(Device* device) 
{
  _quad = new BillboardQuad(device);
}

Billboard::~Billboard() 
{
  SAFE_RELEASE(_quad);
}


void Billboard::SetPosition() 
{

}

void Billboard::SetSize() {}
