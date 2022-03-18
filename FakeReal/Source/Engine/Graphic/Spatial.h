#pragma once

#include "Object.h"
#include "Transform.h"

namespace FakeReal {
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

		virtual const glm::vec3& GetWorldTranslate();
		virtual glm::vec3& GetWorldTranslateRef();
		virtual const glm::vec3& GetWorldRotate();
		virtual const glm::quat& GetWorldRotateQ();
		virtual const glm::vec3& GetWorldScale();

		virtual const glm::vec3& GetLocalTranslate();
		virtual const glm::vec3& GetLocalRotate();
		virtual const glm::quat& GetLocalRotateQ();
		virtual const glm::vec3& GetLocalScale();


		virtual void UpdateAll(float appTime);
		virtual void UpdateTransform(float appTime);

		void SetParent(Spatial* pParent);
		Spatial* GetParent() const;

		const Transform& GetWorldTransform() const { return mWorldTransform; }
		glm::mat4 GetWorldMatrix() { return mWorldTransform.GetLocalToWorldMatrix(); }
		glm::mat4 GetLocalMatrix() { return mLocalTransform.GetLocalToWorldMatrix(); }
		void SetDynamic(bool bIsDynamic);
		bool IsDynamic() const { return !mbStatic; }
	protected:
		Spatial();
		virtual void UpdateNodeAll(float appTime) = 0;
	protected:
		Transform mWorldTransform;
		Transform mLocalTransform;
		Spatial* m_pParent;
		bool mbIsChange;
		bool mbStatic;
	};
	FR_TYPE_MARCO(Spatial)
}
