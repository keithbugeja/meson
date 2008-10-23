#include "VstTargetRenderState.h"
#include "VstRenderDevice.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
TargetRenderState::TargetRenderState() 
	: m_strRenderTargetName(), 
	  m_dwColourMask(IRenderDevice::CM_RED_GREEN_BLUE_ALPHA)
{
}
//----------------------------------------------------------------------------------------------
IRenderState::RenderStateType TargetRenderState::GetType(void) const
{ 
	return RST_TARGET; 
}
//----------------------------------------------------------------------------------------------
const TargetRenderState& TargetRenderState::operator=(const TargetRenderState& p_targetRenderState)
{
	m_strRenderTargetName = p_targetRenderState.m_strRenderTargetName;
	m_dwColourMask = p_targetRenderState.m_dwColourMask;

	return *this;
}
//----------------------------------------------------------------------------------------------
bool TargetRenderState::operator==(const TargetRenderState& p_targetRenderState)
{
	if (&p_targetRenderState == this)
		return true;

	return (m_strRenderTargetName == p_targetRenderState.m_strRenderTargetName &&
			m_dwColourMask == p_targetRenderState.m_dwColourMask);
}
//----------------------------------------------------------------------------------------------
void TargetRenderState::SetRenderTarget(const Meson::Common::Text::String& p_strRenderTargetName)
{ 
	m_strRenderTargetName = p_strRenderTargetName; 
}
//----------------------------------------------------------------------------------------------
void TargetRenderState::SetColourMask(dword p_dwColourMask)
{ 
	m_dwColourMask = p_dwColourMask;
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& TargetRenderState::GetRenderTarget(void) const
{ 
	return m_strRenderTargetName; 
}
//----------------------------------------------------------------------------------------------
dword TargetRenderState::GetColourMask(void) const
{ 
	return m_dwColourMask; 
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
