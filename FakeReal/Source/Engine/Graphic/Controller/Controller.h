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
		//appTime是动画时间而非引擎的时间
		virtual bool UpdateEx(double appTime);

		enum RepeatType
		{
			RT_NONE,
			RT_CLAMP,//正向时间阈值控制
			RT_WRAP,//正向时间循环控制
			RT_CYCLE,//正向时间来回控制a->b b->a
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
		//添加动画开始事件
		void AddTriggerBeginStart(TriggerAnimEventType::Handler handler);
		//添加动画停止事件
		void AddTriggerStop(TriggerAnimEventType::Handler handler);
		//添加动画起始事件
		void AddTriggerStart(TriggerAnimEventType::Handler handler);
		//添加动画结束事件
		void AddTriggerEnd(TriggerAnimEventType::Handler handler);
	protected:
		unsigned int mRepeatType;
		double m_dMinTime;//最短动画播放时间
		double m_dMaxTime;//动画播放的最长时间
		double m_dPhase;//动画播放中的位移
		double m_dFrequency;//动画播放的频率
		Object* m_pObject;//控制的对象
		double m_dNowAnimTime;//当前动画时间
		double m_dLastEngineTime;//上帧引擎时间
		double m_dLastAnimTime;//上帧动画时间
		double m_dIntervalTime;//当前引擎时间间隔
		double m_dIntervalAnimTime;//当前动画时间间隔
		double m_dTimeSum;//运行动画时间长度
		double m_dStartSystemTime;//开始的游戏引擎时间
		bool m_bStart;//是否已经开始运行
		bool m_bEnable;
		TriggerAnimEventType mTriggerBeginStart;
		TriggerAnimEventType mTriggerStop;
		std::vector<TimeEvent*> m_pTimeEventArray;
	};
}
