#pragma once

#include "GrvCollisionDetector.h"

#include "TriangleMesh.h"

Meson_Gravitas_Collision_BEGIN

class TriangleMeshToSphereCollisionDetector
	: public ICollisionDetector
{
private: // private variables;
	Meson::Common::Text::String m_strGeometryType1;
	Meson::Common::Text::String m_strGeometryType2;

private: // private methods
	bool TestIntersection(
		const Meson::Gravitas::Geometry::TriangleMeshNode* pTriangleMeshNode,
		const Meson::Gravitas::Geometry::BoundingSphere& p_boundingSphere) const;
	void ComputeContactManifold(
		const Meson::Gravitas::Geometry::TriangleMeshNode* pTriangleMeshNode,
		const Meson::Gravitas::Geometry::BoundingSphere& p_boundingSphere,
		ContactManifold& p_contactManifold) const;

public: // public methods
	TriangleMeshToSphereCollisionDetector(void);
	~TriangleMeshToSphereCollisionDetector(void);

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
