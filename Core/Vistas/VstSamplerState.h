//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstColour.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class VISTAS_API SamplerState : public Meson::Common::TReferenceCounter<SamplerState>
	{
	public:
		enum SamplerType
		{
			ST_SAMPLER_1D,
			ST_SAMPLER_2D,
			ST_SAMPLER_3D,
			ST_SAMPLER_CUBE,
			ST_SAMPLER_PROJ,
			ST_COUNT
		};

		enum WrapType
		{
			WT_WRAP,
			WT_MIRROR,
			WT_CLAMP,
			WT_BORDER,
			WT_MIRRORONCE,
			WT_COUNT
		};

		enum FilterType
		{
			FT_NONE,
			FT_POINT,
			FT_LINEAR,
			FT_ANISOTROPIC,
			FT_COUNT
		};

	public:
		SamplerState(void);
		SamplerState(const SamplerState& p_samplerState);

		~SamplerState(void);

		void SetSamplerType(SamplerType p_eSamplerType);
		SamplerType GetSamplerType(void) const;

		void SetUWrapType(WrapType p_eWrapType);
		void SetVWrapType(WrapType p_eWrapType);
		void SetWWrapType(WrapType p_eWrapType);

		WrapType GetUWrapType(void) const;
		WrapType GetVWrapType(void) const;
		WrapType GetWWrapType(void) const;

		void SetMipmapFilter(FilterType p_eFilterType);
		FilterType GetMipmapFilter(void) const;

		void SetMagnificationFilter(FilterType p_eFilterType);
		FilterType GetMagnificationFilter(void) const;

		void SetMinificationFilter(FilterType p_eFilterType);
		FilterType GetMinificationFilter(void) const;

		void SetBorderColour(const Colour& p_borderColour);
		const Colour& GetBorderColour(void) const;

		const SamplerState& operator= (const SamplerState& p_samplerState);

	protected:
		SamplerType m_samplerType;

		WrapType	m_uWrapType,
					m_vWrapType,
					m_wWrapType;

		FilterType	m_mipmapFilerType,
					m_minFilterType,
					m_magFilterType;

		Colour		m_borderColour;
	};

	typedef Meson::Common::TPointer<SamplerState> SamplerStatePtr;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END
