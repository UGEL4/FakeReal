#pragma once
#include "../../Core/CoreMarco.h"

namespace FakeReal {
	class FR_ENGINE_API ResourceIdentifier
	{
	public:
		virtual ~ResourceIdentifier() {}

		//virtual void Bind(unsigned int slot = 0) = 0;
	protected:
		ResourceIdentifier() {}
	};
}