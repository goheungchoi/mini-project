#include "TimeSystem.h"
#include <windows.h>

double TimeSystem::_secondsPerCount = 0;
double TimeSystem::_deltaTime = -1.0;
int64_t TimeSystem::_baseTime = 0;
int64_t TimeSystem::_pausedTime = 0;
int64_t TimeSystem::_stopTime = 0;
int64_t TimeSystem::_prevTime = 0;
int64_t TimeSystem::_currTime = 0;
float TimeSystem::_timeScale = 1.0f;
bool TimeSystem::_isStopped = false;

void TimeSystem::Initialize()
{
  __int64 countsPerSec;
  QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
  _secondsPerCount = 1.0 / (double)countsPerSec;
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

float TimeSystem::TotalTime()
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