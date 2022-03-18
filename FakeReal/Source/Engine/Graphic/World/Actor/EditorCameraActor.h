#pragma once

#include "Actor.h"

namespace FakeReal
{
	class Event;
	class MouseScrolledEvent;
	class WindowResizeEvent;
	class FR_ENGINE_API EditorCameraActor : public Actor
	{
		DECLARE_RTTI
		DECLARE_INITIAL

	public:
		EditorCameraActor();
		virtual ~EditorCameraActor();

		virtual void Update(float appTime) override;
		virtual void ProcessInput() override;
		virtual void CreateDefaultComponent() override;

		void MousePan(const glm::vec2& delta);
		std::pair<float, float> PanSpeed() const;
		void MoveFowrard(float value);
		void MoveRight(float value);
		void MoveUp(float value);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);
		void Rotate();

		void OnEvent(Event& e);
		void SetViewPortSize(float width, float height);

		glm::mat4 GetViewProjMatrix() const;
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetProjMatrix() const;

	private:
		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnResize(WindowResizeEvent& e);

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };
		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
		float m_RotateSpeed = 15.0f;
		float mForwardSpeed = 0.0f;
		float mRightSpeed = 0.0f;
		float mUpSpeed = 0.0f;
	};
	FR_TYPE_MARCO(EditorCameraActor)
}