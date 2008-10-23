//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstAttribute.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Vistas;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
Attribute::Attribute(void)
: m_uiMaterial(0),
  m_uiSmoothingGroup(0),
  m_bIsSmoothingEnabled(false),
  m_bIsMarked(false)
{
}
//----------------------------------------------------------------------------------------------
Attribute::Attribute(uint p_uiMaterial, uint p_uiSmoothingGroup, bool p_bSmoothingEnabled)
: m_uiMaterial(p_uiMaterial),
  m_uiSmoothingGroup(p_uiSmoothingGroup),
  m_bIsSmoothingEnabled(p_bSmoothingEnabled),
  m_bIsMarked(false)
{
}
//----------------------------------------------------------------------------------------------
bool Attribute::IsMarked(void)
{
	return m_bIsMarked;
}
//----------------------------------------------------------------------------------------------
void Attribute::SetMark(bool p_bEnabled)
{
	m_bIsMarked = p_bEnabled;
}
//----------------------------------------------------------------------------------------------
bool Attribute::IsSmoothingEnabled(void)
{
	return m_bIsSmoothingEnabled;
}
//----------------------------------------------------------------------------------------------
void Attribute::SetSmoothing(bool p_bEnabled)
{
	m_bIsSmoothingEnabled = p_bEnabled;
}
//----------------------------------------------------------------------------------------------
uint Attribute::GetMaterial(void)
{
	return m_uiMaterial;
}
//----------------------------------------------------------------------------------------------
void Attribute::SetMaterial(uint p_uiMaterial)
{
	m_uiMaterial = p_uiMaterial;
}
//----------------------------------------------------------------------------------------------
uint Attribute::GetSmoothingGroup(void)
{
	return m_uiSmoothingGroup;
}
//----------------------------------------------------------------------------------------------
void Attribute::SetSmoothingGroup(uint p_uiSmoothingGroup)
{
	m_uiSmoothingGroup = p_uiSmoothingGroup;
}
//----------------------------------------------------------------------------------------------
bool Attribute::operator== (const Attribute& p_attribute)
{
	return (m_bIsSmoothingEnabled == p_attribute.m_bIsSmoothingEnabled &&
			m_uiSmoothingGroup == p_attribute.m_uiSmoothingGroup &&
			m_uiMaterial == p_attribute.m_uiMaterial);
}
//----------------------------------------------------------------------------------------------
Attribute& Attribute::operator=(const Attribute& p_attribute)
{
	m_bIsMarked = p_attribute.m_bIsMarked;
	m_bIsSmoothingEnabled = p_attribute.m_bIsSmoothingEnabled;

	m_uiMaterial = p_attribute.m_uiMaterial;
	m_uiSmoothingGroup = p_attribute.m_uiSmoothingGroup;

	return *this;
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END