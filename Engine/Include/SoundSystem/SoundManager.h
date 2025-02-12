#pragma once

#include "Core/Common.h"

namespace SoundManager
{
bool LoadSound(const std::wstring& path, bool loop = false);

int PlaySound(const std::wstring& path);

void PlaySound(int channel, const std::wstring& path);

void StopSound(const std::wstring& path);

void StopSound(int channel);

void SetVolume(float volume);

void SetVolume(int channel, float volume);

bool IsPlaying(int channel);

bool IsPlaying(const std::wstring& path);

}; // namespace SoundManager

