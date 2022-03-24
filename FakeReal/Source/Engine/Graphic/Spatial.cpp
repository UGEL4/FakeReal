#include "Spatial.h"
#include "Stream/Property.h"
#include "Controller/Controller.h"
namespace FakeReal
{
	IMPLEMENT_RTTI_NO_CREATE_FUNCTION(Spatial, Object);
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(Spatial)
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END;
	BEGIN_ADD_PROPERTY(Spatial, Object)
	REGISTER_PROPERTY(mWorldTransform, "World Transform", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mLocalTransform, "Local Transform", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pParent, "Parent", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mbStatic, "Static", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	Spatial::Spatial()
	{
		m_pParent = nullptr;
		mWorldTransform = Transform({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		mLocalTransform = Transform({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		mbIsChange = true;
		mbStatic = false;
		m_pControllerArray.clear();
		//std::cout << "¹¹ÔìSpatial:" << this << std::endl;
	}

	Spatial::~Spatial()
	{
		m_pParent = nullptr;
		//TODO:É¾³ýcontroller
		//std::cout << "Îö¹¹Spatial:" << this << std::endl;
	}

	void Spatial::SetWorldTranslate(const glm::vec3& translate)
	{
		mbIsChange = true;
		if (m_pParent)
		{
			Transform Inv;
			m_pParent->mWorldTransform.Inverse(Inv);

			Transform NewWorld;
			NewWorld.SetPosition(translate);
			Transform NewLocal;
			NewLocal.Product(NewWorld, Inv);
			SetLocalTransform(NewLocal);
		}
		else
		{
			SetLocalTranslate(translate);
		}
	}

	void Spatial::SetWorldRotation(const glm::vec3& rotate)
	{
		if (m_pParent)
		{
			Transform Inv;
			m_pParent->mWorldTransform.Inverse(Inv);

			Transform NewWorld;
			NewWorld.SetRotation(rotate);
			Transform NewLocal;
			NewLocal.Product(NewWorld, Inv);
			SetLocalTransform(NewLocal);
		}
		else
		{
			SetLocalRotation(rotate);
		}
	}

	void Spatial::SetWorldRotation(const glm::quat& rotate)
	{
		if (m_pParent)
		{
			Transform Inv;
			m_pParent->mWorldTransform.Inverse(Inv);

			Transform NewWorld;
			NewWorld.SetRotation(rotate);
			Transform NewLocal;
			NewLocal.Product(NewWorld, Inv);
			SetLocalTransform(NewLocal);
		}
		else
		{
			SetLocalRotation(rotate);
		}
	}

	void Spatial::SetWorldScale(const glm::vec3& scale)
	{
		if (m_pParent)
		{
			Transform Inv;
			m_pParent->mWorldTransform.Inverse(Inv);

			Transform NewWorld;
			NewWorld.SetScale(scale);
			Transform NewLocal;
			NewLocal.Product(NewWorld, Inv);
			SetLocalTransform(NewLocal);
		}
		else
		{
			SetLocalScale(scale);
		}
	}

	void Spatial::SetWorldTransform(const Transform& transform)
	{
		if (m_pParent)
		{
			Transform Inv;
			m_pParent->mWorldTransform.Inverse(Inv);

			Transform NewWorld = transform;

			Transform NewLocal;
			NewLocal.Product(NewWorld, Inv);
			SetLocalTransform(NewLocal);
		}
		else
		{
			SetLocalTransform(transform);
		}
	}

	void Spatial::SetLocalTranslate(const glm::vec3& translate)
	{
		mbIsChange = true;
		mLocalTransform.SetPosition(translate);
		UpdateAll(0.0f);
	}

	void Spatial::SetLocalRotation(const glm::vec3& rotate)
	{
		mbIsChange = true;
		mLocalTransform.SetRotation({glm::radians(rotate.x), glm::radians(rotate.y), glm::radians(rotate.z)});
		UpdateAll(0.0f);
	}

	void Spatial::SetLocalRotation(const glm::quat& rotate)
	{
		mbIsChange = true;
		mLocalTransform.SetRotation(rotate);
		UpdateAll(0.0f);
	}

	void Spatial::SetLocalScale(const glm::vec3& scale)
	{
		mbIsChange = true;
		mLocalTransform.SetScale(scale);
		UpdateAll(0.0f);
	}

	void Spatial::SetLocalTransform(const Transform& transform)
	{
		mbIsChange = true;
		mLocalTransform = transform;
		UpdateAll(0.0f);
	}

	void Spatial::SetLocalTransform(const glm::vec3& pos, const glm::quat& rotate, const glm::vec3& scale)
	{
		mbIsChange = true;
		mLocalTransform.SetPosition(pos);
		mLocalTransform.SetRotation(rotate);
		mLocalTransform.SetScale(scale);
		UpdateAll(0.0f);
	}

	const glm::vec3& Spatial::GetWorldTranslate()
	{
		return mWorldTransform.GetPosition();
	}

	glm::vec3& Spatial::GetWorldTranslateRef()
	{
		return mWorldTransform.GetPosition();
	}

	const glm::vec3& Spatial::GetWorldRotate()
	{
		return mWorldTransform.GetRotation();
	}

	const glm::quat& Spatial::GetWorldRotateQ()
	{
		return mWorldTransform.GetRotationQ();
	}

	const glm::vec3& Spatial::GetWorldScale()
	{
		return mWorldTransform.GetScale();
	}

	const glm::vec3& Spatial::GetLocalTranslate()
	{
		return mLocalTransform.GetPosition();
	}

	const glm::vec3& Spatial::GetLocalRotate()
	{
		return mLocalTransform.GetRotation();
	}

	const glm::quat& Spatial::GetLocalRotateQ()
	{
		return mLocalTransform.GetRotationQ();
	}

	const glm::vec3& Spatial::GetLocalScale()
	{
		return mLocalTransform.GetScale();
	}

	void Spatial::UpdateAll(float appTime)
	{
		UpdateNodeAll(appTime);
	}

	void Spatial::UpdateTransform(float appTime)
	{
		if (m_pParent)
		{
			if (!m_pParent->mbStatic)
			{
				mbStatic = false;
			}
			if (m_pParent->mbIsChange)
			{
				mbIsChange = true;
			}
		}
		if (mbIsChange)
		{
			if (m_pParent)
				mWorldTransform.Product(mLocalTransform, m_pParent->mWorldTransform);
			else
				mWorldTransform = mLocalTransform;
		}
	}

	void Spatial::SetParent(Spatial* pParent)
	{
		m_pParent = pParent;
		mbIsChange = true;
	}

	Spatial* Spatial::GetParent() const
	{
		return m_pParent;
	}

	void Spatial::SetDynamic(bool bIsDynamic)
	{
		mbStatic = !bIsDynamic;
	}

	void Spatial::AddController(Controller* pController)
	{
		for (size_t i = 0; i < m_pControllerArray.size(); i++)
		{
			if (pController == m_pControllerArray[i])
				return;
		}
		m_pControllerArray.emplace_back(pController);
	}

	void Spatial::UpdateController(float appTime)
	{
		for (size_t i = 0; i < m_pControllerArray.size(); i++)
		{
			m_pControllerArray[i]->SetObject(this);
			m_pControllerArray[i]->Update(appTime);
		}
	}

}