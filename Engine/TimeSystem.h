#pragma once
#include <stdint.h>

class TimeSystem
{
public:
  TimeSystem();
  ~TimeSystem() = default;

public:
  void Reset(); // (== Init)
  void Update();

  // 일시정지
  void Start(); // 일시 정지 해제 시 호출
  void Stop();  // 일시 정지 시 호출

  void SetTimeScale(float timeScale) { _timeScale = timeScale; }
  float GetTimeScale() const { return _timeScale; }

  // 프레임 간 경과 시간(초 단위)
  float GetDeltaTime() const { return (float)_deltaTime; }
  float GetScaledDeltaTime() const
  {
    return static_cast<float>(_deltaTime * _timeScale);
  }

  /*
  Reset()이 호출된 이후의 경과 시간을 반환합니다.
  단, 시계가 멈춘 동안의 시간은 포함되지 않습니다.
  */
  float TotalTime() const;

public:
  static TimeSystem* m_Instance;

private:
  double _secondsPerCount;      // 카운터당 초 단위 변환 비율
  double _deltaTime;            // 이전 프레임에서 현재 프레임까지의 시간 차이

  int64_t _baseTime;            // 타이머 시작 시간
  int64_t _pausedTime;          // 멈춘 동안 누적된 시간
  int64_t _stopTime;            // 멈춘 시간
  int64_t _prevTime;            // 이전 프레임 시간
  int64_t _currTime;            // 현재 프레임 시간

  float _timeScale;             // 타이머의 속도
  bool _isStopped;              // 타이머 멈춤 상태 플래그
};
