#pragma once

#include "GrvCollisionDetector.h"

#include "TriangleMesh.h"
#include "ConvexPolyhedron.h"

Meson_Gravitas_Collision_BEGIN

class TriangleMeshToConvexPolyhedronCollisionDetector
	: public ICollisionDetector
{
private: // private variables;
	Meson::Common::Text::String m_strGeometryType1;
	Meson::Common::Text::String m_strGeometryType2;

private: // private methods
	bool TestIntersection(
		const Meson::Gravitas::Geometry::Triangle& p_triangle,
		const Meson::Gravitas::Geometry::ConvexPolyhedron& p_convexPolyhedron) const;
	bool TestIntersection(
		const Meson::Gravitas::Geometry::TriangleMeshNode* pTriangleMeshNode,
		const Meson::Gravitas::Geometry::ConvexPolyhedron& p_convexPolyhedron,
		const Meson::Gravitas::Geometry::BoundingAxisAlignedBox& p_boundingVolumePolyhedron) const;
	void ComputeContactManifold(
		const Meson::Gravitas::Geometry::Triangle& p_triangle,
		const Meson::Gravitas::Geometry::ConvexPolyhedron& p_convexPolyhedron,
		ContactManifold& p_contactManifold) const;
	void ComputeContactManifold(
		const Meson::Gravitas::Geometry::TriangleMeshNode* pTriangleMeshNode,
		const Meson::Gravitas::Geometry::ConvexPolyhedron& p_convexPolyhedron,
		const Meson::Gravitas::Geometry::BoundingAxisAlignedBox& p_boundingVolumePolyhedron,
		ContactManifold& p_contactManifold) const;

public: // public methods
	TriangleMeshToConvexPolyhedronCollisionDetector(void);
	~TriangleMeshToConvexPolyhedronCollisionDetector(void);

	// ICollisionDetector interface
	const Meson::Common::Text::String& GetGeometryType1(void);
	const Meson::Common::Text::String& GetGeometryType2(void);
	bool TestIntersection(
		const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
		const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
		const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement);
	bool EstimateImpact(
		const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
		const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
		const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
		const Meson::Common::Maths::TVector3<Real>& p_vecRelativeLinearVelocity,
		const Meson::Common::Maths::TVector3<Real>& p_vecRelativeAngularVelocity,
		Real p_rMaxTime,
		Real& p_rTime);
	void ComputeContactManifold(
		const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
		const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
		const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
		ContactManifold& p_contactManifold);
};

Meson_Gravitas_Collision_END
