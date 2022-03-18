#pragma once

#include "../../Graphic/Object.h"
#include "../../Core/FRMath.h"
#include "../../Core/CoreMarco.h"
namespace FakeReal
{
	class FR_ENGINE_API SamplerDesc : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		enum class CoordinateMode : unsigned char
		{
			CM_CLAMP,
			CM_REPEAT, //D3D WRAP
			CM_MIRRORED,
			CM_BORDER,
			CM_MAX
		};
		enum class FliterMode : unsigned char
		{
			FM_NONE,
			FM_NEAREST,
			FM_LINER,
			FM_ANISOTROPIC,//各项异性过滤/多级渐远纹理
			FM_MAX
		};

		SamplerDesc()
		{
			CoordMode_U = CoordinateMode::CM_REPEAT;
			CoordMode_V = CoordinateMode::CM_REPEAT;
			CoordMode_W = CoordinateMode::CM_REPEAT;
			BorderColor = FRColorRGBA(0, 0, 0, 0);

			FliterMode_Min = FliterMode::FM_LINER;
			FliterMode_Mag = FliterMode::FM_LINER;
			FliterMode_Mip = FliterMode::FM_NONE;
			MipLevel = 0;
			AniLevel = 1;
		}

		~SamplerDesc() {}

		CoordinateMode CoordMode_U;
		CoordinateMode CoordMode_V;
		CoordinateMode CoordMode_W;
		FRColorRGBA BorderColor;

		FliterMode FliterMode_Min;
		FliterMode FliterMode_Mag;
		FliterMode FliterMode_Mip;//如果设置为FM_NONE表示不启用多级渐远纹理，FM_ANISOTROPIC表示启用多级渐远纹理
		unsigned char MipLevel;
		unsigned char AniLevel;

	};
	FR_TYPE_MARCO(SamplerDesc)

	class FR_ENGINE_API SamplerState : public Object
	{
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		SamplerState();
		virtual ~SamplerState();

		void SetSamplerDesc(const SamplerDesc& desc) { mSamplerDesc = desc; }
		const SamplerDesc& GetSamplerDesc() const { return mSamplerDesc; }
	protected:
		SamplerDesc mSamplerDesc;
	};
	FR_TYPE_MARCO(SamplerState)
}