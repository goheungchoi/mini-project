#include "TimeSystem.h"
#include <Windows.h>

TimeSystem::TimeSystem()
    : _secondsPerCount{0}, _deltaTime{-1.0}, _baseTime{0}, _pausedTime{0},
      _stopTime{0}, _prevTime{0}, _currTime{0}, _timeScale{1.0f},
      _isStopped{false}
{
  __int64 countsPerSec;
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  _secondsPerCount = 1.0 / (double)countsPerSec;

  if (m_Instance == nullptr)
  {
    m_Instance = this;
  }
}

void TimeSystem::Reset()
{
  int64_t currTime;

  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

  _baseTime = currTime;
  _prevTime = currTime;
  _stopTime = 0;
  _timeScale = 1.0f;
  _isStopped = false;
}

void TimeSystem::Update()
{
  if (_isStopped)
  {
    _deltaTime = 0.0;
    return;
  }

  int64_t currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  _currTime = currTime;

  _deltaTime = (currTime - _prevTime) * _secondsPerCount;

  _prevTime = _currTime;

  if (_deltaTime < 0.0)
  {
    _deltaTime = 0.0;
  }
}

void TimeSystem::Start()
{
  int64_t startTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

  if (_isStopped)
  {
    _pausedTime += (startTime - _stopTime);

    _prevTime = startTime;
    _stopTime = 0;
    _isStopped = false;
  }
}

void TimeSystem::Stop()
{
  if (!_isStopped)
  {
    __int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

    _stopTime = currTime;
    _isStopped = true;
  }
}

float TimeSystem::TotalTime() const
{
  if (_isStopped)
  {
    return (float)(((_stopTime - _pausedTime) - _baseTime) * _secondsPerCount);
  }
  else
  {
    return (float)(((_currTime - _pausedTime) - _baseTime) * _secondsPerCount);
  }
}
