#include "VstDepthRenderState.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
DepthRenderState::DepthRenderState(void) 
	: m_bIsDepthTestEnabled(true), 
	  m_bIsDepthWriteEnabled(true), 
	  m_compareType(DRSCMP_LESSEQUAL) 
{ 
}
//----------------------------------------------------------------------------------------------
IRenderState::RenderStateType DepthRenderState::GetType(void) const
{ 
	return RST_DEPTH; 
}
//----------------------------------------------------------------------------------------------
const DepthRenderState& DepthRenderState::operator=(const DepthRenderState& p_depthRenderState)
{
	m_bIsDepthTestEnabled = p_depthRenderState.m_bIsDepthTestEnabled;
	m_bIsDepthWriteEnabled = p_depthRenderState.m_bIsDepthWriteEnabled;
	m_compareType = p_depthRenderState.m_compareType;

	return *this;
}
//----------------------------------------------------------------------------------------------
bool DepthRenderState::operator==(const DepthRenderState& p_depthRenderState)
{
	if (&p_depthRenderState == this)
		return true;

	return (m_bIsDepthTestEnabled == p_depthRenderState.m_bIsDepthTestEnabled &&
			m_bIsDepthWriteEnabled == p_depthRenderState.m_bIsDepthWriteEnabled &&
			m_compareType == p_depthRenderState.m_compareType);
}
//----------------------------------------------------------------------------------------------
DepthRenderState::DepthCompare DepthRenderState::GetCompareType(void) const
{ 
	return m_compareType; 
}
//----------------------------------------------------------------------------------------------
bool DepthRenderState::IsDepthTestEnabled(void) const
{ 
	return m_bIsDepthTestEnabled; 
}
//----------------------------------------------------------------------------------------------
bool DepthRenderState::IsDepthWriteEnabled(void) const
{ 
	return m_bIsDepthWriteEnabled; 
}
//----------------------------------------------------------------------------------------------
void DepthRenderState::SetCompareType( DepthRenderState::DepthCompare p_compareType ) 
{ 
	m_compareType = p_compareType; 
}
//----------------------------------------------------------------------------------------------
void DepthRenderState::SetDepthTest(bool p_bEnabled) 
{ 
	m_bIsDepthTestEnabled = p_bEnabled; 
}
//----------------------------------------------------------------------------------------------
void DepthRenderState::SetDepthWrite(bool p_bEnabled) 
{ 
	m_bIsDepthWriteEnabled = p_bEnabled; 
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
