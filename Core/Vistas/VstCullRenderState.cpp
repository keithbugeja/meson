#include "VstCullRenderState.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
CullRenderState::CullRenderState() 
	: m_bIsCullingEnabled(true), 
	  m_faceWinding(CRSFW_CCW), 
	  m_faceCulling(CRSFC_BACK) 
{ 
}
//----------------------------------------------------------------------------------------------
IRenderState::RenderStateType CullRenderState::GetType(void) const
{ 
	return RST_CULL; 
}
//----------------------------------------------------------------------------------------------
const CullRenderState& CullRenderState::operator=(const CullRenderState& p_cullRenderState)
{
	m_bIsCullingEnabled = p_cullRenderState.m_bIsCullingEnabled;
	m_faceCulling = p_cullRenderState.m_faceCulling;
	m_faceWinding = p_cullRenderState.m_faceWinding;

	return *this;
}
//----------------------------------------------------------------------------------------------
bool CullRenderState::operator==(const CullRenderState& p_cullRenderState)
{
	if (&p_cullRenderState == this)
		return true;

	return (m_bIsCullingEnabled == p_cullRenderState.m_bIsCullingEnabled &&
			m_faceCulling == p_cullRenderState.m_faceCulling &&
			m_faceWinding == p_cullRenderState.m_faceWinding);
}
//----------------------------------------------------------------------------------------------
bool CullRenderState::IsCullingEnabled(void) const
{ 
	return m_bIsCullingEnabled; 
}
//----------------------------------------------------------------------------------------------
CullRenderState::FaceWinding CullRenderState::GetFaceWindingMode(void) const
{ 
	return m_faceWinding; 
}
//----------------------------------------------------------------------------------------------
CullRenderState::FaceCulling CullRenderState::GetFaceCullingMode(void) const
{ 
	return m_faceCulling; 
}
//----------------------------------------------------------------------------------------------
void CullRenderState::SetFaceWindingMode(CullRenderState::FaceWinding p_faceWinding) 
{ 
	m_faceWinding = p_faceWinding; 
}
//----------------------------------------------------------------------------------------------
void CullRenderState::SetFaceCullingMode(CullRenderState::FaceCulling p_faceCulling) 
{ 
	m_faceCulling = p_faceCulling; 
}
//----------------------------------------------------------------------------------------------
void CullRenderState::SetCulling(bool p_bEnabled) 
{ 
	m_bIsCullingEnabled = p_bEnabled; 
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
