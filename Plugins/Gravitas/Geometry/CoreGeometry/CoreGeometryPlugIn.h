//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::CoreGeometryPlugIn
//	PlugIn interface for Vistas Direct3D9 renderer.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		09/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "String8.h"
#include "Version.h"
#include "PlugIn.h"

#include "GrvNamespaces.h"

#include "ParticleFactory.h"
#include "SphereFactory.h"
#include "BoxFactory.h"
#include "CylinderFactory.h"
#include "ConvexPolyhedronFactory.h"
#include "HalfspaceFactory.h"
#include "TriangleMeshFactory.h"

#include "ParticleCollisionDetector.h"
#include "SphereCollisionDetector.h"
#include "BoxCollisionDetector.h"
#include "CylinderCollisionDetector.h"
#include "ConvexPolyhedronCollisionDetector.h"
#include "TriangleMeshCollisionDetector.h"

#include "SphereToParticleCollisionDetector.h"
#include "BoxToSphereCollisionDetector.h"
#include "BoxToCylinderCollisionDetector.h"
#include "CylinderToSphereCollisionDetector.h"
#include "HalfspaceToSphereCollisionDetector.h"
#include "HalfspaceToBoxCollisionDetector.h"
#include "HalfspaceToCylinderCollisionDetector.h"
#include "HalfspaceToConvexPolyhedronCollisionDetector.h"
#include "ConvexPolyhedronToSphereCollisionDetector.h"
#include "ConvexPolyhedronToBoxCollisionDetector.h"
#include "ConvexPolyhedronToCylinderCollisionDetector.h"
#include "TriangleMeshToSphereCollisionDetector.h"
#include "TriangleMeshToBoxCollisionDetector.h"
#include "TriangleMeshToCylinderCollisionDetector.h"
#include "TriangleMeshToConvexPolyhedronCollisionDetector.h"
#include "TriangleMeshToHalfspaceCollisionDetector.h"

Meson_Gravitas_BEGIN

class CoreGeometryPlugIn : public Meson::Common::PlugIn::IPlugIn
{
private:
	Meson::Common::Text::String	m_pName;
	Meson::Common::Version	m_version;
	Meson::Common::PlugIn::SharedObjectMap *m_pSharedObjectMap;

	// geometry factories
	Meson::Gravitas::Geometry::ParticleFactory* m_pParticleFactory;
	Meson::Gravitas::Geometry::SphereFactory* m_pSphereFactory;
	Meson::Gravitas::Geometry::BoxFactory* m_pBoxFactory;
	Meson::Gravitas::Geometry::CylinderFactory* m_pCylinderFactory;
	Meson::Gravitas::Geometry::ConvexPolyhedronFactory* m_pConvexPolyhedronFactory;
	Meson::Gravitas::Geometry::HalfspaceFactory* m_pHalfspaceFactory;
	Meson::Gravitas::Geometry::TriangleMeshFactory* m_pTriangleMeshFactory;

	// self-collision detectors
	Meson::Gravitas::Collision::ParticleCollisionDetector* m_pParticleCollisionDetector;
	Meson::Gravitas::Collision::SphereCollisionDetector* m_pSphereCollisionDetector;
	Meson::Gravitas::Collision::BoxCollisionDetector* m_pBoxCollisionDetector;
	Meson::Gravitas::Collision::CylinderCollisionDetector* m_pCylinderCollisionDetector;
	Meson::Gravitas::Collision::ConvexPolyhedronCollisionDetector* m_pConvexPolyhedronCollisionDetector;
	Meson::Gravitas::Collision::TriangleMeshCollisionDetector* m_pTriangleMeshCollisionDetector;

	// mixed-collision detectors
	Meson::Gravitas::Collision::SphereToParticleCollisionDetector* m_pSphereToParticleCollisionDetector;

	Meson::Gravitas::Collision::BoxToSphereCollisionDetector* m_pBoxToSphereCollisionDetector;
	Meson::Gravitas::Collision::BoxToCylinderCollisionDetector* m_pBoxToCylinderCollisionDetector;

	Meson::Gravitas::Collision::CylinderToSphereCollisionDetector* m_pCylinderToSphereCollisionDetector;

	Meson::Gravitas::Collision::HalfspaceToSphereCollisionDetector* m_pHalfspaceToSphereCollisionDetector;
	Meson::Gravitas::Collision::HalfspaceToBoxCollisionDetector* m_pHalfspaceToBoxCollisionDetector;
	Meson::Gravitas::Collision::HalfspaceToCylinderCollisionDetector* m_pHalfspaceToCylinderCollisionDetector;
	Meson::Gravitas::Collision::HalfspaceToConvexPolyhedronCollisionDetector* m_pHalfspaceToConvexPolyhedronCollisionDetector;

	Meson::Gravitas::Collision::ConvexPolyhedronToSphereCollisionDetector* m_pConvexPolyhedronToSphereCollisionDetector;
	Meson::Gravitas::Collision::ConvexPolyhedronToBoxCollisionDetector* m_pConvexPolyhedronToBoxCollisionDetector;
	Meson::Gravitas::Collision::ConvexPolyhedronToCylinderCollisionDetector* m_pConvexPolyhedronToCylinderCollisionDetector;

	Meson::Gravitas::Collision::TriangleMeshToSphereCollisionDetector* m_pTriangleMeshToSphereCollisionDetector;
	Meson::Gravitas::Collision::TriangleMeshToBoxCollisionDetector* m_pTriangleMeshToBoxCollisionDetector;
	Meson::Gravitas::Collision::TriangleMeshToCylinderCollisionDetector* m_pTriangleMeshToCylinderCollisionDetector;
	Meson::Gravitas::Collision::TriangleMeshToConvexPolyhedronCollisionDetector* m_pTriangleMeshToConvexPolyhedronCollisionDetector;
	Meson::Gravitas::Collision::TriangleMeshToHalfspaceCollisionDetector* m_pTriangleMeshToHalfspaceCollisionDetector;

public:
	CoreGeometryPlugIn();
	~CoreGeometryPlugIn();

	const Meson::Common::Text::String& GetName(void) const;
	const Meson::Common::Version& GetVersion(void) const;

	void Register(Meson::Common::PlugIn::SharedObjectMap* p_pSharedObjectMap);
	void Unregister();

	void Initialise(void);
	void Shutdown(void);
};

Meson_Gravitas_END
