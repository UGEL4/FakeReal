#pragma once

#include "Layer.h"
namespace FakeReal {
	class FR_ENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnImGuiRender();

		void Begin();
		void End();
		void SetDarkThemeColors();
		void SetDockSpace();
	};
}