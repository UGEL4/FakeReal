#include "Application.h"
#include "stdio.h"
#include "Window/Window.h"
#include "Input/Input.h"
#include "../Render/Renderer.h"
#include "../Render/Shader/Shader.h"
#include "../Graphic/Resource/ResourceManager.h"
#include "../Graphic/World/World.h"
#include "../Graphic/World/Scene/SceneManager.h"
#include "Layer/ImguiLayer.h"
#include <glfw/glfw3.h>

namespace FakeReal {
	Application* Application::ms_pApplication = nullptr;
	Application::Application()
	{
		ms_pApplication = this;
		m_pWindow = nullptr;
		m_pImguiLayer = nullptr;
		m_bRunning = false;
		m_LastFrameTime = 0.0f;
		Init();
	}

	Application::~Application()
	{
	}

	void Application::Init()
	{
		CreateWindow();
		CreateInput();
		InitRenderer();
		CreateSceneManager();
		CreateWorld();

		m_pImguiLayer = new ImGuiLayer;
		PushLayerOverlay(m_pImguiLayer);
	}

	void Application::Close()
	{
		ReleaseWorld();
		ReleaseSceneManager();
		ReleaseRender();
		ReleaseInput();
		ReleaseWindow();
	}

	bool Application::Main()
	{
		ResourceManager::LoadDefaultResource();

		for (Layer* layer : mLayerStack)
		{
			layer->Start();
		}

		m_bRunning = true;
		Run();
		Close();
		return true;
	}

	bool Application::CreateInput()
	{
		if (!Input::ms_pInstance)
			Input::CreateInput();
		return true;
	}

	bool Application::ReleaseInput()
	{
		if (!Input::ms_pInstance)
			delete Input::ms_pInstance;
		return true;
	}

	bool Application::CreateWindow()
	{
		m_pWindow = Window::Create("Fake Real", 1280, 720);
		m_pWindow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		return true;
	}

	bool Application::ReleaseWindow()
	{
		if (m_pWindow)
			delete m_pWindow;
		return true;
	}

	bool Application::InitRenderer()
	{
		Renderer::Init();
		ShaderLibrary::gs_pShaderLib->LoadDefaultShader();
		return true;
	}

	void Application::ReleaseRender()
	{
		delete ShaderLibrary::gs_pShaderLib;
		Renderer::Release();
	}

	bool Application::CreateWorld()
	{
		World* pWorld = new World;
		return pWorld != nullptr;
	}

	void Application::ReleaseWorld()
	{
		if (World::ms_pWorld)
			delete World::ms_pWorld;
		World::ms_pWorld = nullptr;
	}

	bool Application::CreateSceneManager()
	{
		SceneManager* pSM = new SceneManager;
		return pSM != nullptr;
	}

	void Application::ReleaseSceneManager()
	{
		if (SceneManager::ms_pSceneManager)
			delete SceneManager::ms_pSceneManager;
		SceneManager::ms_pSceneManager = nullptr;
	}

	void Application::Run()
	{
		while (m_bRunning)
		{
			float time = (float)glfwGetTime();
			float timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (Input::ms_pInstance)
				Input::ms_pInstance->Update();

			if (World::ms_pWorld)
				World::ms_pWorld->ProcessInput();

			if (SceneManager::ms_pSceneManager)
				SceneManager::ms_pSceneManager->Update(time);

			if (World::ms_pWorld)
				World::ms_pWorld->Update(time);

			for (Layer* layer : mLayerStack)
			{
				layer->OnUpdate(time);
			}

			m_pImguiLayer->Begin();
			for (Layer* layer : mLayerStack)
			{
				layer->OnImGuiRender();
			}
			m_pImguiLayer->End();

			m_pWindow->Update();
		}
	}

	void Application::OnClose()
	{
		m_bRunning = false;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = mLayerStack.rbegin(); it != mLayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			//m_bMinimized = true;
			return false;
		}
		//m_bMinimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

	void Application::PushLayer(Layer* layer)
	{
		mLayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushLayerOverlay(Layer* layer)
	{
		mLayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

}