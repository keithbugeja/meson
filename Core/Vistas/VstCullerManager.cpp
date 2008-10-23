//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstCullerManager.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
CullerManager::CullerManager(void)
{
}
//----------------------------------------------------------------------------------------------
CullerManager::~CullerManager(void)
{
}
//----------------------------------------------------------------------------------------------
CullerFactoryEnumerator CullerManager::GetCullerFactories(void)
{
	return m_cullerFactoryMap.GetValueEnumerator();
}
//----------------------------------------------------------------------------------------------
ICullerFactory* CullerManager::GetCullerFactory(const String& p_strName)
{
	ICullerFactory* pCullerFactory;

	if (m_cullerFactoryMap.ContainsKey(p_strName, pCullerFactory))
		return pCullerFactory;

	throw new MesonException("Specified culler factory is not registered.", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void CullerManager::RegisterCullerFactory(ICullerFactory *p_pCullerFactory)
{
	if (m_cullerFactoryMap.ContainsKey(p_pCullerFactory->GetName()))
		throw new MesonException("Culler factory '" + p_pCullerFactory->GetName() + "' has already been registered.", __FILE__, __LINE__);

	m_cullerFactoryMap[p_pCullerFactory->GetName()] = p_pCullerFactory;
}
//----------------------------------------------------------------------------------------------
void CullerManager::UnregisterCullerFactory(ICullerFactory *p_pCullerFactory)
{
	if (!m_cullerFactoryMap.ContainsKey(p_pCullerFactory->GetName()))
		throw new MesonException("Culler factory '" + p_pCullerFactory->GetName() + "' is not registered and cannot be removed.", __FILE__, __LINE__);

	m_cullerFactoryMap.RemoveKey(p_pCullerFactory->GetName());
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END