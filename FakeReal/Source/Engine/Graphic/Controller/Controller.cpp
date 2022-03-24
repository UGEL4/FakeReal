#include "Controller.h"
#include "../Stream/Property.h"

namespace FakeReal
{
	IMPLEMENT_RTTI_NO_CREATE_FUNCTION(Controller, Object);
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(Controller)
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END;
	BEGIN_ADD_PROPERTY(Controller, Object)
	REGISTER_PROPERTY(mRepeatType, "RepeatType", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_dMinTime, "MinTime", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_dMaxTime, "MaxTime", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_dPhase, "Phase", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_dFrequency, "Frequency", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pObject, "Object", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_bEnable, "Enable", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(m_pTimeEventArray, "TimeEventArray", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY;
	Controller::Controller()
	{
		mRepeatType = RT_NONE;
		m_dMinTime = 0.0;
		m_dMaxTime = 0.0;
		m_dPhase = 0.0;
		m_dFrequency = 1.0;
		m_pObject = nullptr;
		m_dNowAnimTime = 0.0;
		m_dLastEngineTime = 0.0;
		m_dLastAnimTime = 0.0;
		m_dIntervalTime = 0.0;
		m_dIntervalAnimTime = 0.0;
		m_dTimeSum = 0.0;
		m_dStartSystemTime = 0.0;
		m_bStart = false;
		m_bEnable = true;
		AddTimeEvent(new StartAnimEvent(this));
		AddTimeEvent(new EndAnimEvent(this));
	}

	Controller::~Controller()
	{
		m_pObject = nullptr;
		for (size_t i = 0; i < m_pTimeEventArray.size(); i++)
		{
			if (m_pTimeEventArray[i])
			{
				delete m_pTimeEventArray[i];
				m_pTimeEventArray[i] = nullptr;
			}
		}
		m_pTimeEventArray.clear();
	}

	bool Controller::Update(double appTime)
	{
		if (!m_bEnable)
		{
			m_bStart = false;
			return false;
		}
		//m_bStartΪfalse��ʾ��ʼ����
		if (!m_bStart)
		{
			//��¼��ʼ���ŵ�ʱ��
			m_dStartSystemTime = appTime;
			m_bStart = true;
			//��һ֡���ŵ�ʱ��
			m_dLastEngineTime = 0.0;
			m_dLastAnimTime = 0.0;
			m_dTimeSum = 0.0;
			mTriggerBeginStart();
		}
		//�ӿ�ʼʱ�����ʱ��
		appTime = appTime - m_dStartSystemTime;
		m_dIntervalTime = abs(appTime - m_dLastEngineTime);
		//����ʱ�����������ʱ�����Ǿ������ź�������ʱ����
		m_dIntervalAnimTime = m_dIntervalTime * m_dFrequency;
		m_dLastEngineTime = appTime;
		//����ʱ��ת�ɶ���ʱ��
		appTime = GetControlTime(appTime);
		m_dNowAnimTime = appTime;
		m_dTimeSum += m_dIntervalTime;
		//ʵ��UpdateEx��ʵ�ֶ����߼�
		UpdateEx(appTime);
		//����ʱ���¼�
		TimeEventFunc(appTime);
		m_dLastAnimTime = appTime;
		return true;
	}

	bool Controller::UpdateEx(double appTime)
	{
		return true;
	}

	Object* Controller::GetObject() const
	{
		return m_pObject;
	}

	bool Controller::SetObject(Object* pObj)
	{
		if (!pObj)
			return false;
		m_pObject = pObj;
		return true;
	}

	double Controller::GetControlTime(double dAppTime)
	{
		//m_dFrequency���ڶ���ʱ��Ŀ�����m_dPhase���ڶ���ʱ���ƽ��
		double CtrlTime = m_dFrequency * dAppTime + m_dPhase;
		if (mRepeatType == RT_NONE)
			return CtrlTime;

		if (mRepeatType == RT_CLAMP)
		{
			if (CtrlTime < m_dMinTime)
				return m_dMinTime;

			if (CtrlTime > m_dMaxTime)
				return m_dMaxTime;

			return CtrlTime;
		}

		//���㶯��ʱ�䳤��
		double Range = m_dMaxTime - m_dMinTime;
		if (Range > 0.0)
		{
			//���������ʱ�䳤�ȵı���
			double Multiples = (CtrlTime - m_dMinTime) / Range;
			//ȡ��
			double IntTime = floor(Multiples);
			//ȡС������
			double FrcTime = Multiples - IntTime;
			if (mRepeatType == RT_WRAP)
			{
				return m_dMinTime + FrcTime * Range;
			}
			if (mRepeatType == RT_CYCLE)
			{
				//��������Ϊ����
				if (int(IntTime) & 1)
				{
					//�����Ӻ���ǰ����
					return m_dMaxTime - FrcTime * Range;
				}
				else
				{
					//������ǰ��󲥷�
					return m_dMinTime + FrcTime * Range;
				}
			}

			return m_dMinTime;
		}
		else
		{
			return m_dMinTime;
		}
	}

	void Controller::ClearTime()
	{
		for (size_t i = 0; i < m_pTimeEventArray.size(); i++)
		{
			if (m_pTimeEventArray[i])
			{
				delete m_pTimeEventArray[i];
				m_pTimeEventArray[i] = nullptr;
			}
		}
		m_pTimeEventArray.clear();
	}

	void Controller::AddTimeEvent(TimeEvent* pEvent)
	{
		if (!pEvent)
			return;
		m_pTimeEventArray.emplace_back(pEvent);
	}

	void Controller::DeleteTimeEvent(TimeEvent* pEvent)
	{
		if (!pEvent)
			return;
		for (size_t i = 0; i < m_pTimeEventArray.size(); i++)
		{
			if (pEvent == m_pTimeEventArray[i])
			{
				delete m_pTimeEventArray[i];
				m_pTimeEventArray.erase(m_pTimeEventArray.begin() + i);
				return;
			}
		}
	}

	void Controller::TimeEventFunc(double appTime)
	{
		for (size_t i = 0; i < m_pTimeEventArray.size(); i++)
		{
			double Range = m_dMaxTime - m_dMinTime;
			double RealTime = Range * m_pTimeEventArray[i]->mTraggerPercent;
			if (mRepeatType == RT_WRAP)
			{
				//���ڽ�����
				if (RealTime + m_dIntervalAnimTime > m_dMaxTime)
				{
					if (RealTime >= m_dLastAnimTime || RealTime <= appTime)
						m_pTimeEventArray[i]->Trigger();
				}
				else
				{
					if (RealTime >= m_dLastAnimTime || RealTime <= appTime)
						m_pTimeEventArray[i]->Trigger();
				}
			}
			//else if (mRepeatType == RT_CYCLE)
			else
			{
				if (RealTime >= m_dLastAnimTime || RealTime <= appTime)
					m_pTimeEventArray[i]->Trigger();
			}
		}
	}

	void Controller::SetEnable(bool enable)
	{
		m_bEnable = enable;
		if (!m_bEnable)
			mTriggerStop();
	}

	void Controller::AddTriggerBeginStart(TriggerAnimEventType::Handler handler)
	{
		mTriggerBeginStart += handler;
	}

	void Controller::AddTriggerStop(TriggerAnimEventType::Handler handler)
	{
		mTriggerStop += handler;
	}

	void Controller::AddTriggerStart(TriggerAnimEventType::Handler handler)
	{
		m_pTimeEventArray[0]->mTriggerAnimEvent += handler;
	}

	void Controller::AddTriggerEnd(TriggerAnimEventType::Handler handler)
	{
		m_pTimeEventArray[1]->mTriggerAnimEvent += handler;
	}

	/*************************************************************************************************/
	IMPLEMENT_RTTI(StartAnimEvent, TimeEvent);
	IMPLEMENT_INITIAL_BEGIN(StartAnimEvent)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(StartAnimEvent, TimeEvent)
	END_ADD_PROPERTY;
	StartAnimEvent::StartAnimEvent()
	{

	}

	StartAnimEvent::StartAnimEvent(Controller* pController)
		: TimeEvent(pController)
	{

	}

	IMPLEMENT_RTTI(EndAnimEvent, TimeEvent);
	IMPLEMENT_INITIAL_BEGIN(EndAnimEvent)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(EndAnimEvent, TimeEvent)
	END_ADD_PROPERTY;
	EndAnimEvent::EndAnimEvent()
	{
		mTraggerPercent = 1.0f;
	}

	EndAnimEvent::EndAnimEvent(Controller* pController)
		: TimeEvent(pController)
	{
		mTraggerPercent = 1.0f;
	}

}
