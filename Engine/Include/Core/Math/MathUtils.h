#pragma once

#include <directxtk/simplemath.h>							// 수학 연산을 단순화하는 클래스들을 제공
using namespace DirectX;
using namespace DirectX::SimpleMath;

#include <cmath>
#include <numbers>

namespace MathUtils
{
struct AABB
{
  XMVECTOR min;
  XMVECTOR max;
};

constexpr float kQuaterPi_f{0.25 * std::numbers::pi};
constexpr float kHalfPi_f{0.5 * std ::numbers::pi};
constexpr float kPi_f{std::numbers::pi_v<float>};
constexpr float kTwoPi_f{2.0 * std::numbers::pi};

constexpr double kQuaterPi{0.25 * std::numbers::pi};
constexpr double kHalfPi{std::numbers::pi * 0.5};
constexpr double kPi{std::numbers::pi};
constexpr double kTwoPi{2.0 * std::numbers::pi};

constexpr float kInvPi_f{std::numbers::inv_pi_v<float>};
constexpr double kInvPi{std::numbers::inv_pi};

constexpr float kE_f{std::numbers::e_v<float>};
constexpr double kE{std::numbers::e};

constexpr float kSqrt2_f{std::numbers::sqrt2_v<float>};
constexpr double kSqrt2{std::numbers::sqrt2};

}
