#include "BoxToCylinderCollisionDetector.h"

#include "Box.h"
#include "Cylinder.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

BoxToCylinderCollisionDetector::BoxToCylinderCollisionDetector(void)
	: m_strGeometryType1("Box")
	, m_strGeometryType2("Cylinder")
	, m_listVertices1()
	, m_listVertices2()
{
}

BoxToCylinderCollisionDetector::~BoxToCylinderCollisionDetector(void)
{
}

const String& BoxToCylinderCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& BoxToCylinderCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool BoxToCylinderCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Box", "Expected box geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Cylinder", "Expected cylinder geometry.")

	const Box& box = (Box &) p_geometry1;
	const Cylinder& cylinder = (Cylinder &) p_geometry2;

	BoundingAxisAlignedBox boundingAxisAlignedBox(-box.Extent, box.Extent);

	static LineSegment lnsShaft;
	Real rHalfHeight = cylinder.Height * (Real) 0.5;
	lnsShaft.Start = TVector3<Real>((Real) 0.0, -rHalfHeight, (Real) 0.0);
	lnsShaft.End = TVector3<Real>((Real) 0.0, rHalfHeight, (Real) 0.0);
	lnsShaft.Transform(p_trnRelativePlacement);
	Real rRadius = cylinder.Radius;

	// start separating axis tests
	static TInterval<Real> itvProjection1, itvProjection2;

	// box axis 1
	itvProjection1.Min = boundingAxisAlignedBox.Min.X;
	itvProjection1.Max = boundingAxisAlignedBox.Max.X;
	Cylinder::ProjectToInterval(lnsShaft, rRadius, TVector3<Real>::Right, itvProjection2);
	if (!itvProjection1.Intersects(itvProjection2)) return false;

	// box axis 2
	itvProjection1.Min = boundingAxisAlignedBox.Min.Y;
	itvProjection1.Max = boundingAxisAlignedBox.Max.Y;
	Cylinder::ProjectToInterval(lnsShaft, rRadius, TVector3<Real>::Up, itvProjection2);
	if (!itvProjection1.Intersects(itvProjection2)) return false;

	// box axis 3
	itvProjection1.Min = boundingAxisAlignedBox.Min.Z;
	itvProjection1.Max = boundingAxisAlignedBox.Max.Z;
	Cylinder::ProjectToInterval(lnsShaft, rRadius, TVector3<Real>::Out, itvProjection2);
	if (!itvProjection1.Intersects(itvProjection2)) return false;

	TVector3<Real> vecShaftDirection = lnsShaft.Direction();

	// cylinder shaft
	boundingAxisAlignedBox.ProjectToInterval(vecShaftDirection, itvProjection1);
	Cylinder::ProjectToInterval(lnsShaft, rRadius, vecShaftDirection, itvProjection2);
	if (!itvProjection1.Intersects(itvProjection2)) return false;

	// cross-product between shaft and box axis 1
	TVector3<Real> vecAxis = vecShaftDirection ^ TVector3<Real>::Right;
	if (!vecAxis.IsZero())
	{
		vecAxis.Normalise();
		boundingAxisAlignedBox.ProjectToInterval(vecAxis, itvProjection1);
		Cylinder::ProjectToInterval(lnsShaft, rRadius, vecAxis, itvProjection2);
		if (!itvProjection1.Intersects(itvProjection2)) return false;
	}

	// cross-product between shaft and box axis 2
	vecAxis = vecShaftDirection ^ TVector3<Real>::Up;
	if (!vecAxis.IsZero())
	{
		vecAxis.Normalise();
		boundingAxisAlignedBox.ProjectToInterval(vecAxis, itvProjection1);
		Cylinder::ProjectToInterval(lnsShaft, rRadius, vecAxis, itvProjection2);
		if (!itvProjection1.Intersects(itvProjection2)) return false;
	}

	// cross-product between shaft and box axis 3
	vecAxis = vecShaftDirection ^ TVector3<Real>::Out;
	if (!vecAxis.IsZero())
	{
		vecAxis.Normalise();
		boundingAxisAlignedBox.ProjectToInterval(vecAxis, itvProjection1);
		Cylinder::ProjectToInterval(lnsShaft, rRadius, vecAxis, itvProjection2);
		if (!itvProjection1.Intersects(itvProjection2)) return false;
	}

	// otherwise, must be intersecting
	return true;
}

bool BoxToCylinderCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for Box and Cylinder not implemented yet.",
		__FILE__, __LINE__);
}

void BoxToCylinderCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Box", "Expected box geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Cylinder", "Expected cylinder geometry.")

	p_contactManifold.ContactPoints.Clear();

	const Box& box = (Box &) p_geometry1;
	const Cylinder& cylinder = (Cylinder &) p_geometry2;

	BoundingAxisAlignedBox boundingAxisAlignedBox(-box.Extent, box.Extent);

	static LineSegment lnsShaft;
	Real rHalfHeight = cylinder.Height * (Real) 0.5;
	lnsShaft.Start = TVector3<Real>((Real) 0.0, -rHalfHeight, (Real) 0.0);
	lnsShaft.End = TVector3<Real>((Real) 0.0, rHalfHeight, (Real) 0.0);
	lnsShaft.Transform(p_trnRelativePlacement);
	Real rRadius = cylinder.Radius;

	// start separating axis tests
	static TInterval<Real> itvProjection1, itvProjection2, itvOverlap;
	Real rLeastOverlap = TMaths<Real>::Maximum, rCurrentOverlap;
	TVector3<Real> vecAxisLeastOverlap;

	// box axis 1
	itvProjection1.Min = boundingAxisAlignedBox.Min.X;
	itvProjection1.Max = boundingAxisAlignedBox.Max.X;
	Cylinder::ProjectToInterval(lnsShaft, rRadius, TVector3<Real>::Right, itvProjection2);

	itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
	if (itvOverlap.IsEmpty()) return;

	rCurrentOverlap = itvOverlap.Span();
	if (rLeastOverlap > rCurrentOverlap)
	{
		vecAxisLeastOverlap = TVector3<Real>::Right;
		rLeastOverlap = rCurrentOverlap;
	}

	// box axis 2
	itvProjection1.Min = boundingAxisAlignedBox.Min.Y;
	itvProjection1.Max = boundingAxisAlignedBox.Max.Y;
	Cylinder::ProjectToInterval(lnsShaft, rRadius, TVector3<Real>::Up, itvProjection2);

	itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
	if (itvOverlap.IsEmpty()) return;

	rCurrentOverlap = itvOverlap.Span();
	if (rLeastOverlap > rCurrentOverlap)
	{
		vecAxisLeastOverlap = TVector3<Real>::Up;
		rLeastOverlap = rCurrentOverlap;
	}

	// box axis 3
	itvProjection1.Min = boundingAxisAlignedBox.Min.Z;
	itvProjection1.Max = boundingAxisAlignedBox.Max.Z;
	Cylinder::ProjectToInterval(lnsShaft, rRadius, TVector3<Real>::Out, itvProjection2);

	itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
	if (itvOverlap.IsEmpty()) return;

	rCurrentOverlap = itvOverlap.Span();
	if (rLeastOverlap > rCurrentOverlap)
	{
		vecAxisLeastOverlap = TVector3<Real>::Out;
		rLeastOverlap = rCurrentOverlap;
	}

	// cylinder shaft
	TVector3<Real> vecShaftDirection = lnsShaft.Direction();
	boundingAxisAlignedBox.ProjectToInterval(vecShaftDirection, itvProjection1);
	Cylinder::ProjectToInterval(lnsShaft, rRadius, vecShaftDirection, itvProjection2);

	itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
	if (itvOverlap.IsEmpty()) return;

	rCurrentOverlap = itvOverlap.Span();
	if (rLeastOverlap > rCurrentOverlap)
	{
		vecAxisLeastOverlap = vecShaftDirection;
		rLeastOverlap = rCurrentOverlap;
	}

	// cross-product between shaft and box axis 1
	TVector3<Real> vecAxis = vecShaftDirection ^ TVector3<Real>::Right;
	if (!vecAxis.IsZero())
	{
		vecAxis.Normalise();
		boundingAxisAlignedBox.ProjectToInterval(vecAxis, itvProjection1);
		Cylinder::ProjectToInterval(lnsShaft, rRadius, vecAxis, itvProjection2);

		itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
		if (itvOverlap.IsEmpty()) return;

		rCurrentOverlap = itvOverlap.Span();
		if (rLeastOverlap > rCurrentOverlap)
		{
			vecAxisLeastOverlap = vecAxis;
			rLeastOverlap = rCurrentOverlap;
		}
	}

	// cross-product between shaft and box axis 2
	vecAxis = vecShaftDirection ^ TVector3<Real>::Up;
	if (!vecAxis.IsZero())
	{
		vecAxis.Normalise();
		boundingAxisAlignedBox.ProjectToInterval(vecAxis, itvProjection1);
		Cylinder::ProjectToInterval(lnsShaft, rRadius, vecAxis, itvProjection2);

		itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
		if (itvOverlap.IsEmpty()) return;

		rCurrentOverlap = itvOverlap.Span();
		if (rLeastOverlap > rCurrentOverlap)
		{
			vecAxisLeastOverlap = vecAxis;
			rLeastOverlap = rCurrentOverlap;
		}
	}

	// cross-product between shaft and box axis 3
	vecAxis = vecShaftDirection ^ TVector3<Real>::Out;
	if (!vecAxis.IsZero())
	{
		vecAxis.Normalise();
		boundingAxisAlignedBox.ProjectToInterval(vecAxis, itvProjection1);
		Cylinder::ProjectToInterval(lnsShaft, rRadius, vecAxis, itvProjection2);

		itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
		if (itvOverlap.IsEmpty()) return;

		rCurrentOverlap = itvOverlap.Span();
		if (rLeastOverlap > rCurrentOverlap)
		{
			vecAxisLeastOverlap = vecAxis;
			rLeastOverlap = rCurrentOverlap;
		}
	}

	// otherwise, must be intersecting - prepare contacts

	// align axis of least overlap along CM offset
	if (vecAxisLeastOverlap * p_trnRelativePlacement.Translation < (Real) 0.0)
		vecAxisLeastOverlap.Negate();

	// prepare contact info
	static ContactPoint contactPoint;
	contactPoint.Normal = vecAxisLeastOverlap;
	contactPoint.Penetration = rLeastOverlap;

	// compute maximal / minimal vertices
	boundingAxisAlignedBox.EnumerateMaximalVertices(vecAxisLeastOverlap, m_listVertices1);
	Cylinder::EnumerateMaximalVertices(
		lnsShaft, rRadius, -vecAxisLeastOverlap, m_listVertices2);

	// contact cases

	// single point - single point (cylinder end edge contact with box vertex)
	if (m_listVertices1.Size() == 1 && m_listVertices2.Size() == 1)
	{
		TVector3<Real> vecPosition = m_listVertices1[0] + (m_listVertices2[0] - m_listVertices1[0]) * (Real) 0.5;
		contactPoint.Position = vecPosition;
		p_contactManifold.ContactPoints.Add(contactPoint);
		return;
	}

	// edge to edge
	if (m_listVertices1.Size() == 2 && m_listVertices2.Size() == 2)
	{
		LineSegment lnsEdge1(m_listVertices1[0], m_listVertices1[1]),
			lnsEdge2(m_listVertices2[0], m_listVertices2[1]);
		TVector3<Real> vecClosest1, vecClosest2;
		lnsEdge1.ClosestPointsToSegment(lnsEdge2, vecClosest1, vecClosest2);

		contactPoint.Position = (vecClosest1 + vecClosest2) * (Real) 0.5;
		p_contactManifold.ContactPoints.Add(contactPoint);
		return;
	}

	// single point to edge / face
	if (m_listVertices1.Size() == 1 && m_listVertices2.Size() > 1)
	{
		contactPoint.Position = m_listVertices1[0];
		p_contactManifold.ContactPoints.Add(contactPoint);
		return;
	}

	// edge / face to single point
	if (m_listVertices1.Size() > 1 && m_listVertices2.Size() == 1)
	{
		contactPoint.Position = m_listVertices2[0];
		p_contactManifold.ContactPoints.Add(contactPoint);
		return;
	}

	// edge / face to edge / face
	size_t unCount = m_listVertices1.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		TVector3<Real>& vecVertex = m_listVertices1[unIndex];
		if (Cylinder::IntersectsPoint(lnsShaft, cylinder.Radius, vecVertex))
		{
			contactPoint.Position = vecVertex;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
	}

	unCount = m_listVertices2.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		TVector3<Real>& vecVertex = m_listVertices2[unIndex];
		if (boundingAxisAlignedBox.Contains(vecVertex))
		{
			contactPoint.Position = vecVertex;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
	}
}
