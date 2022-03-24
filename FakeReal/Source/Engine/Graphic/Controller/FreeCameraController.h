#pragma once

#include "Controller.h"
#include <glm/vec2.hpp>
namespace FakeReal
{
	class FR_ENGINE_API FreeCameraController : public Controller
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		FreeCameraController();
		~FreeCameraController();

		virtual bool UpdateEx(double appTime) override;

		void MousePan(const glm::vec2& delta, const std::pair<float, float>& panSpeed);
		void MoveForward(float value) const;

	public:
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };
		float m_RotateSpeed = 15.0f;
		float mForwardSpeed = 0.0f;
		float mRightSpeed = 0.0f;
		float mUpSpeed = 0.0f;
	};
	FR_TYPE_MARCO(FreeCameraController)
}