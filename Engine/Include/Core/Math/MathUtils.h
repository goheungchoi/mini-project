#pragma once

#include <directxtk/simplemath.h>							// ���� ������ �ܼ�ȭ�ϴ� Ŭ�������� ����
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
