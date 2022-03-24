#pragma once
#include "ModelMeshNode.h"
#include "Skeleton.h"

namespace FakeReal {
	class AnimationSet;
	class AnimSequenceFunc;
	class FR_ENGINE_API SkeletonMeshNode : public ModelMeshNode
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		SkeletonMeshNode();
		~SkeletonMeshNode();

		virtual void UpdateAll(float appTime) override;
		virtual void UpdateNodeAll(float appTime) override;

		virtual unsigned int GetResourceType() const override { return RT_SKELETON_MODEL; }
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		static const SkeletonMeshNode* GetDefault()
		{ 
			return m_pDefault; 
		}
		Skeleton* GetSkeleton() const { return m_pSkeleton; }
		void SetSkeleton(Skeleton* pSkeleton);
		void SetAnimationSet(AnimationSet* pAnimSet);
		AnimationSet* GetAnimSet() const { return m_pAnimSet; }
		bool PlayAnimation(const std::string& AnimName, float Ratio, unsigned int RepeatType);
		void StopAnimation();
		virtual void UpdateController(float appTime) override;
	private:
		Skeleton* m_pSkeleton;
		static SkeletonMeshNode* m_pDefault;
		AnimationSet* m_pAnimSet;
		AnimSequenceFunc* m_pAnimSequence;
	};
	FR_TYPE_MARCO(SkeletonMeshNode)
	DECLARE_Proxy(SkeletonMeshNode)
}