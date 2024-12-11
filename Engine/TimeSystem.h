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

  // �Ͻ�����
  static void Start(); // �Ͻ� ���� ���� �� ȣ��
  static void Stop();  // �Ͻ� ���� �� ȣ��

  static void SetTimeScale(float timeScale) { _timeScale = timeScale; }
  static float GetTimeScale() { return _timeScale; }

  // ������ �� �氣 �ð�(�� ����)
  static float GetDeltaTime() { return (float)_deltaTime; }
  static float GetScaledDeltaTime()
  {
    return static_cast<float>(_deltaTime * _timeScale);
  }

  /*
  Reset()�� ȣ��� ������ �氣 �ð��� ��ȯ�մϴ�.
  ��, �ð谡 ������ ������ �ð��� ���Ե��� �ʽ��ϴ�.
  */
  static float TotalTime();

private:
  static double _secondsPerCount; // ī���ʹ� �� ���� ��ȭ ����
  static double _deltaTime; // ���� �����ӿ��� ���� �����ӱ����� �ð� ����

  static int64_t _baseTime;   // Ÿ�̸� ���� �ð�
  static int64_t _pausedTime; // ������ ���� ������ �ð�
  static int64_t _stopTime;   // ������ �ð�
  static int64_t _prevTime;   // ���� ������ �ð�
  static int64_t _currTime;   // ���� ������ �ð�

  static float _timeScale; // Ÿ�̸��� �ӵ�
  static bool _isStopped;  // Ÿ�̸� ���� ��Ȳ �÷���
};
