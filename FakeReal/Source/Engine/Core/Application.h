#pragma once
#include "Layer/LayerStack.h"
#include "EventS/ApplicationEvent.h"
#include "CoreMarco.h"
namespace FakeReal {
	class Window;
	class FR_ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Init();
		virtual void Close();
		virtual bool Main();
		bool CreateInput();
		bool ReleaseInput();
		bool CreateWindow();
		bool ReleaseWindow();
		bool InitRenderer();
		void ReleaseRender();
		bool CreateWorld();
		void ReleaseWorld();
		bool CreateSceneManager();
		void ReleaseSceneManager();
		void Run();
		void OnClose();

		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		void PushLayer(Layer* layer);
		void PushLayerOverlay(Layer* layer);

	public:
		static Application* ms_pApplication;
		static Application* GetInstance()
		{
			return ms_pApplication;
		}

	public:
		Window* GetWindow() { return m_pWindow; }
	private:
		Window* m_pWindow;
		LayerStack mLayerStack;
		class ImGuiLayer* m_pImguiLayer;
		float m_LastFrameTime;
		bool m_bRunning;

	};

	Application* Create();
}