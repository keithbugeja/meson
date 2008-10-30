#include "ConvexPolyhedronToBoxCollisionDetector.h"

#include "ArrayList.h"

#include "ConvexPolyhedron.h"
#include "Box.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

ConvexPolyhedronToBoxCollisionDetector::ConvexPolyhedronToBoxCollisionDetector(void)
	: m_strGeometryType1("ConvexPolyhedron")
	, m_strGeometryType2("Box")
	, m_listVertices1(8)
	, m_listVertices2(8)
{
}

ConvexPolyhedronToBoxCollisionDetector::~ConvexPolyhedronToBoxCollisionDetector(void)
{
}

const String& ConvexPolyhedronToBoxCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& ConvexPolyhedronToBoxCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool ConvexPolyhedronToBoxCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "ConvexPolyhedron", "Expected convex polyhedron geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Box", "Expected box geometry.")

	const ConvexPolyhedron& convexPolyhedron = (ConvexPolyhedron &) p_geometry1;
	const Box& box = (Box &) p_geometry2;

	BoundingOrientedBox boundingOrientedBox(TVector3<Real>::Zero, box.Extent);
	boundingOrientedBox.Transform(p_trnRelativePlacement);

	// projection intervals for SATs
	static TInterval<Real> itvProjection1, itvProjection2;

	// try SAT with box principal axis
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		TVector3<Real>& vecAxis = boundingOrientedBox.Axis[unIndex];
		convexPolyhedron.ProjectToInterval(vecAxis, itvProjection1);
		boundingOrientedBox.ProjectToInterval(vecAxis, itvProjection2);
		if (!itvProjection1.Intersects(itvProjection2))
			return false;
	}

	// try SAT with poly face normals
	size_t unFaceCount(convexPolyhedron.Faces.Size());
	for (size_t unIndex = 0; unIndex < unFaceCount; unIndex++)
	{
		const ConvexPolyhedronFace& face = convexPolyhedron.Faces[unIndex];
		convexPolyhedron.ProjectToInterval(face.Normal, itvProjection1);
		boundingOrientedBox.ProjectToInterval(face.Normal, itvProjection2);
		if (!itvProjection1.Intersects(itvProjection2))
			return false;
	}

	// try SAT with edge cross prods
	size_t unEdgeCount(convexPolyhedron.Edges.Size());
	for (size_t unIndex1 = 0; unIndex1 < 3; unIndex1++)
	{
		const TVector3<Real>& vecEdge1 = boundingOrientedBox.Axis[unIndex1];

		for (size_t unIndex2 = 0; unIndex2 < unEdgeCount; unIndex2++)
		{
			const TVector3<Real>& vecEdge2 = convexPolyhedron.Edges[unIndex2].Displacement;
			if (vecEdge2.IsZero())
				continue;

			TVector3<Real> vecAxis(vecEdge1 ^ vecEdge2);
			if (vecAxis.IsZero())
				continue;

			convexPolyhedron.ProjectToInterval(vecAxis, itvProjection1);
			boundingOrientedBox.ProjectToInterval(vecAxis, itvProjection2);
			if (!itvProjection1.Intersects(itvProjection2))
				return false;
		}
	}

	// all SATs failed => intersecting
	return true;
}

bool ConvexPolyhedronToBoxCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for convex polyhedron and box not implemented yet.",
		__FILE__, __LINE__);
}

void ConvexPolyhedronToBoxCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "ConvexPolyhedron", "Expected convex polyhedron geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Box", "Expected box geometry.")

	const ConvexPolyhedron& convexPolyhedron = (ConvexPolyhedron &) p_geometry1;
	const Box& box = (Box &) p_geometry2;

	BoundingOrientedBox boundingOrientedBox(TVector3<Real>::Zero, box.Extent);
	boundingOrientedBox.Transform(p_trnRelativePlacement);

	// clear contact manifold
	p_contactManifold.ContactPoints.Clear();

	// projection intervals for SATs
	// determine axis of least overlap
	Real rLeastOverlap = TMaths<Real>::Maximum;
	static TInterval<Real> itvProjection1, itvProjection2, itvOverlap;
	static TVector3<Real> vecLeastOverlapAxis;

	itvOverlap.MakeUniversal();

	// try SAT with box principal axes
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		TVector3<Real>& vecAxis = boundingOrientedBox.Axis[unIndex];
		convexPolyhedron.ProjectToInterval(vecAxis, itvProjection1);
		boundingOrientedBox.ProjectToInterval(vecAxis, itvProjection2);
		itvOverlap = itvProjection1 & itvProjection2;

		// separating axis found - no contacts
		if (itvOverlap.IsEmpty()) return;

		// axis with lesser overlap found
		if (rLeastOverlap > itvOverlap.Span())
		{
			vecLeastOverlapAxis = vecAxis;
			rLeastOverlap = itvOverlap.Span();
		}
	}

	// try SAT with poly face normals
	size_t unFaceCount(convexPolyhedron.Faces.Size());
	for (size_t unIndex = 0; unIndex < unFaceCount; unIndex++)
	{
		const ConvexPolyhedronFace& face = convexPolyhedron.Faces[unIndex];
		convexPolyhedron.ProjectToInterval(face.Normal, itvProjection1);
		boundingOrientedBox.ProjectToInterval(face.Normal, itvProjection2);
		itvOverlap = itvProjection1 & itvProjection2;

		// separating axis found - no contacts
		if (itvOverlap.IsEmpty()) return;

		// axis with lesser overlap found
		if (rLeastOverlap > itvOverlap.Span())
		{
			vecLeastOverlapAxis = face.Normal;
			rLeastOverlap = itvOverlap.Span();
		}
	}

	// try SAT with edge cross prods
	size_t unEdgeCount(convexPolyhedron.Edges.Size());
	for (size_t unIndex1 = 0; unIndex1 < 3; unIndex1++)
	{
		const TVector3<Real>& vecEdge1 = boundingOrientedBox.Axis[unIndex1];

		for (size_t unIndex2 = 0; unIndex2 < unEdgeCount; unIndex2++)
		{
			const TVector3<Real>& vecEdge2 = convexPolyhedron.Edges[unIndex2].Displacement;
			if (vecEdge2.IsZero())
				continue;

			TVector3<Real> vecAxis(vecEdge1 ^ vecEdge2);
			if (vecAxis.IsZero())
				continue;

			// normalise SA to ensure correct penetretion calculation
			vecAxis.Normalise();

			convexPolyhedron.ProjectToInterval(vecAxis, itvProjection1);
			boundingOrientedBox.ProjectToInterval(vecAxis, itvProjection2);
			itvOverlap = itvProjection1 & itvProjection2;

			// separating axis found - no contacts
			if (itvOverlap.IsEmpty()) return;

			// axis with lesser overlap found
			if (rLeastOverlap > itvOverlap.Span())
			{
				vecLeastOverlapAxis = vecAxis;
				rLeastOverlap = itvOverlap.Span();
			}
		}
	}

	// determine direction vectors in general direction from box 1 to 2
	static TVector3<Real> vecNormal12;
	if (p_trnRelativePlacement.Translation * vecLeastOverlapAxis > (Real) 0.0)
		vecNormal12 = vecLeastOverlapAxis;
	else
		vecNormal12 = -vecLeastOverlapAxis;

	// determine maximal vertices of poly
	convexPolyhedron.EnumerateMaximalVertices(vecNormal12, m_listVertices1);

	// determine minimal vertices of box
	boundingOrientedBox.EnumerateMaximalVertices(-vecNormal12, m_listVertices2);

	// ensure lists in increasing size order
	VectorList *pListVertices1, *pListVertices2;
	if (m_listVertices1.Size() <= m_listVertices2.Size())
	{
		pListVertices1 = &m_listVertices1;
		pListVertices2 = &m_listVertices2;
	}
	else
	{
		pListVertices1 = &m_listVertices2;
		pListVertices2 = &m_listVertices1;
	}

	// handle contact cases

	// common data
	static ContactPoint contactPoint;
	contactPoint.Penetration = rLeastOverlap;
	contactPoint.Normal = vecNormal12;

	// vertex - vertex contact
	if ((pListVertices1->Size() == 1) && (pListVertices2->Size() == 1))
	{
		TVector3<Real> vecVertex1 = (*pListVertices1)[0];
		TVector3<Real> vecVertex2 = (*pListVertices2)[0];
		contactPoint.Position = (vecVertex1 + vecVertex2) * (Real) 0.5;
		p_contactManifold.ContactPoints.Add(contactPoint);
		return;
	}

	// vertex - edge contact
	if ((pListVertices1->Size() == 1) && (pListVertices2->Size() == 2))
	{
		LineSegment lsgEdge((*pListVertices1)[0], (*pListVertices2)[0]);
		static TVector3<Real> vecClosesEdgePoint;
		lsgEdge.DistanceFromPoint(TVector3<Real>::Zero, vecClosesEdgePoint);

		contactPoint.Position = (*pListVertices1)[0];
		contactPoint.Normal = vecClosesEdgePoint;
		if (contactPoint.Normal.IsZero())
			contactPoint.Normal = TVector3<Real>::Right;
		else
			contactPoint.Normal.Normalise();
		p_contactManifold.ContactPoints.Add(contactPoint);
		return;
	}

	// vertex - face contact
	if ((pListVertices1->Size() == 1) && (pListVertices2->Size() > 2))
	{
		contactPoint.Position = (*pListVertices1)(0);
		p_contactManifold.ContactPoints.Add(contactPoint);
		return;
	}

	// edge - edge contact
	if ((pListVertices1->Size() == 2) && (pListVertices2->Size() == 2))
	{
		LineSegment lsgEdge1((*pListVertices1)(0), (*pListVertices1)(1));
		LineSegment lsgEdge2((*pListVertices2)(0), (*pListVertices2)(1));
		static TVector3<Real> vecClosest1, vecClosest2;
		lsgEdge1.ClosestPointsToSegment(lsgEdge2, vecClosest1, vecClosest2);

		contactPoint.Position = (vecClosest1 + vecClosest2) * (Real) 0.5;
		p_contactManifold.ContactPoints.Add(contactPoint);
		return;
	}

	// edge/face - face contact
	for (size_t unIndex = 0; unIndex < pListVertices1->Size(); unIndex++)
	{
		TVector3<Real>& vecVertex = (*pListVertices1)[unIndex];
		if (convexPolyhedron.ContainsPoint(vecVertex)
			&& boundingOrientedBox.Contains(vecVertex))
		{
			contactPoint.Position = vecVertex;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
	}
	for (size_t unIndex = 0; unIndex < pListVertices2->Size(); unIndex++)
	{
		TVector3<Real>& vecVertex = (*pListVertices2)[unIndex];
		if (convexPolyhedron.ContainsPoint(vecVertex)
			&& boundingOrientedBox.Contains(vecVertex))
		{
			contactPoint.Position = vecVertex;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
	}

	// hack in case no contacts detected
	TVector3<Real> vecAverageContact(TVector3<Real>::Zero);
	for (size_t unIndex = 0; unIndex < pListVertices1->Size(); unIndex++)
		vecAverageContact += (*pListVertices1)[unIndex];
	for (size_t unIndex = 0; unIndex < pListVertices2->Size(); unIndex++)
		vecAverageContact += (*pListVertices2)[unIndex];
	vecAverageContact /= (pListVertices1->Size() + pListVertices2->Size());
	contactPoint.Position = vecAverageContact;
	p_contactManifold.ContactPoints.Add(contactPoint);
}
