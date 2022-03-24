#include "TimeEvent.h"
#include "Controller.h"
#include "../Stream/Property.h"

namespace FakeReal
{
	IMPLEMENT_RTTI_NO_CREATE_FUNCTION(TimeEvent, Object);
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(TimeEvent)
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END;
	BEGIN_ADD_PROPERTY(TimeEvent, Object)
	REGISTER_PROPERTY(m_bEnable, "Enable", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mTraggerPercent, "TraggerPercent", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pController, "Controller", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY;

	TimeEvent::TimeEvent(Controller* pController)
	{
		m_bEnable = true;
		m_pController = pController;
		mTraggerPercent = 0.0f;
	}

	TimeEvent::TimeEvent()
	{
		m_bEnable = true;
		mTraggerPercent = 0.0f;
		m_pController = nullptr;
	}

	TimeEvent::~TimeEvent()
	{
		m_pController = nullptr;
	}

	void TimeEvent::Trigger()
	{
		mTriggerAnimEvent();
	}

}
