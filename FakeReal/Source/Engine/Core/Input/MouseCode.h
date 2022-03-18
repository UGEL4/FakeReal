#pragma once

#include <stdint.h>
namespace FakeReal {
	using MouseCode = uint16_t;
	enum Mouse: MouseCode
	{
		// Layout From glfw3.h
		MC_BUTTON_LEFT,
		MC_BUTTON_RIGHT,
		MC_BUTTON_MIDDLE,
		MC_BUTTON4,
		MC_BUTTON5,
		MC_BUTTON6,
		MC_BUTTON7,
		MC_BUTTON_LAST,

		MC_MAX,
	};
}