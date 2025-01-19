#include "ResourcePool.h"

#include "Shared/Serialize/model_generated.h"
#include <flatbuffers/flatbuffers.h>
#include <fstream>

static void ProcessAnimation(AnimationData& animation,
                             const GameResource::Animation* flatAnimation)
{
  animation.name = flatAnimation->name()->c_str();

	animation.duration = flatAnimation->duration();
  animation.ticksPerSecond = flatAnimation->ticksPerSecond();

	animation.globalInverseTransform =
      XMMATRIX((float*)flatAnimation->globalInverseTransform());

	animation.animationChannels.resize(
      flatAnimation->animationChannels()->size());
  for (uint32_t i = 0; i < animation.animationChannels.size(); ++i)
  {
    const auto* flatChannel = flatAnimation->animationChannels()->operator[](i);

		AnimationChannel channel{.boneId = flatChannel->boneId(),
                             .nodeName = flatChannel->nodeName()->c_str(),

                             .numKeyPositions = flatChannel->numKeyPositions(),
                             .numKeyRotations = flatChannel->numKeyRotations(),
                             .numKeyScalings = flatChannel->numKeyScalings()};

		channel.keyPositions.reserve(channel.numKeyPositions);
		for (const auto* keyPos : *flatChannel->keyPositions())
    {
      channel.keyPositions.push_back({{keyPos->position()->operator[](0),
                                       keyPos->position()->operator[](1),
                                       keyPos->position()->operator[](2),
                                       keyPos->position()->operator[](3)},
                                      keyPos->timeStamp()});
		}

		channel.keyRotations.reserve(channel.numKeyRotations);
    for (const auto* keyRot : *flatChannel->keyRotations())
    {
      channel.keyRotations.push_back({{keyRot->orientation()->operator[](0),
                                       keyRot->orientation()->operator[](1),
                                       keyRot->orientation()->operator[](2),
                                       keyRot->orientation()->operator[](3)},
                                      keyRot->timeStamp()});
    }

		channel.keyScalings.reserve(channel.numKeyScalings);
    for (const auto* keyScale : *flatChannel->keyScalings())
    {
      channel.keyScalings.push_back({{keyScale->scaling()->operator[](0),
                                      keyScale->scaling()->operator[](1),
                                      keyScale->scaling()->operator[](2),
                                      keyScale->scaling()->operator[](3)},
                                     keyScale->timeStamp()});
    }

		animation.animationChannels[i] = std::move(channel);
	}
}

template<>
Handle ResourcePool<AnimationData>::LoadImpl(xUUID uuid, void* pUser)
{
  fs::path path = GetResourcePath(uuid);

  std::ifstream ifs(path, std::ios::binary);
  std::vector<char> buffer((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());

  const GameResource::Animation* flatAnimation =
      flatbuffers::GetRoot<GameResource::Animation>(buffer.data());

  AnimationData animation;
  ProcessAnimation(animation, flatAnimation);

  Handle handle = _handleTable.ClaimHandle(std::move(animation),
                                           (uint16_t)ResourceType::kAnimation);
  _uuidMap[uuid] = handle.index;
  return handle;
}
