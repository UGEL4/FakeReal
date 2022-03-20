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
			RT_CLAMP,//����ʱ����ֵ����
			RT_WRAP,//����ʱ��ѭ������
			RT_CYCLE,//����ʱ�����ؿ���a->b b->a
			RT_MAX
		};

		Object* GetObject() const;
		virtual bool SetObject(Object* pObj);
		double GetControlTime(double dAppTime);
		void ClearTime();
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
		double m_dStartEngineTime;//��ʼ����Ϸ����ʱ��
		bool m_bStart;//�Ƿ��Ѿ���ʼ����
	};
}
