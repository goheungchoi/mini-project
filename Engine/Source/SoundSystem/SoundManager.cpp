#include "SoundSystem/SoundManager.h"

#include "Shared/Config/Config.h"
#include "Core/Utils/IOUtils.h"

#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"  // Include FMOD error strings
#pragma comment(lib, "fmod_vc.lib")
using namespace FMOD;

static constexpr int kNumSoundChannels = 32;

static FMOD::System* fmodSystem = []() {
  FMOD::System* _system{nullptr};
  FMOD::System_Create(&_system);
  if (!_system)
  {
    throw std::runtime_error("Can't create fmod system!");
  }
  FMOD_RESULT res = _system->init(kNumSoundChannels, FMOD_INIT_NORMAL, 0);

  if (res != FMOD_OK)
  {
    throw std::runtime_error("fmod system initialization failed: "s + FMOD_ErrorString(res));
  }

  return _system;
}();
static FMOD::Channel* channels[kNumSoundChannels]{};
static std::vector<float> channelVolumes(kNumSoundChannels, 1.f);

static std::unordered_map<std::wstring, FMOD::Sound*> pathSoundDataMap;

static std::list<std::pair<std::wstring, FMOD::Channel**>> soundChannelMap;

bool SoundManager::LoadSound(const std::wstring& path, bool loop)
{
  fs::path absolutePath = ns::kProjectDir;
  absolutePath /= "Library\\Sound";
  absolutePath /= path;

  FMOD::Sound* sound;
  FMOD_RESULT res = fmodSystem->createSound(
      (const char*)absolutePath.generic_u8string().c_str(),
      (loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF),
      NULL, &sound);

  if (res != FMOD_OK)
  {
    throw std::runtime_error("fmod sound creation failed: "s +
                             FMOD_ErrorString(res));
  }

  pathSoundDataMap[path] = sound;

  return true;
}

int SoundManager::PlaySound(const std::wstring& path)
{
  // Find a channel that is not playing a sound.
  int i = 0;
  for (; i < kNumSoundChannels; ++i)
  {
    if (IsPlaying(i))
    {
      continue;
    }
    else
    {
      break;
    }
  }

  if (i >= kNumSoundChannels)
  {
    return -1;
  }

  if (auto it = pathSoundDataMap.find(path); it == pathSoundDataMap.end())
  {
    throw std::runtime_error("Sound is not found!");
  }

  // Play sound
  FMOD_RESULT res = fmodSystem->playSound(pathSoundDataMap[path], NULL, 0, &channels[i]);
  if (res != FMOD_OK)
  {
    throw std::runtime_error("fmod playing sound failed: "s +
                             FMOD_ErrorString(res));
  }
  soundChannelMap.push_back({path, &channels[i]});
  channels[i]->setVolume(channelVolumes[i]);
  return i;
}

void SoundManager::PlaySound(int channel, const std::wstring& path)
{
  channels[channel]->stop();
  FMOD_RESULT res = fmodSystem->playSound(pathSoundDataMap[path], NULL, 0,
                        &channels[channel]);
  if (res != FMOD_OK)
  {
    throw std::runtime_error("fmod playing sound failed: "s +
                             FMOD_ErrorString(res));
  }
  soundChannelMap.push_back({path, &channels[channel]});
  channels[channel]->setVolume(channelVolumes[channel]);
}

void SoundManager::StopSound(const std::wstring& path) {
  auto it = std::find_if(
      std::begin(soundChannelMap), std::end(soundChannelMap),
      [&path](const std::pair<std::wstring, FMOD::Channel**>& soundChannel) {
        return soundChannel.first == path;
      });
  
  if (it != soundChannelMap.end())
  {
    if (it->second)
      (*it->second)->stop();

    soundChannelMap.erase(it);
  }
}

void SoundManager::StopSound(int channel)
{
  if (channels[channel])
    channels[channel]->stop();
}

void SoundManager::SetVolume(float volume)
{
  for (unsigned int i = 0; i < kNumSoundChannels; ++i)
	{
		channelVolumes[i] = volume;
    if (channels[i])
      channels[i]->setVolume(volume);
	}
}

void SoundManager::SetVolume(int channel, float volume) {
  channelVolumes[channel] = volume;
  if (channels[channel])
    channels[channel]->setVolume(volume);
}

bool SoundManager::IsPlaying(int channel)
{
  if (!channels[channel])
    return false;

  bool res{false};
  channels[channel]->isPlaying(&res);
  return res;
}

bool SoundManager::IsPlaying(const std::wstring& path)
{
  auto it = std::find_if(
      std::begin(soundChannelMap), std::end(soundChannelMap),
      [&path](const std::pair<std::wstring, FMOD::Channel**>& soundChannel) {
        return soundChannel.first == path;
      });
  if (it != soundChannelMap.end())
  {
    if (it->second)
    {
      bool res{false};
      (*it->second)->isPlaying(&res);
      if (res)
      {
        return true;
      }
      else
      {
        soundChannelMap.erase(it);
        return false;
      }
    }
    else
    {
      soundChannelMap.erase(it);
      return false;
    }
  }
  else
  {
    return false;
  }

}
