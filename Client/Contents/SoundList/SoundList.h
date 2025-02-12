#pragma once

#include "Core/Common.h"

#include "Core/Utils/RandomGenerator.h"

namespace SoundList
{

// Background music
extern const wchar_t* Background_Title;
extern const wchar_t* Background_Pub;
extern const wchar_t* Background_Pub_Ambient;
extern const wchar_t* Background_Storage;
extern const wchar_t* Background_Storage_Ambient;
extern const wchar_t* Background_ConferenceHall;
extern const wchar_t* Background_ConferenceHall_Ambient;

extern const wchar_t* Background_Dialog;

// Button
extern const wchar_t* Button_Click;
extern const wchar_t* Button_Hover;

extern const wchar_t* Button_Start;	// DONE
extern const wchar_t* Button_Retry; // X

// Grid interaction
extern const wchar_t* Grid_Placement_Click; // DONE
extern const wchar_t* Grid_Placement_Hover; // DONE
extern const wchar_t* Placeholder_Rotation; // DONE

// Character
extern const wchar_t* Character_Hover;  // DONE
 
// Brawler
extern const wchar_t* Brawler_Ready;  // DONE
extern const wchar_t* Brawler_Fire;   // DONE

// Slasher
extern const wchar_t* Slasher_Ready1;     // DONE
extern const wchar_t* Slasher_Ready2;     // DONE
extern const wchar_t* Slasher_Rush;       // DONE
extern const wchar_t* Slasher_Fire;       // DONE

// Gunman
extern const wchar_t* Gunman_Ready1;      // DONE
extern const wchar_t* Gunman_Ready2;      // DONE
extern const wchar_t* Gunman_Ready3;      // DONE
extern const wchar_t* Gunman_Fire[3];        // DONE

// Death
extern const wchar_t* Enemy_Die[5];       // DONE

extern const wchar_t* Ally_Brawler_Die[2];  // DONE
extern const wchar_t* Ally_Slasher_Die[1];  // DONE
extern const wchar_t* Ally_Gunman_Die[2];   // DONE

extern const wchar_t* Civilian_Die[1];      // DONE

// Snipping
extern const wchar_t* Snipping_Selection;
extern const wchar_t* Snipping_Shot;
extern const wchar_t* Snipping_Voice;

extern const wchar_t* AudioDrama_Stage_01;
extern const wchar_t* AudioDrama_Stage_04;
extern const wchar_t* AudioDrama_Stage_07;
extern const wchar_t* AudioDrama_BadEnding;
extern const wchar_t* AudioDrama_GoodEnding;

};

template <typename T, std::size_t N>
constexpr T GetAny(std::span<T, N> sp)
{
  std::size_t n = Random::Get(0ULL, N - 1);
  return sp[n];
}

template <typename T, std::size_t N>
T GetAny(T (&arr)[N])
{
  return GetAny(std::span{arr});
}

void RegisterSoundList();
