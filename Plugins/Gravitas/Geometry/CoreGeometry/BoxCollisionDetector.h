#pragma once

#include "GrvCollisionDetector.h"

Meson_Gravitas_Collision_BEGIN

class BoxCollisionDetector
	: public ICollisionDetector
{
private: // private variables;
	Meson::Common::Text::String m_strGeometryType;
	VectorArrayList m_listVertices1;
	VectorArrayList m_listVertices2;

private: // private methods
	bool EstimateImpact(
		const Meson::Common::Maths::TVector3<Real>& p_vecAxis,
		const Meson::Gravitas::Geometry::BoundingOrientedBox& p_boundingOrientedBox1,
		const Meson::Gravitas::Geometry::BoundingOrientedBox& p_boundingOrientedBox2,
		const Meson::Common::Maths::TVector3<Real>& p_vecRelativeLinearVelocity,
		Meson::Common::Maths::TInterval<Real>& p_itvContact);
	bool Intersects(
		const Meson::Gravitas::Geometry::BoundingOrientedBox& p_boundingOrientedBox,
		const Meson::Common::Maths::TVector3<Real>& p_vecTestPoint);

public: // public methods
	BoxCollisionDetector(void);
	~BoxCollisionDetector(void);

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
