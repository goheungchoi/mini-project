#pragma once

#ifndef _WIN64
#include "fmod.hpp"
#pragma comment (lib, "fmod_vc.lib")
#endif
#ifdef _WIN64
#include "fmod.hpp"
#pragma comment (lib, "fmod_vc.lib")
using namespace FMOD;
#endif

#include "Singleton.h"


class SoundManager : public Singleton<SoundManager>
{
	using SoundChannel = int;
private:
	SoundManager(const SoundManager&) = delete;
	SoundManager(SoundManager&&) noexcept = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager& operator=(SoundManager&&) noexcept = delete;

	static constexpr unsigned int SoundChnalList = 32;

	static FMOD::System* system;
	static FMOD::Channel* channel[SoundChnalList];
	static float			volume[SoundChnalList];
	static int				curIndex;

public:
	SoundManager() {};
	~SoundManager();

	static void Initialize();

	static int PlayMusic(std::wstring _key);
	static void PlayMusic(std::wstring _key, SoundChannel _channel);
	static void StopMusic(SoundChannel _channel);
	static void SetVolume(float _volume);
	static void SetVolume(float _volume, SoundChannel _channel);
	static bool IsPlay(SoundChannel _channel);

	static void CreateSound(std::wstring _key, bool loopcheck, FMOD::Sound*& _sound);

};

