//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreSimulatorsPlugIn
//	PlugIn interface for the Gravitas core simulator implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		28/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------

#include "CoreSimulatorsPlugIn.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas::Simulation;

Meson_Gravitas_BEGIN

//----------------------------------------------------------------------------------------------
///	CoreSimulatorsPlugIn Constructor
//----------------------------------------------------------------------------------------------
CoreSimulatorsPlugIn::CoreSimulatorsPlugIn(void)
	: m_pName("Core Simulators PlugIn")
	, m_version(0,0,1)
	, m_pBasicSimulatorFactory(NULL)
{
}

//----------------------------------------------------------------------------------------------
///	CoreSimulatorsPlugIn Destructor
//----------------------------------------------------------------------------------------------
CoreSimulatorsPlugIn::~CoreSimulatorsPlugIn(void)
{
}

//----------------------------------------------------------------------------------------------
/// Returns the name of the Plug-In
///	\return Plug-In name
//----------------------------------------------------------------------------------------------
const String& CoreSimulatorsPlugIn::GetName(void) const
{
	return m_pName;
}

//----------------------------------------------------------------------------------------------
/// Returns the version number of the Plug-In
/// \return Version number
//----------------------------------------------------------------------------------------------
const Version& CoreSimulatorsPlugIn::GetVersion(void) const
{
	return m_version;
}

//----------------------------------------------------------------------------------------------
/// Registers the Plug-In with the Engine subsystem
//----------------------------------------------------------------------------------------------
void CoreSimulatorsPlugIn::Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap) 
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	GravitasEngine* pGravitasEngine = (GravitasEngine*)m_pSharedObjectMap->GetRegisteredObject("GravitasEngine");
	MESON_ASSERT(pGravitasEngine != NULL, "GravitasEngine is not a registered shared object!");

	GravitasEngine::SetInstance(pGravitasEngine);
	pGravitasEngine->Logger().Out << "Registering core simulators...\n";


	// simulator factories
	SimulationManager *pSimulationManager = pGravitasEngine->GetSimulationManager();

	if (m_pBasicSimulatorFactory == NULL)
	{
		m_pBasicSimulatorFactory = new BasicSimulatorFactory();
		pSimulationManager->RegisterSimulatorFactory(m_pBasicSimulatorFactory);
		pGravitasEngine->Logger().Out << "Registered basic simulator factory.\n";
	}
}

//----------------------------------------------------------------------------------------------
/// Initialises the Plug-In
//----------------------------------------------------------------------------------------------
void CoreSimulatorsPlugIn::Initialise(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Shuts the Plug-In down
//----------------------------------------------------------------------------------------------
void CoreSimulatorsPlugIn::Shutdown(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Unregisters the Plug-In from the engine subsystem.
//----------------------------------------------------------------------------------------------
void CoreSimulatorsPlugIn::Unregister(void) 
{ 
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();
	pGravitasEngine->Logger().Out << "Unregistering Core Spaces...\n";

	// simulator factories (in reverse order)
	SimulationManager *pSimulationManager = pGravitasEngine->GetSimulationManager();

	if (m_pBasicSimulatorFactory != NULL)
	{
		pSimulationManager->UnregisterSimulatorFactory(m_pBasicSimulatorFactory);
		pGravitasEngine->Logger().Out << "Unregistered basic simulator factory.\n";
		delete m_pBasicSimulatorFactory;
		m_pBasicSimulatorFactory = NULL;
	}

	GravitasEngine::SetInstance(NULL);
}

Meson_Gravitas_END
