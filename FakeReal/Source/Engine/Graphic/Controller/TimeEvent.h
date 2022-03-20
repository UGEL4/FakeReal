#pragma once
#include "../Object.h"

namespace FakeReal
{
	class Controller;
	class FR_ENGINE_API TimeEvent : public Object
	{
	public:
		TimeEvent(Controller* pController);
		virtual ~TimeEvent() = 0;

		Controller* GetObject() const;
		virtual void Trigger() = 0;

		bool m_bEnable;
		float mTraggerPercent;
		Controller* m_pController;

	};
}
