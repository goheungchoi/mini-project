#include "Core/Input/InputSystem.h"

InputSystem* InputSystem::m_pInstance = nullptr;

InputSystem* InputSystem::GetInstance()
{
  if (m_pInstance == nullptr)
  {
    m_pInstance = new InputSystem();
  }

  return m_pInstance;
}

bool InputSystem::Initialize(HWND hWnd)
{
  _hWnd = hWnd;

  // DirectXTK�� Keyboard�� Mouse ��ü ����
  _keyboard = std::make_unique<DirectX::Keyboard>();
  _mouse = std::make_unique<DirectX::Mouse>();
  _mouse->SetWindow(hWnd);

  return true;
}

void InputSystem::Finalize()
{
  // Ű���� �� ���콺 ������ ����
  _keyboard.reset();
  _mouse.reset();

  // ���� ������ �ʱ�ȭ
  _keyboardTracker.Reset();
  _mouseTracker.Reset();

  // ������ �ڵ� �ʱ�ȭ
  _hWnd = nullptr;

  // ��Ÿ �÷��� �ʱ�ȭ
  _handleOut = false;

  if (m_pInstance)
  {
    delete m_pInstance;
    m_pInstance = nullptr;
  }
}

void InputSystem::Update(const float& dt)
{
  _prevMouseState = _mouseState;

  // ���� �����찡 ��Ŀ���� �Ҿ����� Ȯ��
  if (GetForegroundWindow() != _hWnd)
  {
    _handleOut = true;
    return;
  }

  // ��Ŀ���� �ٽ� ����� �� ���콺 �� �� �ʱ�ȭ
  if (_handleOut)
  {
    _mouse->ResetScrollWheelValue();
    _handleOut = false;
  }

  // Ű���� ���� ������Ʈ
  _keyboardState = _keyboard->GetState();
  _keyboardTracker.Update(_keyboardState);

  // ���콺 ���� ������Ʈ
  _mouseState = _mouse->GetState();
  _mouseTracker.Update(_mouseState);
}

bool InputSystem::IsKeyPress(DirectX::Keyboard::Keys key) const
{
  // Ű���� Ű�� ���ȴ��� Ȯ��
  return _keyboardState.IsKeyDown(key);
}

bool InputSystem::IsKeyPress(Input::MouseState mouseState) const
{
  // ���콺 ��ư�� ���ȴ��� Ȯ��
  switch (mouseState)
  {
  case Input::LB:
    return _mouseTracker.leftButton == DirectX::Mouse::ButtonStateTracker::HELD;
  case Input::RB:
    return _mouseTracker.rightButton ==
           DirectX::Mouse::ButtonStateTracker::HELD;
  case Input::WM:
    return _mouseTracker.middleButton ==
           DirectX::Mouse::ButtonStateTracker::HELD;
  default:
    return false;
  }
}

bool InputSystem::IsKeyDown(DirectX::Keyboard::Keys key) const
{
  // Ű���� Ű�� �̹� �����ӿ��� ���ȴ��� Ȯ��
  return _keyboardTracker.IsKeyPressed(key);
}

bool InputSystem::IsKeyDown(Input::MouseState mouseState) const
{
  // ���콺 ��ư�� �̹� �����ӿ��� ���ȴ��� Ȯ��
  switch (mouseState)
  {
  case Input::LB:
    return _mouseTracker.leftButton ==
           DirectX::Mouse::ButtonStateTracker::PRESSED;
  case Input::RB:
    return _mouseTracker.rightButton ==
           DirectX::Mouse::ButtonStateTracker::PRESSED;
  case Input::WM:
    return _mouseTracker.middleButton ==
           DirectX::Mouse::ButtonStateTracker::PRESSED;
  default:
    return false;
  }
}

bool InputSystem::IsKeyUp(DirectX::Keyboard::Keys key) const
{
  return _keyboardTracker.IsKeyReleased(key);
}

bool InputSystem::IsKeyUp(Input::MouseState mouseState) const
{
  switch (mouseState)
  {
  case Input::LB:
    return _mouseTracker.leftButton ==
           DirectX::Mouse::ButtonStateTracker::RELEASED;
  case Input::RB:
    return _mouseTracker.rightButton ==
           DirectX::Mouse::ButtonStateTracker::RELEASED;
  case Input::WM:
    return _mouseTracker.middleButton ==
           DirectX::Mouse::ButtonStateTracker::RELEASED;
  default:
    return false;
  }
}

bool InputSystem::IsMouseWheel(Input::MouseState mouseState) const
{
  switch (mouseState)
  {
  case Input::WHDN:
    if (_prevMouseState.scrollWheelValue > _mouseState.scrollWheelValue)
      return true; // �� �ٿ�
    break;

  case Input::WHUP:
    if (_prevMouseState.scrollWheelValue < _mouseState.scrollWheelValue)
      return true; // �� ��
    break;
  }

  return false;
}

Vector2 InputSystem::GetMouseDelta() const
{
  Vector2 delta;

  delta.x =
      static_cast<float>(_prevMouseState.x) - static_cast<float>(_mouseState.x);
  delta.y =
      static_cast<float>(_prevMouseState.y) - static_cast<float>(_mouseState.y);

  return delta;
}