#include "Priority.h"

namespace FakeReal {

	Priority::Priority()
	{
		m_uiPriorityNum = 0;
	}

	Priority::Priority(unsigned int uiPriorityNum)
	{
		m_uiPriorityNum = uiPriorityNum;
	}

	Priority::~Priority()
	{
		mPriorityThenList.clear();
	}

	bool Priority::AddPriorityThen(Priority* pPriority)
	{
		if (!pPriority)
			return false;
		//环形依赖检测
		if (pPriority->CheckIsCircle(this))
		{
			return false;
		}
		m_uiPriorityNum = 0;
		mPriorityThenList.emplace_back(pPriority);
		return true;
	}

	unsigned int Priority::GetPriority()
	{
		CheckPriorityNum();
		return m_uiPriorityNum;
	}

	bool Priority::operator!=(Priority& priority)
	{
		priority.CheckPriorityNum();
		CheckPriorityNum();
		return m_uiPriorityNum != priority.m_uiPriorityNum;

	}

	bool Priority::operator==(Priority& priority)
	{
		priority.CheckPriorityNum();
		CheckPriorityNum();
		return m_uiPriorityNum == priority.m_uiPriorityNum;
	}

	bool Priority::operator>=(Priority& priority)
	{
		priority.CheckPriorityNum();
		CheckPriorityNum();
		return m_uiPriorityNum >= priority.m_uiPriorityNum;
	}

	bool Priority::operator<=(Priority& priority)
	{
		priority.CheckPriorityNum();
		CheckPriorityNum();
		return m_uiPriorityNum <= priority.m_uiPriorityNum;
	}

	bool Priority::operator>(Priority& priority)
	{
		priority.CheckPriorityNum();
		CheckPriorityNum();
		return m_uiPriorityNum > priority.m_uiPriorityNum;
	}

	bool Priority::operator<(Priority& priority)
	{
		priority.CheckPriorityNum();
		CheckPriorityNum();
		return m_uiPriorityNum < priority.m_uiPriorityNum;
	}

	Priority& Priority::operator=(Priority& priority)
	{
		if (&priority == this)
			return *this;
		m_uiPriorityNum = priority.CheckPriorityNum();
		CheckPriorityNum();
		mPriorityThenList.clear();
		mPriorityThenList = priority.mPriorityThenList;
		return *this;
	}

	unsigned int Priority::CheckPriorityNum()
	{
		if (m_uiPriorityNum || mPriorityThenList.size() == 0)
			return m_uiPriorityNum;
		unsigned int uiMax = 0;
		for (size_t i = 0; i < mPriorityThenList.size(); i++)
		{
			unsigned int num = mPriorityThenList[i]->CheckPriorityNum();
			if (uiMax < num)
				uiMax = num;
		}
		m_uiPriorityNum = uiMax + 1;//自己的优先级等于自已依赖的所有类中最大的优先级+1
		return m_uiPriorityNum;
	}

	bool Priority::CheckIsCircle(const Priority* pPriority) const
	{
		if (!pPriority)
			return false;

		for (size_t i = 0; i < mPriorityThenList.size(); i++)
		{
			if (pPriority == mPriorityThenList[i])
				return true;
			else
				return mPriorityThenList[i]->CheckIsCircle(pPriority);
		}

		return false;
	}

}
