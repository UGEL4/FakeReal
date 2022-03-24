#include "SkeletonMeshNode.h"
#include "../GeometryNode.h"
#include "../../Anim/Animation.h"
#include "../../Anim/AnimSequenceFunc.h"
#include "../../Stream/Property.h"

namespace FakeReal {
	SkeletonMeshNode* SkeletonMeshNode::m_pDefault = nullptr;
	IMPLEMENT_RTTI(SkeletonMeshNode, ModelMeshNode);
	IMPLEMENT_INITIAL_BEGIN(SkeletonMeshNode)
	ADD_PRIORITY(Geometry)
	ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
	ADD_TERMINATE_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(SkeletonMeshNode, ModelMeshNode)
	REGISTER_PROPERTY(m_pSkeleton, "Skeleton", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	SkeletonMeshNode::SkeletonMeshNode()
	{
		m_pSkeleton = nullptr;
		m_pAnimSet = nullptr;
		m_pAnimSequence = nullptr;
	}

	SkeletonMeshNode::~SkeletonMeshNode()
	{
		if (m_pAnimSequence)
			delete m_pAnimSequence;
		m_pAnimSequence = nullptr;
		//TODO:delete ptr
		std::cout << "析构SkeletonMeshNode:" << this << std::endl;
	}

	void SkeletonMeshNode::UpdateAll(float appTime)
	{
		ModelMeshNode::UpdateAll(appTime);
	}

	void SkeletonMeshNode::UpdateNodeAll(float appTime)
	{
		if (appTime > 0.0f)
		{
			UpdateController(appTime);
		}
		UpdateTransform(appTime);

		if (m_pSkeleton)
		{
			m_pSkeleton->UpdateNodeAll(appTime);
		}

		for (size_t i = 0; i < m_pChildList.size(); i++)
		{
			if (m_pChildList[i])
				m_pChildList[i]->UpdateNodeAll(appTime);
		}

		if (mbIsChange)
		{
		}
		mbIsChange = false;
	}

	bool SkeletonMeshNode::InitialDefaultState()
	{
		m_pDefault = new SkeletonMeshNode();
		GeometryNode* pGeometryNode = new GeometryNode();
		m_pDefault->AddChild(pGeometryNode);
		//TODO:这里要克隆
		Geometry* pGeometry = (Geometry*)Object::CloneCreateObject(Geometry::GetDefaultRenderCube());
		//Geometry* pGeometry = Geometry::GetDefaultRenderCube();
		pGeometryNode->AddChild(pGeometry);
		//m_pDefault->CreateLocalAABB();
		pGeometryNode->SetLocalScale({ 1.0f, 1.0f, 1.0f });
		return true;
	}

	bool SkeletonMeshNode::TerminalDefaultState()
	{
		delete m_pDefault;
		m_pDefault = nullptr;
		return true;
	}

	void SkeletonMeshNode::SetSkeleton(Skeleton* pSkeleton)
	{
		if (pSkeleton)
		{
			if (m_pSkeleton)
			{
				m_pSkeleton->SetParent(nullptr);
				delete m_pSkeleton;
			}
			m_pSkeleton = pSkeleton;
			m_pSkeleton->SetParent(this);
		}
	}

	void SkeletonMeshNode::SetAnimationSet(AnimationSet* pAnimSet)
	{
		if (m_pAnimSet == pAnimSet)
			return;
		m_pAnimSet = pAnimSet;
	}

	bool SkeletonMeshNode::PlayAnimation(const std::string& AnimName, float Ratio, unsigned int RepeatType)
	{
		if (m_pAnimSequence == nullptr)
		{
			m_pAnimSequence = new AnimSequenceFunc(this);
			mbStatic = false;
		}
		m_pAnimSequence->m_bEnable = true;
		m_pAnimSequence->SetAnimation(AnimName);
		m_pAnimSequence->mRepeatType = RepeatType;
		m_pAnimSequence->m_dFrequency = Ratio;
		return true;
	}

	void SkeletonMeshNode::StopAnimation()
	{
		if (m_pAnimSequence)
		{
			m_pAnimSequence->m_bEnable = false;
		}
	}

	void SkeletonMeshNode::UpdateController(float appTime)
	{
		ModelMeshNode::UpdateController(appTime);
		if (m_pAnimSequence && m_pAnimSequence->m_bEnable)
		{
			m_pAnimSequence->Update(appTime);
			m_pAnimSequence->UpdateBone();
		}
	}

}