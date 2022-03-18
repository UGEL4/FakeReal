#pragma once
#include <string_view>
#include <functional>
#include "../CoreMarco.h"
namespace FakeReal {
	class Event;
	class FR_ENGINE_API Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;
		virtual ~Window() = default;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual const std::string_view& GetTitle() const = 0;

		virtual void Update() = 0;
		virtual void* GetNaviteWindow() const = 0;

		virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;

	public:
		static Window* Create(const std::string_view& title, unsigned int width, unsigned int height);
	};
}