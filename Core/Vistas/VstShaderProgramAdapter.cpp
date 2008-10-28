//----------------------------------------------------------------------------------------------
#include "VstShaderProgramAdapter.h"
#include "VstShaderProgramFactory.h"

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
ShaderProgramAdapter::ShaderProgramAdapter(IShaderProgramFactory* p_pCreator, Meson::Common::Text::String p_strId)
	: IShaderProgram(p_strId)
	, m_pCreator(p_pCreator)
	, m_bIsBound(false)
	, m_bIsCompiled(false)
{
}
//----------------------------------------------------------------------------------------------
ShaderProgramAdapter::ShaderProgramAdapter(IShaderProgramFactory* p_pCreator)
	: IShaderProgram()
	, m_pCreator(p_pCreator)
	, m_bIsBound(false)
	, m_bIsCompiled(false)
{
}
//----------------------------------------------------------------------------------------------
ShaderProgramAdapter::~ShaderProgramAdapter(void)
{
}
//----------------------------------------------------------------------------------------------
IShaderProgramFactory* ShaderProgramAdapter::GetCreator(void) 
{ 
	return m_pCreator; 
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramAdapter::IsBound(void) const 
{ 
	return m_bIsBound;
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramAdapter::IsCompiled(void) const 
{ 
	return m_bIsCompiled;
}
//----------------------------------------------------------------------------------------------
void ShaderProgramAdapter::SetSource(const Meson::Common::Text::String& p_strSourceCode) 
{ 
	m_strSourceCode = p_strSourceCode; 
}
//----------------------------------------------------------------------------------------------
void ShaderProgramAdapter::SetEntryPoint(const Meson::Common::Text::String& p_strEntryPoint) 
{ 
	m_strEntryPoint = p_strEntryPoint; 
}
//----------------------------------------------------------------------------------------------
void ShaderProgramAdapter::SetProfile(const Meson::Common::Text::String& p_strProfile) 
{ 
	m_strProfile = p_strProfile; 
}
//----------------------------------------------------------------------------------------------
void ShaderProgramAdapter::AddVariable(ShaderProgramVariable* p_pVariable) 
{ 
	m_shaderVariableMap.Insert(p_pVariable->GetName(), p_pVariable); 
}
//----------------------------------------------------------------------------------------------
ShaderProgramVariable* ShaderProgramAdapter::GetVariable(const Meson::Common::Text::String& p_strVariableName)
{
	if (m_shaderVariableMap.ContainsKey(p_strVariableName))
		return m_shaderVariableMap[p_strVariableName];

	throw new Meson::Common::MesonException("Cannot return shader variable : Shader variable is not bound.");
}
//----------------------------------------------------------------------------------------------
void ShaderProgramAdapter::RemoveVariable(const Meson::Common::Text::String& p_strVariableName)
{
	if (m_shaderVariableMap.ContainsKey(p_strVariableName))
		m_shaderVariableMap.RemoveKey(p_strVariableName);
	else
		throw new Meson::Common::MesonException("Cannot remove shader variable : Shader variable is not bound.");
}
//----------------------------------------------------------------------------------------------
void ShaderProgramAdapter::AddRenderConstant(RenderConstantType::RenderConstantType p_eRenderConstantType)
{
	if (m_renderConstantList.IndexOf(p_eRenderConstantType) == -1)
		m_renderConstantList.Add(p_eRenderConstantType);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramAdapter::RemoveRenderConstant(RenderConstantType::RenderConstantType p_eRenderConstantType)
{
	if (m_renderConstantList.IndexOf(p_eRenderConstantType) != -1)
		m_renderConstantList.Remove(p_eRenderConstantType);
	else
		throw new Meson::Common::MesonException("Cannot remove render constant : Render constant is not bound.");
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END