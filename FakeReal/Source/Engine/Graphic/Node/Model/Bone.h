#pragma once

#include "../../../Core/CoreMarco.h"
#include "../Node.h"

namespace FakeReal {
	class FR_ENGINE_API Bone : public Node
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		Bone();
		Bone(const std::string& name, int index);
		~Bone();

		void SetInvBindPos(const glm::mat4& matrix) { mInvBindPos = matrix; }
		const glm::mat4& GetInvBindPos() const { return mInvBindPos; }
		int GetIndex() const { return mIndex; }
		int GetParentIndex() const;
		const std::string& GetName() const { return mName; }
		void SetName(const std::string& name) { mName = name; }
		void GetAllBoneArray(std::vector<Bone*>& BoneArray);
		void SetLocalMat(const glm::mat4& matrix);
	private:
		glm::mat4 mInvBindPos;
		int mIndex;
		std::string mName;
	};
	FR_TYPE_MARCO(Bone)
}
