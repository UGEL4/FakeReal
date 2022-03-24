#include "Camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include "../../../Stream/Property.h"

namespace FakeReal {
	IMPLEMENT_RTTI(Camera, NodeComponent);
	IMPLEMENT_INITIAL_BEGIN(Camera)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(Camera, NodeComponent)
	REGISTER_PROPERTY(mFov, "FOV", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mNear, "Near", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mFar, "far", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	Camera::Camera()
	{
		mRotX = 0.0f;
		mRotY = 0.0f;
		mRotZ = 0.0f;
		mAspect = 0.0f;
		mFov = 0.0f;
		mNear = 0.0f;
		mFar = 0.0f;
	}

	Camera::~Camera()
	{
		std::cout << "Îö¹¹Camera:" << this << std::endl;
	}

	void Camera::CreateFromLookAt(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& worldUp)
	{
		mbIsChange = true;
		glm::mat4 lookMat = glm::lookAtRH(pos, lookAt, worldUp);
		glm::mat4 invMat = glm::inverse(lookMat);
		glm::quat q = glm::quat_cast(lookMat);
		SetLocalRotation(q);
		SetLocalTranslate(pos);
		UpdateAll(0.0f);
	}

	void Camera::SetPerspectiveFov(float fov, float aspect, float near, float far)
	{
		mFov = fov;
		mAspect = aspect;
		mNear = near;
		mFar = far;
		mProjMatrix = glm::perspective(fov, aspect, near, far);
	}

	void Camera::SetAspect(float aspect)
	{
		mAspect = aspect;
		mProjMatrix = glm::perspective(mFov, mAspect, mNear, mFar);
	}

	bool Camera::AddViewPort(const ViewPort& viewPort)
	{
		if (viewPort.XMin < 0.0f || viewPort.XMax > 1.0f || viewPort.XMin > viewPort.XMax ||
			viewPort.YMin < 0.0f || viewPort.YMax > 1.0f || viewPort.YMin > viewPort.YMax ||
			viewPort.ZMin < 0.0f || viewPort.ZMax > 1.0f || viewPort.ZMin > viewPort.ZMax)
		{
			return false;
		}
		mViewPort.emplace_back(viewPort);
		return true;
	}

	ViewPort* Camera::GetViewPort(unsigned int i) const
	{
		return const_cast<ViewPort*>(&mViewPort[i]);
	}

	void Camera::UpdateTransform(float appTime)
	{
		if (appTime > 0.0f)
		{
			UpdateController(appTime);
		}

		NodeComponent::UpdateTransform(appTime);
		if (mbIsChange)
		{
			mViewMatrix = mWorldTransform.GetWorldToLocalMatrix();
		}
		mbIsChange = false;
	}

	void Camera::Rotate()
	{
		SetWorldRotation({ -mRotX, -mRotY, 0.0f });
	}

}