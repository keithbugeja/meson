//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreGeometryPlugIn
//	PlugIn interface for Gravitas Direct3D9 renderer.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		09/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------

#include "CoreGeometryPlugIn.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Collision;

Meson_Gravitas_BEGIN

//----------------------------------------------------------------------------------------------
///	CoreGeometryPlugIn Constructor
//----------------------------------------------------------------------------------------------
CoreGeometryPlugIn::CoreGeometryPlugIn(void)
	: m_pName("Core Geometry PlugIn")
	, m_version(0,0,1)
	, m_pParticleFactory(NULL)
	, m_pSphereFactory(NULL)
	, m_pBoxFactory(NULL)
	, m_pCylinderFactory(NULL)
	, m_pConvexPolyhedronFactory(NULL)
	, m_pHalfspaceFactory(NULL)
	, m_pTriangleMeshFactory(NULL)
	, m_pParticleCollisionDetector(NULL)
	, m_pSphereCollisionDetector(NULL)
	, m_pBoxCollisionDetector(NULL)
	, m_pCylinderCollisionDetector(NULL)
	, m_pConvexPolyhedronCollisionDetector(NULL)
	, m_pTriangleMeshCollisionDetector(NULL)
	, m_pSphereToParticleCollisionDetector(NULL)
	, m_pBoxToSphereCollisionDetector(NULL)
	, m_pBoxToCylinderCollisionDetector(NULL)
	, m_pCylinderToSphereCollisionDetector(NULL)
	, m_pHalfspaceToSphereCollisionDetector(NULL)
	, m_pHalfspaceToBoxCollisionDetector(NULL)
	, m_pHalfspaceToCylinderCollisionDetector(NULL)
	, m_pHalfspaceToConvexPolyhedronCollisionDetector(NULL)
	, m_pConvexPolyhedronToSphereCollisionDetector(NULL)
	, m_pConvexPolyhedronToBoxCollisionDetector(NULL)
	, m_pConvexPolyhedronToCylinderCollisionDetector(NULL)
	, m_pTriangleMeshToSphereCollisionDetector(NULL)
	, m_pTriangleMeshToBoxCollisionDetector(NULL)
	, m_pTriangleMeshToCylinderCollisionDetector(NULL)
	, m_pTriangleMeshToConvexPolyhedronCollisionDetector(NULL)
	, m_pTriangleMeshToHalfspaceCollisionDetector(NULL)
{
}

//----------------------------------------------------------------------------------------------
///	CoreGeometryPlugIn Destructor
//----------------------------------------------------------------------------------------------
CoreGeometryPlugIn::~CoreGeometryPlugIn(void)
{
}

//----------------------------------------------------------------------------------------------
/// Returns the name of the Plug-In
///	\return Plug-In name
//----------------------------------------------------------------------------------------------
const String& CoreGeometryPlugIn::GetName(void) const
{
	return m_pName;
}

//----------------------------------------------------------------------------------------------
/// Returns the version number of the Plug-In
/// \return Version number
//----------------------------------------------------------------------------------------------
const Version& CoreGeometryPlugIn::GetVersion(void) const
{
	return m_version;
}

//----------------------------------------------------------------------------------------------
/// Registers the Plug-In with the Engine subsystem
//----------------------------------------------------------------------------------------------
void CoreGeometryPlugIn::Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap) 
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	GravitasEngine* pGravitasEngine = (GravitasEngine*)m_pSharedObjectMap->GetRegisteredObject("GravitasEngine");
	MESON_ASSERT(pGravitasEngine != NULL, "GravitasEngine is not a registered shared object!");

	GravitasEngine::SetInstance(pGravitasEngine);
	pGravitasEngine->Logger().Out << "Registering Core Geometry...\n";

	// geometry factories
	GeometryManager *pGeometryManager = pGravitasEngine->GetGeometryManager();

	if (m_pParticleFactory == NULL)
	{
		m_pParticleFactory = new ParticleFactory();
		pGeometryManager->RegisterGeometryFactory(m_pParticleFactory);
		pGravitasEngine->Logger().Out << "Registered particle factory.\n";
	}

	if (m_pSphereFactory == NULL)
	{
		m_pSphereFactory = new SphereFactory();
		pGeometryManager->RegisterGeometryFactory(m_pSphereFactory);
		pGravitasEngine->Logger().Out << "Registered sphere factory.\n";
	}

	if (m_pBoxFactory == NULL)
	{
		m_pBoxFactory = new BoxFactory();
		pGeometryManager->RegisterGeometryFactory(m_pBoxFactory);
		pGravitasEngine->Logger().Out << "Registered box factory.\n";
	}

	if (m_pCylinderFactory == NULL)
	{
		m_pCylinderFactory = new CylinderFactory();
		pGeometryManager->RegisterGeometryFactory(m_pCylinderFactory);
		pGravitasEngine->Logger().Out << "Registered cylinder factory.\n";
	}

	if (m_pConvexPolyhedronFactory == NULL)
	{
		m_pConvexPolyhedronFactory = new ConvexPolyhedronFactory();
		pGeometryManager->RegisterGeometryFactory(m_pConvexPolyhedronFactory);
		pGravitasEngine->Logger().Out << "Registered convex polyhedron factory.\n";
	}

	if (m_pHalfspaceFactory == NULL)
	{
		m_pHalfspaceFactory = new HalfspaceFactory();
		pGeometryManager->RegisterGeometryFactory(m_pHalfspaceFactory);
		pGravitasEngine->Logger().Out << "Registered halfspace factory.\n";
	}

	if (m_pTriangleMeshFactory == NULL)
	{
		m_pTriangleMeshFactory = new TriangleMeshFactory();
		pGeometryManager->RegisterGeometryFactory(m_pTriangleMeshFactory);
		pGravitasEngine->Logger().Out << "Registered triangle mesh factory.\n";
	}

	// collision detectors
	CollisionManager *pCollisionManager = GravitasEngine::GetInstance()->GetCollisionManager();

	if (m_pParticleCollisionDetector == NULL)
	{
		m_pParticleCollisionDetector = new ParticleCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pParticleCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered particle-to-particle collision detector.\n";
	}

	if (m_pSphereCollisionDetector == NULL)
	{
		m_pSphereCollisionDetector = new SphereCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered sphere-to-sphere collision detector.\n";
	}

	if (m_pBoxCollisionDetector == NULL)
	{
		m_pBoxCollisionDetector = new BoxCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pBoxCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered box-to-box collision detector.\n";
	}

	if (m_pCylinderCollisionDetector == NULL)
	{
		m_pCylinderCollisionDetector = new CylinderCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pCylinderCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered cylinder-to-cylinder collision detector.\n";
	}

	if (m_pConvexPolyhedronCollisionDetector == NULL)
	{
		m_pConvexPolyhedronCollisionDetector = new ConvexPolyhedronCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pConvexPolyhedronCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered convex polyhedron-to-polyhedron collision detector.\n";
	}

	if (m_pTriangleMeshCollisionDetector == NULL)
	{
		m_pTriangleMeshCollisionDetector = new TriangleMeshCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pTriangleMeshCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered triangle-mesh-to-triangle-mesh collision detector.\n";
	}

	// mixed collision detectors
	if (m_pSphereToParticleCollisionDetector == NULL)
	{
		m_pSphereToParticleCollisionDetector = new SphereToParticleCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pSphereToParticleCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered sphere-to-particle collision detector.\n";
	}

	if (m_pBoxToSphereCollisionDetector == NULL)
	{
		m_pBoxToSphereCollisionDetector = new BoxToSphereCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pBoxToSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered box-to-sphere collision detector.\n";
	}

	if (m_pBoxToCylinderCollisionDetector == NULL)
	{
		m_pBoxToCylinderCollisionDetector = new BoxToCylinderCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pBoxToCylinderCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered box-to-cylinder collision detector.\n";
	}

	if (m_pCylinderToSphereCollisionDetector == NULL)
	{
		m_pCylinderToSphereCollisionDetector = new CylinderToSphereCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pCylinderToSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered cylinder-to-sphere collision detector.\n";
	}

	if (m_pHalfspaceToSphereCollisionDetector == NULL)
	{
		m_pHalfspaceToSphereCollisionDetector = new HalfspaceToSphereCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pHalfspaceToSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered halfspace-to-sphere collision detector.\n";
	}

	if (m_pHalfspaceToBoxCollisionDetector == NULL)
	{
		m_pHalfspaceToBoxCollisionDetector = new HalfspaceToBoxCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pHalfspaceToBoxCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered halfspace-to-box collision detector.\n";
	}

	if (m_pHalfspaceToCylinderCollisionDetector == NULL)
	{
		m_pHalfspaceToCylinderCollisionDetector = new HalfspaceToCylinderCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pHalfspaceToCylinderCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered halfspace-to-cylinder collision detector.\n";
	}

	if (m_pHalfspaceToConvexPolyhedronCollisionDetector == NULL)
	{
		m_pHalfspaceToConvexPolyhedronCollisionDetector = new HalfspaceToConvexPolyhedronCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pHalfspaceToConvexPolyhedronCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered halfspace-to-convex polyherdon collision detector.\n";
	}

	if (m_pConvexPolyhedronToSphereCollisionDetector == NULL)
	{
		m_pConvexPolyhedronToSphereCollisionDetector = new ConvexPolyhedronToSphereCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pConvexPolyhedronToSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered convex polyherdon-to-sphere collision detector.\n";
	}

	if (m_pConvexPolyhedronToBoxCollisionDetector == NULL)
	{
		m_pConvexPolyhedronToBoxCollisionDetector = new ConvexPolyhedronToBoxCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pConvexPolyhedronToBoxCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered convex polyherdon-to-box collision detector.\n";
	}

	if (m_pConvexPolyhedronToCylinderCollisionDetector == NULL)
	{
		m_pConvexPolyhedronToCylinderCollisionDetector = new ConvexPolyhedronToCylinderCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pConvexPolyhedronToCylinderCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered convex polyherdon-to-cylinder collision detector.\n";
	}

	if (m_pTriangleMeshToSphereCollisionDetector == NULL)
	{
		m_pTriangleMeshToSphereCollisionDetector = new TriangleMeshToSphereCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pTriangleMeshToSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered triangle mesh-to-sphere collision detector.\n";
	}

	if (m_pTriangleMeshToBoxCollisionDetector == NULL)
	{
		m_pTriangleMeshToBoxCollisionDetector = new TriangleMeshToBoxCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pTriangleMeshToBoxCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered triangle mesh-to-box collision detector.\n";
	}

	if (m_pTriangleMeshToCylinderCollisionDetector == NULL)
	{
		m_pTriangleMeshToCylinderCollisionDetector = new TriangleMeshToCylinderCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pTriangleMeshToCylinderCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered triangle mesh-to-cylinder collision detector.\n";
	}

	if (m_pTriangleMeshToConvexPolyhedronCollisionDetector == NULL)
	{
		m_pTriangleMeshToConvexPolyhedronCollisionDetector = new TriangleMeshToConvexPolyhedronCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pTriangleMeshToConvexPolyhedronCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered triangle mesh-to-convex polyhedron collision detector.\n";
	}

	if (m_pTriangleMeshToHalfspaceCollisionDetector == NULL)
	{
		m_pTriangleMeshToHalfspaceCollisionDetector = new TriangleMeshToHalfspaceCollisionDetector();
		pCollisionManager->RegisterCollisionDetector(m_pTriangleMeshToHalfspaceCollisionDetector);
		pGravitasEngine->Logger().Out << "Registered triangle mesh-to-halfspace collision detector.\n";
	}
}

//----------------------------------------------------------------------------------------------
/// Initialises the Plug-In
//----------------------------------------------------------------------------------------------
void CoreGeometryPlugIn::Initialise(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Shuts the Plug-In down
//----------------------------------------------------------------------------------------------
void CoreGeometryPlugIn::Shutdown(void) 
{ 
}

//----------------------------------------------------------------------------------------------
/// Unregisters the Plug-In from the engine subsystem.
//----------------------------------------------------------------------------------------------
void CoreGeometryPlugIn::Unregister(void) 
{ 
	GravitasEngine* pGravitasEngine = GravitasEngine::GetInstance();
	pGravitasEngine->Logger().Out << "Unregistering Core Geometry...\n";

	// mixed collision detectors (in reverse order)
	CollisionManager *pCollisionManager = pGravitasEngine->GetCollisionManager();

	if (m_pTriangleMeshToHalfspaceCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pTriangleMeshToHalfspaceCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered triangle mesh-to-halfspace collision detector.\n";
		delete m_pTriangleMeshToHalfspaceCollisionDetector;
		m_pTriangleMeshToHalfspaceCollisionDetector = NULL;
	}

	if (m_pTriangleMeshToConvexPolyhedronCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pTriangleMeshToConvexPolyhedronCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered triangle mesh-to-convex polyhedron collision detector.\n";
		delete m_pTriangleMeshToConvexPolyhedronCollisionDetector;
		m_pTriangleMeshToConvexPolyhedronCollisionDetector = NULL;
	}

	if (m_pTriangleMeshToCylinderCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pTriangleMeshToCylinderCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered triangle mesh-to-cylinder collision detector.\n";
		delete m_pTriangleMeshToCylinderCollisionDetector;
		m_pTriangleMeshToCylinderCollisionDetector = NULL;
	}

	if (m_pTriangleMeshToBoxCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pTriangleMeshToBoxCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered triangle mesh-to-box collision detector.\n";
		delete m_pTriangleMeshToBoxCollisionDetector;
		m_pTriangleMeshToBoxCollisionDetector = NULL;
	}

	if (m_pTriangleMeshToSphereCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pTriangleMeshToSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered triangle mesh-to-sphere collision detector.\n";
		delete m_pTriangleMeshToSphereCollisionDetector;
		m_pTriangleMeshToSphereCollisionDetector = NULL;
	}

	if (m_pConvexPolyhedronToCylinderCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pConvexPolyhedronToCylinderCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered convex polyhedron-to-cylinder collision detector.\n";
		delete m_pConvexPolyhedronToCylinderCollisionDetector;
		m_pConvexPolyhedronToCylinderCollisionDetector = NULL;
	}

	if (m_pConvexPolyhedronToBoxCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pConvexPolyhedronToBoxCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered convex polyhedron-to-box collision detector.\n";
		delete m_pConvexPolyhedronToBoxCollisionDetector;
		m_pConvexPolyhedronToBoxCollisionDetector = NULL;
	}

	if (m_pConvexPolyhedronToSphereCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pConvexPolyhedronToSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered convex polyhedron-to-sphere collision detector.\n";
		delete m_pConvexPolyhedronToSphereCollisionDetector;
		m_pConvexPolyhedronToSphereCollisionDetector = NULL;
	}

	if (m_pHalfspaceToConvexPolyhedronCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pHalfspaceToConvexPolyhedronCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered halfspace-to-convex polyhedron collision detector.\n";
		delete m_pHalfspaceToConvexPolyhedronCollisionDetector;
		m_pHalfspaceToConvexPolyhedronCollisionDetector = NULL;
	}

	if (m_pHalfspaceToCylinderCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pHalfspaceToCylinderCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered halfspace-to-cylinder collision detector.\n";
		delete m_pHalfspaceToCylinderCollisionDetector;
		m_pHalfspaceToCylinderCollisionDetector = NULL;
	}

	if (m_pHalfspaceToBoxCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pHalfspaceToBoxCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered halfspace-to-box collision detector.\n";
		delete m_pHalfspaceToBoxCollisionDetector;
		m_pHalfspaceToBoxCollisionDetector = NULL;
	}

	if (m_pHalfspaceToSphereCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pHalfspaceToSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered halfspace-to-sphere collision detector.\n";
		delete m_pHalfspaceToSphereCollisionDetector;
		m_pHalfspaceToSphereCollisionDetector = NULL;
	}

	if (m_pCylinderToSphereCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pCylinderToSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered cylinder-to-sphere collision detector.\n";
		delete m_pCylinderToSphereCollisionDetector;
		m_pCylinderToSphereCollisionDetector = NULL;
	}

	if (m_pBoxToCylinderCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pBoxToCylinderCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered box-to-cylinder collision detector.\n";
		delete m_pBoxToCylinderCollisionDetector;
		m_pBoxToCylinderCollisionDetector = NULL;
	}

	if (m_pBoxToSphereCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pBoxToSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered box-to-sphere collision detector.\n";
		delete m_pBoxToSphereCollisionDetector;
		m_pBoxToSphereCollisionDetector = NULL;
	}

	if (m_pSphereToParticleCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pSphereToParticleCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered sphere-to-particle collision detector.\n";
		delete m_pSphereToParticleCollisionDetector;
		m_pSphereToParticleCollisionDetector = NULL;
	}

	// collision detectors (in reverse order)
	if (m_pTriangleMeshCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pTriangleMeshCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered triangle-mesh-to-triangle-mesh collision detector.\n";
		delete m_pTriangleMeshCollisionDetector;
		m_pTriangleMeshCollisionDetector = NULL;
	}
	if (m_pConvexPolyhedronCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pConvexPolyhedronCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered convex polyhedron-to-polyhedron collision detector.\n";
		delete m_pConvexPolyhedronCollisionDetector;
		m_pConvexPolyhedronCollisionDetector = NULL;
	}

	if (m_pCylinderCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pCylinderCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered cylinder-to-cylinder collision detector.\n";
		delete m_pCylinderCollisionDetector;
		m_pCylinderCollisionDetector = NULL;
	}

	if (m_pBoxCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pBoxCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered box-to-box collision detector.\n";
		delete m_pBoxCollisionDetector;
		m_pBoxCollisionDetector = NULL;
	}

	if (m_pSphereCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pSphereCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered sphere-to-sphere collision detector.\n";
		delete m_pSphereCollisionDetector;
		m_pSphereCollisionDetector = NULL;
	}

	if (m_pParticleCollisionDetector != NULL)
	{
		pCollisionManager->UnregisterCollisionDetector(m_pParticleCollisionDetector);
		pGravitasEngine->Logger().Out << "Unregistered particle-to-particle collision detector.\n";
		delete m_pParticleCollisionDetector;
		m_pParticleCollisionDetector = NULL;
	}

	// geometry factories (in reverse order)
	GeometryManager *pGeometryManager = pGravitasEngine->GetGeometryManager();

	if (m_pTriangleMeshFactory != NULL)
	{
		pGeometryManager->UnregisterGeometryFactory(m_pTriangleMeshFactory);
		pGravitasEngine->Logger().Out << "Unregistered triangle mesh factory.\n";
		delete m_pTriangleMeshFactory;
		m_pTriangleMeshFactory = NULL;
	}

	if (m_pHalfspaceFactory != NULL)
	{
		pGeometryManager->UnregisterGeometryFactory(m_pHalfspaceFactory);
		pGravitasEngine->Logger().Out << "Unregistered halfspace factory.\n";
		delete m_pHalfspaceFactory;
		m_pHalfspaceFactory = NULL;
	}

	if (m_pConvexPolyhedronFactory != NULL)
	{
		pGeometryManager->UnregisterGeometryFactory(m_pConvexPolyhedronFactory);
		pGravitasEngine->Logger().Out << "Unregistered convex polyhedron factory.\n";
		delete m_pConvexPolyhedronFactory;
		m_pConvexPolyhedronFactory = NULL;
	}

	if (m_pCylinderFactory != NULL)
	{
		pGeometryManager->UnregisterGeometryFactory(m_pCylinderFactory);
		pGravitasEngine->Logger().Out << "Unregistered cylinder factory.\n";
		delete m_pCylinderFactory;
		m_pCylinderFactory = NULL;
	}

	if (m_pBoxFactory != NULL)
	{
		pGeometryManager->UnregisterGeometryFactory(m_pBoxFactory);
		pGravitasEngine->Logger().Out << "Unregistered box factory.\n";
		delete m_pBoxFactory;
		m_pBoxFactory = NULL;
	}

	if (m_pSphereFactory != NULL)
	{
		pGeometryManager->UnregisterGeometryFactory(m_pSphereFactory);
		pGravitasEngine->Logger().Out << "Unregistered sphere factory.\n";
		delete m_pSphereFactory;
		m_pSphereFactory = NULL;
	}

	if (m_pParticleFactory != NULL)
	{
		pGeometryManager->UnregisterGeometryFactory(m_pParticleFactory);
		pGravitasEngine->Logger().Out << "Unregistered particle factory.\n";
		delete m_pParticleFactory;
		m_pParticleFactory = NULL;
	}

	GravitasEngine::SetInstance(NULL);
}

Meson_Gravitas_END
