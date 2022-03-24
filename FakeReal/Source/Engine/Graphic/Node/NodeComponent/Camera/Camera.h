#pragma once
#include "../NodeComponent.h"
#include <glm/glm.hpp>
namespace FakeReal {
	//ViewPort
	typedef struct VIEWPORT_TYPE
	{
		float XMin;
		float XMax;
		float YMin;
		float YMax;
		float ZMin;
		float ZMax;
		bool bChangeAspect;//是否维持相机当前的长宽比
		void Reset()
		{
			XMin = 0.0f;
			XMax = 1.0f;
			YMin = 0.0f;
			YMax = 1.0f;
			ZMin = 0.0f;
			ZMax = 1.0f;
			bChangeAspect = false;
		}
		VIEWPORT_TYPE()
		{
			Reset();
		}
	}ViewPort;
	class FR_ENGINE_API Camera : public NodeComponent
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		enum//相机平面
		{
			CP_RIGHT,
			CP_LEFT,
			CP_TOP,
			CP_BOTTOM,
			CP_FAR,
			CP_NEAR,
			CP_MAX = 6
		};

		Camera();
		virtual ~Camera();

		void CreateFromLookAt(const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& worldUp = { 0.0f, 1.0f, 0.0f });
		void SetPerspectiveFov(float fov, float aspect, float near, float far);
		void SetAspect(float aspect);
		bool AddViewPort(const ViewPort& viewPort);
		ViewPort* GetViewPort(unsigned int i) const;
		unsigned int GetViewPortNum() const { return mViewPort.size(); }

		virtual void UpdateTransform(float appTime) override;

		glm::mat4& GetViewMatrix() { return mViewMatrix; }
		glm::mat4& GetProjMatrix() { return mProjMatrix; }
		glm::mat4 GetViewProjMatrix() const { return mProjMatrix * mViewMatrix; }

		void SetYaw(float val) { mbIsChange = true;  mRotY = val; }
		void SetPitch(float val) { mbIsChange = true;  mRotX = val; }
		void SetRoll(float val) { mbIsChange = true;  mRotZ = val; }
		float GetYaw() const { return mRotY; }
		float GetPitch() const { return mRotX; }
		float GetRoll() const { return mRotZ; }

		void Rotate();
	protected:
		float mRotX;
		float mRotY;
		float mRotZ;
		float mAspect;
		float mFov;
		float mNear;
		float mFar;
		glm::mat4 mViewMatrix;
		glm::mat4 mProjMatrix;
		std::vector<ViewPort> mViewPort;
	};
	FR_TYPE_MARCO(Camera)
}