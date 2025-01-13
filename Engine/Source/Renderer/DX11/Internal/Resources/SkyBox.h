#include "Core/Common.h"
#include "../Common.h"
#include "../Device.h"
#include "Buffer.h"
class SkyBox
{
private:
  Device* _device;
  MeshBuffer* _mesh;

public:
  SkyBox(Device* device) : _device{device} {}
  ~SkyBox() { SAFE_RELEASE(_mesh); }

public:
  void Init() 
  {
    _mesh = new MeshBuffer;
    ModelHandle modelHandle = LoadModel("Models\\Skybox\\Skybox.obj");
    ModelData modelData = AccessModelData(modelHandle);
    std::ranges::for_each(modelData.meshes,[&](MeshHandle meshHandle){
      MeshData meshData = AccessMeshData(meshHandle);
         
    });
  }
};