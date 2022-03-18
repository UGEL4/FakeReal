#pragma once

#include "../Object.h"
#include "../Resource/Resource.h"
#include "../../Core/Events/Delegate.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace FakeReal {
	class FR_ENGINE_API KeyTimeInfo
	{
	public:
		KeyTimeInfo() {}
		~KeyTimeInfo() {}
		float m_fKeyTime;
	};

	class FR_ENGINE_API VectorKeyTimeInfo : public KeyTimeInfo
	{
	public:
		VectorKeyTimeInfo() {}
		~VectorKeyTimeInfo() {}
		glm::vec3 mVector;
	};

	class FR_ENGINE_API QuaternionKeyTimeInfo : public KeyTimeInfo
	{
	public:
		QuaternionKeyTimeInfo() {}
		~QuaternionKeyTimeInfo() {}
		glm::quat mQuat;
	};

	class FR_ENGINE_API BoneKey : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL

	public:
		BoneKey();
		~BoneKey();

		std::vector<VectorKeyTimeInfo> mTranslationArray;
		std::vector<VectorKeyTimeInfo> mScaleArray;
		std::vector<QuaternionKeyTimeInfo> mRotationArray;

		void GetMatrix(glm::mat4& outPose, unsigned int uiKey);
	};
	FR_TYPE_MARCO(BoneKey)

	typedef DelegateEvent<void> AddAnimEventType;
	class Skeleton;
	class FR_ENGINE_API Animation : public Object, public Resource
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		Animation();
		~Animation();

		virtual unsigned int GetResourceType() const
		{
			return RT_ANIMATION;
		}

		unsigned int GetNumBones() const { return m_uiNumBones; }
		unsigned int GetNumFrames() const { return m_uiNumFrames; }
		float GetDuration() const { return m_fDuration; }
		float GetFrameDuration() const { return m_fFrameDuration; }
		float GetAnimationLength() const { return m_fLength; }
		void SetBoneKeyArray(const std::vector<BoneKey*>& pBoneKeyArray);

		// Fills the provided vector with the global (current) pose matrices for each
		// bone at the specified time in the animation. It is expected that the time
		// is >= 0.0f and <= mDuration
		//void GetGlobalPoseAtTime(std::vector<glm::mat4>& outPoses, const Skeleton* inSkeleton, float inTime) const;
		void ComputeGlobalPoseAtTime(std::vector<glm::mat4>& outPoses, Skeleton* pSkeleton, float inTime) const;

		glm::mat4 Interpolate(const glm::mat4& a, const glm::mat4& b, float f);
	private:
		void ComputePose(std::vector<glm::mat4>& outPoses, Skeleton* pSkeleton, unsigned int uiFrame) const;
		// Number of bones for the animation
		unsigned int m_uiNumBones;
		// Number of frames in the animation
		unsigned int m_uiNumFrames;
		// Duration of the animation in seconds
		float m_fDuration;
		// Duration of each frame in the animation
		float m_fFrameDuration = 1.0f / 30.0f;
		//Animation length
		float m_fLength;
		std::vector<BoneKey*> m_pBoneKeyArray;
	};
	FR_TYPE_MARCO(Animation)
	DECLARE_Proxy(Animation)

	class FR_ENGINE_API AnimationSet : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		AnimationSet();
		~AnimationSet();

		void AddAnimation(const std::string& name, AnimationRPtr anim);
		AnimationRPtr GetAnimation(const std::string& name) const;
	private:
		std::unordered_map<std::string, AnimationRPtr> mAnimArray;

	public:
		AddAnimEventType m_AddAnimEvent;
	};
}
