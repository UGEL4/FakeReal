#include "EditorCameraActor.h"
#include "../../Stream/Property.h"
#include "../../Node/NodeComponent/Camera/Camera.h"
#include "../../../Core/Input/Input.h"
#include "../../../Core/Events/Event.h"
#include "../../../Core/Events/MouseEvent.h"
#include "../../../Core/Events/ApplicationEvent.h"
#include "../../../Core/CoreMarco.h"
#include "../../Controller/FreeCameraController.h"
#include <algorithm>

namespace FakeReal {
	IMPLEMENT_RTTI(EditorCameraActor, Actor);
	IMPLEMENT_INITIAL_BEGIN(EditorCameraActor)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(EditorCameraActor, Actor)
	END_ADD_PROPERTY

	EditorCameraActor::EditorCameraActor()
	{
		m_pFreeCameraController = nullptr;
		std::cout << "构造EditorCameraActor:" << this << std::endl;
	}

	EditorCameraActor::~EditorCameraActor()
	{
		std::cout << "析构EditorCameraActor:" << this << std::endl;
	}

	void EditorCameraActor::Update(float appTime)
	{
		Actor::Update(appTime);
		m_pNode->UpdateAll(appTime);
	}

	void EditorCameraActor::ProcessInput()
	{
		m_pFreeCameraController->mForwardSpeed = 0.0f;
		m_pFreeCameraController->mRightSpeed = 0.0f;
		m_pFreeCameraController->mUpSpeed = 0.0f;
		m_pFreeCameraController->m_RotateSpeed = 0.0f;

		const glm::vec2& mouse{ Input::ms_pInstance->GetMouseX(), Input::ms_pInstance->GetMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;
		if (Input::ms_pInstance->IsMouseButtonPressed(Mouse::MC_BUTTON_MIDDLE))
		{
			m_pFreeCameraController->MousePan(delta, PanSpeed());
		}
		else if (Input::ms_pInstance->IsMouseButtonPressed(Mouse::MC_BUTTON_RIGHT))
		{
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_W))
			{
				m_pFreeCameraController->mForwardSpeed = 5.0f;
			}
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_S))
			{
				m_pFreeCameraController->mForwardSpeed = -5.0f;
			}
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_A))
			{
				m_pFreeCameraController->mRightSpeed = -5.0f;
			}
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_D))
			{
				m_pFreeCameraController->mRightSpeed = 5.0f;
			}
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_Q))
			{
				m_pFreeCameraController->mUpSpeed = -5.0f;
			}
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_E))
			{
				m_pFreeCameraController->mUpSpeed = 5.0f;
			}
			if (Input::ms_pInstance->IsMouseButtonPressed(Mouse::MC_BUTTON_LEFT))
			{
				m_pFreeCameraController->m_RotateSpeed = 15.0f;
			}
		}
	}

	void EditorCameraActor::CreateDefaultComponent()
	{
		m_pNode = NodeComponent::CreateComponent<Camera>();
		if (m_pNode)
		{
			Camera* pCamera = (Camera*)m_pNode;
			pCamera->CreateFromLookAt({ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f });
			pCamera->SetPerspectiveFov(45.0f, m_ViewportWidth / m_ViewportHeight, 0.1f, 1000.0f);

			m_pFreeCameraController = new FreeCameraController;
			pCamera->AddController(m_pFreeCameraController);
		}
	}

	std::pair<float, float> EditorCameraActor::PanSpeed() const
	{
		Camera* pCamera = (Camera*)m_pNode;
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	void EditorCameraActor::MouseZoom(float delta)
	{
		//onevent是window的回调，时机位于actor的update之后
		if (delta > 0.0001)
			m_pFreeCameraController->MoveForward(0.1f);
		else if (delta < 0.0001f)
			m_pFreeCameraController->MoveForward(-0.1f);
	}

	void EditorCameraActor::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorCameraActor::OnMouseScroll));

		//test
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(EditorCameraActor::OnResize));
	}

	void EditorCameraActor::SetViewPortSize(float width, float height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		Camera* pCamera = (Camera*)m_pNode;
		pCamera->SetAspect(m_ViewportWidth / m_ViewportHeight);
	}

	glm::mat4 EditorCameraActor::GetViewProjMatrix() const
	{
		Camera* pCamera = (Camera*)m_pNode;
		return pCamera->GetViewProjMatrix();
	}

	const glm::mat4& EditorCameraActor::GetViewMatrix() const
	{
		Camera* pCamera = (Camera*)m_pNode;
		return pCamera->GetViewMatrix();
	}

	const glm::mat4& EditorCameraActor::GetProjMatrix() const
	{
		Camera* pCamera = (Camera*)m_pNode;
		return pCamera->GetProjMatrix();
	}

	float EditorCameraActor::GetYaw() const
	{
		if (m_pNode)
			return ((Camera*)m_pNode)->GetYaw();
		else
			return 0.0f;
	}

	float EditorCameraActor::GetPitch() const
	{
		if (m_pNode)
			return ((Camera*)m_pNode)->GetPitch();
		else
			return 0.0f;
	}

	void EditorCameraActor::SetYaw(float yaw)
	{
		if (m_pNode)
			return ((Camera*)m_pNode)->SetYaw(yaw);
	}

	void EditorCameraActor::SetPitch(float pitch)
	{
		if (m_pNode)
			return ((Camera*)m_pNode)->SetPitch(pitch);
	}

	bool EditorCameraActor::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		return false;
	}

	//test
	bool EditorCameraActor::OnResize(WindowResizeEvent& e)
	{
		SetViewPortSize(e.GetWidth(), e.GetHeight());
		return true;
	}

}