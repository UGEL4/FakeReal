#include "Controller.h"

namespace FakeReal
{

	Controller::Controller()
	{
		mRepeatType = RT_NONE;
		m_dMinTime = 0.0;
		m_dMaxTime = 0.0;
		m_dPhase = 0.0;
		m_dFrequency = 0.0;
		m_pObject = nullptr;
		m_dNowAnimTime = 0.0;
		m_dLastEngineTime = 0.0;
		m_dLastAnimTime = 0.0;
		m_dIntervalTime = 0.0;
		m_dIntervalAnimTime = 0.0;
		m_dTimeSum = 0.0;
		m_dStartEngineTime = 0.0;
		m_bStart = false;
	}

	Controller::~Controller()
	{
		m_pObject = nullptr;
	}

	bool Controller::Update(double ts)
	{

	}

	bool Controller::UpdateEx(double ts)
	{

	}

	FakeReal::Object* Controller::GetObject() const
	{

	}

	bool Controller::SetObject(Object* pObj)
	{

	}

	double Controller::GetControlTime(double dAppTime)
	{
		//m_dFrequency调节动画时间的快慢，m_dPhase调节动画时间的平移
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

		//计算动画时间长度
		double Range = m_dMaxTime - m_dMinTime;
		if (Range > 0.0)
		{
			//计算出动画时间长度的倍数
			double Multiples = (CtrlTime - m_dMinTime) / Range;
			//取整
			double IntTime = floor(Multiples);
			//取小数部分
			double FrcTime = Multiples - IntTime;
			if (mRepeatType == RT_WRAP)
			{
				return m_dMinTime + FrcTime * Range;
			}
			if (mRepeatType == RT_CYCLE)
			{
				//整数部分为奇数
				if (int(IntTime) & 1)
				{
					//动画从后向前播放
					return m_dMaxTime - FrcTime * Range;
				}
				else
				{
					//动画从前向后播放
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

	}

}
