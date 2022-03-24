#pragma once

#include "../Controller/Controller.h"
#include "AnimAtom.h"

namespace FakeReal
{
	class FR_ENGINE_API AnimFunction : public Controller
	{
		DECLARE_RTTI
		DECLARE_INITIAL_NO_CLASS_FACTORY

		virtual ~AnimFunction() = 0;
		virtual bool SetObject(Object* pObj) override;

	protected:
		AnimFunction();

	public:
		std::vector<AnimAtom> mBoneOutPut;
	};
	FR_TYPE_MARCO(AnimFunction)
}
