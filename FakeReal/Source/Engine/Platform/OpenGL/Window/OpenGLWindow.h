#pragma once

#include "../../../Core/Window/Window.h"
#include "../../../Core/Input/KeyCode.h"
#include "../../../Core/Input/MouseCode.h"
#include "../../../Core/CoreMarco.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace FakeReal {
	class FR_ENGINE_API Window_GL : public Window
	{
	public:
		Window_GL(const std::string_view& title, unsigned int width, unsigned int height);
		~Window_GL();

		virtual unsigned int GetWidth() const override;
		virtual unsigned int GetHeight() const override;
		virtual const std::string_view& GetTitle() const override;

		virtual void Update() override;
		virtual void* GetNaviteWindow() const override;
		virtual void SetEventCallback(const EventCallbackFunc& callback) override;

		constexpr static int MAX_KEY = 349;
		static KeyCode ms_GlfwToEngineKey[MAX_KEY];
		static MouseCode ms_GlfwToEngineMouseBtn[MC_MAX];
	private:
		void InitKeyCodeAndMouseCode();

		struct WindowData
		{
			std::string_view mTitle;
			unsigned int mWidth;
			unsigned int mHeight;

			EventCallbackFunc EventCallback;
		};

		GLFWwindow* m_pWindow;
		WindowData mWindData;
	};
}