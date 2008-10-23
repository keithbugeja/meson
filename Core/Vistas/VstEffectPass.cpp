//----------------------------------------------------------------------------------------------
#include "VstEffectPass.h"

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
EffectPass::EffectPass(void) 
	: IPass(), 
	  m_uiIterationCount(1) 
{ 
}
//----------------------------------------------------------------------------------------------
EffectPass::EffectPass(const Meson::Common::Text::String& p_strName) 
	: IPass(p_strName), 
	  m_uiIterationCount(1) 
{ 
}
//----------------------------------------------------------------------------------------------
IPass::PassType EffectPass::GetType(void) const 
{ 
	return IPass::PT_EFFECT_PASS; 
} 
//----------------------------------------------------------------------------------------------
bool EffectPass::IsSingleIteration(void) const 
{ 
	return (m_uiIterationCount == 1); 
}
//----------------------------------------------------------------------------------------------
uint EffectPass::GetIterationCount(void) const 
{ 
	return m_uiIterationCount; 
}
//----------------------------------------------------------------------------------------------
void EffectPass::SetIterationCount(uint p_uiIterations) 
{ 
	m_uiIterationCount = p_uiIterations; 
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END