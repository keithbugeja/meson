#include "VstSamplerUnit.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
SamplerUnit::SamplerUnit(uint p_uiSamplerIndex, TextureStatePtr p_pTextureState, SamplerStatePtr p_pSamplerState)
	: m_uiSamplerIndex(p_uiSamplerIndex),
	  m_pTextureState(p_pTextureState),
	  m_pSamplerState(p_pSamplerState)
{
}
//----------------------------------------------------------------------------------------------
SamplerUnit::SamplerUnit(uint p_uiSamplerIndex)
	: m_uiSamplerIndex(p_uiSamplerIndex),
	  m_pTextureState(NULL),
	  m_pSamplerState(NULL)
{
}
//----------------------------------------------------------------------------------------------
SamplerUnit::SamplerUnit(const SamplerUnit& p_samplerUnit)
	: m_uiSamplerIndex(p_samplerUnit.m_uiSamplerIndex),
	  m_pTextureState(p_samplerUnit.m_pTextureState),
	  m_pSamplerState(p_samplerUnit.m_pSamplerState)
{
}
//----------------------------------------------------------------------------------------------
SamplerUnit::~SamplerUnit(void)
{
	m_pTextureState = NULL;
	m_pSamplerState = NULL;
}
//----------------------------------------------------------------------------------------------
uint SamplerUnit::GetSamplerIndex(void) const
{
	return m_uiSamplerIndex;
}
//----------------------------------------------------------------------------------------------
TextureStatePtr SamplerUnit::GetTextureState(void)
{
	return m_pTextureState;
}
//----------------------------------------------------------------------------------------------
void SamplerUnit::SetTextureState(TextureStatePtr p_pTextureState)
{
	m_pTextureState = p_pTextureState;
}
//----------------------------------------------------------------------------------------------
SamplerStatePtr SamplerUnit::GetSamplerState(void)
{
	return m_pSamplerState;
}
//----------------------------------------------------------------------------------------------
void SamplerUnit::SetSamplerState(SamplerStatePtr p_pSamplerState)
{
	m_pSamplerState = p_pSamplerState;
}
//----------------------------------------------------------------------------------------------
const SamplerUnit& SamplerUnit::operator=(const SamplerUnit& p_samplerUnit)
{
	m_uiSamplerIndex = p_samplerUnit.m_uiSamplerIndex;
	m_pTextureState = p_samplerUnit.m_pTextureState;
	m_pSamplerState = p_samplerUnit.m_pSamplerState;

	return *this;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END