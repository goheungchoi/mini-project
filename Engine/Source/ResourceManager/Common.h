#pragma once

#include "Core/Handle/Handle.h"
#include "Core/Handle/ResourceHandle.h

#include "Core/Types/ShaderData.h"
#include "Core/Types/MeshData.h"
#include "Core/Types/MaterialData.h"
#include "Core/Types/TextureData.h"

enum class ResourceType {
  kUnknown = 0,
	kShader,
	kMesh,
	kMaterial,
	kTexture,

	kAnimator,
	kAnimation,

	kAudio
};

