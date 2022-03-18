#pragma once

#include "../Events/Event.h"
#include "../CoreMarco.h"
namespace FakeReal {
	class FR_ENGINE_API Layer
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(float ts);
		virtual void OnImGuiRender();
		virtual void OnEvent(Event& event);
		virtual void Start();
	};
}