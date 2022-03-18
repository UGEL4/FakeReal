#pragma once
/*
	���ȼ��࣬ ���ڳ�ʼ����̬��Ա����ʱ�����ȼ��жϣ�����ྲ̬��Ա����֮��ĳ�ʼ��������ϵ��
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