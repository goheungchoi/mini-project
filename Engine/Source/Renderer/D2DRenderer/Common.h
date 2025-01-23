#pragma once

// D2D
#include "d2d1_1.h"
#include "d2d1helper.h"
#include <dwrite.h>

#ifndef DEBUG
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#endif

// D3D
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DirectXTK.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include "directxtk/SimpleMath.h"
#include "directxtk/SpriteBatch.h"

#include <wrl.h>

#include "Core/Types/TextureData.h"
using namespace DirectX::SimpleMath;

#include <unordered_map>
#include  <memory>