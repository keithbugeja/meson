//----------------------------------------------------------------------------------------------
#include "VstShaderProgramCatalogue.h"

Meson_Vistas_BEGIN
	
//----------------------------------------------------------------------------------------------
ShaderProgramCatalogue::ShaderProgramCatalogue(const Meson::Common::Text::String& p_strName) 
	: m_strName(p_strName)
{
}
//----------------------------------------------------------------------------------------------
ShaderProgramCatalogue::~ShaderProgramCatalogue(void)
{
	RemoveAllShaderPrograms();
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& ShaderProgramCatalogue::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramCatalogue::AddShaderProgram(IShaderProgram* p_pShaderProgram)
{
	if (m_shaderProgramMap.ContainsKey(p_pShaderProgram->GetName()))
		throw new Meson::Common::MesonException("Shader program already in catalogue.", __FILE__, __LINE__);
	
	m_shaderProgramMap.Insert(p_pShaderProgram->GetName(), p_pShaderProgram);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramCatalogue::RemoveShaderProgram(const Meson::Common::Text::String& p_strName)
{
	if (!m_shaderProgramMap.ContainsKey(p_strName))
		throw new Meson::Common::MesonException("Shader program not in catalogue.", __FILE__, __LINE__);
	
	m_shaderProgramMap.RemoveKey(p_strName);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramCatalogue::RemoveAllShaderPrograms(void)
{
	m_shaderProgramMap.Clear();
}
//----------------------------------------------------------------------------------------------
IShaderProgram* ShaderProgramCatalogue::GetShaderProgram(const Meson::Common::Text::String& p_strName)
{
	IShaderProgram* pShaderProgram;

	if (m_shaderProgramMap.ContainsKey(p_strName, pShaderProgram))
		return pShaderProgram;

	throw new Meson::Common::MesonException("Shader program not in catalogue.", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramCatalogue::ContainsShaderProgram(const Meson::Common::Text::String& p_strName)
{
	return m_shaderProgramMap.ContainsKey(p_strName);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramCatalogue::ContainsShaderProgram(const Meson::Common::Text::String& p_strName, IShaderProgram*& p_pShaderProgram)
{
	return m_shaderProgramMap.ContainsKey(p_strName, p_pShaderProgram);
}
//----------------------------------------------------------------------------------------------
ShaderProgramEnumerator ShaderProgramCatalogue::GetShaderProgramEnumeration(void)
{
	return m_shaderProgramMap.GetValueEnumerator();
}

Meson_Vistas_END