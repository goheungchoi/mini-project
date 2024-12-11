#pragma once
#include <stdint.h>

class TimeSystem
{
private:
  TimeSystem() = delete;
  ~TimeSystem() = delete;

public:
  static void Initialize();
  static void Reset();
  static void Update();

  // 일시정지
  static void Start(); // 일시 정지 해제 시 호출
  static void Stop();  // 일시 정지 시 호출

  static void SetTimeScale(float timeScale) { _timeScale = timeScale; }
  static float GetTimeScale() { return _timeScale; }

  // 프레임 간 경간 시간(초 단위)
  static float GetDeltaTime() { return (float)_deltaTime; }
  static float GetScaledDeltaTime()
  {
    return static_cast<float>(_deltaTime * _timeScale);
  }

  /*
  Reset()이 호출된 이후의 경간 시간을 반환합니다.
  단, 시계가 몰지된 동안의 시간은 포함되지 않습니다.
  */
  static float TotalTime();

private:
  static double _secondsPerCount; // 카우터당 초 단위 변화 비율
  static double _deltaTime; // 이전 프레임에서 현재 프레임까지의 시간 차이

  static int64_t _baseTime;   // 타이머 시작 시간
  static int64_t _pausedTime; // 몰지된 동안 누적된 시간
  static int64_t _stopTime;   // 몰지된 시간
  static int64_t _prevTime;   // 이전 프레임 시간
  static int64_t _currTime;   // 현재 프레임 시간

  static float _timeScale; // 타이머의 속도
  static bool _isStopped;  // 타이머 몰지 상황 플래그
};
