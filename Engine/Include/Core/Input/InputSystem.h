#pragma once
#include "Core/Common.h"
#include "Core/Math/MathUtils.h"

#include <directXTK/Keyboard.h>
#include <directXTK/Mouse.h>
#pragma comment(lib, "DirectXTK.lib")

using Key = DirectX::Keyboard::Keys;

// 마우스 상태를 나타내는 열거형
enum MouseState
{
	LB,
	RB,
	WM,
	END,
	WHUP,
	WHDN
};


class InputSystem
{

private:
  // 생성자 및 소멸자
  InputSystem() = default;
  ~InputSystem() = default;

  InputSystem(const InputSystem&) = delete;
  InputSystem& operator=(const InputSystem&) = delete;
  InputSystem(InputSystem&&) noexcept = delete;
  InputSystem& operator=(InputSystem&&) noexcept = delete;

  static InputSystem* m_pInstance;

public:
  static InputSystem* GetInstance();

  // 업데이트 함수 (매 프레임 입력 상태 갱신)
  bool Initialize(HWND hWnd);
  void Finalize();
  void Update(const float& dt);

  // 키, 마우스, 패드 상태 확인 함수
  bool IsKeyPress(DirectX::Keyboard::Keys key) const;
  bool IsKeyPress(MouseState mouseState) const;

  bool IsKeyDown(DirectX::Keyboard::Keys key) const;
  bool IsKeyDown(MouseState mouseState) const;

  bool IsKeyUp(DirectX::Keyboard::Keys key) const;
  bool IsKeyUp(MouseState mouseState) const;

  // 마우스 휠 확인 함수
  bool IsMouseWheel(MouseState mouseState) const;

  // 마우스 이동 값 가져오기
  Vector2 GetMouseDelta() const;

  DirectX::Mouse::State GetCurrMouseState() { return _mouseState; }
  DirectX::Mouse::State GetPrevMouseState() { return _prevMouseState; }

private:
  // DirectXTK 키보드 관련 멤버 변수
  std::unique_ptr<DirectX::Keyboard> _keyboard{};
  DirectX::Keyboard::KeyboardStateTracker _keyboardTracker{}; // 변화 감지
  DirectX::Keyboard::State _keyboardState{}; // 현재 상태 확인

  // DirectXTK 마우스 관련 멤버 변수
  std::unique_ptr<DirectX::Mouse> _mouse{};
  DirectX::Mouse::ButtonStateTracker _mouseTracker{};
  DirectX::Mouse::State _prevMouseState{};
  DirectX::Mouse::State _mouseState{};

  HWND _hWnd{};
  bool _handleOut = false;
};

#define Input (*InputSystem::GetInstance())
