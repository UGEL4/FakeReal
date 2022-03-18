#pragma once

#include "KeyCode.h"
#include "MouseCode.h"
#include "../CoreMarco.h"
namespace FakeReal
{
	class FR_ENGINE_API Input
	{
	public:
		Input();
		virtual ~Input();
		bool IsKeyPressed(KeyCode key);
		bool IsKeyReleased(KeyCode key);
		bool IsMouseButtonPressed(MouseCode button);
		bool IsMouseButtonReleased(MouseCode button);
		float GetMouseX();
		float GetMouseY();
		void Update();

		static Input* ms_pInstance;
		static void CreateInput();
	public:
		virtual bool IsKeyPressedImp(KeyCode key) = 0;
		virtual bool IsKeyReleasedImp(KeyCode key) = 0;
		virtual bool IsMouseButtonPressedImp(MouseCode button) = 0;
		virtual bool IsMouseButtonReleasedImp(MouseCode button) = 0;
		virtual float GetMouseXImp() = 0;
		virtual float GetMouseYImp() = 0;
		virtual void UpdateKeyboard() = 0;
		virtual void UpdateMouse() = 0;

	protected:
		wchar_t m_Keys[256]; //current frame
		wchar_t m_OldKeys[256];//last frame
		wchar_t m_bPressed[8];//mouse pressed
		wchar_t m_bReleased[8];//mouse released
	};
}