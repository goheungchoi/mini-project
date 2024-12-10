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

  // �Ͻ�����
  void Start(); // �Ͻ� ���� ���� �� ȣ��
  void Stop();  // �Ͻ� ���� �� ȣ��

  void SetTimeScale(float timeScale) { _timeScale = timeScale; }
  float GetTimeScale() const { return _timeScale; }

  // ������ �� ��� �ð�(�� ����)
  float GetDeltaTime() const { return (float)_deltaTime; }
  float GetScaledDeltaTime() const
  {
    return static_cast<float>(_deltaTime * _timeScale);
  }

  /*
  Reset()�� ȣ��� ������ ��� �ð��� ��ȯ�մϴ�.
  ��, �ð谡 ���� ������ �ð��� ���Ե��� �ʽ��ϴ�.
  */
  float TotalTime() const;

public:
  static TimeSystem* m_Instance;

private:
  double _secondsPerCount;      // ī���ʹ� �� ���� ��ȯ ����
  double _deltaTime;            // ���� �����ӿ��� ���� �����ӱ����� �ð� ����

  int64_t _baseTime;            // Ÿ�̸� ���� �ð�
  int64_t _pausedTime;          // ���� ���� ������ �ð�
  int64_t _stopTime;            // ���� �ð�
  int64_t _prevTime;            // ���� ������ �ð�
  int64_t _currTime;            // ���� ������ �ð�

  float _timeScale;             // Ÿ�̸��� �ӵ�
  bool _isStopped;              // Ÿ�̸� ���� ���� �÷���
};
