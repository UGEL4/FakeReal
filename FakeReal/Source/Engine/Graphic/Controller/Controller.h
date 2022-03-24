#pragma once
#include "../Object.h"
#include "TimeEvent.h"

namespace FakeReal
{
	class Controller;
	class FR_ENGINE_API StartAnimEvent : public TimeEvent
	{
		DECLARE_RTTI
		DECLARE_INITIAL
		StartAnimEvent();
		StartAnimEvent(Controller* pController);
	};
	FR_TYPE_MARCO(StartAnimEvent)

	class FR_ENGINE_API EndAnimEvent : public TimeEvent
	{
		DECLARE_RTTI
		DECLARE_INITIAL
		EndAnimEvent();
		EndAnimEvent(Controller* pController);
	};
	FR_TYPE_MARCO(EndAnimEvent)

	class FR_ENGINE_API Controller :public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL_NO_CLASS_FACTORY
	public:
		Controller();
		virtual ~Controller();

		virtual bool Update(double appTime);
		//appTime�Ƕ���ʱ����������ʱ��
		virtual bool UpdateEx(double appTime);

		enum RepeatType
		{
			RT_NONE,
			RT_CLAMP,//����ʱ����ֵ����
			RT_WRAP,//����ʱ��ѭ������
			RT_CYCLE,//����ʱ�����ؿ���a->b b->a
			RT_MAX
		};

		Object* GetObject() const;
		virtual bool SetObject(Object* pObj);
		double GetControlTime(double dAppTime);
		void ClearTime();
		void AddTimeEvent(TimeEvent* pEvent);
		void DeleteTimeEvent(TimeEvent* pEvent);
		void TimeEventFunc(double appTime);
		void SetEnable(bool enable);
		void SetFrequency(double f) { m_dFrequency = f; }
		//��Ӷ�����ʼ�¼�
		void AddTriggerBeginStart(TriggerAnimEventType::Handler handler);
		//��Ӷ���ֹͣ�¼�
		void AddTriggerStop(TriggerAnimEventType::Handler handler);
		//��Ӷ�����ʼ�¼�
		void AddTriggerStart(TriggerAnimEventType::Handler handler);
		//��Ӷ��������¼�
		void AddTriggerEnd(TriggerAnimEventType::Handler handler);
	protected:
		unsigned int mRepeatType;
		double m_dMinTime;//��̶�������ʱ��
		double m_dMaxTime;//�������ŵ��ʱ��
		double m_dPhase;//���������е�λ��
		double m_dFrequency;//�������ŵ�Ƶ��
		Object* m_pObject;//���ƵĶ���
		double m_dNowAnimTime;//��ǰ����ʱ��
		double m_dLastEngineTime;//��֡����ʱ��
		double m_dLastAnimTime;//��֡����ʱ��
		double m_dIntervalTime;//��ǰ����ʱ����
		double m_dIntervalAnimTime;//��ǰ����ʱ����
		double m_dTimeSum;//���ж���ʱ�䳤��
		double m_dStartSystemTime;//��ʼ����Ϸ����ʱ��
		bool m_bStart;//�Ƿ��Ѿ���ʼ����
		bool m_bEnable;
		TriggerAnimEventType mTriggerBeginStart;
		TriggerAnimEventType mTriggerStop;
		std::vector<TimeEvent*> m_pTimeEventArray;
	};
}
