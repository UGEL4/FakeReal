#include "FreeCameraController.h"
#include "../World/Actor/EditorCameraActor.h"
#include "../Node/NodeComponent/Camera/Camera.h"
#include "../../Core/Input/Input.h"
#include "../Stream/Property.h"

namespace FakeReal
{
	IMPLEMENT_RTTI(FreeCameraController, Controller);
	IMPLEMENT_INITIAL_BEGIN(FreeCameraController)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(FreeCameraController, Controller)
	END_ADD_PROPERTY;

	FreeCameraController::FreeCameraController()
	{

	}

	FreeCameraController::~FreeCameraController()
	{

	}

	bool FreeCameraController::UpdateEx(double appTime)
	{
		if (m_pObject)
		{
			Camera* pCameraActor = DynamicCast<Camera>(m_pObject);
			if (pCameraActor)
			{
				//Ðý×ª
				const glm::vec2& mouse{ Input::ms_pInstance->GetMouseX(), Input::ms_pInstance->GetMouseY() };
				glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
				m_InitialMousePosition = mouse;
				float yawSign = pCameraActor->GetWorldTransform().GetUpAxis().y < 0 ? -1.0f : 1.0f;
				float yaw = pCameraActor->GetYaw() + yawSign * delta.x *m_RotateSpeed;
				float pitch = pCameraActor->GetPitch() + delta.y * m_RotateSpeed;
				pCameraActor->SetYaw(yaw);
				pCameraActor->SetPitch(pitch);
				pCameraActor->Rotate();

				glm::vec3 v = pCameraActor->GetWorldTransform().GetForwardAxis();
				glm::vec3 pos = v * mForwardSpeed * (float)m_dIntervalTime;
				glm::vec3 p = pCameraActor->GetWorldTransform().GetPosition();
				pCameraActor->SetWorldTranslate(p + pos);

				pos = pCameraActor->GetWorldTransform().GetRightAxis() * mRightSpeed * (float)m_dIntervalTime;
				pCameraActor->SetWorldTranslate(pCameraActor->GetWorldTransform().GetPosition() + pos);

				pos = pCameraActor->GetWorldTransform().GetUpAxis() * mUpSpeed * (float)m_dIntervalTime;
				pCameraActor->SetWorldTranslate(pCameraActor->GetWorldTransform().GetPosition() + pos);

				return true;
			}
		}
		return false;
	}

	void FreeCameraController::MousePan(const glm::vec2& delta, const std::pair<float, float>& panSpeed)
	{
		Camera* pCamera = DynamicCast<Camera>(m_pObject);
		if (pCamera)
		{
			glm::vec3 pos = pCamera->GetWorldTransform().GetRightAxis() * (-delta.x * panSpeed.first * 5.f);
			pCamera->SetWorldTranslate(pCamera->GetWorldTransform().GetPosition() + pos);

			pos = pCamera->GetWorldTransform().GetUpAxis() * delta.y * panSpeed.second * 5.f;
			pCamera->SetWorldTranslate(pCamera->GetWorldTransform().GetPosition() + pos);
		}
	}

	void FreeCameraController::MoveForward(float value) const
	{
		if (m_pObject)
		{
			Camera* pCamera = DynamicCast<Camera>(m_pObject);
			if (pCamera)
			{
				glm::vec3 v = pCamera->GetWorldTransform().GetForwardAxis() * value;
				pCamera->SetWorldTranslate(pCamera->GetWorldTransform().GetPosition() + v);
			}
		}
	}

}
