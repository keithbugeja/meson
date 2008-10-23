//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreIntegratorsPlugIn
//	PlugIn interface for the Gravitas core integrator implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		30/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------

#include "CoreIntegratorsPlugIn.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas::Kinetics;

Meson_Gravitas_BEGIN

//----------------------------------------------------------------------------------------------
///	CoreIntegratorsPlugIn Constructor
//----------------------------------------------------------------------------------------------
CoreIntegratorsPlugIn::CoreIntegratorsPlugIn(void)
	: m_strName("Core Integrators PlugIn")
	, m_version(0,0,1)
	, m_pEulerKineticIntegrator(NULL)
	, m_pVerletKineticIntegrator(NULL)
	, m_pRK4KineticIntegrator(NULL)
{
}

//----------------------------------------------------------------------------------------------
///	CoreIntegratorsPlugIn Destructor
//----------------------------------------------------------------------------------------------
CoreIntegratorsPlugIn::~CoreIntegratorsPlugIn(void)
{
}

//----------------------------------------------------------------------------------------------
/// Returns the name of the Plug-In
///	\return Plug-In name
//----------------------------------------------------------------------------------------------
const String& CoreIntegratorsPlugIn::GetName(void) const
{
	return m_strName;
}

//----------------------------------------------------------------------------------------------
/// Returns the version number of the Plug-In
/// \return Version number
//----------------------------------------------------------------------------------------------
const Version& CoreIntegratorsPlugIn::GetVersion(void) const
{
	return m_version;
}

//----------------------------------------------------------------------------------------------
/// Registers the Plug-In with the Engine subsystem
//----------------------------------------------------------------------------------------------
void CoreIntegratorsPlugIn::Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap) 
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	GravitasEngine* pGravitasEngine = (GravitasEngine*)m_pSharedObjectMap->GetRegisteredObject("GravitasEngine");
	MESON_ASSERT(pGravitasEngine != NULL, "GravitasEngine is not a registered shared object!");

	GravitasEngine::SetInstance(pGravitasEngine);
	pGravitasEngine->Logger().Out << "Registering core integrators...\n";

	// kinetic integrators
	KineticsManager *pKineticsManager = pGravitasEngine->GetKineticsManager();

	if (m_pEulerKineticIntegrator == NULL)
	{
		m_pEulerKineticIntegrator = new EulerKineticIntegrator();
		pKineticsManager->RegisterKineticIntegrator(m_pEulerKineticIntegrator);
		pGravitasEngine->Logger().Out << "Registered Euler kinetic integrator.\n";
	}

	if (m_pVerletKineticIntegrator == NULL)
	{
		m_pVerletKineticIntegrator = new VerletKineticIntegrator();
		pKineticsManager->RegisterKineticIntegrator(m_pVerletKineticIntegrator);
		pGravitasEngine->Logger().Out << "Registered Verlet kinetic integrator.\n";
	}

	if (m_pRK4KineticIntegrator == NULL)
	{
		m_pRK4KineticIntegrator = new RK4KineticIntegrator();
		pKineticsManager->RegisterKineticIntegrator(m_pRK4KineticIntegrator);
		pGravitasEngine->Logger().Out << "Registered RK4 kinetic integrator.\n";
	}
}

//----------------------------------------------------------------------------------------------
/// Initialises the Plug-In
//----------------------------------------------------------------------------------------------
void CoreIntegratorsPlugIn::Initialise(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Shuts the Plug-In down
//----------------------------------------------------------------------------------------------
void CoreIntegratorsPlugIn::Shutdown(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Unregisters the Plug-In from the engine subsystem.
//----------------------------------------------------------------------------------------------
void CoreIntegratorsPlugIn::Unregister(void) 
{ 
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();
	pGravitasEngine->Logger().Out << "Unregistering core integrators...\n";

	// kinetic integrators (in reverse order)
	KineticsManager *pKineticsManager = pGravitasEngine->GetKineticsManager();

	if (m_pRK4KineticIntegrator != NULL)
	{
		pKineticsManager->UnregisterKineticIntegrator(m_pRK4KineticIntegrator);
		pGravitasEngine->Logger().Out << "Unregistered RK4 kinetic integrator.\n";
		m_pRK4KineticIntegrator = NULL;
	}

	if (m_pVerletKineticIntegrator != NULL)
	{
		pKineticsManager->UnregisterKineticIntegrator(m_pVerletKineticIntegrator);
		pGravitasEngine->Logger().Out << "Unregistered Verlet kinetic integrator.\n";
		m_pVerletKineticIntegrator = NULL;
	}

	if (m_pEulerKineticIntegrator != NULL)
	{
		pKineticsManager->UnregisterKineticIntegrator(m_pEulerKineticIntegrator);
		pGravitasEngine->Logger().Out << "Unregistered Euler kinetic integrator.\n";
		m_pEulerKineticIntegrator = NULL;
	}

	GravitasEngine::SetInstance(NULL);
}

Meson_Gravitas_END
