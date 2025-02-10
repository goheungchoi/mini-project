#include "SoundList.h"

#include "SoundSystem/SoundManager.h"

const wchar_t* SoundList::Background_Title = L"TitleBGM.wav";
const wchar_t* SoundList::Background_Pub = L"PubBGM_01.wav";
const wchar_t* SoundList::Background_Pub_Ambient = L"PubAmbient.WAV";
const wchar_t* SoundList::Background_Storage = L"StorageBGM.wav";
const wchar_t* SoundList::Background_Storage_Ambient = L"StorageAmbient.wav";
const wchar_t* SoundList::Background_ConferenceHall = L"ConferenceHallBGM_01.wav";
const wchar_t* SoundList::Background_ConferenceHall_Ambient = L"ConferenceHallAmbient.wav";


const wchar_t* SoundList::Button_Click = L"ButtonClickSound_01.wav";
const wchar_t* SoundList::Button_Hover = L"ButtonHoverSound_01.wav";

const wchar_t* SoundList::Button_Start = L"StartSFX.wav";
const wchar_t* SoundList::Button_Retry = L""; // TODO:

const wchar_t* SoundList::Grid_Placement_Click = L"C_PlacementSound.wav";
const wchar_t* SoundList::Grid_Placement_Hover = L"C_PlacementHoverSound.wav";
const wchar_t* SoundList::Placeholder_Rotation = L"C_RotationSound.wav";

const wchar_t* SoundList::Character_Hover = L"CharacterHoverSound.wav";

const wchar_t* SoundList::Brawler_Ready = L"Punch_ATK_001.wav";
const wchar_t* SoundList::Brawler_Fire = L"Punch_ATK_002 (Speedy).wav";

const wchar_t* SoundList::Slasher_Ready1 = L"Rush_ATK_001.wav";
const wchar_t* SoundList::Slasher_Ready2 = L"Rush_ATK_002.wav";
const wchar_t* SoundList::Slasher_Rush = L"Rush_ATK_002.5 (Footstep).mp3";
const wchar_t* SoundList::Slasher_Fire = L"Rush_ATK_003.wav";

const wchar_t* SoundList::Gunman_Ready1 = L"Gun_ATK_001.wav";
const wchar_t* SoundList::Gunman_Ready2 = L"Gun_ATK_002.wav";
const wchar_t* SoundList::Gunman_Ready3 = L"Gun_ATK_003.wav";
const wchar_t* SoundList::Gunman_Fire = L"Gun_ATK_004.wav";


const wchar_t* SoundList::Enemy_Die[5] = {
    L"Enemy_HitSound_01.wav", L"Enemy_HitSound_02.wav",
    L"Enemy_HitSound_03.wav", L"Enemy_HitSound_04.wav",
    L"Enemy_HitSound_05.wav"};

const wchar_t* SoundList::Ally_Brawler_Die[2] = {L"Punch_HitSound_01.wav", L"Punch_HitSound_02.wav"};
const wchar_t* SoundList::Ally_Slasher_Die[1] = {L"Rush_HitSound_01.wav"};
const wchar_t* SoundList::Ally_Gunman_Die[2] = {L"Gun_HitSound_01.wav",
                                                L"Gun_HitSound_02.wav"};

const wchar_t* SoundList::Civilian_Die[1] = {L"Civil_HitSound (Long).wav"};

const wchar_t* SoundList::Snipping_Selection = L"SnipingSelect.wav";
const wchar_t* SoundList::Snipping_Shot = L"SnipingShot.wav";
const wchar_t* SoundList::Snipping_Voice = L"SnipingVoice.wav";

template <typename T, std::size_t N>
void RegisterSoundArray(std::span<T, N> sp)
{
  for (T sound : sp)
  {
    SoundManager::LoadSound(sound);
  }
}

template <typename T, std::size_t N>
void RegisterSoundArray(T(&arr)[N])
{
  RegisterSoundArray(std::span{arr});
}

void RegisterSound(const wchar_t* sound)
{
  SoundManager::LoadSound(sound);
}

void RegisterSoundList() {
  
  using namespace SoundList;

  RegisterSound(Background_Title);
  RegisterSound(Background_Pub);
  RegisterSound(Background_Pub_Ambient);
  RegisterSound(Background_Storage);
  RegisterSound(Background_Storage_Ambient);
  RegisterSound(Background_ConferenceHall);
  RegisterSound(Background_ConferenceHall_Ambient);

  RegisterSound(Button_Click);
  RegisterSound(Button_Hover);

  RegisterSound(Button_Start);
  // RegisterSound(Button_Retry);

  RegisterSound(Grid_Placement_Click);
  RegisterSound(Grid_Placement_Hover);
  RegisterSound(Placeholder_Rotation);

  RegisterSound(Character_Hover);

  RegisterSound(Brawler_Ready);
  RegisterSound(Brawler_Fire);

  RegisterSound(Slasher_Ready1);
  RegisterSound(Slasher_Ready2);
  RegisterSound(Slasher_Rush);
  RegisterSound(Slasher_Fire);

  RegisterSound(Gunman_Ready1);
  RegisterSound(Gunman_Ready2);
  RegisterSound(Gunman_Ready3);
  RegisterSound(Gunman_Fire);

  RegisterSoundArray(Enemy_Die);

  RegisterSoundArray(Ally_Brawler_Die);

  RegisterSoundArray(Ally_Slasher_Die);
  RegisterSoundArray(Ally_Gunman_Die);
  RegisterSoundArray(Civilian_Die);

  RegisterSound(Snipping_Selection);
  RegisterSound(Snipping_Shot);
  RegisterSound(Snipping_Voice);

}
