#include "Input.h"
#include <memory>
#include "OpenGLInput.h"
namespace FakeReal {
	Input* Input::ms_pInstance = nullptr;

	void Input::CreateInput()
	{
		new OpenGLInput();
	}

	Input::Input()
	{
		memset(m_Keys, 0, sizeof(m_Keys));
		memset(m_OldKeys, 0, sizeof(m_OldKeys));
		memset(m_bPressed, 0, sizeof(m_bPressed));
		memset(m_bReleased, 0, sizeof(m_bReleased));
		ms_pInstance = this;
	}

	Input::~Input()
	{

	}

	bool Input::IsKeyPressed(KeyCode key)
	{
		return IsKeyPressedImp(key);
	}

	bool Input::IsKeyReleased(KeyCode key)
	{
		return IsKeyReleasedImp(key);
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		return IsMouseButtonPressedImp(button);
	}

	bool Input::IsMouseButtonReleased(MouseCode button)
	{
		return IsMouseButtonReleasedImp(button);
	}

	float Input::GetMouseX()
	{
		return GetMouseXImp();
	}

	float Input::GetMouseY()
	{
		return GetMouseYImp();
	}

	void Input::Update()
	{
		UpdateKeyboard();
		UpdateMouse();
	}

}