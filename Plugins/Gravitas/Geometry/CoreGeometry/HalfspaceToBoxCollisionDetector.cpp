#include "HalfspaceToBoxCollisionDetector.h"

#include "ArrayList.h"

#include "Halfspace.h"
#include "Box.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

HalfspaceToBoxCollisionDetector::HalfspaceToBoxCollisionDetector(void)
	: m_strGeometryType1("Halfspace")
	, m_strGeometryType2("Box")
{
}

HalfspaceToBoxCollisionDetector::~HalfspaceToBoxCollisionDetector(void)
{
}

const String& HalfspaceToBoxCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& HalfspaceToBoxCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool HalfspaceToBoxCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Halfspace", "Expected halfspace geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Box", "Expected box geometry.")

	const Box& box = (Box &) p_geometry2;

	BoundingOrientedBox boundingOrientedBox(TPoint3<Real>::Origin, box.Extent);
	boundingOrientedBox.Transform(p_trnRelativePlacement);
	static TInterval<Real> itvProjection;
	boundingOrientedBox.ProjectToInterval(TVector3<Real>::Up, itvProjection);

	return itvProjection.Min <= (Real) 0.0;
}

bool HalfspaceToBoxCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for halfspace and box not implemented yet.",
		__FILE__, __LINE__);
}

void HalfspaceToBoxCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Halfspace", "Expected halfspace geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Box", "Expected box geometry.")

	const Box& box = (Box &) p_geometry2;

	BoundingOrientedBox boundingOrientedBox(TPoint3<Real>::Origin, box.Extent);
	boundingOrientedBox.Transform(p_trnRelativePlacement);

	static PointArrayList listVertices;
	boundingOrientedBox.EnumerateMaximalVertices(TVector3<Real>::Down, listVertices);

	p_contactManifold.ContactPoints.Clear();

	static ContactPoint contactPoint;
	contactPoint.Normal = TVector3<Real>::Up;

	size_t unCount = listVertices.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		TPoint3<Real>& ptVertex = listVertices[unIndex];
		if (ptVertex.Y > (Real) 0.0)
			continue;

		contactPoint.Position = ptVertex;
		contactPoint.Penetration = -ptVertex.Y;
		p_contactManifold.ContactPoints.Add(contactPoint);
	}
}
