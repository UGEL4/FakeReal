#include "AnimFunction.h"
#include "../Stream/Property.h"
#include "../Node/Model/SkeletonMeshNode.h"
#include "../Node/Model/Skeleton.h"

namespace FakeReal
{
	IMPLEMENT_RTTI_NO_CREATE_FUNCTION(AnimFunction, Controller);
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(AnimFunction)
	IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END;
	BEGIN_ADD_PROPERTY(AnimFunction, Controller)
	END_ADD_PROPERTY;

	AnimFunction::AnimFunction()
	{
		mBoneOutPut.clear();
	}

	AnimFunction::~AnimFunction()
	{
		mBoneOutPut.clear();
	}

	bool AnimFunction::SetObject(Object* pObj)
	{
		if (Controller::SetObject(pObj))
		{
			const SkeletonMeshNode* pMesh = DynamicCast<SkeletonMeshNode>(m_pObject);
			assert(pMesh);

			Skeleton* pSkeleton = pMesh->GetSkeleton();
			assert(pSkeleton);

			size_t BoneNum = pSkeleton->GetBoneNum();
			mBoneOutPut.resize(BoneNum);
			for (size_t i = 0; i < BoneNum; i++)
			{
				mBoneOutPut[pSkeleton->GetBone(i)->GetIndex()].FromTransform(pSkeleton->GetBone(i)->GetLocalTransform());
			}
			return true;
		}
		return false;
	}

}
