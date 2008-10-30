#include "ConvexPolyhedronCollisionDetector.h"

#include "DebugSupport.h"
#include "Maths.h"

#include "ConvexPolyhedron.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Collision;

ConvexPolyhedronCollisionDetector::ConvexPolyhedronCollisionDetector(void)
	: m_strGeometryType("ConvexPolyhedron")
{
}

ConvexPolyhedronCollisionDetector::~ConvexPolyhedronCollisionDetector(void)
{
}

const String &ConvexPolyhedronCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType;
}

const String &ConvexPolyhedronCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType;
}

bool ConvexPolyhedronCollisionDetector::TestIntersection(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "ConvexPolyhedron", "Both geometries must be convex polyhedrons.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "ConvexPolyhedron", "Both geometries must be convex polyhedrons.")

	const ConvexPolyhedron& convexPolyhedron1 = (ConvexPolyhedron &) p_geometry1;
	const ConvexPolyhedron& convexPolyhedron2Ref = (ConvexPolyhedron &) p_geometry2;

	static ConvexPolyhedron convexPolyhedron2;
	convexPolyhedron2Ref.TransformCopy(p_trnRelativePlacement, convexPolyhedron2);

	const TVector3<Real>& vecPosition2 = p_trnRelativePlacement.Translation;

	// projection intervals for SATs
	static TInterval<Real> itvProjection1, itvProjection2;

	// try SAT with poly 1 face normals
	size_t unFaceCount1(convexPolyhedron1.Faces.Size());
	for (size_t unIndex = 0; unIndex < unFaceCount1; unIndex++)
	{
		const ConvexPolyhedronFace& face = convexPolyhedron1.Faces[unIndex];

		if (face.IsPointBelowPlane(vecPosition2))
			continue;

		convexPolyhedron1.ProjectToInterval(face.Normal, itvProjection1);
		convexPolyhedron2.ProjectToInterval(face.Normal, itvProjection2);
		if (!itvProjection1.Intersects(itvProjection2))
			return false;
	}

	// try SAT with poly 2 face normals
	size_t unFaceCount2(convexPolyhedron2.Faces.Size());
	for (size_t unIndex = 0; unIndex < unFaceCount2; unIndex++)
	{
		const ConvexPolyhedronFace& face = convexPolyhedron2.Faces[unIndex];

		if (face.IsPointBelowPlane(TVector3<Real>::Zero))
			continue;

		convexPolyhedron1.ProjectToInterval(face.Normal, itvProjection1);
		convexPolyhedron2.ProjectToInterval(face.Normal, itvProjection2);
		if (!itvProjection1.Intersects(itvProjection2))
			return false;
	}

	// try SAT with edge cross prods
	size_t unEdgeCount1(convexPolyhedron1.Edges.Size());
	size_t unEdgeCount2(convexPolyhedron2.Edges.Size());
	for (size_t unIndex1 = 0; unIndex1 < unEdgeCount1; unIndex1++)
	{
		const TVector3<Real>& vecEdge1 = convexPolyhedron1.Edges[unIndex1].Displacement;
		if (vecEdge1.IsZero())
			continue;

		for (size_t unIndex2 = 0; unIndex2 < unEdgeCount2; unIndex2++)
		{
			const TVector3<Real>& vecEdge2 = convexPolyhedron2.Edges[unIndex2].Displacement;
			if (vecEdge2.IsZero())
				continue;

			TVector3<Real> vecAxis(vecEdge1 ^ vecEdge2);
			if (vecAxis.IsZero())
				continue;

			convexPolyhedron1.ProjectToInterval(vecAxis, itvProjection1);
			convexPolyhedron2.ProjectToInterval(vecAxis, itvProjection2);
			if (!itvProjection1.Intersects(itvProjection2))
				return false;
		}
	}

	// all SATs failed => intersecting
	return true;
}

bool ConvexPolyhedronCollisionDetector::EstimateImpact(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real &p_rTime)
{
	throw new MesonException(
		"EstimateImpact for convex polyhedrons not implemented yet.",
		__FILE__, __LINE__);
}

void ConvexPolyhedronCollisionDetector::ComputeContactManifold(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement,
	ContactManifold &p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "ConvexPolyhedron", "Both geometries must be convex polyhedrons.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "ConvexPolyhedron", "Both geometries must be convex polyhedrons.")

	const ConvexPolyhedron& convexPolyhedron1 = (ConvexPolyhedron &) p_geometry1;
	const ConvexPolyhedron& convexPolyhedron2Ref = (ConvexPolyhedron &) p_geometry2;

	static ConvexPolyhedron convexPolyhedron2;
	convexPolyhedron2Ref.TransformCopy(p_trnRelativePlacement, convexPolyhedron2);

	const TVector3<Real>& vecPosition2 = p_trnRelativePlacement.Translation;

	// clear contact manifold
	p_contactManifold.ContactPoints.Clear();

	// projection intervals for SATs
	// determine axis of least overlap
	static TInterval<Real> itvProjection1, itvProjection2, itvOverlap;
	static TVector3<Real> vecLeastOverlapAxis;

	Real rLeastOverlap = TMaths<Real>::Maximum;
	itvOverlap.MakeUniversal();

	// try SAT with poly 1 face normals
	size_t unFaceCount1(convexPolyhedron1.Faces.Size());
	for (size_t unIndex = 0; unIndex < unFaceCount1; unIndex++)
	{
		const ConvexPolyhedronFace& face = convexPolyhedron1.Faces[unIndex];

		if (face.IsPointBelowPlane(vecPosition2))
			continue;

		convexPolyhedron1.ProjectToInterval(face.Normal, itvProjection1);
		convexPolyhedron2.ProjectToInterval(face.Normal, itvProjection2);
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

	// try SAT with poly 2 face normals
	size_t unFaceCount2(convexPolyhedron2.Faces.Size());
	for (size_t unIndex = 0; unIndex < unFaceCount2; unIndex++)
	{
		const ConvexPolyhedronFace& face = convexPolyhedron2.Faces[unIndex];

		if (face.IsPointBelowPlane(TVector3<Real>::Zero))
			continue;

		convexPolyhedron1.ProjectToInterval(face.Normal, itvProjection1);
		convexPolyhedron2.ProjectToInterval(face.Normal, itvProjection2);
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
	size_t unEdgeCount1(convexPolyhedron1.Edges.Size());
	size_t unEdgeCount2(convexPolyhedron2.Edges.Size());
	for (size_t unIndex1 = 0; unIndex1 < unEdgeCount1; unIndex1++)
	{
		const TVector3<Real>& vecEdge1 = convexPolyhedron1.Edges[unIndex1].Displacement;
		if (vecEdge1.IsZero())
			continue;

		for (size_t unIndex2 = 0; unIndex2 < unEdgeCount2; unIndex2++)
		{
			const TVector3<Real>& vecEdge2 = convexPolyhedron2.Edges[unIndex2].Displacement;
			if (vecEdge2.IsZero())
				continue;

			TVector3<Real> vecAxis(vecEdge1 ^ vecEdge2);
			if (vecAxis.IsZero())
				continue;

			// normalise SA to ensure correct penetretion calculation
			vecAxis.Normalise();

			convexPolyhedron1.ProjectToInterval(vecAxis, itvProjection1);
			convexPolyhedron2.ProjectToInterval(vecAxis, itvProjection2);
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

	// determine direction vectors in general direction from poly 1 to 2
	static TVector3<Real> vecNormal12;
	if (p_trnRelativePlacement.Translation * vecLeastOverlapAxis > (Real) 0.0)
		vecNormal12 = vecLeastOverlapAxis;
	else
		vecNormal12 = -vecLeastOverlapAxis;

	// determine maximal vertices of poly 1
	static VectorArrayList listVertices1(8);
	convexPolyhedron1.EnumerateMaximalVertices(vecNormal12, listVertices1);

	// determine minimal vertices of poly 2
	static VectorArrayList listVertices2(8);
	convexPolyhedron2.EnumerateMaximalVertices(-vecNormal12, listVertices2);

	// ensure lists in increasing size order
	VectorList *pListVertices1, *pListVertices2;
	if (listVertices1.Size() <= listVertices2.Size())
	{
		pListVertices1 = &listVertices1;
		pListVertices2 = &listVertices2;
	}
	else
	{
		pListVertices1 = &listVertices2;
		pListVertices2 = &listVertices1;
	}

	// handle contact cases

	// common data
	static ContactPoint contactPoint;
	contactPoint.Penetration = rLeastOverlap;
	contactPoint.Normal = vecNormal12;

	// vertex - vertex contact
	if ((pListVertices1->Size() == 1) && (pListVertices2->Size() == 1))
	{
		TVector3<Real>& vecVertex1 = (*pListVertices1)[0];
		TVector3<Real>& vecVertex2 = (*pListVertices2)[0];
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
		contactPoint.Position = (*pListVertices1)[0];
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
		TVector3<Real>& ptVertex = (*pListVertices1)[unIndex];
		if (convexPolyhedron1.ContainsPoint(ptVertex)
			&& convexPolyhedron2.ContainsPoint(ptVertex))
		{
			contactPoint.Position = ptVertex;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
	}
	for (size_t unIndex = 0; unIndex < pListVertices2->Size(); unIndex++)
	{
		TVector3<Real>& vecVertex = (*pListVertices2)[unIndex];
		if (convexPolyhedron1.ContainsPoint(vecVertex)
			&& convexPolyhedron2.ContainsPoint(vecVertex))
		{
			contactPoint.Position = vecVertex;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
	}

	if (p_contactManifold.ContactPoints.Size() > 0)
		return;

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
