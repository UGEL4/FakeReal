#include "Animation.h"
#include "../Node/Model/Skeleton.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Stream/Property.h"
#include "../Stream/Stream.h"

namespace FakeReal {
	IMPLEMENT_RTTI(Animation, Object)
	BEGIN_ADD_PROPERTY(Animation, Object)
	REGISTER_PROPERTY(m_uiNumBones, "Bone counts", Property::F_SAVE_LOAD_CLONE | Property::F_REFLECT_NAME)
	REGISTER_PROPERTY(m_uiNumFrames, "Frames counts", Property::F_SAVE_LOAD_CLONE | Property::F_REFLECT_NAME)
	REGISTER_PROPERTY(m_fDuration, "Duration", Property::F_SAVE_LOAD_CLONE | Property::F_REFLECT_NAME)
	REGISTER_PROPERTY(m_fFrameDuration, "FrameDuration", Property::F_SAVE_LOAD_CLONE | Property::F_REFLECT_NAME)
	REGISTER_PROPERTY(m_fLength, "Animation Length", Property::F_SAVE_LOAD_CLONE | Property::F_REFLECT_NAME)
	REGISTER_PROPERTY(m_pBoneKeyArray, "BoneKeyArray", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY
	IMPLEMENT_INITIAL_BEGIN(Animation)
	IMPLEMENT_INITIAL_END

	Animation::Animation()
	{
		m_pBoneKeyArray.clear();
		std::cout << "¹¹ÔìAnimation" << std::endl;
	}

	Animation::~Animation()
	{
		for (BoneKey* p : m_pBoneKeyArray)
		{
			delete p;
		}
		std::cout << "Ïú»ÙAnimation" << std::endl;
	}

	void Animation::SetBoneKeyArray(const std::vector<BoneKey*>& pBoneKeyArray)
	{
		m_uiNumBones = pBoneKeyArray.size();
		m_uiNumFrames = pBoneKeyArray[0]->mTranslationArray.size();
		m_fLength = m_fDuration = m_uiNumFrames * m_fFrameDuration;
		m_pBoneKeyArray = pBoneKeyArray;
	}

	BoneKey* Animation::GetBoneKey(size_t index) const
	{
		if (index < m_pBoneKeyArray.size())
			return m_pBoneKeyArray[index];

		return nullptr;
	}

	void Animation::ComputeGlobalPoseAtTime(std::vector<glm::mat4>& outPoses, Skeleton* pSkeleton, float inTime) const
	{
		if (outPoses.size() != m_uiNumBones)
		{
			outPoses.resize(m_uiNumBones);
		}

		size_t frame = static_cast<size_t>(inTime / m_fFrameDuration);
		std::vector<glm::mat4> matArray;
		ComputePose(matArray, pSkeleton, frame);
		for (unsigned int i = 0; i < m_uiNumBones; i++)
		{
			outPoses[i] = matArray[i] * pSkeleton->GetBone(i)->GetInvBindPos();
		}
	}

	glm::mat4 Animation::Interpolate(const glm::mat4& a, const glm::mat4& b, float f)
	{
		glm::mat4 retVal(1.f);
		/*retVal.mRotation = Quaternion::Slerp(a.mRotation, b.mRotation, f);
		retVal.mTranslation = Vector3::Lerp(a.mTranslation, b.mTranslation, f);*/
		return retVal;
	}

	void Animation::ComputePose(std::vector<glm::mat4>& outPoses, Skeleton* pSkeleton, unsigned int uiFrame) const
	{
		outPoses.resize(m_uiNumBones);
		m_pBoneKeyArray[0]->GetMatrix(outPoses[0], uiFrame);
		for (size_t i = 1; i < m_uiNumBones; i++)
		{
			glm::mat4 mat;
			m_pBoneKeyArray[i]->GetMatrix(mat, uiFrame);
			int parentIndex = pSkeleton->GetBone(i)->GetParentIndex();
			outPoses[i] = outPoses[parentIndex] * mat;
		}
	}

/**************************************** BoneKey **********************************************/
	IMPLEMENT_RTTI(BoneKey, Object)
	IMPLEMENT_INITIAL_BEGIN(BoneKey)
	IMPLEMENT_INITIAL_END
	BEGIN_ADD_PROPERTY(BoneKey, Object)
	REGISTER_PROPERTY(mTranslationArray, "Translation", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mScaleArray, "Scale", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mRotationArray, "Rotation", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(mBoneName, "BoneName", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	BoneKey::BoneKey()
	{
		mTranslationArray.clear();
		mScaleArray.clear();
		mRotationArray.clear();
	}

	BoneKey::~BoneKey()
	{
		mTranslationArray.clear();
		mScaleArray.clear();
		mRotationArray.clear();
	}

	void BoneKey::GetMatrix(glm::mat4& outPose, unsigned int uiKey)
	{
		glm::mat4 trans(1.f);
		glm::mat4 scale(1.f);
		glm::mat4 rotate(1.f);
		if (uiKey < mTranslationArray.size())
		{
			trans = glm::translate(trans, mTranslationArray[uiKey].mVector);
		}
		if (uiKey < mScaleArray.size())
		{
			scale = glm::scale(scale, mScaleArray[uiKey].mVector);
		}
		if (uiKey < mRotationArray.size())
		{
			rotate = glm::mat4_cast(mRotationArray[uiKey].mQuat);
		}
		outPose = trans * rotate * scale;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI(AnimationSet, Object);
	IMPLEMENT_INITIAL_BEGIN(AnimationSet)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(AnimationSet, Object)
	REGISTER_PROPERTY(mAnimArray, "Animations", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY;

	AnimationSet::AnimationSet()
	{
		mAnimArray.clear();
	}

	AnimationSet::~AnimationSet()
	{
		for (auto& animSet : mAnimArray)
		{
			if (animSet.second)
			{
				delete animSet.second;
			}

		}
		mAnimArray.clear();
	}

	void AnimationSet::AddAnimation(const std::string& name, AnimationRPtr anim)
	{
		if (!anim)
			return;

		if (mAnimArray.find(name) == mAnimArray.end())
		{
			mAnimArray.emplace(name, anim);
		}
	}

	AnimationRPtr AnimationSet::GetAnimation(const std::string& name) const
	{
		std::unordered_map<std::string, AnimationRPtr>::const_iterator iter = mAnimArray.find(name);
		if (iter != mAnimArray.end())
		{
			return iter->second;
		}
		return nullptr;
	}

}
