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


void Billboard::SetPosition(Vector4 position)
{
  _quad->position = position;
}

void Billboard::SetSize(Vector3 scale) 
{
  _quad->scale = scale;
}

void Billboard::SetTexture(TextureHandle texHandle)
{
  _quad->SetTexture(texHandle);
}
