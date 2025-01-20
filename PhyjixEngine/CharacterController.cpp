#include "pch.h"
#include "CharacterController.h"
CharacterController::CharacterController()
    : m_controller(nullptr), m_manager(nullptr), m_lastCollisionFlags(physx::PxControllerCollisionFlag::eCOLLISION_SIDES)
{
}

CharacterController::~CharacterController()
{
    Release();
}

void CharacterController::Initialize(physx::PxControllerManager* manager, const physx::PxCapsuleControllerDesc& desc)
{
    m_manager = manager;
    m_controller = m_manager->createController(desc);
    m_MoveDirection = PxExtendedVec3(0, 0, 0);
}

void CharacterController::Release()
{
    if (m_controller)
    {
        m_controller->release();
        m_controller = nullptr;
    }
}

physx::PxControllerCollisionFlags CharacterController::Move(const physx::PxVec3& displacement, float minDist, float elapsedTime)
{
    m_lastCollisionFlags = m_controller->move(displacement, minDist, elapsedTime, physx::PxControllerFilters());
    return m_lastCollisionFlags;
}

physx::PxExtendedVec3 CharacterController::GetPosition() const
{
    return m_controller->getPosition();
}

void CharacterController::SetMoveDirection(const physx::PxExtendedVec3& position)
{
    m_MoveDirection = position;
}

void CharacterController::SetPosition(const physx::PxExtendedVec3& position)
{
    m_controller->setPosition(position);
}

float CharacterController::GetHeight() const
{
    return static_cast<physx::PxCapsuleController*>(m_controller)->getHeight();
}

void CharacterController::SetHeight(float height)
{
    static_cast<physx::PxCapsuleController*>(m_controller)->setHeight(height);
}

float CharacterController::GetRadius() const
{
    return static_cast<physx::PxCapsuleController*>(m_controller)->getRadius();
}

void CharacterController::SetRadius(float radius)
{
    static_cast<physx::PxCapsuleController*>(m_controller)->setRadius(radius);
}

bool CharacterController::IsGrounded() const
{
    return (m_lastCollisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN);
}

void CharacterController::Update(float deltaTime)
{
    if (!IsGrounded())
    {
        m_MoveDirection.y -= 9.8f * deltaTime;
    }

    // 수직 이동 적용
    DirectX::XMFLOAT3 verticalMove(0.0f, m_MoveDirection.y * deltaTime, 0.0f);
    Move(  toVec3(m_MoveDirection), 0.0f, deltaTime);
    // 지면에 닿았는지 확인
    if (IsGrounded())
    {
        m_MoveDirection.y = 0.0f;
    }

}
