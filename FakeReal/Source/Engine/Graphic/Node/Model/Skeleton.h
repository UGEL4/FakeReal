#pragma once

#include "../../../Core/CoreMarco.h"
#include "../Node.h"
#include "Bone.h"

namespace FakeReal {
	class FR_ENGINE_API Skeleton : public Node
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		Skeleton();
		~Skeleton();

		size_t GetBoneNum() const { return mBoneArray.size(); }
		Bone* GetBone(unsigned int index);
		Bone* GetBone(const std::string& name);
		std::vector<Bone*>& GetBoneArray() { return mBoneArray; }
		void CreateBoneArray();
	private:
		std::vector<Bone*> mBoneArray;
	};
	FR_TYPE_MARCO(Skeleton)
}
