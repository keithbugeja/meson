//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstMaterial.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
Material::Material(const Meson::Common::Text::String& p_strName)
	: TechniqueGroup(),
	  m_strName(p_strName)
{
}
//----------------------------------------------------------------------------------------------
Material::Material(const Material& p_material)
	: TechniqueGroup(p_material),
	  m_strName(p_material.m_strName)
{
}
//----------------------------------------------------------------------------------------------
Material::~Material(void)
{
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& Material::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
TechniquePtr Material::GetBestTechnique(void)
{
	return m_techniqueList[0];
}
//----------------------------------------------------------------------------------------------
Material& Material::operator =(const Meson::Vistas::Material &p_material)
{
	m_strName = p_material.m_strName;
	m_techniqueList = p_material.m_techniqueList;

	return *this;
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END