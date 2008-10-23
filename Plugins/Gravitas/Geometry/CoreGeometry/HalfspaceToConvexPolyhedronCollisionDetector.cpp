#include "HalfspaceToConvexPolyhedronCollisionDetector.h"

#include "Halfspace.h"
#include "ConvexPolyhedron.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

HalfspaceToConvexPolyhedronCollisionDetector::HalfspaceToConvexPolyhedronCollisionDetector(void)
	: m_strGeometryType1("Halfspace")
	, m_strGeometryType2("ConvexPolyhedron")
{
}

HalfspaceToConvexPolyhedronCollisionDetector::~HalfspaceToConvexPolyhedronCollisionDetector(void)
{
}

const String& HalfspaceToConvexPolyhedronCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& HalfspaceToConvexPolyhedronCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool HalfspaceToConvexPolyhedronCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Halfspace", "Expected halfspace geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "ConvexPolyhedron", "Expected convex polyhedron geometry.")

	const ConvexPolyhedron& convexPolyhedron = (ConvexPolyhedron &) p_geometry2;

	for (size_t unIndex = 0; unIndex < convexPolyhedron.Vertices.Size(); unIndex++)
	{
		TPoint3<Real>& ptVertex = convexPolyhedron.Vertices[unIndex];
		TPoint3<Real> ptVertexRelative = p_trnRelativePlacement.ApplyCopy(ptVertex);
		if (ptVertexRelative.Y <= (Real) 0.0)
			return true;
	}

	return false;
}

bool HalfspaceToConvexPolyhedronCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for halfspace and convex polyhedron not implemented yet.",
		__FILE__, __LINE__);
}

void HalfspaceToConvexPolyhedronCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Halfspace", "Expected halfspace geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "ConvexPolyhedron", "Expected convex polyhedron geometry.")

	const ConvexPolyhedron& convexPolyhedron = (ConvexPolyhedron &) p_geometry2;

	p_contactManifold.ContactPoints.Clear();
	ContactPoint contactPoint;
	contactPoint.Normal = TVector3<Real>::Up;

	for (size_t unIndex = 0; unIndex < convexPolyhedron.Vertices.Size(); unIndex++)
	{
		TPoint3<Real>& ptVertex = convexPolyhedron.Vertices[unIndex];
		TPoint3<Real> ptVertexRelative = p_trnRelativePlacement.ApplyCopy(ptVertex);
		if (ptVertexRelative.Y > (Real) 0.0)
			continue;

		contactPoint.Position = ptVertexRelative;
		contactPoint.Penetration = -ptVertexRelative.Y;

		p_contactManifold.ContactPoints.Add(contactPoint);
	}
}
