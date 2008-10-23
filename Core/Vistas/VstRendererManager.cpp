//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstRendererManager.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
RendererManager::RendererManager(void)
{
}
//----------------------------------------------------------------------------------------------
RendererManager::~RendererManager(void)
{
}
//----------------------------------------------------------------------------------------------
RendererFactoryEnumerator RendererManager::GetRendererFactories(void)
{
	return m_rendererFactoryMap.GetValueEnumerator();
}
//----------------------------------------------------------------------------------------------
IRendererFactory* RendererManager::GetRendererFactory(const String& p_strName)
{
	IRendererFactory* pRendererFactory;

	if (m_rendererFactoryMap.ContainsKey(p_strName, pRendererFactory))
		return pRendererFactory;

	throw new MesonException("Specified renderer factory is not registered.", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void RendererManager::RegisterRendererFactory(IRendererFactory* p_pRendererFactory)
{
	if (m_rendererFactoryMap.ContainsKey(p_pRendererFactory->GetName()))
		throw new MesonException("Renderer factory '" + p_pRendererFactory->GetName() + "' has already been registered.", __FILE__, __LINE__);

	m_rendererFactoryMap[p_pRendererFactory->GetName()] = p_pRendererFactory;
}
//----------------------------------------------------------------------------------------------
void RendererManager::UnregisterRendererFactory(IRendererFactory* p_pRendererFactory)
{
	if (!m_rendererFactoryMap.ContainsKey(p_pRendererFactory->GetName()))
		throw new MesonException("Renderer factory '" + p_pRendererFactory->GetName() + "' is not registered and cannot be removed.", __FILE__, __LINE__);

	m_rendererFactoryMap.RemoveKey(p_pRendererFactory->GetName());
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END