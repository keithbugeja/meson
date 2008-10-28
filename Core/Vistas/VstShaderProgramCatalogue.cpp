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
	if (m_shaderProgramMap.ContainsKey(p_pShaderProgram->GetId()))
		throw new Meson::Common::MesonException("Shader program already in catalogue.", __FILE__, __LINE__);
	
	m_shaderProgramMap.Insert(p_pShaderProgram->GetId(), p_pShaderProgram);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramCatalogue::RemoveShaderProgram(const Meson::Common::Text::String& p_strId)
{
	if (!m_shaderProgramMap.ContainsKey(p_strId))
		throw new Meson::Common::MesonException("Shader program not in catalogue.", __FILE__, __LINE__);
	
	m_shaderProgramMap.RemoveKey(p_strId);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramCatalogue::RemoveAllShaderPrograms(void)
{
	m_shaderProgramMap.Clear();
}
//----------------------------------------------------------------------------------------------
IShaderProgram* ShaderProgramCatalogue::GetShaderProgram(const Meson::Common::Text::String& p_strId)
{
	IShaderProgram* pShaderProgram;

	if (m_shaderProgramMap.ContainsKey(p_strId, pShaderProgram))
		return pShaderProgram;

	throw new Meson::Common::MesonException("Shader program not in catalogue.", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramCatalogue::ContainsShaderProgram(const Meson::Common::Text::String& p_strId)
{
	return m_shaderProgramMap.ContainsKey(p_strId);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramCatalogue::ContainsShaderProgram(const Meson::Common::Text::String& p_strId, IShaderProgram*& p_pShaderProgram)
{
	return m_shaderProgramMap.ContainsKey(p_strId, p_pShaderProgram);
}
//----------------------------------------------------------------------------------------------
ShaderProgramEnumerator ShaderProgramCatalogue::GetShaderProgramEnumeration(void)
{
	return m_shaderProgramMap.GetValueEnumerator();
}

Meson_Vistas_END