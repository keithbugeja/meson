#include "CylinderCollisionDetector.h"

#include "DebugSupport.h"
#include "Maths.h"

#include "Cylinder.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Collision;

CylinderCollisionDetector::CylinderCollisionDetector(void)
	: m_strGeometryType("Cylinder")
	, m_listVertices1()
	, m_listVertices2()
{
}

CylinderCollisionDetector::~CylinderCollisionDetector(void)
{
}

const String &CylinderCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType;
}

const String &CylinderCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType;
}

bool CylinderCollisionDetector::TestIntersection(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	// ensure geometry of correct type
	MESON_ASSERT(p_geometry1.GetTypeName() == "Cylinder", "Both geometries must be cylinders.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Cylinder", "Both geometries must be cylinders.")

	// cast geometry to cylinders
	const Cylinder &cylinder1 = (Cylinder &) p_geometry1;
	const Cylinder &cylinder2 = (Cylinder &) p_geometry2;

	// compute cylinder shafts
	static LineSegment lnsShaft1, lnsShaft2;

	Real rHalfHeight1 = cylinder1.Height * (Real) 0.5;
	lnsShaft1.Start = TPoint3<Real>((Real) 0.0, -rHalfHeight1, (Real) 0.0);
	lnsShaft1.End = TPoint3<Real>((Real) 0.0, rHalfHeight1, (Real) 0.0);

	Real rHalfHeight2 = cylinder2.Height * (Real) 0.5;
	lnsShaft2.Start = TPoint3<Real>((Real) 0.0, -rHalfHeight2, (Real) 0.0);
	lnsShaft2.End = TPoint3<Real>((Real) 0.0, rHalfHeight2, (Real) 0.0);
	lnsShaft2.Transform(p_trnRelativePlacement);

	// start seperating axis tests
	static TVector3<Real> vecAxis;
	static TInterval<Real> itvProjection1, itvProjection2;

	// try shaft 1 as separating axis
	vecAxis = lnsShaft1.Direction();

	Cylinder::ProjectToInterval(lnsShaft1, cylinder1.Radius, vecAxis, itvProjection1);
	Cylinder::ProjectToInterval(lnsShaft2, cylinder2.Radius, vecAxis, itvProjection2);

	if (!itvProjection1.Intersects(itvProjection2)) return false;

	// try shaft 2 as separating axis
	vecAxis = lnsShaft2.Direction();

	Cylinder::ProjectToInterval(lnsShaft1, cylinder1.Radius, vecAxis, itvProjection1);
	Cylinder::ProjectToInterval(lnsShaft2, cylinder2.Radius, vecAxis, itvProjection2);

	if (!itvProjection1.Intersects(itvProjection2)) return false;

	// compute test axis as direction along closest points on shafts
	TPoint3<Real> ptClosestPoint1, ptClosestPoint2;
	lnsShaft1.ClosestPointsToSegment(lnsShaft2, ptClosestPoint1, ptClosestPoint2);
	vecAxis = ptClosestPoint2 - ptClosestPoint1;
	if (vecAxis.IsZero())
		vecAxis.X = (Real) 1.0;
	else
		vecAxis.Normalise();

	Cylinder::ProjectToInterval(lnsShaft1, cylinder1.Radius, vecAxis, itvProjection1);
	Cylinder::ProjectToInterval(lnsShaft2, cylinder2.Radius, vecAxis, itvProjection2);

	if (!itvProjection1.Intersects(itvProjection2)) return false;

	// otherwise cylinders must be intersecting
	return true;
}

bool CylinderCollisionDetector::EstimateImpact(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real &p_rTime)
{
	throw new MesonException(
		"Impact time estimation algorithm for cylinders not implemented yet.",
		__FILE__, __LINE__);
}

void CylinderCollisionDetector::ComputeContactManifold(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement,
	ContactManifold &p_contactManifold)
{
	// ensure geometry of correct type
	MESON_ASSERT(p_geometry1.GetTypeName() == "Cylinder", "Both geometries must be cylinders.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Cylinder", "Both geometries must be cylinders.")

	p_contactManifold.ContactPoints.Clear();

	// cast geometry to cylinders
	const Cylinder &cylinder1 = (Cylinder &) p_geometry1;
	const Cylinder &cylinder2 = (Cylinder &) p_geometry2;

	// compute cylinder shafts
	static LineSegment lnsShaft1, lnsShaft2;

	Real rHalfHeight1 = cylinder1.Height * (Real) 0.5;
	lnsShaft1.Start = TPoint3<Real>((Real) 0.0, -rHalfHeight1, (Real) 0.0);
	lnsShaft1.End = TPoint3<Real>((Real) 0.0, rHalfHeight1, (Real) 0.0);

	Real rHalfHeight2 = cylinder2.Height * (Real) 0.5;
	lnsShaft2.Start = TPoint3<Real>((Real) 0.0, -rHalfHeight2, (Real) 0.0);
	lnsShaft2.End = TPoint3<Real>((Real) 0.0, rHalfHeight2, (Real) 0.0);
	lnsShaft2.Transform(p_trnRelativePlacement);

	// start seperating axis tests
	static TVector3<Real> vecAxis;
	static TInterval<Real> itvProjection1, itvProjection2, itvOverlap;
	static TVector3<Real> vecAxisLeastOverlap;
	Real rLeastOverlap = TMaths<Real>::Maximum, rCurrentOverlap;

	// try shaft 1 as separating axis
	vecAxis = lnsShaft1.Direction();

	Cylinder::ProjectToInterval(lnsShaft1, cylinder1.Radius, vecAxis, itvProjection1);
	Cylinder::ProjectToInterval(lnsShaft2, cylinder2.Radius, vecAxis, itvProjection2);

	itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
	if (itvOverlap.IsEmpty()) return;

	rCurrentOverlap = itvOverlap.Span();
	if (rLeastOverlap > rCurrentOverlap)
	{
		vecAxisLeastOverlap = vecAxis;
		rLeastOverlap = rCurrentOverlap;
	}

	// try shaft 2 as separating axis
	vecAxis = lnsShaft2.Direction();

	Cylinder::ProjectToInterval(lnsShaft1, cylinder1.Radius, vecAxis, itvProjection1);
	Cylinder::ProjectToInterval(lnsShaft2, cylinder2.Radius, vecAxis, itvProjection2);

	itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
	if (itvOverlap.IsEmpty()) return;

	rCurrentOverlap = itvOverlap.Span();
	if (rLeastOverlap > rCurrentOverlap)
	{
		vecAxisLeastOverlap = vecAxis;
		rLeastOverlap = rCurrentOverlap;
	}

	// compute test axis as direction along closest points on shafts
	static TPoint3<Real> ptClosestPoint1, ptClosestPoint2;
	lnsShaft1.ClosestPointsToSegment(lnsShaft2, ptClosestPoint1, ptClosestPoint2);
	vecAxis = ptClosestPoint2 - ptClosestPoint1;
	if (vecAxis.IsZero())
		vecAxis.X = (Real) 1.0;
	else
		vecAxis.Normalise();

	Cylinder::ProjectToInterval(lnsShaft1, cylinder1.Radius, vecAxis, itvProjection1);
	Cylinder::ProjectToInterval(lnsShaft2, cylinder2.Radius, vecAxis, itvProjection2);

	itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
	if (itvOverlap.IsEmpty()) return;

	rCurrentOverlap = itvOverlap.Span();
	if (rLeastOverlap > rCurrentOverlap)
	{
		vecAxisLeastOverlap = vecAxis;
		rLeastOverlap = rCurrentOverlap;
	}

	// align axis of least overlap along CM offset
	if (vecAxisLeastOverlap * p_trnRelativePlacement.Translation < (Real) 0.0)
		vecAxisLeastOverlap.Negate();

	// prepare contact info
	static ContactPoint contactPoint;
	contactPoint.Normal = vecAxisLeastOverlap;
	contactPoint.Penetration = rLeastOverlap;

	// compute maximal / minimal vertices
	Cylinder::EnumerateMaximalVertices(
		lnsShaft1, cylinder1.Radius, vecAxisLeastOverlap, m_listVertices1);
	Cylinder::EnumerateMaximalVertices(
		lnsShaft2, cylinder2.Radius, -vecAxisLeastOverlap, m_listVertices2);

	// contact cases

	// single point - single point (end edge contact)
	if (m_listVertices1.Size() == 1 && m_listVertices2.Size() == 1)
	{
		TPoint3<Real>& ptPoint1 = m_listVertices1[0];
		TPoint3<Real>& ptPoint2 = m_listVertices2[0];
		if (Cylinder::IntersectsPoint(lnsShaft2, cylinder2.Radius, ptPoint1))
		{
			contactPoint.Position = ptPoint1;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
		if (Cylinder::IntersectsPoint(lnsShaft1, cylinder1.Radius, ptPoint2))
		{
			contactPoint.Position = ptPoint2;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
		return;
	}

	// edge to edge
	if (m_listVertices1.Size() == 2 && m_listVertices2.Size() == 2)
	{
		LineSegment lnsEdge1(m_listVertices1[0], m_listVertices1[1]),
			lnsEdge2(m_listVertices2[0], m_listVertices2[1]);
		static TPoint3<Real> ptClosest1, ptClosest2;
		lnsEdge1.ClosestPointsToSegment(lnsEdge2, ptClosest1, ptClosest2);

		contactPoint.Position = ptClosest1 + (ptClosest2 - ptClosest1) * (Real) 0.5;
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
		TPoint3<Real>& ptVertex = m_listVertices1[unIndex];
		if (Cylinder::IntersectsPoint(lnsShaft2, cylinder2.Radius, ptVertex))
		{
			contactPoint.Position = ptVertex;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
	}

	unCount = m_listVertices2.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		TPoint3<Real>& ptVertex = m_listVertices2[unIndex];
		if (Cylinder::IntersectsPoint(lnsShaft1, cylinder1.Radius, ptVertex))
		{
			contactPoint.Position = ptVertex;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
	}
}
