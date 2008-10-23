#include "VstBiasRenderState.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
BiasRenderState::BiasRenderState() 
	: m_fScaleBias(0.0f), 
	  m_fDepthBias(0.0f) 
{ 
}
//----------------------------------------------------------------------------------------------
IRenderState::RenderStateType BiasRenderState::GetType(void) const
{ 
	return RST_BIAS; 
}
//----------------------------------------------------------------------------------------------
const BiasRenderState& BiasRenderState::operator=(const BiasRenderState& p_biasRenderState)
{
	m_fScaleBias = p_biasRenderState.m_fScaleBias;
	m_fDepthBias = p_biasRenderState.m_fDepthBias;

	return *this;
}
//----------------------------------------------------------------------------------------------
bool BiasRenderState::operator==(const BiasRenderState& p_biasRenderState)
{
	if (&p_biasRenderState == this)
		return true;

	return (m_fScaleBias == p_biasRenderState.m_fScaleBias &&
			m_fDepthBias == p_biasRenderState.m_fDepthBias);
}
//----------------------------------------------------------------------------------------------
void BiasRenderState::SetScaleBias(float p_fScaleBias)
{ 
	m_fScaleBias = p_fScaleBias; 
}
//----------------------------------------------------------------------------------------------
void BiasRenderState::SetDepthBias(float p_fDepthBias)
{ 
	m_fDepthBias = p_fDepthBias; 
}
//----------------------------------------------------------------------------------------------
float BiasRenderState::GetScaleBias(void) const
{ 
	return m_fScaleBias; 
}
//----------------------------------------------------------------------------------------------
float BiasRenderState::GetDepthBias(void) const
{ 
	return m_fDepthBias; 
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
