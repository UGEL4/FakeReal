#pragma once
#include "AnimFunction.h"
#include "Animation.h"

namespace FakeReal
{
	class SkeletonMeshNode;
	class FR_ENGINE_API AnimSequenceFunc : public AnimFunction
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		AnimSequenceFunc();
		~AnimSequenceFunc();

		void SetAnimation(const std::string& AnimName);
		virtual void LoadedEvent(ResourceProxyBase* pResourceProxy, int data = 0) override;
		
		virtual bool SetObject(Object* pObj) override;
		virtual bool UpdateEx(double appTime) override;

		friend class SkeletonMeshNode;

		void BeginStart();
	public:
		void UpdateBone();
		AnimSequenceFunc(SkeletonMeshNode* pMeshNode);

		AnimationR* m_pAnimR;
		std::string mAnimName;

		struct LAST_KEY_TYPE
		{
			LAST_KEY_TYPE()
			{
				LKTranslation = 0;
				LKScale = 0;
				LKRotation = 0;
			}
			unsigned int LKTranslation;//上一帧的索引
			unsigned int LKScale;
			unsigned int LKRotation;
		};
		std::vector<LAST_KEY_TYPE> mLastKey;
	};
}
