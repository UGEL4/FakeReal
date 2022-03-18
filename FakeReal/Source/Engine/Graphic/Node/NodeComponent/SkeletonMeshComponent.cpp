#include "SkeletonMeshComponent.h"
#include "../../Resource/ResourceManager.h"
#include "../../Stream/Property.h"
#include "../../../Render/Shader/Shader.h"
//≤‚ ‘
#include "../../Stream/Stream.h"
#include "../../Anim/Animation.h"
namespace FakeReal {
	IMPLEMENT_RTTI(SkeletonMeshComponent, MeshNodeComponent);
	IMPLEMENT_INITIAL_BEGIN(SkeletonMeshComponent)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(SkeletonMeshComponent, MeshNodeComponent)
	REGISTER_PROPERTY(m_pRMeshNode, "SkeletonMeshNode Resource", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	SkeletonMeshComponent::SkeletonMeshComponent()
	{
		m_pRMeshNode = ResourceManager::ms_pDefaultSkeletonMeshNodeResource;
		mbUseDefaultMesh = true;
		m_pAnimation = nullptr;
	}

	SkeletonMeshComponent::~SkeletonMeshComponent()
	{
		if (!mbUseDefaultMesh)
			delete m_pRMeshNode;
		std::cout << "ŒˆππSkeletonMeshComponent:" << this << std::endl;
	}

	SkeletonMeshNode* SkeletonMeshComponent::GetSkeletonMeshNode()
	{
		if (!m_pRMeshNode)
			return nullptr;
		
		return m_pRMeshNode->GetResource();
	}

	void SkeletonMeshComponent::SetSkeletonMeshResource(SkeletonMeshNodeRPtr pResource)
	{
		if (pResource == m_pRMeshNode)
			return;
		m_pRMeshNode = pResource;
		mbUseDefaultMesh = false;
		PostCreate();
	}

	void SkeletonMeshComponent::PostCreate()
	{
		if (!m_pRMeshNode)
			return;
		LoadedEvent(nullptr);
		if (!m_pRMeshNode->IsLoaded())
		{
			m_pRMeshNode->AddLoadEventObject(this);
		}
	}

	void SkeletonMeshComponent::LoadedEvent(ResourceProxyBase* pResourceProxy, int data)
	{
		if (m_pRMeshNode)
		{
			if (m_pMeshNode)
			{
				m_pMeshNode->SetParent(nullptr);
				delete m_pMeshNode;
			}
			m_pMeshNode = (ModelMeshNode*)Object::CloneCreateObject(m_pRMeshNode->GetResource());
			//m_pMeshNode = m_pRMeshNode->GetResource();
			m_pMeshNode->SetParent(this);
			mbStatic = !m_pMeshNode->IsDynamic();
			mbUseDefaultMesh = false;
		}
	}

	void SkeletonMeshComponent::ComputeMatrixPalette()
	{
		if (m_pRMeshNode)
		{
			size_t size = 100;
			if (m_pRMeshNode->GetResource())
				size = m_pRMeshNode->GetResource()->GetSkeleton()->GetBoneNum();

			if (mMatrixPalette.size() != size)
			{
				mMatrixPalette.clear();
				mMatrixPalette.resize(size);
			}
			m_pAnimation->ComputeGlobalPoseAtTime(mMatrixPalette, m_pRMeshNode->GetResource()->GetSkeleton(), 0.0f);

			//std::vector<glm::mat4> outPos;
			//outPos.resize(size);
			//ComputeGlobalPos(outPos);
			//for (size_t i = 0; i < size; i++)
			//{
			//	//mMatrixPalette[i] = outPos[i] * m_pRMeshNode->GetResource()->GetSkeleton()->GetBone(i)->GetInvBindPos();
			//	mMatrixPalette[i] = glm::mat4(1.0f);
			//}
		}
	}

	void SkeletonMeshComponent::ComputeGlobalPos(std::vector<glm::mat4>& outPos)
	{
		size_t size = m_pRMeshNode->GetResource()->GetSkeleton()->GetBoneNum();
		outPos[0] = m_pRMeshNode->GetResource()->GetSkeleton()->GetBone(0)->GetLocalMatrix();
		//outPos[0] = glm::mat4(1.0f);
		for (size_t i = 1; i < size; i++)
		{
			glm::mat4 matrix(1.0f);
			matrix = m_pRMeshNode->GetResource()->GetSkeleton()->GetBone(i)->GetLocalMatrix();

			int parentIndex = m_pRMeshNode->GetResource()->GetSkeleton()->GetBone(i)->GetParentIndex();
			outPos[i] = outPos[parentIndex] * matrix;
		}
	}

	void SkeletonMeshComponent::TestPlayAnim(class Shader* pShader)
	{
		pShader->Bind();
		if (mMatrixPalette.size() > 0)
			pShader->SetMatrix4Array("uMatrixPalette", mMatrixPalette.size(), &mMatrixPalette[0][0][0]);
	}

	void SkeletonMeshComponent::SetAnimation()
	{
		Stream loadStream;
		loadStream.NewLoad("Resources/models/silly_dancing.fbx.animation.assert");
		m_pAnimation = (Animation*)loadStream.GetObjectByRtti(Animation::ms_Type);
	}

}