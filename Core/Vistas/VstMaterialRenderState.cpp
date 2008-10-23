#include "VstMaterialRenderState.h"

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
MaterialRenderState::MaterialRenderState(void)
	: m_ambient(Colour::Black)
	, m_diffuse(Colour::White)
	, m_specular(Colour::Black)
	, m_emissive(Colour::Black)
	, m_fShininess(0.0f)
	, m_fAlpha(1.0f)
	, m_bIsWireframe(false)
{
}
//----------------------------------------------------------------------------------------------
IRenderState::RenderStateType MaterialRenderState::GetType(void) const
{
	return RST_MATERIAL;
}
//----------------------------------------------------------------------------------------------
const MaterialRenderState& MaterialRenderState::operator= (const MaterialRenderState& p_materialRenderState)
{
	m_ambient = p_materialRenderState.m_ambient;
	m_diffuse = p_materialRenderState.m_diffuse;
	m_specular = p_materialRenderState.m_specular;
	m_emissive = p_materialRenderState.m_emissive;
	
	m_fShininess = p_materialRenderState.m_fShininess;
	m_fAlpha = p_materialRenderState.m_fAlpha;

	m_bIsWireframe = p_materialRenderState.m_bIsWireframe;

	return *this;
}
//----------------------------------------------------------------------------------------------
bool MaterialRenderState::operator==(const MaterialRenderState& p_materialRenderState)
{
	if (&p_materialRenderState == this)
		return true;

	return (m_ambient == p_materialRenderState.m_ambient &&
		m_diffuse == p_materialRenderState.m_diffuse &&
		m_specular == p_materialRenderState.m_specular &&
		m_emissive == p_materialRenderState.m_emissive &&
		m_fShininess == p_materialRenderState.m_fShininess &&
		m_fAlpha == p_materialRenderState.m_fAlpha &&
		m_bIsWireframe == p_materialRenderState.m_bIsWireframe);
}
//----------------------------------------------------------------------------------------------
void MaterialRenderState::SetAmbient(float p_fRed, float p_fGreen, float p_fBlue)
{
	m_ambient.R = p_fRed;
	m_ambient.G = p_fGreen;
	m_ambient.B = p_fBlue;
}
//----------------------------------------------------------------------------------------------
void MaterialRenderState::SetAmbient(const Colour& p_ambient)
{
	m_ambient = p_ambient;
}
//----------------------------------------------------------------------------------------------
void MaterialRenderState::SetDiffuse(float p_fRed, float p_fGreen, float p_fBlue)
{
	m_diffuse.R = p_fRed;
	m_diffuse.G = p_fGreen;
	m_diffuse.B = p_fBlue;
}
//----------------------------------------------------------------------------------------------
void MaterialRenderState::SetDiffuse(const Colour& p_diffuse)
{
	m_diffuse = p_diffuse;
}
//----------------------------------------------------------------------------------------------
void MaterialRenderState::SetEmissive(float p_fRed, float p_fGreen, float p_fBlue)
{
	m_emissive.R = p_fRed;
	m_emissive.G = p_fGreen;
	m_emissive.B = p_fBlue;
}
//----------------------------------------------------------------------------------------------
void MaterialRenderState::SetEmissive(const Colour& p_emissive)
{
	m_emissive = p_emissive;
}
//----------------------------------------------------------------------------------------------
void MaterialRenderState::SetSpecular(float p_fRed, float p_fGreen, float p_fBlue)
{
	m_specular.R = p_fRed;
	m_specular.G = p_fGreen;
	m_specular.B = p_fBlue;
}
//----------------------------------------------------------------------------------------------
void MaterialRenderState::SetSpecular(const Colour& p_specular)
{
	m_specular = p_specular;
}
//----------------------------------------------------------------------------------------------
void MaterialRenderState::SetShininess(float p_fShininess)
{
	m_fShininess = p_fShininess;
}
//----------------------------------------------------------------------------------------------
void MaterialRenderState::SetAlpha(float p_fAlpha)
{
	m_fAlpha = p_fAlpha;
}
//----------------------------------------------------------------------------------------------
void MaterialRenderState::SetWireframe(bool p_bEnabled)
{
	m_bIsWireframe = p_bEnabled;	
}
//----------------------------------------------------------------------------------------------
Colour MaterialRenderState::GetAmbient(void) const
{
	return m_ambient;
}
//----------------------------------------------------------------------------------------------
Colour MaterialRenderState::GetDiffuse(void) const
{
	return m_diffuse;
}
//----------------------------------------------------------------------------------------------
Colour MaterialRenderState::GetEmissive(void) const
{
	return m_emissive;
}
//----------------------------------------------------------------------------------------------
Colour MaterialRenderState::GetSpecular(void) const
{
	return m_specular;
}
//----------------------------------------------------------------------------------------------
float MaterialRenderState::GetShininess(void) const
{
	return m_fShininess;
}
//----------------------------------------------------------------------------------------------
float MaterialRenderState::GetAlpha(void) const
{
	return m_fAlpha;
}
//----------------------------------------------------------------------------------------------
bool MaterialRenderState::IsWireframeEnabled(void) const
{
	return m_bIsWireframe;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
