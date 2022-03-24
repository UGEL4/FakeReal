#pragma once
#include "../Object.h"
#include "../../Core/Events/Delegate.h"

namespace FakeReal
{
	typedef DelegateEvent<void> TriggerAnimEventType;
	class Controller;
	class FR_ENGINE_API TimeEvent : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		TimeEvent();
		TimeEvent(Controller* pController);
		virtual ~TimeEvent() = 0;

		virtual void Trigger();

		bool m_bEnable;
		float mTraggerPercent;
		Controller* m_pController;
		TriggerAnimEventType mTriggerAnimEvent;
	};
	FR_TYPE_MARCO(TimeEvent)
}
