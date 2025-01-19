#include "ResourcePool.h"

#include "Shared/Serialize/model_generated.h"
#include <flatbuffers/flatbuffers.h>
#include <fstream>

static void ProcessSkeleton(SkeletonData& skeleton, const GameResource::Skeleton* flatSkeleton) {
  skeleton.name = flatSkeleton->name()->c_str();
	
	skeleton.bones.resize(flatSkeleton->bones()->size());
  for (int i = 0; i < skeleton.bones.size(); ++i)
  {
    const GameResource::Bone* bone = flatSkeleton->bones()->operator[](i);
    skeleton.bones[i].id = bone->id();
    skeleton.bones[i].name = bone->name()->c_str();
    skeleton.bones[i].inverseBindMatrix = XMMATRIX((float*)bone->offset());
	}

	skeleton.nodes.resize(flatSkeleton->nodes()->size());
  for (int i = 0; i < skeleton.nodes.size(); ++i)
  {
    const GameResource::SkeletonNode* node =
        flatSkeleton->nodes()->operator[](i);
    skeleton.nodes[i].name = node->name()->c_str();
    skeleton.nodes[i].level = node->level();
    skeleton.nodes[i].parent = node->parent();
    skeleton.nodes[i].firstChild = node->firstChild();
    skeleton.nodes[i].nextSibling = node->nextSibling();
    skeleton.nodes[i].boneId = node->boneId();
	}
}

template <>
Handle ResourcePool<SkeletonData>::LoadImpl(xUUID uuid, void* pUser)
{
  fs::path path = GetResourcePath(uuid);

  std::ifstream ifs(path, std::ios::binary);
  std::vector<char> buffer((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());

  const GameResource::Skeleton* flatSkeleton =
      flatbuffers::GetRoot<GameResource::Skeleton>(buffer.data());

  SkeletonData skeleton;
  ProcessSkeleton(skeleton, flatSkeleton);

  Handle handle = _handleTable.ClaimHandle(std::move(skeleton),
                                           (uint16_t)ResourceType::kSkeleton);
  _uuidMap[uuid] = handle.index;
  return handle;
}
