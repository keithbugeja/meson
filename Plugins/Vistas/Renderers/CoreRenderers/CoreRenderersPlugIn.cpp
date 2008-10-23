//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVistasEngine.h"
#include "CoreRenderersPlugIn.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
CoreRenderersPlugIn::CoreRenderersPlugIn(void) 
	: m_pName("CoreRenderers PlugIn"), 
	  m_version(0,0,1),
	  m_pBasicRendererFactory(NULL)
{
}
//----------------------------------------------------------------------------------------------
CoreRenderersPlugIn::~CoreRenderersPlugIn(void)
{
}
//----------------------------------------------------------------------------------------------
const String& CoreRenderersPlugIn::GetName(void) const
{
	return m_pName;
}
//----------------------------------------------------------------------------------------------
const Version& CoreRenderersPlugIn::GetVersion(void) const
{
	return m_version;
}
//----------------------------------------------------------------------------------------------
void CoreRenderersPlugIn::Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap) 
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	VistasEngine* pVistasEngine = (VistasEngine*)m_pSharedObjectMap->GetRegisteredObject("VistasEngine");
	MESON_ASSERT(pVistasEngine != NULL, "VistasEngine is not a registered shared object.");

	VistasEngine::SetInstance(pVistasEngine);
	pVistasEngine->Logger().Out << "Registering Core Renderers...\n";

	if (m_pBasicRendererFactory == NULL)
	{
		m_pBasicRendererFactory = new BasicRendererFactory();
		pVistasEngine->GetRendererManager()->RegisterRendererFactory(m_pBasicRendererFactory);
		pVistasEngine->Logger().Out << "Registered Basic Renderer.\n";

		m_pInstrumentationRendererFactory = new InstrumentationRendererFactory();
		pVistasEngine->GetRendererManager()->RegisterRendererFactory(m_pInstrumentationRendererFactory);
		pVistasEngine->Logger().Out << "Registered Instrumentation Renderer.\n";
	}
}
//----------------------------------------------------------------------------------------------
void CoreRenderersPlugIn::Initialise(void) 
{ 
}
//----------------------------------------------------------------------------------------------
void CoreRenderersPlugIn::Shutdown(void) 
{ 
}
//----------------------------------------------------------------------------------------------
void CoreRenderersPlugIn::Unregister(void) 
{ 
	VistasEngine* pVistasEngine = VistasEngine::GetInstance();
	pVistasEngine->GetInstance()->Logger().Out << "Unregistering Core Renderers...\n";

	if (m_pBasicRendererFactory != NULL)
	{
		pVistasEngine->GetRendererManager()->UnregisterRendererFactory(m_pBasicRendererFactory);
		pVistasEngine->Logger().Out << "Unregistered Basic Renderer.\n";
		SAFE_DELETE(m_pBasicRendererFactory);
	}

	if (m_pInstrumentationRendererFactory != NULL)
	{
		pVistasEngine->GetRendererManager()->UnregisterRendererFactory(m_pInstrumentationRendererFactory);
		pVistasEngine->Logger().Out << "Unregistered Instrumentation Renderer.\n";
		SAFE_DELETE(m_pInstrumentationRendererFactory);
	}

	VistasEngine::SetInstance(NULL);
}

Meson_Vistas_END
