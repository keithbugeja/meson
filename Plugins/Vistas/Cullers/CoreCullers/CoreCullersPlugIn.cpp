//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVistasEngine.h"
#include "CoreCullersPlugIn.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
CoreCullersPlugIn::CoreCullersPlugIn(void) 
	: m_pName("CoreCullers PlugIn"), 
	  m_version(0,0,1),
	  m_pBasicCullerFactory(NULL)
{
}
//----------------------------------------------------------------------------------------------
CoreCullersPlugIn::~CoreCullersPlugIn(void)
{
}
//----------------------------------------------------------------------------------------------
const String& CoreCullersPlugIn::GetName(void) const
{
	return m_pName;
}
//----------------------------------------------------------------------------------------------
const Version& CoreCullersPlugIn::GetVersion(void) const
{
	return m_version;
}
//----------------------------------------------------------------------------------------------
void CoreCullersPlugIn::Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap) 
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	VistasEngine* pVistasEngine = (VistasEngine*)m_pSharedObjectMap->GetRegisteredObject("VistasEngine");
	MESON_ASSERT(pVistasEngine != NULL, "VistasEngine is not a registered shared object.");

	VistasEngine::SetInstance(pVistasEngine);
	pVistasEngine->Logger().Out << "Registering Core Cullers...\n";

	if (m_pBasicCullerFactory == NULL)
	{
		m_pBasicCullerFactory = new BasicCullerFactory();
		pVistasEngine->GetCullerManager()->RegisterCullerFactory(m_pBasicCullerFactory);
		pVistasEngine->Logger().Out << "Registered Basic Culler.\n";
	}
}
//----------------------------------------------------------------------------------------------
void CoreCullersPlugIn::Initialise(void) 
{ 
}
//----------------------------------------------------------------------------------------------
void CoreCullersPlugIn::Shutdown(void) 
{ 
}
//----------------------------------------------------------------------------------------------
void CoreCullersPlugIn::Unregister(void) 
{ 
	VistasEngine* pVistasEngine = VistasEngine::GetInstance();
	pVistasEngine->GetInstance()->Logger().Out << "Unregistering Core Cullers...\n";

	if (m_pBasicCullerFactory != NULL)
	{
		pVistasEngine->GetCullerManager()->UnregisterCullerFactory(m_pBasicCullerFactory);
		pVistasEngine->Logger().Out << "Unregistered Basic Culler.\n";
		SAFE_DELETE(m_pBasicCullerFactory);
	}

	VistasEngine::SetInstance(NULL);
}

Meson_Vistas_END
