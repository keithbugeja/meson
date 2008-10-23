#include "CoreForcesPlugin.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::PlugIn;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Collision;

CoreForcesPlugIn::CoreForcesPlugIn(void)
	: m_pName("Core Forces PlugIn")
	, m_version(0,0,1)
	, m_pBasicForceFactory(NULL)
	, m_pGravityForceFactory(NULL)
	, m_pVortexForceFactory(NULL)
{
}

CoreForcesPlugIn::~CoreForcesPlugIn(void)
{
}

const String& CoreForcesPlugIn::GetName(void) const
{
	return m_pName;
}

const Version& CoreForcesPlugIn::GetVersion(void) const
{
	return m_version;
}

void CoreForcesPlugIn::Register(SharedObjectMap* p_pSharedObjectMap) 
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	GravitasEngine* pGravitasEngine
		= (GravitasEngine*)m_pSharedObjectMap->GetRegisteredObject("GravitasEngine");
	MESON_ASSERT(pGravitasEngine != NULL, "GravitasEngine is not a registered shared object!");

	GravitasEngine::SetInstance(pGravitasEngine);
	pGravitasEngine->Logger().Out << "Registering core forces...\n";

	// force factories
	ForceManager* pForceManager = pGravitasEngine->GetForceManager();

	if (m_pBasicForceFactory == NULL)
	{
		m_pBasicForceFactory = new BasicForceFactory();
		pForceManager->RegisterForceFactory(m_pBasicForceFactory);
		pGravitasEngine->Logger().Out << "Registered basic force factory.\n";
	}

	if (m_pGravityForceFactory == NULL)
	{
		m_pGravityForceFactory = new GravityForceFactory();
		pForceManager->RegisterForceFactory(m_pGravityForceFactory);
		pGravitasEngine->Logger().Out << "Registered gravity force factory.\n";
	}

	if (m_pVortexForceFactory == NULL)
	{
		m_pVortexForceFactory = new VortexForceFactory();
		pForceManager->RegisterForceFactory(m_pVortexForceFactory);
		pGravitasEngine->Logger().Out << "Registered vortex force factory.\n";
	}
}

void CoreForcesPlugIn::Initialise(void) 
{ 
}

void CoreForcesPlugIn::Shutdown(void) 
{ 
}

void CoreForcesPlugIn::Unregister(void) 
{ 
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();
	pGravitasEngine->Logger().Out << "Unregistering core forces...\n";

	// force factories (in reverse order)
	ForceManager* pForceManager = pGravitasEngine->GetForceManager();

	if (m_pVortexForceFactory != NULL)
	{
		pForceManager->UnregisterForceFactory(m_pVortexForceFactory);
		pGravitasEngine->Logger().Out << "Unregistered vortex force factory.\n";
		delete m_pVortexForceFactory;
		m_pVortexForceFactory = NULL;
	}

	if (m_pGravityForceFactory != NULL)
	{
		pForceManager->UnregisterForceFactory(m_pGravityForceFactory);
		pGravitasEngine->Logger().Out << "Unregistered gravity force factory.\n";
		delete m_pGravityForceFactory;
		m_pGravityForceFactory = NULL;
	}

	if (m_pBasicForceFactory != NULL)
	{
		pForceManager->UnregisterForceFactory(m_pBasicForceFactory);
		pGravitasEngine->Logger().Out << "Unregistered basic force factory.\n";
		delete m_pBasicForceFactory;
		m_pBasicForceFactory = NULL;
	}

	GravitasEngine::SetInstance(NULL);
}
