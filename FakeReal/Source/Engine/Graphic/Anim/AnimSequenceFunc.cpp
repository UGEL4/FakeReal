#include "AnimSequenceFunc.h"
#include "../Stream/Property.h"
#include "../Node/Model/Skeleton.h"
#include "../Node/Model/SkeletonMeshNode.h"

namespace FakeReal
{
	IMPLEMENT_RTTI(AnimSequenceFunc, AnimFunction);
	IMPLEMENT_INITIAL_BEGIN(AnimSequenceFunc)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(AnimSequenceFunc, AnimFunction)
	END_ADD_PROPERTY;

	AnimSequenceFunc::AnimSequenceFunc()
	{
		m_pAnimR = nullptr;
		mLastKey.clear();
	}

	AnimSequenceFunc::AnimSequenceFunc(SkeletonMeshNode* pMeshNode)
	{
		m_pObject = pMeshNode;

		Skeleton* pSkeleton = pMeshNode->GetSkeleton();
		assert(pSkeleton);

		size_t BoneNum = pSkeleton->GetBoneNum();
		mBoneOutPut.resize(BoneNum);
		for (size_t i = 0; i < BoneNum; i++)
		{
			mBoneOutPut[pSkeleton->GetBone(i)->GetIndex()].FromTransform(pSkeleton->GetBone(i)->GetLocalTransform());
		}
		m_pAnimR = nullptr;
		mLastKey.resize(BoneNum);
		memset(&mLastKey[0], 0, sizeof(LAST_KEY_TYPE) * BoneNum);
		mRepeatType = Controller::RT_WRAP;
		mTriggerBeginStart += TriggerAnimEventType::Handler::FromMethod<AnimSequenceFunc, &AnimSequenceFunc::BeginStart>(this);
	}

	AnimSequenceFunc::~AnimSequenceFunc()
	{
		m_pAnimR = nullptr;
		mLastKey.clear();
	}

	void AnimSequenceFunc::SetAnimation(const std::string& AnimName)
	{
		const SkeletonMeshNode* pMesh = DynamicCast<SkeletonMeshNode>(m_pObject);
		if (!pMesh)
			return;

		Skeleton* pSkeleton = pMesh->GetSkeleton();
		if (!pSkeleton)
			return;

		const AnimationSet* pAnimSet = pMesh->GetAnimSet();
		if (!pAnimSet)
			return;

		AnimationR* pAnimR = pAnimSet->GetAnimation(AnimName);
		if (!pAnimR)
			return;
		if (m_pAnimR == pAnimR)
			return;
		m_pAnimR = pAnimR;
		mAnimName = AnimName;
		m_pAnimR->AddLoadEventObject(this);
		m_bStart = false;
	}

	void AnimSequenceFunc::LoadedEvent(ResourceProxyBase* pResourceProxy, int data)
	{
		const SkeletonMeshNode* pMesh = DynamicCast<SkeletonMeshNode>(m_pObject);
		assert(pMesh);
		if (!pMesh)
			return;

		Skeleton* pSkeleton = pMesh->GetSkeleton();
		assert(pSkeleton);
		if (!pSkeleton)
			return;

		Animation* pAnim = m_pAnimR->GetResource();
		//初始化最长时间
		m_dMaxTime = pAnim->GetAnimationLength();
	}

	bool AnimSequenceFunc::SetObject(Object* pObj)
	{
		if (AnimFunction::SetObject(pObj))
		{
			const SkeletonMeshNode* pMesh = DynamicCast<SkeletonMeshNode>(m_pObject);
			assert(pMesh);
			Skeleton* pSkeleton = pMesh->GetSkeleton();
			assert(pSkeleton);
			size_t BoneNum = pSkeleton->GetBoneNum();
			mLastKey.resize(BoneNum);
			memset(&mLastKey[0], 0, sizeof(LAST_KEY_TYPE) * BoneNum);
			SetAnimation(mAnimName);
			return true;
		}
		return false;
	}

	bool AnimSequenceFunc::UpdateEx(double appTime)
	{
		if (!AnimFunction::UpdateEx(appTime))
			return false;

		const SkeletonMeshNode* pMesh = DynamicCast<SkeletonMeshNode>(m_pObject);
		if (!pMesh)
			return false;

		Skeleton* pSkeleton = pMesh->GetSkeleton();
		if (!pSkeleton)
			return false;

		Animation* pAnim = m_pAnimR->GetResource();
		for (size_t i = 0; i < pSkeleton->GetBoneNum(); i++)
		{
			int BoneIndex = pSkeleton->GetBone(i)->GetIndex();
			BoneKey* pBoneKey = pAnim->GetBoneKey(BoneIndex);
			//缩放
			size_t ScaleNum = pBoneKey->mScaleArray.size();
			if (ScaleNum)
			{
				if (pBoneKey->mScaleArray[mLastKey[BoneIndex].LKScale].m_fKeyTime > m_dNowAnimTime)
				{
					mLastKey[BoneIndex].LKScale = 0;
				}
				//从上一帧开始查找
				size_t j = 0;
				for (j = mLastKey[BoneIndex].LKScale; j < ScaleNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->mScaleArray[j].m_fKeyTime)
					{
						break;
					}
				}
				if (j == ScaleNum)
				{
					j = ScaleNum - 1;
				}
				unsigned int Key1, Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				//记录上一帧
				mLastKey[BoneIndex].LKScale = Key1;
				//计算两帧之间的时间长度
				double Diff = pBoneKey->mScaleArray[Key2].m_fKeyTime - pBoneKey->mScaleArray[Key1].m_fKeyTime;
				if (Diff <= 0.0)
					Diff = 1.0;
				//计算插值因子
				float Factor = (float)((m_dNowAnimTime - pBoneKey->mScaleArray[Key1].m_fKeyTime) / Diff);
				if (Factor < 0.0f)
					Factor = 0.0f;
				if (Factor > 1.0f)
					Factor = 1.0f;
				//插值
				glm::vec3 Scale = pBoneKey->mScaleArray[Key1].mVector * (1 - Factor) + pBoneKey->mScaleArray[Key2].mVector * Factor;
				mBoneOutPut[BoneIndex].mScale = Scale;
			}
			//旋转
			size_t RotateNum = pBoneKey->mRotationArray.size();
			if (RotateNum)
			{
				if (pBoneKey->mRotationArray[mLastKey[BoneIndex].LKRotation].m_fKeyTime > m_dNowAnimTime)
				{
					mLastKey[BoneIndex].LKRotation = 0;
				}
				//从上一帧开始查找
				size_t j = 0;
				for (j = mLastKey[BoneIndex].LKRotation; j < RotateNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->mRotationArray[j].m_fKeyTime)
					{
						break;
					}
				}
				if (j == RotateNum)
				{
					j = RotateNum - 1;
				}
				unsigned int Key1, Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				//记录上一帧
				mLastKey[BoneIndex].LKRotation = Key1;
				//计算两帧之间的时间长度
				double Diff = pBoneKey->mRotationArray[Key2].m_fKeyTime - pBoneKey->mRotationArray[Key1].m_fKeyTime;
				if (Diff <= 0.0)
					Diff = 1.0;
				//计算插值因子
				float Factor = (float)((m_dNowAnimTime - pBoneKey->mRotationArray[Key1].m_fKeyTime) / Diff);
				if (Factor < 0.0f)
					Factor = 0.0f;
				if (Factor > 1.0f)
					Factor = 1.0f;
				//插值
				glm::quat Rotate = glm::slerp(pBoneKey->mRotationArray[Key1].mQuat, pBoneKey->mRotationArray[Key2].mQuat, Factor);
				mBoneOutPut[BoneIndex].mRotation = Rotate;
			}
			//平移
			size_t TransNum = pBoneKey->mTranslationArray.size();
			if (TransNum)
			{
				if (pBoneKey->mTranslationArray[mLastKey[BoneIndex].LKTranslation].m_fKeyTime > m_dNowAnimTime)
				{
					mLastKey[BoneIndex].LKTranslation = 0;
				}
				//从上一帧开始查找
				size_t j = 0;
				for (j = mLastKey[BoneIndex].LKTranslation; j < TransNum; j++)
				{
					if (m_dNowAnimTime <= pBoneKey->mTranslationArray[j].m_fKeyTime)
					{
						break;
					}
				}
				if (j == TransNum)
				{
					j = TransNum - 1;
				}
				unsigned int Key1, Key2;
				if (j == 0)
					Key1 = j;
				else
					Key1 = j - 1;
				Key2 = j;
				//记录上一帧
				mLastKey[BoneIndex].LKTranslation = Key1;
				//计算两帧之间的时间长度
				double Diff = pBoneKey->mTranslationArray[Key2].m_fKeyTime - pBoneKey->mTranslationArray[Key1].m_fKeyTime;
				if (Diff <= 0.0)
					Diff = 1.0;
				//计算插值因子
				float Factor = (float)((m_dNowAnimTime - pBoneKey->mTranslationArray[Key1].m_fKeyTime) / Diff);
				if (Factor < 0.0f)
					Factor = 0.0f;
				if (Factor > 1.0f)
					Factor = 1.0f;
				//插值
				glm::vec3 Trans = pBoneKey->mTranslationArray[Key1].mVector * (1 - Factor) + pBoneKey->mTranslationArray[Key2].mVector * Factor;
				mBoneOutPut[BoneIndex].mPosition = Trans;
			}
		}

		return true;
	}

	void AnimSequenceFunc::UpdateBone()
	{
		if (!m_bEnable)
			return;

		const SkeletonMeshNode* pMesh = DynamicCast<SkeletonMeshNode>(m_pObject);
		if (!pMesh)
			return;

		Skeleton* pSkeleton = pMesh->GetSkeleton();
		if (!pSkeleton)
			return;

		if (!m_pAnimR || !m_pAnimR->IsLoaded())
			return;

		Animation* pAnim = m_pAnimR->GetResource();
		for (size_t i = 0; i < pSkeleton->GetBoneNum(); i++)
		{
			Bone* pBone = pSkeleton->GetBone(i);
			if (pBone)
			{
				glm::mat4 matrix(1.0f);
				mBoneOutPut[pBone->GetIndex()].GetMatrix(matrix);
				pBone->SetLocalMat(matrix);
				//TODO:检查一下为什么使用下面的代码时帧数很低
				/*pBone->SetLocalTransform(
					mBoneOutPut[pBone->GetIndex()].mPosition,
					mBoneOutPut[pBone->GetIndex()].mRotation,
					mBoneOutPut[pBone->GetIndex()].mScale
				);*/
			}
		}
	}

	void AnimSequenceFunc::BeginStart()
	{

	}

}
