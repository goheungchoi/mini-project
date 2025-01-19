#pragma once

#include "GameFramework/Common.h"

#include "Core/Math/MathUtils.h"

class ColliderComponent;
struct HitResult
{
  bool bBlockingHit : 1; // ���ŷ �浹�� ������� ǥ��
  bool bStartPenetrating : 1; // �浹�� ��� ���� �Ǿ�����
  ColliderComponent* hitComponent; // �浹 ���� ������Ʈ

  XMVECTOR impactNormal; // �浹���� ������Ʈ�� ��Ʈ ���������� �븻 ����
  XMVECTOR impactPoint;   // Ʈ���̽� ������ ��Ʈ ����

  float distance; // Ʈ���̽� ���������� �����̼Ǳ����� �Ÿ�
  XMVECTOR location; // ������ ������Ʈ�� �浹�� ��ġ

  XMVECTOR normal; // �浹���� ������Ʈ�� �߽������� ������ ������Ʈ
                   // �߽���������
                 // ����
  float penetrationDepth; // ���� ������ ����� ���͸� ���� �� �ִ� ���, �븻
                          // ���͸� ���� �浹���� ����� ���� �Ÿ�.

  float time; // (0.0 ���� 1.0) �浹�� ���� ��� Ʈ���̽� �������� Ʈ���̽�
              // ������ ������ ��
  XMVECTOR traceStart; // Ʈ���̽��� ������
  XMVECTOR traceEnd;   // Ʈ���̽��� ���� �� ������

  class GameObject* GetHitGameObject();

  ColliderComponent* GetHitComponent() { return hitComponent; }
};

