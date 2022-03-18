#pragma once
#include "ModelMeshNode.h"
#include "Skeleton.h"

namespace FakeReal {
	class FR_ENGINE_API SkeletonMeshNode : public ModelMeshNode
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		SkeletonMeshNode();
		~SkeletonMeshNode();

		virtual unsigned int GetResourceType() const override { return RT_SKELETON_MODEL; }
	public:
		static bool InitialDefaultState();
		static bool TerminalDefaultState();
		static const SkeletonMeshNode* GetDefault()
		{ 
			return m_pDefault; 
		}
		Skeleton* GetSkeleton() const { return m_pSkeleton; }
		void SetSkeleton(Skeleton* pSkeleton) { m_pSkeleton = pSkeleton; }
	private:
		Skeleton* m_pSkeleton;
		static SkeletonMeshNode* m_pDefault;
	};
	FR_TYPE_MARCO(SkeletonMeshNode)
	DECLARE_Proxy(SkeletonMeshNode)
}