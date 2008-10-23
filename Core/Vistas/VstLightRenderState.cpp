//----------------------------------------------------------------------------------------------
#include "MesonException.h"
#include "VstVistasEngine.h"
#include "VstLightRenderState.h"
#include "VstLight.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
LightRenderState::LightRenderState()
	:	m_bIsLightingEnabled(false),
		m_ambientLight(Colour::Black)
{
}
//----------------------------------------------------------------------------------------------
LightRenderState::~LightRenderState(void)
{
}
//----------------------------------------------------------------------------------------------
IRenderState::RenderStateType LightRenderState::GetType(void) const
{
	return RST_LIGHT;
}
//----------------------------------------------------------------------------------------------
const LightRenderState& LightRenderState::operator=(const LightRenderState& p_lightRenderState)
{
	m_bIsLightingEnabled = p_lightRenderState.m_bIsLightingEnabled;
	m_ambientLight = p_lightRenderState.m_ambientLight;

	return *this;
}
//----------------------------------------------------------------------------------------------
bool LightRenderState::operator==(const LightRenderState& p_lightRenderState)
{
	if (&p_lightRenderState == this)
		return true;

	return (m_bIsLightingEnabled == p_lightRenderState.m_bIsLightingEnabled &&
			m_ambientLight == p_lightRenderState.m_ambientLight);
}
//----------------------------------------------------------------------------------------------
bool LightRenderState::IsLightingEnabled(void) const
{
	return m_bIsLightingEnabled;
}
//----------------------------------------------------------------------------------------------
const Colour& LightRenderState::GetAmbient(void) const
{
	return m_ambientLight;
}
//----------------------------------------------------------------------------------------------
void LightRenderState::SetLighting(const bool p_bEnabled)
{
	m_bIsLightingEnabled = p_bEnabled;
}
//----------------------------------------------------------------------------------------------
void LightRenderState::SetAmbient(const Colour& p_ambient)
{
	m_ambientLight = p_ambient;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
