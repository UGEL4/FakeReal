#include "OpenGLWindow.h"
#include "../../../Core/Events/ApplicationEvent.h"
#include "../../../Core/Events/KeyEvent.h"
#include "../../../Core/Events/MouseEvent.h"
namespace FakeReal {
	KeyCode Window_GL::ms_GlfwToEngineKey[MAX_KEY];
	MouseCode Window_GL::ms_GlfwToEngineMouseBtn[MC_MAX];

	Window_GL::Window_GL(const std::string_view& title, unsigned int width, unsigned int height)
	{
		mWindData.mTitle = title;
		mWindData.mWidth = width;
		mWindData.mHeight = height;

		if (!glfwInit())
		{
			//std::cout << "Could not initialize GLFW!" << std::endl;
			return;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		m_pWindow = glfwCreateWindow(mWindData.mWidth, mWindData.mHeight, mWindData.mTitle.data(), nullptr, nullptr);

		glfwMakeContextCurrent(m_pWindow);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			//std::cout << "Failed to initialize Glad!" << std::endl;
			return;
		}

		glfwSetWindowUserPointer(m_pWindow, &mWindData);

		//set callback
		glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.mWidth = width;
			data.mHeight = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				if (key != GLFW_KEY_UNKNOWN)
				{
					KeyPressedEvent event(ms_GlfwToEngineKey[key], 0);
					data.EventCallback(event);
				}
				break;
			}
			case GLFW_RELEASE:
			{
				if (key != GLFW_KEY_UNKNOWN)
				{
					KeyReleasedEvent event(ms_GlfwToEngineKey[key]);
					data.EventCallback(event);
				}
				break;
			}
			case GLFW_REPEAT:
			{
				if (key != GLFW_KEY_UNKNOWN)
				{
					KeyPressedEvent event(ms_GlfwToEngineKey[key], 1);
					data.EventCallback(event);
				}
				break;
			}
			}
		});

		glfwSetCharCallback(m_pWindow, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			//std::cout << "keycode: " << keycode << ", char: " << (char)keycode << std::endl;
			//printf("keycode:%d, char:%c\n", keycode, (char)keycode);
			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(ms_GlfwToEngineMouseBtn[button]);
				//printf("MousePressed:%d\n", button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(ms_GlfwToEngineMouseBtn[button]);
				//printf("MouseReleased:%d\n", button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_pWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
		//set callback end
	}

	Window_GL::~Window_GL()
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	unsigned int Window_GL::GetWidth() const
	{
		return mWindData.mWidth;
	}

	unsigned int Window_GL::GetHeight() const
	{
		return mWindData.mHeight;
	}

	const std::string_view& Window_GL::GetTitle() const
	{
		return mWindData.mTitle;
	}

	void Window_GL::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);
	}

	void* Window_GL::GetNaviteWindow() const
	{
		return m_pWindow;
	}

	void Window_GL::SetEventCallback(const EventCallbackFunc& callback)
	{
		mWindData.EventCallback = callback;
	}

	void Window_GL::InitKeyCodeAndMouseCode()
	{
		//Keyboard
		ms_GlfwToEngineKey[GLFW_KEY_ESCAPE] = KC_ESC;

		ms_GlfwToEngineKey[GLFW_KEY_F1] = KC_F1;
		ms_GlfwToEngineKey[GLFW_KEY_F2] = KC_F2;
		ms_GlfwToEngineKey[GLFW_KEY_F3] = KC_F3;
		ms_GlfwToEngineKey[GLFW_KEY_F4] = KC_F4;
		ms_GlfwToEngineKey[GLFW_KEY_F5] = KC_F5;
		ms_GlfwToEngineKey[GLFW_KEY_F6] = KC_F6;
		ms_GlfwToEngineKey[GLFW_KEY_F7] = KC_F7;
		ms_GlfwToEngineKey[GLFW_KEY_F8] = KC_F8;
		ms_GlfwToEngineKey[GLFW_KEY_F9] = KC_F9;
		ms_GlfwToEngineKey[GLFW_KEY_F10] = KC_F10;
		ms_GlfwToEngineKey[GLFW_KEY_F11] = KC_F11;
		ms_GlfwToEngineKey[GLFW_KEY_F12] = KC_F12;

		ms_GlfwToEngineKey[GLFW_KEY_GRAVE_ACCENT] = KC_GRAVE;
		ms_GlfwToEngineKey[GLFW_KEY_1] = KC_1;
		ms_GlfwToEngineKey[GLFW_KEY_2] = KC_2;
		ms_GlfwToEngineKey[GLFW_KEY_3] = KC_3;
		ms_GlfwToEngineKey[GLFW_KEY_4] = KC_4;
		ms_GlfwToEngineKey[GLFW_KEY_5] = KC_5;
		ms_GlfwToEngineKey[GLFW_KEY_6] = KC_6;
		ms_GlfwToEngineKey[GLFW_KEY_7] = KC_7;
		ms_GlfwToEngineKey[GLFW_KEY_8] = KC_8;
		ms_GlfwToEngineKey[GLFW_KEY_9] = KC_9;
		ms_GlfwToEngineKey[GLFW_KEY_0] = KC_0;
		ms_GlfwToEngineKey[GLFW_KEY_MINUS] = KC_MINUS;
		ms_GlfwToEngineKey[GLFW_KEY_EQUAL] = KC_EQUALS;
		ms_GlfwToEngineKey[GLFW_KEY_BACKSPACE] = KC_BACKSPACE;

		ms_GlfwToEngineKey[GLFW_KEY_TAB] = KC_TAB;
		ms_GlfwToEngineKey[GLFW_KEY_Q] = KC_Q;
		ms_GlfwToEngineKey[GLFW_KEY_W] = KC_W;
		ms_GlfwToEngineKey[GLFW_KEY_E] = KC_E;
		ms_GlfwToEngineKey[GLFW_KEY_R] = KC_R;
		ms_GlfwToEngineKey[GLFW_KEY_T] = KC_T;
		ms_GlfwToEngineKey[GLFW_KEY_Y] = KC_Y;
		ms_GlfwToEngineKey[GLFW_KEY_U] = KC_U;
		ms_GlfwToEngineKey[GLFW_KEY_I] = KC_I;
		ms_GlfwToEngineKey[GLFW_KEY_O] = KC_O;
		ms_GlfwToEngineKey[GLFW_KEY_P] = KC_P;
		ms_GlfwToEngineKey[GLFW_KEY_LEFT_BRACKET] = KC_LBRACKET;
		ms_GlfwToEngineKey[GLFW_KEY_RIGHT_BRACKET] = KC_RBRACKET;
		ms_GlfwToEngineKey[GLFW_KEY_ENTER] = KC_ENTER;

		ms_GlfwToEngineKey[GLFW_KEY_CAPS_LOCK] = KC_CAPSLOCK;
		ms_GlfwToEngineKey[GLFW_KEY_A] = KC_A;
		ms_GlfwToEngineKey[GLFW_KEY_S] = KC_S;
		ms_GlfwToEngineKey[GLFW_KEY_D] = KC_D;
		ms_GlfwToEngineKey[GLFW_KEY_F] = KC_F;
		ms_GlfwToEngineKey[GLFW_KEY_G] = KC_G;
		ms_GlfwToEngineKey[GLFW_KEY_H] = KC_H;
		ms_GlfwToEngineKey[GLFW_KEY_J] = KC_J;
		ms_GlfwToEngineKey[GLFW_KEY_K] = KC_K;
		ms_GlfwToEngineKey[GLFW_KEY_L] = KC_L;
		ms_GlfwToEngineKey[GLFW_KEY_SEMICOLON] = KC_SEMICOLON;
		ms_GlfwToEngineKey[GLFW_KEY_APOSTROPHE] = KC_APOSTROPHE;
		ms_GlfwToEngineKey[GLFW_KEY_BACKSLASH] = KC_KC_BACKSLASH;

		ms_GlfwToEngineKey[GLFW_KEY_LEFT_SHIFT] = KC_LSHIFT;
		ms_GlfwToEngineKey[GLFW_KEY_Z] = KC_Z;
		ms_GlfwToEngineKey[GLFW_KEY_X] = KC_X;
		ms_GlfwToEngineKey[GLFW_KEY_C] = KC_C;
		ms_GlfwToEngineKey[GLFW_KEY_V] = KC_V;
		ms_GlfwToEngineKey[GLFW_KEY_B] = KC_B;
		ms_GlfwToEngineKey[GLFW_KEY_N] = KC_N;
		ms_GlfwToEngineKey[GLFW_KEY_M] = KC_M;
		ms_GlfwToEngineKey[GLFW_KEY_COMMA] = KC_COMMA;
		ms_GlfwToEngineKey[GLFW_KEY_PERIOD] = KC_PERIOD;
		ms_GlfwToEngineKey[GLFW_KEY_SLASH] = KC_SLASH;
		ms_GlfwToEngineKey[GLFW_KEY_RIGHT_SHIFT] = KC_RSHIFT;

		ms_GlfwToEngineKey[GLFW_KEY_LEFT_CONTROL] = KC_LCTRL;
		ms_GlfwToEngineKey[GLFW_KEY_LEFT_SUPER] = KC_LWIN;
		ms_GlfwToEngineKey[GLFW_KEY_LEFT_ALT] = KC_LALT;
		ms_GlfwToEngineKey[GLFW_KEY_SPACE] = KC_SPACE;
		ms_GlfwToEngineKey[GLFW_KEY_RIGHT_ALT] = KC_RALT;
		ms_GlfwToEngineKey[GLFW_KEY_RIGHT_SUPER] = KC_RWIN;
		ms_GlfwToEngineKey[GLFW_KEY_RIGHT_CONTROL] = KC_RCTRL;

		ms_GlfwToEngineKey[GLFW_KEY_PRINT_SCREEN] = KC_PRINT_SCREEN;
		ms_GlfwToEngineKey[GLFW_KEY_SCROLL_LOCK] = KC_SCROLL_LOCK;

		ms_GlfwToEngineKey[GLFW_KEY_INSERT] = KC_INSERT;
		ms_GlfwToEngineKey[GLFW_KEY_DELETE] = KC_DELETE;
		ms_GlfwToEngineKey[GLFW_KEY_HOME] = KC_HOME;
		ms_GlfwToEngineKey[GLFW_KEY_END] = KC_END;
		ms_GlfwToEngineKey[GLFW_KEY_PAGE_UP] = KC_PAGE_UP;
		ms_GlfwToEngineKey[GLFW_KEY_PAGE_DOWN] = KC_PAGE_DOWN;

		ms_GlfwToEngineKey[GLFW_KEY_UP] = KC_UP;
		ms_GlfwToEngineKey[GLFW_KEY_DOWN] = KC_DOWN;
		ms_GlfwToEngineKey[GLFW_KEY_LEFT] = KC_LEFT;
		ms_GlfwToEngineKey[GLFW_KEY_RIGHT] = KC_RIGHT;

		ms_GlfwToEngineKey[GLFW_KEY_NUM_LOCK] = KC_NUM_LOCK;
		ms_GlfwToEngineKey[GLFW_KEY_KP_DIVIDE] = KC_NP_DIVIDE;
		ms_GlfwToEngineKey[GLFW_KEY_KP_MULTIPLY] = KC_NP_MULTIPLY;
		ms_GlfwToEngineKey[GLFW_KEY_KP_SUBTRACT] = KC_NP_SUBTRACT;
		ms_GlfwToEngineKey[GLFW_KEY_KP_ADD] = KC_NP_ADD;
		ms_GlfwToEngineKey[GLFW_KEY_KP_ENTER] = KC_NP_ENTER;
		ms_GlfwToEngineKey[GLFW_KEY_KP_DECIMAL] = KC_NP_DECIMAL;
		ms_GlfwToEngineKey[GLFW_KEY_KP_0] = KC_NP_0;
		ms_GlfwToEngineKey[GLFW_KEY_KP_1] = KC_NP_1;
		ms_GlfwToEngineKey[GLFW_KEY_KP_2] = KC_NP_2;
		ms_GlfwToEngineKey[GLFW_KEY_KP_3] = KC_NP_3;
		ms_GlfwToEngineKey[GLFW_KEY_KP_4] = KC_NP_4;
		ms_GlfwToEngineKey[GLFW_KEY_KP_5] = KC_NP_5;
		ms_GlfwToEngineKey[GLFW_KEY_KP_6] = KC_NP_6;
		ms_GlfwToEngineKey[GLFW_KEY_KP_7] = KC_NP_7;
		ms_GlfwToEngineKey[GLFW_KEY_KP_8] = KC_NP_8;
		ms_GlfwToEngineKey[GLFW_KEY_KP_9] = KC_NP_9;

		//Mouse
		ms_GlfwToEngineMouseBtn[GLFW_MOUSE_BUTTON_1] = MC_BUTTON_LEFT;
		ms_GlfwToEngineMouseBtn[GLFW_MOUSE_BUTTON_2] = MC_BUTTON_RIGHT;
		ms_GlfwToEngineMouseBtn[GLFW_MOUSE_BUTTON_3] = MC_BUTTON_MIDDLE;
		ms_GlfwToEngineMouseBtn[GLFW_MOUSE_BUTTON_4] = MC_BUTTON4;
		ms_GlfwToEngineMouseBtn[GLFW_MOUSE_BUTTON_5] = MC_BUTTON5;
		ms_GlfwToEngineMouseBtn[GLFW_MOUSE_BUTTON_6] = MC_BUTTON6;
		ms_GlfwToEngineMouseBtn[GLFW_MOUSE_BUTTON_7] = MC_BUTTON7;
		ms_GlfwToEngineMouseBtn[GLFW_MOUSE_BUTTON_8] = MC_BUTTON_LAST;
	}
}