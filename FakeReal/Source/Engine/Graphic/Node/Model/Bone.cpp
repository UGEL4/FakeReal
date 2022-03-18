#include "Bone.h"
#include "../../Stream/Property.h"

namespace FakeReal {
	IMPLEMENT_RTTI(Bone, Node);
	IMPLEMENT_INITIAL_BEGIN(Bone)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(Bone, Node)
	REGISTER_PROPERTY(mInvBindPos, "InvBindPos", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mIndex, "Bone Index", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mName, "Name", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY;

	Bone::Bone()
	{
		mIndex = -1;
	}

	Bone::Bone(const std::string& name, int index)
	{
		mName = name;
		mIndex = index;
	}

	Bone::~Bone()
	{

	}

	int Bone::GetParentIndex() const
	{
		if (m_pParent)
		{
			Bone* pParent = DynamicCast<Bone>(m_pParent);
			if (pParent)
				return pParent->GetIndex();
		}
		return -1;
	}

	void Bone::GetAllBoneArray(std::vector<Bone*>& BoneArray)
	{
		BoneArray.emplace_back(this);
		for (size_t i = 0; i < m_pChildList.size(); i++)
		{
			Bone* pBone = DynamicCast<Bone>(m_pChildList[i]);
			if (pBone)
				pBone->GetAllBoneArray(BoneArray);
		}
	}

	void Bone::SetLocalMat(const glm::mat4& matrix)
	{
		mbIsChange = true;
		mLocalTransform.SetMatrix(matrix);
	}

}
