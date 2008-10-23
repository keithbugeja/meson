//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstVistasEngine.h"
#include "CoreGeometryPlugIn.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
CoreGeometryPlugIn::CoreGeometryPlugIn(void) 
	: m_pName("CoreGeometry PlugIn")
	, m_version(0,0,1)
	, m_pBoxFactory(NULL)
	, m_pPlaneFactory(NULL)
	, m_pSphereFactory(NULL)
	, m_pCylinderFactory(NULL)
	, m_pWavefrontMeshFactory(NULL)
{
}
//----------------------------------------------------------------------------------------------
CoreGeometryPlugIn::~CoreGeometryPlugIn(void)
{
}
//----------------------------------------------------------------------------------------------
const String& CoreGeometryPlugIn::GetName(void) const
{
	return m_pName;
}
//----------------------------------------------------------------------------------------------
const Version& CoreGeometryPlugIn::GetVersion(void) const
{
	return m_version;
}
//----------------------------------------------------------------------------------------------
void CoreGeometryPlugIn::Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap) 
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	VistasEngine* pVistasEngine = (VistasEngine*)m_pSharedObjectMap->GetRegisteredObject("VistasEngine");
	MESON_ASSERT(pVistasEngine != NULL, "VistasEngine is not a registered shared object.");

	VistasEngine::SetInstance(pVistasEngine);
	pVistasEngine->Logger().Out << "Registering Core Geometry...\n";

	if (m_pBoxFactory == NULL)
	{
		m_pBoxFactory = new BoxFactory();
		pVistasEngine->GetGeometryManager()->RegisterGeometryFactory(m_pBoxFactory);
		pVistasEngine->Logger().Out << "Registered Box Factory.\n";
	}

	if (m_pCylinderFactory == NULL)
	{
		m_pCylinderFactory = new CylinderFactory();
		pVistasEngine->GetGeometryManager()->RegisterGeometryFactory(m_pCylinderFactory);
		pVistasEngine->Logger().Out << "Registered Cylinder Factory.\n";
	}

	if (m_pPlaneFactory == NULL)
	{
		m_pPlaneFactory = new PlaneFactory();
		pVistasEngine->GetGeometryManager()->RegisterGeometryFactory(m_pPlaneFactory);
		pVistasEngine->Logger().Out << "Registered Plane Factory.\n";
	}

	if (m_pSphereFactory == NULL)
	{
		m_pSphereFactory = new SphereFactory();
		pVistasEngine->GetGeometryManager()->RegisterGeometryFactory(m_pSphereFactory);
		pVistasEngine->Logger().Out << "Registered Sphere Factory.\n";
	}
	
	if (m_pWavefrontMeshFactory == NULL)
	{
		m_pWavefrontMeshFactory = new WavefrontMeshFactory();
		pVistasEngine->GetGeometryManager()->RegisterGeometryFactory(m_pWavefrontMeshFactory);
		pVistasEngine->Logger().Out << "Registered Wavefront Mesh Factory.\n";
	}
}
//----------------------------------------------------------------------------------------------
void CoreGeometryPlugIn::Initialise(void) 
{ 
}
//----------------------------------------------------------------------------------------------
void CoreGeometryPlugIn::Shutdown(void) 
{ 
}
//----------------------------------------------------------------------------------------------
void CoreGeometryPlugIn::Unregister(void) 
{ 
	VistasEngine* pVistasEngine = VistasEngine::GetInstance();
	pVistasEngine->GetInstance()->Logger().Out << "Unregistering Core Geometry...\n";

	if (m_pBoxFactory == NULL)
	{
		pVistasEngine->GetGeometryManager()->UnregisterGeometryFactory(m_pBoxFactory);
		pVistasEngine->Logger().Out << "Unregistered Box Factory.\n";
		SAFE_DELETE(m_pBoxFactory);
	}
	
	if (m_pCylinderFactory == NULL)
	{
		pVistasEngine->GetGeometryManager()->UnregisterGeometryFactory(m_pCylinderFactory);
		pVistasEngine->Logger().Out << "Unregistered Cylinder Factory.\n";
		SAFE_DELETE(m_pCylinderFactory);
	}

	if (m_pPlaneFactory == NULL)
	{
		pVistasEngine->GetGeometryManager()->UnregisterGeometryFactory(m_pPlaneFactory);
		pVistasEngine->Logger().Out << "Unregistered Plane Factory.\n";
		SAFE_DELETE(m_pPlaneFactory);
	}
	
	if (m_pSphereFactory == NULL)
	{
		pVistasEngine->GetGeometryManager()->UnregisterGeometryFactory(m_pSphereFactory);
		pVistasEngine->Logger().Out << "Unregistered Sphere Factory.\n";
		SAFE_DELETE(m_pSphereFactory);
	}

	if (m_pWavefrontMeshFactory == NULL)
	{
		pVistasEngine->GetGeometryManager()->UnregisterGeometryFactory(m_pWavefrontMeshFactory);
		pVistasEngine->Logger().Out << "Unregistered Wavefront Mesh Factory.\n";
		SAFE_DELETE(m_pWavefrontMeshFactory);
	}

	VistasEngine::SetInstance(NULL);
}

Meson_Vistas_END
