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
}
