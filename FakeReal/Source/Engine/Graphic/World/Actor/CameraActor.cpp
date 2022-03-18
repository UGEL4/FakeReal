#include "CameraActor.h"
#include "../../Stream/Property.h"
#include "../../Node/NodeComponent/Camera/Camera.h"

namespace FakeReal {
	IMPLEMENT_RTTI(CameraActor, Actor);
	IMPLEMENT_INITIAL_BEGIN(CameraActor)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(CameraActor, Actor)
	END_ADD_PROPERTY

	CameraActor::CameraActor()
	{
		std::cout << "¹¹ÔìCameraActor:" << this << std::endl;
	}

	CameraActor::~CameraActor()
	{
		std::cout << "Îö¹¹CameraActor:" << this << std::endl;
	}

	void CameraActor::Update(float appTime)
	{
		Actor::Update(appTime);
	}

	void CameraActor::CreateDefaultComponent()
	{
		m_pNode = NodeComponent::CreateComponent<Camera>();
	}

}