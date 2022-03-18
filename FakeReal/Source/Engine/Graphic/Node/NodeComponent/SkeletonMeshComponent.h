#pragma once
#include "MeshNodeComponent.h"
#include "../Model/SkeletonMeshNode.h"

namespace FakeReal {
	class Animation;
	class FR_ENGINE_API SkeletonMeshComponent : public MeshNodeComponent
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		SkeletonMeshComponent();
		~SkeletonMeshComponent();

		SkeletonMeshNode* GetSkeletonMeshNode();
		void SetSkeletonMeshResource(SkeletonMeshNodeRPtr pResource);

		virtual void PostCreate() override;
		virtual void LoadedEvent(ResourceProxyBase* pResourceProxy, int data = 0) override;

		//≤‚ ‘
		void ComputeMatrixPalette();
		void ComputeGlobalPos(std::vector<glm::mat4>& outPos);
		void TestPlayAnim(class Shader* pShader);
		void SetAnimation();

	private:
		SkeletonMeshNodeRPtr m_pRMeshNode;
		bool mbUseDefaultMesh;
		//≤‚ ‘
		std::vector<glm::mat4> mMatrixPalette;
		Animation* m_pAnimation;
	};
	FR_TYPE_MARCO(SkeletonMeshComponent)

}