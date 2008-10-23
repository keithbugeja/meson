//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreConstraintsPlugIn
//	PlugIn interface for the Gravitas core constraints implementations.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		18/02/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#include "CoreConstraintsPlugIn.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas::Constraint;

Meson_Gravitas_BEGIN

//----------------------------------------------------------------------------------------------
///	CoreConstraintsPlugIn Constructor
//----------------------------------------------------------------------------------------------
CoreConstraintsPlugIn::CoreConstraintsPlugIn(void)
	: m_pName("Core Constraints PlugIn")
	, m_version(0,0,1)
	, m_pSphericalJointConstraintFactory(NULL)
	, m_pRevoluteJointConstraintFactory(NULL)
	, m_pAngularMotorConstraintFactory(NULL)
	, m_pRigidJointConstraintFactory(NULL)
	, m_pSequentialConstraintSolver(NULL)
	, m_pPenaltyBasedConstraintSolver(NULL)
{
}

//----------------------------------------------------------------------------------------------
///	CoreConstraintsPlugIn Destructor
//----------------------------------------------------------------------------------------------
CoreConstraintsPlugIn::~CoreConstraintsPlugIn(void)
{
}

//----------------------------------------------------------------------------------------------
/// Returns the name of the Plug-In
///	\return Plug-In name
//----------------------------------------------------------------------------------------------
const String& CoreConstraintsPlugIn::GetName(void) const
{
	return m_pName;
}

//----------------------------------------------------------------------------------------------
/// Returns the version number of the Plug-In
/// \return Version number
//----------------------------------------------------------------------------------------------
const Version& CoreConstraintsPlugIn::GetVersion(void) const
{
	return m_version;
}

//----------------------------------------------------------------------------------------------
/// Registers the Plug-In with the Engine subsystem
//----------------------------------------------------------------------------------------------
void CoreConstraintsPlugIn::Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap) 
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	GravitasEngine* pGravitasEngine = (GravitasEngine*)m_pSharedObjectMap->GetRegisteredObject("GravitasEngine");
	MESON_ASSERT(pGravitasEngine != NULL, "GravitasEngine is not a registered shared object!");

	GravitasEngine::SetInstance(pGravitasEngine);
	pGravitasEngine->Logger().Out << "Registering Core Constraints...\n";

	ConstraintManager *pConstraintManager = pGravitasEngine->GetConstraintManager();

	// constraint factories
	if (m_pSphericalJointConstraintFactory == NULL)
	{
		m_pSphericalJointConstraintFactory = new SphericalJointConstraintFactory();
		pConstraintManager->RegisterConstraintFactory(m_pSphericalJointConstraintFactory);
		pGravitasEngine->Logger().Out << "Registered SphericalJoint constraint factory.\n";
	}

	if (m_pRevoluteJointConstraintFactory == NULL)
	{
		m_pRevoluteJointConstraintFactory = new RevoluteJointConstraintFactory();
		pConstraintManager->RegisterConstraintFactory(m_pRevoluteJointConstraintFactory);
		pGravitasEngine->Logger().Out << "Registered RevoluteJoint constraint factory.\n";
	}

	if (m_pAngularMotorConstraintFactory == NULL)
	{
		m_pAngularMotorConstraintFactory = new AngularMotorConstraintFactory();
		pConstraintManager->RegisterConstraintFactory(m_pAngularMotorConstraintFactory);
		pGravitasEngine->Logger().Out << "Registered AngularMotor constraint factory.\n";
	}

	if (m_pRigidJointConstraintFactory == NULL)
	{
		m_pRigidJointConstraintFactory = new RigidJointConstraintFactory();
		pConstraintManager->RegisterConstraintFactory(m_pRigidJointConstraintFactory);
		pGravitasEngine->Logger().Out << "Registered RigidJoint constraint factory.\n";
	}

	// constraint solvers
	if (m_pSequentialConstraintSolver == NULL)
	{
		m_pSequentialConstraintSolver = new SequentialConstraintSolver();
		pConstraintManager->RegisterConstraintSolver(m_pSequentialConstraintSolver);
		pGravitasEngine->Logger().Out << "Registered sequential constraint solver.\n";
	}

	if (m_pPenaltyBasedConstraintSolver == NULL)
	{
		m_pPenaltyBasedConstraintSolver = new PenaltyBasedConstraintSolver();
		pConstraintManager->RegisterConstraintSolver(m_pPenaltyBasedConstraintSolver);
		pGravitasEngine->Logger().Out << "Registered penalty-based constraint solver.\n";
	}
}

//----------------------------------------------------------------------------------------------
/// Initialises the Plug-In
//----------------------------------------------------------------------------------------------
void CoreConstraintsPlugIn::Initialise(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Shuts the Plug-In down
//----------------------------------------------------------------------------------------------
void CoreConstraintsPlugIn::Shutdown(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Unregisters the Plug-In from the engine subsystem.
//----------------------------------------------------------------------------------------------
void CoreConstraintsPlugIn::Unregister(void) 
{ 
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();
	pGravitasEngine->Logger().Out << "Unregistering Core Constraints...\n";

	ConstraintManager *pConstraintManager = pGravitasEngine->GetConstraintManager();

	// constraint solvers (in reverse order)
	if (m_pPenaltyBasedConstraintSolver != NULL)
	{
		pConstraintManager->UnregisterConstraintSolver(m_pPenaltyBasedConstraintSolver);
		pGravitasEngine->Logger().Out << "Unregistered penalty-based constraint solver.\n";
		delete m_pPenaltyBasedConstraintSolver;
		m_pPenaltyBasedConstraintSolver = NULL;
	}

	if (m_pSequentialConstraintSolver != NULL)
	{
		pConstraintManager->UnregisterConstraintSolver(m_pSequentialConstraintSolver);
		pGravitasEngine->Logger().Out << "Unregistered sequential constraint solver.\n";
		delete m_pSequentialConstraintSolver;
		m_pSequentialConstraintSolver = NULL;
	}

	// constraint factories (in reverse order)
	if (m_pRigidJointConstraintFactory != NULL)
	{
		pConstraintManager->UnregisterConstraintFactory(m_pRigidJointConstraintFactory);
		pGravitasEngine->Logger().Out << "Unregistered RigidJoint constraint factory.\n";
		delete m_pRigidJointConstraintFactory;
		m_pRigidJointConstraintFactory = NULL;
	}

	if (m_pAngularMotorConstraintFactory != NULL)
	{
		pConstraintManager->UnregisterConstraintFactory(m_pAngularMotorConstraintFactory);
		pGravitasEngine->Logger().Out << "Unregistered AngularMotor constraint factory.\n";
		delete m_pAngularMotorConstraintFactory;
		m_pAngularMotorConstraintFactory = NULL;
	}

	if (m_pRevoluteJointConstraintFactory != NULL)
	{
		pConstraintManager->UnregisterConstraintFactory(m_pRevoluteJointConstraintFactory);
		pGravitasEngine->Logger().Out << "Unregistered RevoluteJoint constraint factory.\n";
		delete m_pRevoluteJointConstraintFactory;
		m_pRevoluteJointConstraintFactory = NULL;
	}

	if (m_pSphericalJointConstraintFactory != NULL)
	{
		pConstraintManager->UnregisterConstraintFactory(m_pSphericalJointConstraintFactory);
		pGravitasEngine->Logger().Out << "Unregistered SphericalJoint constraint factory.\n";
		delete m_pSphericalJointConstraintFactory;
		m_pSphericalJointConstraintFactory = NULL;
	}

	GravitasEngine::SetInstance(NULL);
}

Meson_Gravitas_END
