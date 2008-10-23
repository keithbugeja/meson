#include "HalfspaceToCylinderCollisionDetector.h"

#include "ArrayList.h"

#include "Halfspace.h"
#include "Cylinder.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

HalfspaceToCylinderCollisionDetector::HalfspaceToCylinderCollisionDetector(void)
	: m_strGeometryType1("Halfspace")
	, m_strGeometryType2("Cylinder")
{
}

HalfspaceToCylinderCollisionDetector::~HalfspaceToCylinderCollisionDetector(void)
{
}

const String& HalfspaceToCylinderCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& HalfspaceToCylinderCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool HalfspaceToCylinderCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Halfspace", "Expected halfspace geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Cylinder", "Expected cylinder geometry.")

	const Cylinder& cylinder = (Cylinder &) p_geometry2;

	Real rHalfHeight(cylinder.Height * (Real) 0.5);
	LineSegment lnsShaft(
		TPoint3<Real>((Real) 0.0, -rHalfHeight, (Real) 0.0),
		TPoint3<Real>((Real) 0.0,  rHalfHeight, (Real) 0.0));
	p_trnRelativePlacement.Apply(lnsShaft.Start);
	p_trnRelativePlacement.Apply(lnsShaft.End);

	static TInterval<Real> itvProjection;
	Cylinder::ProjectToInterval(lnsShaft, cylinder.Radius, TVector3<Real>::Up, itvProjection);

	return itvProjection.Min <= (Real) 0.0;
}

bool HalfspaceToCylinderCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for halfspace and cylinder not implemented yet.",
		__FILE__, __LINE__);
}

void HalfspaceToCylinderCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Halfspace", "Expected halfspace geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Cylinder", "Expected cylinder geometry.")

	const Cylinder& cylinder = (Cylinder &) p_geometry2;

	Real rHalfHeight(cylinder.Height * (Real) 0.5);
	LineSegment lnsShaft(
		TPoint3<Real>((Real) 0.0, -rHalfHeight, (Real) 0.0),
		TPoint3<Real>((Real) 0.0,  rHalfHeight, (Real) 0.0));
	p_trnRelativePlacement.Apply(lnsShaft.Start);
	p_trnRelativePlacement.Apply(lnsShaft.End);

	static PointArrayList listVertices;
	Cylinder::EnumerateMaximalVertices(lnsShaft, cylinder.Radius, TVector3<Real>::Down,
		listVertices);

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
