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

  // DirectXTK의 Keyboard와 Mouse 객체 생성
  _keyboard = std::make_unique<DirectX::Keyboard>();
  _mouse = std::make_unique<DirectX::Mouse>();
  _mouse->SetWindow(hWnd);

  return true;
}

void InputSystem::Finalize()
{
  // 키보드 및 마우스 포인터 해제
  _keyboard.reset();
  _mouse.reset();

  // 상태 추적기 초기화
  _keyboardTracker.Reset();
  _mouseTracker.Reset();

  // 윈도우 핸들 초기화
  _hWnd = nullptr;

  // 기타 플래그 초기화
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

  // 현재 윈도우가 포커스를 잃었는지 확인
  if (GetForegroundWindow() != _hWnd)
  {
    _handleOut = true;
    return;
  }

  // 포커스를 다시 얻었을 때 마우스 휠 값 초기화
  if (_handleOut)
  {
    _mouse->ResetScrollWheelValue();
    _handleOut = false;
  }

  // 키보드 상태 업데이트
  _keyboardState = _keyboard->GetState();
  _keyboardTracker.Update(_keyboardState);

  // 마우스 상태 업데이트
  _mouseState = _mouse->GetState();
  _mouseTracker.Update(_mouseState);
}

bool InputSystem::IsKeyPress(DirectX::Keyboard::Keys key) const
{
  // 키보드 키가 눌렸는지 확인
  return _keyboardState.IsKeyDown(key);
}

bool InputSystem::IsKeyPress(MouseState mouseState) const
{
  // 마우스 버튼이 눌렸는지 확인
  switch (mouseState)
  {
  case LB:
    return _mouseTracker.leftButton == DirectX::Mouse::ButtonStateTracker::HELD;
  case RB:
    return _mouseTracker.rightButton ==
           DirectX::Mouse::ButtonStateTracker::HELD;
  case WM:
    return _mouseTracker.middleButton ==
           DirectX::Mouse::ButtonStateTracker::HELD;
  default:
    return false;
  }
}

bool InputSystem::IsKeyDown(DirectX::Keyboard::Keys key) const
{
  // 키보드 키가 이번 프레임에서 눌렸는지 확인
  return _keyboardTracker.IsKeyPressed(key);
}

bool InputSystem::IsKeyDown(MouseState mouseState) const
{
  // 마우스 버튼이 이번 프레임에서 눌렸는지 확인
  switch (mouseState)
  {
  case LB:
    return _mouseTracker.leftButton ==
           DirectX::Mouse::ButtonStateTracker::PRESSED;
  case RB:
    return _mouseTracker.rightButton ==
           DirectX::Mouse::ButtonStateTracker::PRESSED;
  case WM:
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

bool InputSystem::IsKeyUp(MouseState mouseState) const
{
  switch (mouseState)
  {
  case LB:
    return _mouseTracker.leftButton ==
           DirectX::Mouse::ButtonStateTracker::RELEASED;
  case RB:
    return _mouseTracker.rightButton ==
           DirectX::Mouse::ButtonStateTracker::RELEASED;
  case WM:
    return _mouseTracker.middleButton ==
           DirectX::Mouse::ButtonStateTracker::RELEASED;
  default:
    return false;
  }
}

bool InputSystem::IsMouseWheel(MouseState mouseState) const
{
  switch (mouseState)
  {
  case WHDN:
    if (_prevMouseState.scrollWheelValue > _mouseState.scrollWheelValue)
      return true; // 휠 다운
    break;

  case WHUP:
    if (_prevMouseState.scrollWheelValue < _mouseState.scrollWheelValue)
      return true; // 휠 업
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

Vector2 InputSystem::GetCurrMouseStateVec()
{
  POINT cursorPos;
  GetCursorPos(&cursorPos);
  ScreenToClient(_hWnd, &cursorPos);

  return Vector2(static_cast<float>(cursorPos.x),
                 static_cast<float>(cursorPos.y));
}
