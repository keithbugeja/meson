//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstSamplerState.h"

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
SamplerState::SamplerState(void) 
	: m_borderColour(Colour::Black),
	  m_minFilterType(SamplerState::FT_LINEAR),
	  m_magFilterType(SamplerState::FT_LINEAR),
	  m_mipmapFilerType(SamplerState::FT_LINEAR),
	  m_samplerType(SamplerState::ST_SAMPLER_2D),
	  m_uWrapType(SamplerState::WT_WRAP),
	  m_vWrapType(SamplerState::WT_WRAP),
	  m_wWrapType(SamplerState::WT_WRAP)
{
}
//----------------------------------------------------------------------------------------------
SamplerState::SamplerState(const SamplerState& p_samplerState)
	: m_borderColour(p_samplerState.m_borderColour),
	  m_minFilterType(p_samplerState.m_minFilterType),
	  m_magFilterType(p_samplerState.m_magFilterType),
	  m_mipmapFilerType(p_samplerState.m_mipmapFilerType),
	  m_samplerType(p_samplerState.m_samplerType),
	  m_uWrapType(p_samplerState.m_uWrapType),
	  m_vWrapType(p_samplerState.m_vWrapType),
	  m_wWrapType(p_samplerState.m_wWrapType)
{
}
//----------------------------------------------------------------------------------------------
SamplerState::~SamplerState(void)
{
}
//----------------------------------------------------------------------------------------------
void SamplerState::SetSamplerType(SamplerState::SamplerType p_eSamplerType)
{
	m_samplerType = p_eSamplerType;
}
//----------------------------------------------------------------------------------------------
SamplerState::SamplerType SamplerState::GetSamplerType(void) const
{
	return m_samplerType;
}
//----------------------------------------------------------------------------------------------
void SamplerState::SetUWrapType(SamplerState::WrapType p_eWrapType)
{
	m_uWrapType = p_eWrapType;
}
//----------------------------------------------------------------------------------------------
void SamplerState::SetVWrapType(SamplerState::WrapType p_eWrapType)
{
	m_vWrapType = p_eWrapType;
}
//----------------------------------------------------------------------------------------------
void SamplerState::SetWWrapType(SamplerState::WrapType p_eWrapType)
{
	m_wWrapType = p_eWrapType;
}
//----------------------------------------------------------------------------------------------
SamplerState::WrapType SamplerState::GetUWrapType(void) const
{
	return m_uWrapType;
}
//----------------------------------------------------------------------------------------------
SamplerState::WrapType SamplerState::GetVWrapType(void) const
{
	return m_vWrapType;
}
//----------------------------------------------------------------------------------------------
SamplerState::WrapType SamplerState::GetWWrapType(void) const
{
	return m_wWrapType;
}
//----------------------------------------------------------------------------------------------
void SamplerState::SetMipmapFilter(SamplerState::FilterType p_eFilterType)
{
	m_mipmapFilerType = p_eFilterType;
}
//----------------------------------------------------------------------------------------------
SamplerState::FilterType SamplerState::GetMipmapFilter(void) const
{
	return m_mipmapFilerType;
}
//----------------------------------------------------------------------------------------------
void SamplerState::SetMagnificationFilter(SamplerState::FilterType p_eFilterType)
{
	m_magFilterType = p_eFilterType;
}
//----------------------------------------------------------------------------------------------
SamplerState::FilterType SamplerState::GetMagnificationFilter(void) const
{
	return m_magFilterType;
}
//----------------------------------------------------------------------------------------------
void SamplerState::SetMinificationFilter(SamplerState::FilterType p_eFilterType)
{
	m_minFilterType = p_eFilterType;
}
//----------------------------------------------------------------------------------------------
SamplerState::FilterType SamplerState::GetMinificationFilter(void) const
{
	return m_minFilterType;
}
//----------------------------------------------------------------------------------------------
void SamplerState::SetBorderColour(const Colour& p_borderColour)
{
	m_borderColour = p_borderColour;
}
//----------------------------------------------------------------------------------------------
const Colour& SamplerState::GetBorderColour(void) const
{
	return m_borderColour;
}
//----------------------------------------------------------------------------------------------
const SamplerState& SamplerState::operator= (const SamplerState& p_samplerState)
{
	m_borderColour = p_samplerState.m_borderColour;
	m_minFilterType = p_samplerState.m_minFilterType;
	m_magFilterType = p_samplerState.m_magFilterType;
	m_mipmapFilerType = p_samplerState.m_mipmapFilerType;
	m_samplerType = p_samplerState.m_samplerType;
	m_uWrapType = p_samplerState.m_uWrapType;
	m_vWrapType = p_samplerState.m_vWrapType;
	m_wWrapType = p_samplerState.m_wWrapType;

	return *this;
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END
