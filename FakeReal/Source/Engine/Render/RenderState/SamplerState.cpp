#include "SamplerState.h"
#include "../../Graphic/Stream/Property.h"
#include "../../Graphic/Stream/EnumTrans.h"

namespace FakeReal
{
	IMPLEMENT_RTTI(SamplerState, Object);
	IMPLEMENT_INITIAL_BEGIN(SamplerState)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(SamplerState, Object)
	REGISTER_PROPERTY(mSamplerDesc, "SamplerDesc", Property::F_SAVE_LOAD_CLONE)
	END_ADD_PROPERTY

	SamplerState::SamplerState()
	{
		std::cout << "¹¹ÔìSamplerState:" << this << std::endl;
	}

	SamplerState::~SamplerState()
	{
		std::cout << "Îö¹¹SamplerState:" << this << std::endl;
	}

	IMPLEMENT_RTTI(SamplerDesc, Object);
	IMPLEMENT_INITIAL_BEGIN(SamplerDesc)
	IMPLEMENT_INITIAL_END;
	BEGIN_ADD_PROPERTY(SamplerDesc, Object)
	REGISTER_ENUM_PROPERTY(CoordMode_U, "CoordinateMode", "CoordMode_U", Property::F_SAVE_LOAD_CLONE)
	REGISTER_ENUM_PROPERTY(CoordMode_V, "CoordinateMode", "CoordMode_V", Property::F_SAVE_LOAD_CLONE)
	REGISTER_ENUM_PROPERTY(CoordMode_W, "CoordinateMode", "CoordMode_W", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(BorderColor, "BorderColor", Property::F_SAVE_LOAD_CLONE)
	REGISTER_ENUM_PROPERTY(FliterMode_Min, "FliterMode", "FliterMode_Min", Property::F_SAVE_LOAD_CLONE)
	REGISTER_ENUM_PROPERTY(FliterMode_Mag, "FliterMode", "FliterMode_Mag", Property::F_SAVE_LOAD_CLONE)
	REGISTER_ENUM_PROPERTY(FliterMode_Mip, "FliterMode", "FliterMode_Mip", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(MipLevel, "MipLevel", Property::F_SAVE_LOAD_CLONE)
	REGISTER_PROPERTY(AniLevel, "AnisotropicLevel", Property::F_SAVE_LOAD_CLONE)
	/*BEGIN_ADD_ENUM
	ADD_ENUM(CoordinateMode, CM_CLAMP)
	ADD_ENUM(CoordinateMode, CM_REPEAT)
	ADD_ENUM(CoordinateMode, CM_MIRRORED)
	ADD_ENUM(CoordinateMode, CM_BORDER)
	ADD_ENUM(FliterMode, FM_NONE)
	ADD_ENUM(FliterMode, FM_NEAREST)
	ADD_ENUM(FliterMode, FM_LINER)
	ADD_ENUM(FliterMode, FM_ANISOTROPIC)
	END_ADD_ENUM*/
	END_ADD_PROPERTY

}