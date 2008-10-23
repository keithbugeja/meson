//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "MemoryInputStream.h"

#include "VstVistasEngine.h"
#include "VstMaterialManager.h"
#include "VstMaterialScriptParser.h"

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
MaterialManager::MaterialManager(void)
{
}
//----------------------------------------------------------------------------------------------
MaterialManager::~MaterialManager(void)
{
	MaterialEnumerator materialEnumerator = m_materialMap.GetValueEnumerator();

	while(materialEnumerator.HasMoreElements())
		materialEnumerator.NextElement() = NULL;

	m_materialMap.Clear();
}
//----------------------------------------------------------------------------------------------
MaterialPtr MaterialManager::CreateMaterial(const Meson::Common::Text::String& p_strName)
{
	if (ContainsMaterial(p_strName))
		throw new MesonException("Another material already exists with the given name", __FILE__, __LINE__);

	MaterialPtr pMaterial(new Material(p_strName));
	m_materialMap.Insert(p_strName, pMaterial);

	return pMaterial;
}
//----------------------------------------------------------------------------------------------
void MaterialManager::ExecuteMaterialScript(Meson::Common::IO::IInputStream& p_inputStream)
{
	MaterialScriptParser materialParser(&p_inputStream);

	materialParser.Initialise();
	materialParser.Parse();
	materialParser.Shutdown();
}
//----------------------------------------------------------------------------------------------
bool MaterialManager::ContainsMaterial(const Meson::Common::Text::String& p_strName)
{
	return m_materialMap.ContainsKey(p_strName);
}
//----------------------------------------------------------------------------------------------
bool MaterialManager::ContainsMaterial(const Meson::Common::Text::String& p_strName, MaterialPtr& p_pMaterial)
{
	return m_materialMap.ContainsKey(p_strName, p_pMaterial);
}
//----------------------------------------------------------------------------------------------
MaterialPtr MaterialManager::GetMaterial(const Meson::Common::Text::String& p_strName)
{
	return m_materialMap[p_strName];
}
//----------------------------------------------------------------------------------------------
void MaterialManager::ReleaseMaterial(MaterialPtr p_pMaterial)
{
	if (ContainsMaterial(p_pMaterial->GetName()))
		m_materialMap.RemoveKey(p_pMaterial->GetName());
}
//----------------------------------------------------------------------------------------------
void MaterialManager::ReleaseMaterial(const Meson::Common::Text::String& p_strName)
{
	if (ContainsMaterial(p_strName))
		m_materialMap.RemoveKey(p_strName);
}
//----------------------------------------------------------------------------------------------
void MaterialManager::ReleaseAllMaterials(void)
{
	m_materialMap.Clear();
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END