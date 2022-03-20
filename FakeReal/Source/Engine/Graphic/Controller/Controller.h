#pragma once
#include "../Object.h"

namespace FakeReal
{
	class FR_ENGINE_API Controller :public Object
	{
	public:
		Controller();
		virtual ~Controller();

		bool Update(double ts);
		virtual bool UpdateEx(double ts);

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
		double m_dStartEngineTime;//开始的游戏引擎时间
		bool m_bStart;//是否已经开始运行
	};
}
