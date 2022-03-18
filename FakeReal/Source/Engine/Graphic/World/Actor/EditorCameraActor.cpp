#include "EditorCameraActor.h"
#include "../../Stream/Property.h"
#include "../../Node/NodeComponent/Camera/Camera.h"
#include "../../../Core/Input/Input.h"
#include "../../../Core/Events/Event.h"
#include "../../../Core/Events/MouseEvent.h"
#include "../../../Core/Events/ApplicationEvent.h"
#include "../../../Core/CoreMarco.h"
#include <algorithm>

namespace FakeReal {
	IMPLEMENT_RTTI(EditorCameraActor, Actor);
	IMPLEMENT_INITIAL_BEGIN(EditorCameraActor)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(EditorCameraActor, Actor)
	END_ADD_PROPERTY

	EditorCameraActor::EditorCameraActor()
	{
		std::cout << "¹¹ÔìEditorCameraActor:" << this << std::endl;
	}

	EditorCameraActor::~EditorCameraActor()
	{
		std::cout << "Îö¹¹EditorCameraActor:" << this << std::endl;
	}

	void EditorCameraActor::Update(float appTime)
	{
		Actor::Update(appTime);

		if (mForwardSpeed > 0.001f || mForwardSpeed < 0.001f)
			MoveFowrard(mForwardSpeed * appTime);
		if (mRightSpeed > 0.001f || mRightSpeed < 0.001f)
			MoveRight(mRightSpeed * appTime);
		if (mUpSpeed > 0.001f || mUpSpeed < 0.001f)
			MoveUp(mUpSpeed * appTime);
		Rotate();

		m_pNode->UpdateAll(appTime);
	}

	void EditorCameraActor::ProcessInput()
	{
		mForwardSpeed = 0.0f;
		mRightSpeed = 0.0f;
		mUpSpeed = 0.0f;

		const glm::vec2& mouse{ Input::ms_pInstance->GetMouseX(), Input::ms_pInstance->GetMouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
		m_InitialMousePosition = mouse;
		if (Input::ms_pInstance->IsMouseButtonPressed(Mouse::MC_BUTTON_MIDDLE))
		{
			MousePan(delta);
		}
		else if (Input::ms_pInstance->IsMouseButtonPressed(Mouse::MC_BUTTON_RIGHT))
		{
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_W))
			{
				//MoveFowrard(2.0f);
				mForwardSpeed = 5.0f;
			}
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_S))
			{
				//MoveFowrard(-2.0f);
				mForwardSpeed = -5.0f;
			}
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_A))
			{
				//MoveRight(-2.0f);
				mRightSpeed = -5.0f;
			}
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_D))
			{
				//MoveRight(2.0f);
				mRightSpeed = 5.0f;
			}
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_Q))
			{
				//MoveUp(-2.0f);
				mUpSpeed = -5.0f;
			}
			if (Input::ms_pInstance->IsKeyPressed(Key::KC_E))
			{
				//MoveUp(2.0f);
				mUpSpeed = 5.0f;
			}
			if (Input::ms_pInstance->IsMouseButtonPressed(Mouse::MC_BUTTON_LEFT))
				MouseRotate(delta);
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
		}
	}

	void EditorCameraActor::MousePan(const glm::vec2& delta)
	{
		std::pair<float, float> data = PanSpeed();
		MoveRight(-delta.x * data.first * 5.f);
		MoveUp(delta.y * data.second * 5.f);
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

	void EditorCameraActor::MoveFowrard(float value)
	{
		glm::vec3 v = m_pNode->GetWorldTransform().GetForwardAxis();
		glm::vec3 pos = v * value;
		glm::vec3 p = m_pNode->GetWorldTransform().GetPosition();
		SetWorldPos(m_pNode->GetWorldTransform().GetPosition() + pos);
	}

	void EditorCameraActor::MoveRight(float value)
	{
		glm::vec3 pos = m_pNode->GetWorldTransform().GetRightAxis() * value;
		SetWorldPos(m_pNode->GetWorldTransform().GetPosition() + pos);
	}

	void EditorCameraActor::MoveUp(float value)
	{
		glm::vec3 pos = m_pNode->GetWorldTransform().GetUpAxis() * value;
		SetWorldPos(m_pNode->GetWorldTransform().GetPosition() + pos);
	}

	void EditorCameraActor::MouseRotate(const glm::vec2& delta)
	{
		Camera* pCamera = (Camera*)m_pNode;
		float yawSign = m_pNode->GetWorldTransform().GetUpAxis().y < 0 ? -1.0f : 1.0f;
		float yaw =  pCamera->GetYaw() + yawSign * delta.x *m_RotateSpeed;
		float pitch = pCamera->GetPitch() + delta.y * m_RotateSpeed;
		pCamera->SetYaw(yaw);
		pCamera->SetPitch(pitch);
	}

	void EditorCameraActor::MouseZoom(float delta)
	{
		if (delta > 0.0001)
			MoveFowrard(0.1f);
		else if (delta < 0.0001f)
			MoveFowrard(-0.1f);
	}

	void EditorCameraActor::Rotate()
	{
		Camera* pCamera = (Camera*)m_pNode;
		pCamera->Rotate();
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