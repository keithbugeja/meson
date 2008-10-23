//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstIlluminationPass.h"

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
IlluminationPass::IlluminationPass(void) 
	: IPass(),
      m_bIsBoundToLightType(false),
	  m_bIsBoundToNamedLight(false),
	  m_uiSimultaneousLightsProcessed(1)
{ 
}
//----------------------------------------------------------------------------------------------
IlluminationPass::IlluminationPass(const Meson::Common::Text::String& p_strName) 
	: IPass(p_strName), 
      m_bIsBoundToLightType(false),
	  m_bIsBoundToNamedLight(false),
	  m_uiSimultaneousLightsProcessed(1)
{ 
}
//----------------------------------------------------------------------------------------------
IPass::PassType IlluminationPass::GetType(void) const 
{ 
	return IPass::PT_ILLUMINATION_PASS; 
} 
//----------------------------------------------------------------------------------------------
uint IlluminationPass::GetSimultaneousLightsProcessed(void) const 
{ 
	return m_uiSimultaneousLightsProcessed; 
}
//----------------------------------------------------------------------------------------------
void IlluminationPass::SetSimultaneousLightsProcessed(uint p_uiLightCount) 
{ 
	m_uiSimultaneousLightsProcessed = p_uiLightCount; 
}
//----------------------------------------------------------------------------------------------
bool IlluminationPass::IsBoundToNamedLight(void) const 
{ 
	return m_bIsBoundToNamedLight; 
}
//----------------------------------------------------------------------------------------------
void IlluminationPass::UnbindFromNamedLight(void) 
{ 
	m_bIsBoundToNamedLight = false; 
}
//----------------------------------------------------------------------------------------------
void IlluminationPass::BindToNamedLight(const Meson::Common::Text::String& p_strLightName) 
{ 
	m_bIsBoundToNamedLight = true; 
	m_strNamedLight = p_strLightName; 
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& IlluminationPass::GetBoundNamedLight(void) const 
{ 
	return m_strNamedLight; 
}
//----------------------------------------------------------------------------------------------
bool IlluminationPass::IsBoundToLightType(void) const 
{ 
	return m_bIsBoundToLightType; 
}
//----------------------------------------------------------------------------------------------
void IlluminationPass::UnbindFromLightType(void) 
{ 
	m_bIsBoundToLightType = false; 
}
//----------------------------------------------------------------------------------------------
void IlluminationPass::BindToLightType(Light::LightType p_eLightType) 
{ 
	m_bIsBoundToLightType = true; 
	m_eBoundLightType = p_eLightType; 
}
//----------------------------------------------------------------------------------------------
Light::LightType IlluminationPass::GetBoundLightType(void) const 
{ 
	return m_eBoundLightType; 
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END