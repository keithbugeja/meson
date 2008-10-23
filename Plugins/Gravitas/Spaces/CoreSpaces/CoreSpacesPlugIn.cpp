//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreSpacesPlugin
//	PlugIn interface for the Gravitas core space implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		09/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------

#include "CoreSpacesPlugIn.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas::Space;

Meson_Gravitas_BEGIN

//----------------------------------------------------------------------------------------------
///	CoreSpacesPlugin Constructor
//----------------------------------------------------------------------------------------------
CoreSpacesPlugIn::CoreSpacesPlugIn(void)
	: m_pName("Core Spaces PlugIn")
	, m_version(0,0,1)
	, m_pBasicSpaceFactory(NULL)
	, m_pGridSpaceFactory(NULL)
	, m_pKDTreeSpaceFactory(NULL)
	, m_pSweepAndPruneSpaceFactory(NULL)
{
}

//----------------------------------------------------------------------------------------------
///	CoreSpacesPlugin Destructor
//----------------------------------------------------------------------------------------------
CoreSpacesPlugIn::~CoreSpacesPlugIn(void)
{
}

//----------------------------------------------------------------------------------------------
/// Returns the name of the Plug-In
///	\return Plug-In name
//----------------------------------------------------------------------------------------------
const String& CoreSpacesPlugIn::GetName(void) const
{
	return m_pName;
}

//----------------------------------------------------------------------------------------------
/// Returns the version number of the Plug-In
/// \return Version number
//----------------------------------------------------------------------------------------------
const Version& CoreSpacesPlugIn::GetVersion(void) const
{
	return m_version;
}

//----------------------------------------------------------------------------------------------
/// Registers the Plug-In with the Engine subsystem
//----------------------------------------------------------------------------------------------
void CoreSpacesPlugIn::Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap) 
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	GravitasEngine* pGravitasEngine = (GravitasEngine*)m_pSharedObjectMap->GetRegisteredObject("GravitasEngine");
	MESON_ASSERT(pGravitasEngine != NULL, "GravitasEngine is not a registered shared object!");

	GravitasEngine::SetInstance(pGravitasEngine);
	pGravitasEngine->Logger().Out << "Registering Core Spaces...\n";


	// space factories
	SpaceManager *pSpaceManager = pGravitasEngine->GetSpaceManager();

	if (m_pBasicSpaceFactory == NULL)
	{
		m_pBasicSpaceFactory = new BasicSpaceFactory();
		pSpaceManager->RegisterSpaceFactory(m_pBasicSpaceFactory);
		pGravitasEngine->Logger().Out << "Registered Basic space factory.\n";
	}

	if (m_pGridSpaceFactory == NULL)
	{
		m_pGridSpaceFactory = new GridSpaceFactory();
		pSpaceManager->RegisterSpaceFactory(m_pGridSpaceFactory);
		pGravitasEngine->Logger().Out << "Registered Grid space factory.\n";
	}

	if (m_pKDTreeSpaceFactory == NULL)
	{
		m_pKDTreeSpaceFactory = new KDTreeSpaceFactory();
		pSpaceManager->RegisterSpaceFactory(m_pKDTreeSpaceFactory);
		pGravitasEngine->Logger().Out << "Registered KD-Tree space factory.\n";
	}

	if (m_pSweepAndPruneSpaceFactory == NULL)
	{
		m_pSweepAndPruneSpaceFactory = new SweepAndPruneSpaceFactory();
		pSpaceManager->RegisterSpaceFactory(m_pSweepAndPruneSpaceFactory);
		pGravitasEngine->Logger().Out << "Registered Sweep-And-Prune space factory.\n";
	}
}

//----------------------------------------------------------------------------------------------
/// Initialises the Plug-In
//----------------------------------------------------------------------------------------------
void CoreSpacesPlugIn::Initialise(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Shuts the Plug-In down
//----------------------------------------------------------------------------------------------
void CoreSpacesPlugIn::Shutdown(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Unregisters the Plug-In from the engine subsystem.
//----------------------------------------------------------------------------------------------
void CoreSpacesPlugIn::Unregister(void) 
{ 
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();
	pGravitasEngine->Logger().Out << "Unregistering Core Spaces...\n";

	// space factories (in reverse order)
	SpaceManager *pSpaceManager = pGravitasEngine->GetSpaceManager();

	if (m_pSweepAndPruneSpaceFactory != NULL)
	{
		pSpaceManager->UnregisterSpaceFactory(m_pSweepAndPruneSpaceFactory);
		pGravitasEngine->Logger().Out << "Unregistered Sweep-And-Prune space factory.\n";
		delete m_pSweepAndPruneSpaceFactory;
		m_pSweepAndPruneSpaceFactory = NULL;
	}

	if (m_pKDTreeSpaceFactory != NULL)
	{
		pSpaceManager->UnregisterSpaceFactory(m_pKDTreeSpaceFactory);
		pGravitasEngine->Logger().Out << "Unregistered KD-Tree space factory.\n";
		delete m_pKDTreeSpaceFactory;
		m_pKDTreeSpaceFactory = NULL;
	}

	if (m_pGridSpaceFactory != NULL)
	{
		pSpaceManager->UnregisterSpaceFactory(m_pGridSpaceFactory);
		pGravitasEngine->Logger().Out << "Unregistered Grid space factory.\n";
		delete m_pGridSpaceFactory;
		m_pGridSpaceFactory = NULL;
	}

	if (m_pBasicSpaceFactory != NULL)
	{
		pSpaceManager->UnregisterSpaceFactory(m_pBasicSpaceFactory);
		pGravitasEngine->Logger().Out << "Unregistered Basic space factory.\n";
		delete m_pBasicSpaceFactory;
		m_pBasicSpaceFactory = NULL;
	}

	GravitasEngine::SetInstance(NULL);
}

Meson_Gravitas_END
