#pragma once

#include "Core/Common.h"

#include <random>

class Random
{
  std::random_device rd; // a seed source for the random number engine
  std::mt19937 gen;      // mersenne_twister_engine seeded with rd()

  static std::unique_ptr<Random> _instance;

public:
  Random() : gen(rd()) {}
  Random(const Random&) = delete;
  Random(Random&&) noexcept = delete;
  Random& operator=(const Random&) = delete;
  Random& operator=(Random&&) noexcept = delete;
  ~Random() {}

  /**
   * @brief 0 ���� `to`�� ������ ���� �� �ϳ��� ��ȯ�մϴ�.
   *
   * ����1, int ran = Get<int>(5); // 1���� 5������ ������ �� �ϳ�.
   * ����2, double ran = Get<double>(7); // 1���� 7������ �ε� �Ҽ��� ���� ��
   * �ϳ�.
   *
   * @tparam _T ��ȯ�ϰ��� �ϴ� ���� ���� Ÿ��
   * @param to 0 ���� `to`������ ���� ���� ����
   * @return _T Ÿ���� ���� ��
   */
  template <typename _T>
  static _T Get(_T to)
  {
    if (!_instance)
      throw std::runtime_error("Access of uninitialized Random detected!");

    return _instance->operator()<_T>(to);
  }

  /**
   * @brief `from` �� `to`�� ������ ������ ���� �� �ϳ��� �������� ��ȯ�մϴ�.
   *
   * ����1, int ran = Get<int>(-5, 5); // -5���� 5������ ������ �� �ϳ�.
   * ����2, double ran = Get<double>(10, 20); // 10���� 20������ �ε� �Ҽ���
   * ���� �� �ϳ�.
   *
   * @tparam _T ��ȯ�ϰ��� �ϴ� ���� ���� Ÿ��
   * @param from ���� �� ������ ����
   * @param to ���� �� ������ ��
   * @return _T Ÿ���� ���� ��
   */
  template <typename _T>
  static _T Get(_T from, _T to)
  {
    if (!_instance)
      throw std::runtime_error("Access of uninitialized Random detected!");

    return _instance->operator()<_T>(from, to);
  }

  template <typename _T, typename InputIt>
  static _T Get(InputIt first, InputIt last)
  {
    if (!_instance)
      throw std::runtime_error("Access of uninitialized Random detected!");

    return _instance->operator()<_T, InputIt>(first, last);
  }

  static std::mt19937 Engine()
  {
    if (!_instance)
      throw std::runtime_error("Access of uninitialized Random detected!");

    return _instance->gen;
  }

private:
  template <typename Integer>
  std::enable_if_t<std::is_integral<Integer>::value, Integer> operator()(
      Integer to)
  {
    std::uniform_int_distribution<Integer> uniform_dist(0, to);
    return uniform_dist(gen);
  }

  template <typename Float>
  std::enable_if_t<std::is_floating_point<Float>::value, Float> operator()(
      Float to)
  {
    std::uniform_real_distribution<Float> uniform_dist(0.0, to);
    return uniform_dist(gen);
  }

  template <typename Integer>
  std::enable_if_t<std::is_integral<Integer>::value, Integer> operator()(
      Integer from, Integer to)
  {
    std::uniform_int_distribution<Integer> uniform_dist(from, to);
    return uniform_dist(gen);
  }

  template <typename Float>
  std::enable_if_t<std::is_floating_point<Float>::value, Float> operator()(
      Float from, Float to)
  {
    std::uniform_real_distribution<Float> uniform_dist(from, to);
    return uniform_dist(gen);
  }

  template <typename Integer, typename InputIt>
  std::enable_if_t<std::is_integral<Integer>::value, Integer> operator()(
      InputIt first, InputIt last)
  {
    std::discrete_distribution<Integer> discrete_dist(first, last);
    return discrete_dist(gen);
  }

  template <typename Float, typename InputIt>
  std::enable_if_t<std::is_floating_point<Float>::value, Float> operator()(
      InputIt first, InputIt last)
  {
    std::discrete_distribution<Float> discrete_dist(first, last);
    return discrete_dist(gen);
  }

  // public:
  //	static Random& Create() {
  //		if (_instance)
  //			throw std::runtime_error("Recreation of Random detected!");
  //
  //		_instance.reset(new Random());
  //		return *_instance;
  //	}
  //	static void Destory() {
  //		if (!_instance)
  //			throw std::runtime_error("Destruction of uninitialized Random
  //detected!");
  //
  //		_instance.release();
  //	}
};
