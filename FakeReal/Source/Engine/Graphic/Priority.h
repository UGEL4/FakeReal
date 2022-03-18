#pragma once
/*
	优先级类， 用于初始化静态成员变量时的优先级判断，解决类静态成员变量之间的初始化依赖关系。
*/

#include <string>
#include <vector>
#include "../Core/CoreMarco.h"

namespace FakeReal {
	class FR_ENGINE_API Priority
	{
	public:
		Priority();
		Priority(unsigned int uiPriorityNum);
		~Priority();

		bool AddPriorityThen(Priority* pPriority);
		unsigned int GetPriority();

		Priority& operator = (Priority& priority);
		bool operator < (Priority& priority);
		bool operator > (Priority& priority);
		bool operator <= (Priority& priority);
		bool operator >= (Priority& priority);
		bool operator == (Priority& priority);
		bool operator != (Priority& priority);
	private:
		unsigned int CheckPriorityNum();
		bool CheckIsCircle(const Priority* pPriority) const;
		unsigned int m_uiPriorityNum;
		std::vector<Priority*> mPriorityThenList;
	};

#define DECLARE_PRIORITY\
	public:\
		static Priority ms_Priority;

#define IMPLEMENT_PRIORITY(classname)\
	Priority classname::ms_Priority;

#define ADD_PRIORITY(classname)\
	if (!ms_Priority.AddPriorityThen(&classname::ms_Priority))\
		return false;
}