#pragma once

#include "GameFramework/Common.h"

#include "Core/Math/MathUtils.h"

class ColliderComponent;
struct HitResult
{
  bool bBlockingHit : 1; // 블락킹 충돌의 결과인지 표시
  bool bStartPenetrating : 1; // 충돌이 방금 시작 되었는지
  ColliderComponent* hitComponent; // 충돌 당한 컴포넌트

  XMVECTOR impactNormal; // 충돌당한 오브젝트의 히트 접점에서의 노말 벡터
  XMVECTOR impactPoint;   // 트레이싱 도형의 히트 접점

  float distance; // 트레이싱 시작점부터 로케이션까지의 거리
  XMVECTOR location; // 임펙팅 오브젝트의 충돌시 위치

  XMVECTOR normal; // 충돌당한 오브젝트의 중심점에서 임펙팅 오브젝트
                   // 중심점까지의
                 // 방향
  float penetrationDepth; // 만약 관통을 상쇄할 벡터를 구할 수 있는 경우, 노말
                          // 벡터를 따라서 충돌에서 벗어나기 위한 거리.

  float time; // (0.0 부터 1.0) 충돌이 있을 경우 트레이싱 시작점과 트레이싱
              // 종료점 사이의 값
  XMVECTOR traceStart; // 트레이싱의 시작점
  XMVECTOR traceEnd;   // 트레이싱의 가장 먼 종료점

  class GameObject* GetHitGameObject();

  ColliderComponent* GetHitComponent() { return hitComponent; }
};

