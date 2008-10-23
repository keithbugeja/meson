#include "VstStencilRenderState.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
StencilRenderState::StencilRenderState(void) 
	: m_bIsStencilEnabled(false), 
	  m_dwStencilMask(0xFFFFFFFF), 
	  m_dwStencilWriteMask(0xFFFFFFFF),
	  m_dwStencilReference(0x00000000),
	  m_compareType(SRSCMP_NEVER),
	  m_stencilPassOperation(SRSOP_KEEP),
	  m_stencilFailOperation(SRSOP_KEEP),
	  m_stencilDepthFailOperation(SRSOP_KEEP)
{ 
}
//----------------------------------------------------------------------------------------------
IRenderState::RenderStateType StencilRenderState::GetType(void) const
{ 
	return RST_STENCIL; 
}
//----------------------------------------------------------------------------------------------
const StencilRenderState& StencilRenderState::operator= (const StencilRenderState& p_stencilRenderState)
{
	m_bIsStencilEnabled = p_stencilRenderState.m_bIsStencilEnabled;
	m_compareType = p_stencilRenderState.m_compareType;
	m_dwStencilMask = p_stencilRenderState.m_dwStencilMask;
	m_dwStencilWriteMask = p_stencilRenderState.m_dwStencilWriteMask;
	m_dwStencilReference = p_stencilRenderState.m_dwStencilReference;
	m_stencilPassOperation = p_stencilRenderState.m_stencilPassOperation;
	m_stencilFailOperation = p_stencilRenderState.m_stencilFailOperation;
	m_stencilDepthFailOperation = p_stencilRenderState.m_stencilDepthFailOperation;

	return *this;
}
//----------------------------------------------------------------------------------------------
bool StencilRenderState::operator==(const StencilRenderState& p_stencilRenderState)
{
	if (&p_stencilRenderState == this)
		return true;

	return (m_bIsStencilEnabled == p_stencilRenderState.m_bIsStencilEnabled &&
			m_compareType == p_stencilRenderState.m_compareType &&
			m_dwStencilMask == p_stencilRenderState.m_dwStencilMask &&
			m_dwStencilWriteMask == p_stencilRenderState.m_dwStencilWriteMask &&
			m_dwStencilReference == p_stencilRenderState.m_dwStencilReference &&
			m_stencilPassOperation == p_stencilRenderState.m_stencilPassOperation &&
			m_stencilFailOperation == p_stencilRenderState.m_stencilFailOperation &&
			m_stencilDepthFailOperation == p_stencilRenderState.m_stencilDepthFailOperation);

}
//----------------------------------------------------------------------------------------------
StencilRenderState::StencilCompare StencilRenderState::GetCompareType(void) const
{
	return m_compareType;
}
//----------------------------------------------------------------------------------------------
StencilRenderState::StencilOperation StencilRenderState::GetPassOperation(void) const
{
	return m_stencilPassOperation;
}
//----------------------------------------------------------------------------------------------
StencilRenderState::StencilOperation StencilRenderState::GetFailOperation(void) const
{
	return m_stencilFailOperation;
}
//----------------------------------------------------------------------------------------------
StencilRenderState::StencilOperation StencilRenderState::GetDepthFailOperation(void) const
{
	return m_stencilDepthFailOperation;
}
//----------------------------------------------------------------------------------------------
dword StencilRenderState::GetStencilReference(void) const
{
	return m_dwStencilReference;
}
//----------------------------------------------------------------------------------------------
dword StencilRenderState::GetStencilWriteMask(void) const
{
	return m_dwStencilWriteMask;
}
//----------------------------------------------------------------------------------------------
dword StencilRenderState::GetStencilMask(void) const
{
	return m_dwStencilMask;
}
//----------------------------------------------------------------------------------------------
bool StencilRenderState::IsStencilTestEnabled(void) const
{
	return m_bIsStencilEnabled;
}
//----------------------------------------------------------------------------------------------
void StencilRenderState::SetCompareType(StencilRenderState::StencilCompare p_compareType)
{
	m_compareType = p_compareType;
}
//----------------------------------------------------------------------------------------------
void StencilRenderState::SetPassOperation(StencilRenderState::StencilOperation p_passOperation)
{
	m_stencilPassOperation = p_passOperation;
}
//----------------------------------------------------------------------------------------------
void StencilRenderState::SetFailOperation(StencilRenderState::StencilOperation p_failOperation)
{
	m_stencilFailOperation = p_failOperation;
}
//----------------------------------------------------------------------------------------------
void StencilRenderState::SetDepthFailOperation(StencilRenderState::StencilOperation p_depthFailOperation)
{
	m_stencilDepthFailOperation = p_depthFailOperation;
}
//----------------------------------------------------------------------------------------------
void StencilRenderState::SetStencilReference(dword p_dwStencilReference)
{
	m_dwStencilReference = p_dwStencilReference;
}
//----------------------------------------------------------------------------------------------
void StencilRenderState::SetStencilWriteMask(dword p_dwStencilWriteMask)
{
	m_dwStencilWriteMask = p_dwStencilWriteMask;
}
//----------------------------------------------------------------------------------------------
void StencilRenderState::SetStencilMask(dword p_dwStencilMask)
{
	m_dwStencilMask = p_dwStencilMask;
}
//----------------------------------------------------------------------------------------------
void StencilRenderState::SetStencilTest(bool p_bEnabled)
{
	m_bIsStencilEnabled = p_bEnabled;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
