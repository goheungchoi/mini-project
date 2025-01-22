#pragma once
#include "Core/Common.h"
#include "Core/Math/MathUtils.h"

#include <directXTK/Keyboard.h>
#include <directXTK/Mouse.h>
#pragma comment(lib, "DirectXTK.lib")

using Key = DirectX::Keyboard::Keys;

// ���콺 ���¸� ��Ÿ���� ������
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
  // ������ �� �Ҹ���
  InputSystem() = default;
  ~InputSystem() = default;

  InputSystem(const InputSystem&) = delete;
  InputSystem& operator=(const InputSystem&) = delete;
  InputSystem(InputSystem&&) noexcept = delete;
  InputSystem& operator=(InputSystem&&) noexcept = delete;

  static InputSystem* m_pInstance;

public:
  static InputSystem* GetInstance();

  // ������Ʈ �Լ� (�� ������ �Է� ���� ����)
  bool Initialize(HWND hWnd);
  void Finalize();
  void Update(const float& dt);

  // Ű, ���콺, �е� ���� Ȯ�� �Լ�
  bool IsKeyPress(DirectX::Keyboard::Keys key) const;
  bool IsKeyPress(MouseState mouseState) const;

  bool IsKeyDown(DirectX::Keyboard::Keys key) const;
  bool IsKeyDown(MouseState mouseState) const;

  bool IsKeyUp(DirectX::Keyboard::Keys key) const;
  bool IsKeyUp(MouseState mouseState) const;

  // ���콺 �� Ȯ�� �Լ�
  bool IsMouseWheel(MouseState mouseState) const;

  // ���콺 �̵� �� ��������
  Vector2 GetMouseDelta() const;

  DirectX::Mouse::State GetCurrMouseState() { return _mouseState; }
  DirectX::Mouse::State GetPrevMouseState() { return _prevMouseState; }

private:
  // DirectXTK Ű���� ���� ��� ����
  std::unique_ptr<DirectX::Keyboard> _keyboard{};
  DirectX::Keyboard::KeyboardStateTracker _keyboardTracker{}; // ��ȭ ����
  DirectX::Keyboard::State _keyboardState{}; // ���� ���� Ȯ��

  // DirectXTK ���콺 ���� ��� ����
  std::unique_ptr<DirectX::Mouse> _mouse{};
  DirectX::Mouse::ButtonStateTracker _mouseTracker{};
  DirectX::Mouse::State _prevMouseState{};
  DirectX::Mouse::State _mouseState{};

  HWND _hWnd{};
  bool _handleOut = false;
};

#define Input (*InputSystem::GetInstance())
