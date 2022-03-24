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

		std::pair<float, float> PanSpeed() const;
		void MouseZoom(float delta);

		void OnEvent(Event& e);
		void SetViewPortSize(float width, float height);

		glm::mat4 GetViewProjMatrix() const;
		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetProjMatrix() const;
		float GetYaw() const;
		float GetPitch() const;
		void SetYaw(float yaw);
		void SetPitch(float pitch);

	private:
		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnResize(WindowResizeEvent& e);

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };
		float m_ViewportWidth = 1280, m_ViewportHeight = 720;

		class FreeCameraController* m_pFreeCameraController;
	};
	FR_TYPE_MARCO(EditorCameraActor)
}