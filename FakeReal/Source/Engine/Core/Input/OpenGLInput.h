#pragma once

#include "Input.h"
#include "../CoreMarco.h"
namespace FakeReal {
	class FR_ENGINE_API OpenGLInput : public Input
	{
	public:
		OpenGLInput();
		~OpenGLInput() = default;

		virtual bool IsKeyPressedImp(KeyCode key) override;
		virtual bool IsKeyReleasedImp(KeyCode key) override;
		virtual bool IsMouseButtonPressedImp(MouseCode button) override;
		virtual bool IsMouseButtonReleasedImp(MouseCode button) override;
		virtual float GetMouseXImp() override;
		virtual float GetMouseYImp() override;
		virtual void UpdateKeyboard() override;
		virtual void UpdateMouse() override;

	private:
		static uint16_t ms_KeyToGlfwKey[KC_MAX];
		static uint16_t ms_MoustBtnToGlfw[MC_MAX];
	};
}