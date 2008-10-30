#include "ConvexPolyhedronToCylinderCollisionDetector.h"

#include "ConvexPolyhedron.h"
#include "Cylinder.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

ConvexPolyhedronToCylinderCollisionDetector::ConvexPolyhedronToCylinderCollisionDetector(void)
	: m_strGeometryType1("ConvexPolyhedron")
	, m_strGeometryType2("Cylinder")
{
}

ConvexPolyhedronToCylinderCollisionDetector::~ConvexPolyhedronToCylinderCollisionDetector(void)
{
}

const String& ConvexPolyhedronToCylinderCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& ConvexPolyhedronToCylinderCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool ConvexPolyhedronToCylinderCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "ConvexPolyhedron", "Expected convex polyhedron geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Cylinder", "Expected cylinder geometry.")

	const ConvexPolyhedron& convexPolyhedron = (ConvexPolyhedron &) p_geometry1;
	const Cylinder& cylinder = (Cylinder &) p_geometry2;

	static LineSegment lnsShaft;
	Real rHalfHeight = cylinder.Height * (Real) 0.5;
	lnsShaft.Start = TVector3<Real>((Real) 0.0, -rHalfHeight, (Real) 0.0);
	lnsShaft.End = TVector3<Real>((Real) 0.0, rHalfHeight, (Real) 0.0);
	lnsShaft.Transform(p_trnRelativePlacement);
	Real rRadius(cylinder.Radius);

	// projection intervals for SATs
	static TInterval<Real> itvProjection1, itvProjection2;

	// try cylinder shaft as SA
	TVector3<Real> vecShaftDirection(lnsShaft.Direction());
	convexPolyhedron.ProjectToInterval(vecShaftDirection, itvProjection1);
	Cylinder::ProjectToInterval(lnsShaft, rRadius, vecShaftDirection, itvProjection2);
	if (!itvProjection1.Intersects(itvProjection2)) return false;

	// try closest point from shaft to poly centroid as SA
	TVector3<Real> vecAxis(lnsShaft.ClosestPoint(TVector3<Real>::Zero));
	if (!vecAxis.IsZero())
	{
		convexPolyhedron.ProjectToInterval(vecAxis, itvProjection1);
		Cylinder::ProjectToInterval(lnsShaft, rRadius, vecAxis, itvProjection2);
		if (!itvProjection1.Intersects(itvProjection2))
			return false;
	}

	// try SAT with poly face normals
	size_t unFaceCount(convexPolyhedron.Faces.Size());
	for (size_t unIndex = 0; unIndex < unFaceCount; unIndex++)
	{
		const ConvexPolyhedronFace& face = convexPolyhedron.Faces[unIndex];
		convexPolyhedron.ProjectToInterval(face.Normal, itvProjection1);
		Cylinder::ProjectToInterval(lnsShaft, rRadius, face.Normal, itvProjection2);
		if (!itvProjection1.Intersects(itvProjection2))
			return false;
	}

	// try cross product of shaft and poly face normals
	for (size_t unIndex = 0; unIndex < unFaceCount; unIndex++)
	{
		const ConvexPolyhedronFace& face = convexPolyhedron.Faces[unIndex];

		TVector3<Real> vecAxis(vecShaftDirection ^ face.Normal);
		if (vecAxis.IsZero())
			continue;

		convexPolyhedron.ProjectToInterval(vecAxis, itvProjection1);
		Cylinder::ProjectToInterval(lnsShaft, rRadius, vecAxis, itvProjection2);
		if (!itvProjection1.Intersects(itvProjection2))
			return false;
	}

	// otherwise, must be intersecting
	return true;
}

bool ConvexPolyhedronToCylinderCollisionDetector::EstimateImpact(
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

void ConvexPolyhedronToCylinderCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "ConvexPolyhedron", "Expected convex polyhedron geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Cylinder", "Expected cylinder geometry.")

	const ConvexPolyhedron& convexPolyhedron = (ConvexPolyhedron &) p_geometry1;
	const Cylinder& cylinder = (Cylinder &) p_geometry2;

	static LineSegment lnsShaft;
	Real rHalfHeight = cylinder.Height * (Real) 0.5;
	lnsShaft.Start = TVector3<Real>((Real) 0.0, -rHalfHeight, (Real) 0.0);
	lnsShaft.End = TVector3<Real>((Real) 0.0, rHalfHeight, (Real) 0.0);
	lnsShaft.Transform(p_trnRelativePlacement);
	Real rRadius(cylinder.Radius);

	p_contactManifold.ContactPoints.Clear();

	// start separating axis tests
	static TInterval<Real> itvProjection1, itvProjection2, itvOverlap;
	static TVector3<Real> vecAxisLeastOverlap;
	Real rLeastOverlap = TMaths<Real>::Maximum, rCurrentOverlap;

	// try cylinder shaft as SA
	TVector3<Real> vecShaftDirection(lnsShaft.Direction());
	convexPolyhedron.ProjectToInterval(vecShaftDirection, itvProjection1);
	Cylinder::ProjectToInterval(lnsShaft, rRadius, vecShaftDirection, itvProjection2);

	itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
	if (itvOverlap.IsEmpty()) return;

	rCurrentOverlap = itvOverlap.Span();
	if (rLeastOverlap > rCurrentOverlap)
	{
		vecAxisLeastOverlap = vecShaftDirection;
		rLeastOverlap = rCurrentOverlap;
	}

	// try closest point from shaft to poly centroid as SA
	TVector3<Real> vecAxis(lnsShaft.ClosestPoint(TVector3<Real>::Zero));
	if (!vecAxis.IsZero())
	{
		// normalise axis for correct penetration
		vecAxis.Normalise();

		convexPolyhedron.ProjectToInterval(vecAxis, itvProjection1);
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

	// try poly face normals
	size_t unFaceCount(convexPolyhedron.Faces.Size());
	for (size_t unIndex = 0; unIndex < unFaceCount; unIndex++)
	{
		const ConvexPolyhedronFace& face = convexPolyhedron.Faces[unIndex];

		convexPolyhedron.ProjectToInterval(face.Normal, itvProjection1);
		Cylinder::ProjectToInterval(lnsShaft, rRadius, face.Normal, itvProjection2);

		itvOverlap = itvProjection1.IntersectCopy(itvProjection2);
		if (itvOverlap.IsEmpty())
			return;

		rCurrentOverlap = itvOverlap.Span();
		if (rLeastOverlap > rCurrentOverlap)
		{
			vecAxisLeastOverlap = face.Normal;
			rLeastOverlap = rCurrentOverlap;
		}
	}

	// try cross product of shaft and poly face normals
	for (size_t unIndex = 0; unIndex < unFaceCount; unIndex++)
	{
		const ConvexPolyhedronFace& face = convexPolyhedron.Faces[unIndex];

		TVector3<Real> vecAxis(vecShaftDirection ^ face.Normal);
		if (vecAxis.IsZero())
			continue;

		// normalise axis for correct penetration
		vecAxis.Normalise();

		convexPolyhedron.ProjectToInterval(vecAxis, itvProjection1);
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
	static VectorArrayList m_listVertices1, m_listVertices2;
	convexPolyhedron.EnumerateMaximalVertices(vecAxisLeastOverlap, m_listVertices1);
	Cylinder::EnumerateMaximalVertices(
		lnsShaft, rRadius, -vecAxisLeastOverlap, m_listVertices2);

	// contact cases

	// single point - single point (cylinder end edge contact with poly vertex)
	if (m_listVertices1.Size() == 1 && m_listVertices2.Size() == 1)
	{
		contactPoint.Position = (m_listVertices1[0] + m_listVertices2[0]) * (Real) 0.5;
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
		if (convexPolyhedron.ContainsPoint(vecVertex))
		{
			contactPoint.Position = vecVertex;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
	}
}
