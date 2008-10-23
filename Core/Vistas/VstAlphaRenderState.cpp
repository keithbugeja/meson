#include "VstAlphaRenderState.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
AlphaRenderState::AlphaRenderState()
	: m_bIsAlphaTestEnabled(false), 
	  m_bIsAlphaBlendEnabled(false),
	  m_eCompareType(ARSCMP_ALWAYS),
	  m_eSourceBlend(ARSB_ONE),
	  m_eDestinationBlend(ARSB_ONE)
{
}
//----------------------------------------------------------------------------------------------
IRenderState::RenderStateType AlphaRenderState::GetType(void) const
{
	return RST_ALPHA;
}
//----------------------------------------------------------------------------------------------
const AlphaRenderState& AlphaRenderState::operator=(const AlphaRenderState& p_alphaRenderState)
{
	m_bIsAlphaTestEnabled = p_alphaRenderState.m_bIsAlphaTestEnabled;
	m_bIsAlphaBlendEnabled = p_alphaRenderState.m_bIsAlphaBlendEnabled;
	m_dwAlphaReference = p_alphaRenderState.m_dwAlphaReference;
	m_eCompareType = p_alphaRenderState.m_eCompareType;
	m_eSourceBlend = p_alphaRenderState.m_eSourceBlend;
	m_eDestinationBlend = p_alphaRenderState.m_eDestinationBlend;

	return *this;
}
//----------------------------------------------------------------------------------------------
bool AlphaRenderState::operator==(const AlphaRenderState& p_alphaRenderState)
{
	if (&p_alphaRenderState == this)
		return true;

	return (m_bIsAlphaTestEnabled == p_alphaRenderState.m_bIsAlphaTestEnabled &&
			m_bIsAlphaBlendEnabled == p_alphaRenderState.m_bIsAlphaBlendEnabled &&
			m_dwAlphaReference == p_alphaRenderState.m_dwAlphaReference &&
			m_eCompareType == p_alphaRenderState.m_eCompareType &&
			m_eSourceBlend == p_alphaRenderState.m_eSourceBlend &&
			m_eDestinationBlend == p_alphaRenderState.m_eDestinationBlend);
}

//----------------------------------------------------------------------------------------------
AlphaRenderState::AlphaCompare AlphaRenderState::GetCompareType(void) const
{
	return m_eCompareType;
}
//----------------------------------------------------------------------------------------------
dword AlphaRenderState::GetAlphaReference(void) const
{
	return m_dwAlphaReference;
}
//----------------------------------------------------------------------------------------------
bool AlphaRenderState::IsAlphaTestEnabled(void) const
{
	return m_bIsAlphaTestEnabled;
}
//----------------------------------------------------------------------------------------------
void AlphaRenderState::SetCompareType( AlphaCompare p_eCompareType )
{
	m_eCompareType = p_eCompareType;
}
//----------------------------------------------------------------------------------------------
void AlphaRenderState::SetAlphaReference(dword p_dwAlphaReference)
{
	m_dwAlphaReference = p_dwAlphaReference;
}
//----------------------------------------------------------------------------------------------
void AlphaRenderState::SetAlphaTest(bool p_bEnabled)
{
	m_bIsAlphaTestEnabled = p_bEnabled;
}
//----------------------------------------------------------------------------------------------
AlphaRenderState::AlphaBlend AlphaRenderState::GetSourceBlendMode(void) const
{
	return m_eSourceBlend;
}
//----------------------------------------------------------------------------------------------
AlphaRenderState::AlphaBlend AlphaRenderState::GetDestinationBlendMode(void) const
{
	return m_eDestinationBlend;
}
//----------------------------------------------------------------------------------------------
bool AlphaRenderState::IsAlphaBlendEnabled(void) const
{
	return m_bIsAlphaBlendEnabled;
}
//----------------------------------------------------------------------------------------------
void AlphaRenderState::SetSourceBlendMode(AlphaRenderState::AlphaBlend p_eSourceBlendMode)
{
	m_eSourceBlend = p_eSourceBlendMode;
}
//----------------------------------------------------------------------------------------------
void AlphaRenderState::SetDestinationBlendMode(AlphaRenderState::AlphaBlend p_eDestinationBlendMode)
{
	m_eDestinationBlend = p_eDestinationBlendMode;
}
//----------------------------------------------------------------------------------------------
void AlphaRenderState::SetAlphaBlend(bool p_bEnabled)
{
	m_bIsAlphaBlendEnabled = p_bEnabled;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
