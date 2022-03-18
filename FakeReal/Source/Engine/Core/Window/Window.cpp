#include "Window.h"
#include "../../Platform/OpenGL/Window/OpenGLWindow.h"

namespace FakeReal {
	Window* Window::Create(const std::string_view& title, unsigned int width, unsigned int height)
	{
		return new Window_GL(title, width, height);
	}

}