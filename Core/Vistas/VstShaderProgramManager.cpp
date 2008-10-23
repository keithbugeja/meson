//----------------------------------------------------------------------------------------------
#include "VstShaderProgramManager.h"
#include "VstShaderProgramAdapter.h"

#include "VstVistasEngine.h"
#include "MesonException.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN
	
//----------------------------------------------------------------------------------------------
ShaderProgramManager::ShaderProgramManager(void)
	: m_pShaderProgramFactory(NULL)
{
}
//----------------------------------------------------------------------------------------------
ShaderProgramManager::~ShaderProgramManager(void) 
{
	DestoryAllShaderPrograms();
	UnregisterAllShaderProgramFactories();
}
//----------------------------------------------------------------------------------------------
void ShaderProgramManager::RegisterShaderProgramFactory(IShaderProgramFactory* p_pShaderProgramFactory)
{
	if (m_shaderProgramFactoryMap.ContainsKey(p_pShaderProgramFactory->GetName()))
		throw new Meson::Common::MesonException("Shader factory already registered.", __FILE__, __LINE__ );

	m_shaderProgramFactoryMap.Insert(p_pShaderProgramFactory->GetName(), p_pShaderProgramFactory);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramManager::UnregisterShaderProgramFactory(const Meson::Common::Text::String& p_strName)
{
	if (!m_shaderProgramFactoryMap.ContainsKey(p_strName))
		throw new Meson::Common::MesonException("Shader factory does not exist.", __FILE__, __LINE__ );

	m_shaderProgramFactoryMap.RemoveKey(p_strName);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramManager::UnregisterAllShaderProgramFactories(void)
{
	m_shaderProgramFactoryMap.Clear();
}
//----------------------------------------------------------------------------------------------
void ShaderProgramManager::SetShaderProgramFactory(const Meson::Common::Text::String& p_strName)
{
	if (!m_shaderProgramFactoryMap.ContainsKey(p_strName))
		throw new Meson::Common::MesonException("Shader factory does not exist.", __FILE__, __LINE__ );

	m_pShaderProgramFactory = m_shaderProgramFactoryMap[p_strName];
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramManager::IsActiveShaderProgramFactory(const Meson::Common::Text::String& p_strName)
{
	MESON_ASSERT(m_pShaderProgramFactory != NULL, "No shader program factory set!");

	if (!m_shaderProgramFactoryMap.ContainsKey(p_strName))
		throw new Meson::Common::MesonException("Shader factory does not exist.", __FILE__, __LINE__ );

	return (m_shaderProgramFactoryMap[p_strName] == m_pShaderProgramFactory);
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& ShaderProgramManager::GetShaderProfile(const IShaderProgram::ShaderProgramType p_shaderProgramType)
{
	return m_pShaderProgramFactory->GetShaderProfile(p_shaderProgramType);
}
//----------------------------------------------------------------------------------------------
ShaderProfileEnumerator ShaderProgramManager::GetAvailableShaderProfiles(const IShaderProgram::ShaderProgramType p_shaderProgramType)
{
	return m_pShaderProgramFactory->GetAvailableShaderProfiles(p_shaderProgramType);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramManager::IsShaderProfileSupported(const IShaderProgram::ShaderProgramType p_shaderProgramType, const Meson::Common::Text::String& p_strProfile)
{
	return m_pShaderProgramFactory->IsShaderProfileSupported(p_shaderProgramType, p_strProfile);
}
//----------------------------------------------------------------------------------------------
const Meson::Common::Text::String& ShaderProgramManager::GetShaderProgramExtension(void) const
{
	return m_pShaderProgramFactory->GetShaderProgramExtension();
}
//----------------------------------------------------------------------------------------------
IShaderProgram* ShaderProgramManager::CreateShaderProgram(const IShaderProgram::ShaderProgramType p_shaderProgramType, const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName)
{
	ShaderProgramCatalogue* pShaderProgramCatalogue = NULL;

	if (!m_shaderProgramCatalogueMap.ContainsKey(p_strCatalogueName))
	{
		pShaderProgramCatalogue = new ShaderProgramCatalogue(p_strCatalogueName);
		m_shaderProgramCatalogueMap.Insert(p_strCatalogueName, pShaderProgramCatalogue);
	}
	else 
		pShaderProgramCatalogue = m_shaderProgramCatalogueMap[p_strCatalogueName];

	IShaderProgram* pShaderProgram = m_pShaderProgramFactory->CreateShaderProgram(p_shaderProgramType, p_strProgramName);
	pShaderProgramCatalogue->AddShaderProgram(pShaderProgram);
	return pShaderProgram;
}
//----------------------------------------------------------------------------------------------
IShaderProgram* ShaderProgramManager::CreateShaderProgramFromFile(Meson::Common::Text::String p_strFilename, const IShaderProgram::ShaderProgramType p_shaderProgramType, const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName)
{
	String strResourcePath;

	// Verify that resource exists before calling factory method
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strFilename, strResourcePath))
		throw new MesonException("Shader program resource not found!", __FILE__, __LINE__);

	// Resource exists, therefore it can be safely loaded
	ShaderProgramCatalogue* pShaderProgramCatalogue = NULL;

	if (!m_shaderProgramCatalogueMap.ContainsKey(p_strCatalogueName))
	{
		pShaderProgramCatalogue = new ShaderProgramCatalogue(p_strCatalogueName);
		m_shaderProgramCatalogueMap.Insert(p_strCatalogueName, pShaderProgramCatalogue);
	}
	else 
		pShaderProgramCatalogue = m_shaderProgramCatalogueMap[p_strCatalogueName];

	IShaderProgram* pShaderProgram = m_pShaderProgramFactory->CreateShaderProgramFromFile(strResourcePath, p_shaderProgramType, p_strProgramName);
	pShaderProgramCatalogue->AddShaderProgram(pShaderProgram);
	
	return pShaderProgram;
}
//----------------------------------------------------------------------------------------------
IShaderProgram* ShaderProgramManager::GetShaderProgram(const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName)
{
	ShaderProgramCatalogue* pShaderProgramCatalogue;

	if (!m_shaderProgramCatalogueMap.ContainsKey(p_strCatalogueName, pShaderProgramCatalogue))
		throw new Meson::Common::MesonException("Catalogue does not exist.", __FILE__, __LINE__ );

	return pShaderProgramCatalogue->GetShaderProgram(p_strProgramName);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramManager::ContainsShaderProgram(const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName)
{
	ShaderProgramCatalogue* pShaderProgramCatalogue;

	if (!m_shaderProgramCatalogueMap.ContainsKey(p_strCatalogueName, pShaderProgramCatalogue))
		throw new Meson::Common::MesonException("Catalogue does not exist.", __FILE__, __LINE__ );

	return pShaderProgramCatalogue->ContainsShaderProgram(p_strProgramName);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramManager::ContainsShaderProgram(const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName, IShaderProgram*& p_pShaderProgram)
{
	ShaderProgramCatalogue* pShaderProgramCatalogue;

	if (!m_shaderProgramCatalogueMap.ContainsKey(p_strCatalogueName, pShaderProgramCatalogue))
		throw new Meson::Common::MesonException("Catalogue does not exist.", __FILE__, __LINE__ );

	return pShaderProgramCatalogue->ContainsShaderProgram(p_strProgramName, p_pShaderProgram);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramManager::DestroyShaderProgram(const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName)
{
	ShaderProgramCatalogue* pShaderProgramCatalogue;
	
	if (!m_shaderProgramCatalogueMap.ContainsKey(p_strCatalogueName, pShaderProgramCatalogue))
		throw new Meson::Common::MesonException("Catalogue does not exist.", __FILE__, __LINE__ );

	pShaderProgramCatalogue->RemoveShaderProgram(p_strProgramName);

	ShaderProgramAdapter* pShaderProgram = static_cast<ShaderProgramAdapter*>(pShaderProgramCatalogue->GetShaderProgram(p_strProgramName));
	
	IShaderProgramFactory* pShaderProgramFactory = pShaderProgram->GetCreator();
	pShaderProgramFactory->DestroyShaderProgram(pShaderProgram);
}
//----------------------------------------------------------------------------------------------
void ShaderProgramManager::DestoryAllShaderPrograms(void)
{
	Meson::Common::Collections::TEnumerator<ShaderProgramCatalogue*>& shaderCatalogueEnumerator = m_shaderProgramCatalogueMap.GetValueEnumerator();

	ShaderProgramCatalogue* pShaderProgramCatalogue = NULL;
	IShaderProgramFactory *pShaderProgramFactory = NULL;
	ShaderProgramAdapter* pShaderProgram = NULL;

	while (shaderCatalogueEnumerator.HasMoreElements())
	{
		pShaderProgramCatalogue = shaderCatalogueEnumerator.NextElement();
		Meson::Common::Collections::TEnumerator<IShaderProgram*>& shaderProgramEnumerator = pShaderProgramCatalogue->GetShaderProgramEnumeration();

		while (shaderProgramEnumerator.HasMoreElements())
		{
			pShaderProgram = static_cast<ShaderProgramAdapter*>(shaderProgramEnumerator.NextElement());
			pShaderProgramFactory = pShaderProgram->GetCreator();
			pShaderProgramFactory->DestroyShaderProgram(pShaderProgram);
		}

		pShaderProgramCatalogue->RemoveAllShaderPrograms();
	}

	Meson::Common::Collections::FreePointers(m_shaderProgramCatalogueMap);
}
//----------------------------------------------------------------------------------------------
bool ShaderProgramManager::ContainsCatalogue(const Meson::Common::Text::String& p_strCatalogueName)
{
	return m_shaderProgramCatalogueMap.ContainsKey(p_strCatalogueName);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END