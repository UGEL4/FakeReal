#pragma once

#include "Object.h"
#include "Transform.h"

namespace FakeReal {
	class Controller;
	class FR_ENGINE_API Spatial : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		virtual ~Spatial() = 0;
		friend class Node;
		friend class Geometry;
		friend class NodeComponent;

		/*这些是直接设置值*/
		virtual void SetWorldTranslate(const glm::vec3& translate);
		virtual void SetWorldRotation(const glm::vec3& rotate);
		virtual void SetWorldRotation(const glm::quat& rotate);
		virtual void SetWorldScale(const glm::vec3& scale);
		virtual void SetWorldTransform(const Transform& transform);

		virtual void SetLocalTranslate(const glm::vec3& translate);
		virtual void SetLocalRotation(const glm::vec3& rotate);
		virtual void SetLocalRotation(const glm::quat& rotate);
		virtual void SetLocalScale(const glm::vec3& scale);
		virtual void SetLocalTransform(const Transform& transform);
		virtual void SetLocalTransform(const glm::vec3& pos, const glm::quat& rotate, const glm::vec3& scale);

		virtual const glm::vec3& GetWorldTranslate();
		virtual glm::vec3& GetWorldTranslateRef();
		virtual const glm::vec3& GetWorldRotate();
		virtual const glm::quat& GetWorldRotateQ();
		virtual const glm::vec3& GetWorldScale();

		virtual const glm::vec3& GetLocalTranslate();
		virtual const glm::vec3& GetLocalRotate();
		virtual const glm::quat& GetLocalRotateQ();
		virtual const glm::vec3& GetLocalScale();

		virtual const Transform& GetWorldTransform() const { return mWorldTransform; }
		virtual const Transform& GetLocalTransform() const { return mLocalTransform; }


		virtual void UpdateAll(float appTime);
		virtual void UpdateTransform(float appTime);

		void SetParent(Spatial* pParent);
		Spatial* GetParent() const;

		glm::mat4 GetWorldMatrix() { return mWorldTransform.GetLocalToWorldMatrix(); }
		glm::mat4 GetLocalMatrix() { return mLocalTransform.GetLocalToWorldMatrix(); }
		void SetDynamic(bool bIsDynamic);
		bool IsDynamic() const { return !mbStatic; }

		void AddController(Controller* pController);
		virtual void UpdateController(float appTime);
		virtual void UpdateNodeAll(float appTime) = 0;
	protected:
		Spatial();
	protected:
		Transform mWorldTransform;
		Transform mLocalTransform;
		Spatial* m_pParent;
		bool mbIsChange;
		bool mbStatic;
		std::vector<Controller*> m_pControllerArray;
	};
	FR_TYPE_MARCO(Spatial)
}
