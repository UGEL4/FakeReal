#include "Skeleton.h"
#include "../../Stream/Property.h"

namespace FakeReal{
	IMPLEMENT_RTTI(Skeleton, Node);
	IMPLEMENT_INITIAL_BEGIN(Skeleton)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(Skeleton, Node)
	REGISTER_PROPERTY(mBoneArray, "BoneArray", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY;

	Skeleton::Skeleton()
	{
		mBoneArray.clear();
	}

	Skeleton::~Skeleton()
	{

	}

	Bone* Skeleton::GetBone(unsigned int index)
	{
		if (index < mBoneArray.size())
			return mBoneArray[index];

		return nullptr;
	}

	Bone* Skeleton::GetBone(const std::string& name)
	{
		for (size_t i = 0; i < mBoneArray.size(); i++)
		{
			if (name == mBoneArray[i]->GetName())
				return mBoneArray[i];
		}
		return nullptr;
	}

	void Skeleton::CreateBoneArray()
	{
		mBoneArray.clear();
		std::vector<Bone*> Temp;
		Temp.clear();
		for (size_t i = 0; i < m_pChildList.size(); i++)
		{
			Bone* pBone = DynamicCast<Bone>(m_pChildList[i]);
			if (pBone)
				pBone->GetAllBoneArray(Temp);
		}

		mBoneArray.resize(Temp.size());
		for (size_t i = 0; i < Temp.size(); i++)
		{
			mBoneArray[Temp[i]->GetIndex()] = Temp[i];
		}
	}

}
