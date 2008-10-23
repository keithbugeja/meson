#include "BoxCollisionDetector.h"

#include "DebugSupport.h"
#include "Maths.h"

#include "GrvLineSegment.h"
#include "Box.h"
#include "GrvBoundingAxisAlignedBox.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Collision;

bool BoxCollisionDetector::EstimateImpact(
	const TVector3<Real>& p_vecAxis,
	const BoundingOrientedBox& p_boundingOrientedBox1,
	const BoundingOrientedBox& p_boundingOrientedBox2,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	TInterval<Real>& p_itvContact)
{
	p_itvContact.MakeUniversal();

	TInterval<Real> itvProjection1, itvProjection2;

	// compute velocity projection along axis
	Real rProjectedVelocity = p_vecRelativeLinearVelocity * p_vecAxis;

	p_boundingOrientedBox1.ProjectToInterval(p_vecAxis, itvProjection1);
	p_boundingOrientedBox2.ProjectToInterval(p_vecAxis, itvProjection2);

	if (rProjectedVelocity < -TMaths<Real>::Epsilon)
	{
		// test if non-intersecting and moving apart
		if (itvProjection1.Min > itvProjection2.Max) return false;

		// update contact time if later than current
		Real rOffset = itvProjection1.Max - itvProjection2.Min;
		if (rOffset < (Real) 0.0)
			p_itvContact.Min = TMaths<Real>::Max(p_itvContact.Min, rOffset / rProjectedVelocity);

		// update separation time if sooner than current
		rOffset = itvProjection1.Min - itvProjection2.Max;
		if (rOffset < (Real) 0.0)
			p_itvContact.Max = TMaths<Real>::Min(p_itvContact.Max, rOffset / rProjectedVelocity);
	}

	if (rProjectedVelocity > TMaths<Real>::Epsilon)
	{
		// test if non-intersecting and moving apart
		if (itvProjection1.Max < itvProjection2.Min) return false;

		// update contact time if later than current
		Real rOffset = itvProjection1.Min - itvProjection2.Max;
		if (rOffset > (Real) 0.0)
			p_itvContact.Min = TMaths<Real>::Max(p_itvContact.Min, rOffset / rProjectedVelocity);

		// update separation time if sooner than current
		rOffset = itvProjection1.Max - itvProjection2.Min;
		if (rOffset > (Real) 0.0)
			p_itvContact.Max = TMaths<Real>::Min(p_itvContact.Max, rOffset / rProjectedVelocity);
	}

	// no contact possible if contact occurs after separation (in negative time)
	if (p_itvContact.IsEmpty()) return false;

	return true;
}

bool BoxCollisionDetector::Intersects(
	const BoundingOrientedBox& p_boundingOrientedBox,
	const TPoint3<Real>& p_ptTestPoint)
{
	TInterval<Real> itvProjection;
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		// get axis
		const TVector3<Real>& vecAxis =  p_boundingOrientedBox.Axis[unIndex];

		// project box to interval
		p_boundingOrientedBox.ProjectToInterval(vecAxis, itvProjection);

		// project position vector on axis
		Real rPointProjection = p_ptTestPoint.ToVector() * vecAxis;

		// no intersection if separating axis found
		if (!itvProjection.Contains(rPointProjection))
			return false;
	}

	// if this point reached, point intersects box
	return true;
}


BoxCollisionDetector::BoxCollisionDetector(void)
	: m_strGeometryType("Box")
	, m_listVertices1()
	, m_listVertices2()
{
}

BoxCollisionDetector::~BoxCollisionDetector(void)
{
}

const String &BoxCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType;
}

const String &BoxCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType;
}

bool BoxCollisionDetector::TestIntersection(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Box", "Both geometries must be boxes.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Box", "Both geometries must be boxes.")

	const Box &box1 = (Box &) p_geometry1;
	const Box &box2 = (Box &) p_geometry2;

	BoundingAxisAlignedBox boundingAxisAlignedBox(
		TPoint3<Real>::Origin - box1.Extent,
		TPoint3<Real>::Origin + box1.Extent);
	BoundingOrientedBox boundingOrientedBox(TPoint3<Real>::Origin, box2.Extent);
	boundingOrientedBox.Transform(p_trnRelativePlacement);

	return boundingOrientedBox.Intersects(boundingAxisAlignedBox);
}

bool BoxCollisionDetector::EstimateImpact(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real &p_rTime)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Box", "Both geometries must be boxes.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Box", "Both geometries must be boxes.")

	const Box &box1 = (Box &) p_geometry1;
	const Box &box2 = (Box &) p_geometry2;

	BoundingOrientedBox boundingOrientedBox1(TPoint3<Real>::Origin, box1.Extent);
	BoundingOrientedBox boundingOrientedBox2(TPoint3<Real>::Origin, box2.Extent);
	boundingOrientedBox2.Transform(p_trnRelativePlacement);

	// do quick test for current intersection
	if (boundingOrientedBox1.Intersects(boundingOrientedBox2))
	{
		p_rTime = (Real) 0.0;
		return true;
	}

	TInterval<Real> itvContact;
	itvContact.MakeUniversal();

	// test box1 axes
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		// get axis
		TVector3<Real>& vecAxis = boundingOrientedBox1.Axis[unIndex];

		TInterval<Real> itvContactCurrent;

		if (!EstimateImpact(vecAxis, boundingOrientedBox1, boundingOrientedBox2,
			p_vecRelativeLinearVelocity, itvContactCurrent))
			return false;

		// restrict separation interval if possible
		itvContact.Min = TMaths<Real>::Max(itvContact.Min, itvContactCurrent.Min);
		itvContact.Max = TMaths<Real>::Min(itvContact.Max, itvContactCurrent.Max);
	}

	// test box2 axes
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		// get axis
		TVector3<Real>& vecAxis = boundingOrientedBox2.Axis[unIndex];

		// compute contact interval for this axis
		TInterval<Real> itvContactCurrent;
		if (!EstimateImpact(vecAxis, boundingOrientedBox1, boundingOrientedBox2,
			p_vecRelativeLinearVelocity, itvContactCurrent))
			return false;

		// restrict separation interval if possible
		itvContact.Min = TMaths<Real>::Max(itvContact.Min, itvContactCurrent.Min);
		itvContact.Max = TMaths<Real>::Min(itvContact.Max, itvContactCurrent.Max);
	}

	// test edge-edge axes
	for (size_t unIndex1 = 0; unIndex1 < 3; unIndex1++)
	{
		TVector3<Real>& vecEdge1 = boundingOrientedBox1.Axis[unIndex1];
		for (size_t unIndex2 = 0; unIndex2 < 3; unIndex2++)
		{
			TVector3<Real>& vecEdge2 = boundingOrientedBox2.Axis[unIndex2];
			TVector3<Real> vecAxis = vecEdge1 ^ vecEdge2;

			// skip if edges parallel
			if (vecAxis.IsZero()) continue;

			// compute contact interval for this axis
			TInterval<Real> itvContactCurrent;
			if (!EstimateImpact(vecAxis, boundingOrientedBox1, boundingOrientedBox2,
				p_vecRelativeLinearVelocity, itvContactCurrent))
				return false;

			// restrict separation interval if possible
			itvContact.Min = TMaths<Real>::Max(itvContact.Min, itvContactCurrent.Min);
			itvContact.Max = TMaths<Real>::Min(itvContact.Max, itvContactCurrent.Max);
		}
	}

	// if shortest contact interval is empty, no overlap possible (negative time)
	if (itvContact.IsEmpty()) return false;

	// return false if beyond maximum range
	if (itvContact.Max > p_rMaxTime) return false;

	// otherwise collision occurs within max time
	p_rTime = itvContact.Max;
	return true;
}

void BoxCollisionDetector::ComputeContactManifold(
	const IGeometry &p_geometry1, const IGeometry &p_geometry2,
	const Transform& p_trnRelativePlacement,
	ContactManifold &p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Box", "Both geometries must be boxes.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Box", "Both geometries must be boxes.")

	p_contactManifold.ContactPoints.Clear();

	const Box &box1 = (Box &) p_geometry1;
	const Box &box2 = (Box &) p_geometry2;

	BoundingOrientedBox boundingOrientedBox1(TPoint3<Real>::Origin, box1.Extent);
	BoundingOrientedBox boundingOrientedBox2(TPoint3<Real>::Origin, box2.Extent);
	boundingOrientedBox2.Transform(p_trnRelativePlacement);

	// determine axis of least overlap
	Real rLeastOverlap = TMaths<Real>::Maximum;
	TInterval<Real> itvProjection1, itvProjection2, itvOverlap;
	itvOverlap.MakeUniversal();
	TVector3<Real> vecLeastOverlapAxis;

	// test principal axes
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		// test box 1 principal axis
		TVector3<Real>& vecAxis1 = boundingOrientedBox1.Axis[unIndex];
		boundingOrientedBox1.ProjectToInterval(vecAxis1, itvProjection1);
		boundingOrientedBox2.ProjectToInterval(vecAxis1, itvProjection2);
		itvOverlap = itvProjection1 & itvProjection2;

		// separating axis found - no contacts
		if (itvOverlap.IsEmpty()) return;

		// axis with lesser overlap found
		if (rLeastOverlap > itvOverlap.Span())
		{
			vecLeastOverlapAxis = vecAxis1;
			rLeastOverlap = itvOverlap.Span();
		}

		// test box 2 principal axis
		TVector3<Real>& vecAxis2 = boundingOrientedBox2.Axis[unIndex];
		boundingOrientedBox1.ProjectToInterval(vecAxis2, itvProjection1);
		boundingOrientedBox2.ProjectToInterval(vecAxis2, itvProjection2);
		itvOverlap = itvProjection1 & itvProjection2;

		// separating axis found - no contacts
		if (itvOverlap.IsEmpty()) return;

		// axis with lesser overlap found
		if (rLeastOverlap > itvOverlap.Span())
		{
			vecLeastOverlapAxis = vecAxis2;
			rLeastOverlap = itvOverlap.Span();
		}
	}

	// test axes from principal axis cross-products
	for (size_t unIndex1 = 0; unIndex1 < 3; unIndex1++)
	{
		TVector3<Real>& vecAxis1 = boundingOrientedBox1.Axis[unIndex1];
		for (size_t unIndex2 = 0; unIndex2 < 3; unIndex2++)
		{
			TVector3<Real>& vecAxis2 = boundingOrientedBox2.Axis[unIndex2];

			TVector3<Real> vecAxis = vecAxis1 ^ vecAxis2;

			// ignore parallel axes pairs
			if (vecAxis.IsZero())
				continue;

			vecAxis.Normalise();

			boundingOrientedBox1.ProjectToInterval(vecAxis, itvProjection1);
			boundingOrientedBox2.ProjectToInterval(vecAxis, itvProjection2);
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
	TVector3<Real> vecDirection12;
	if (p_trnRelativePlacement.Translation * vecLeastOverlapAxis > (Real) 0.0)
		vecDirection12 = vecLeastOverlapAxis;
	else
		vecDirection12 = -vecLeastOverlapAxis;

	// determine maximal vertices of box 1
	boundingOrientedBox1.EnumerateMaximalVertices(vecDirection12, m_listVertices1);

	// determine minimal vertices of box 2
	boundingOrientedBox2.EnumerateMaximalVertices(-vecDirection12, m_listVertices2);

	// ensure lists in increasing size order
	PointList *pListVertices1, *pListVertices2;
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
	ContactPoint contactPoint;
	contactPoint.Penetration = rLeastOverlap;
	contactPoint.Normal = vecDirection12;

	// vertex - vertex contact
	if ((pListVertices1->Size() == 1) && (pListVertices2->Size() == 1))
	{
		ContactPoint contactPoint;
		TPoint3<Real> ptVertex1 = (*pListVertices1)[0];
		TPoint3<Real> ptVertex2 = (*pListVertices2)[0];
		contactPoint.Position = ptVertex1 + (ptVertex2 - ptVertex1) * (Real) 0.5;
		p_contactManifold.ContactPoints.Add(contactPoint);
		return;
	}

	// vertex - edge contact
	if ((pListVertices1->Size() == 1) && (pListVertices2->Size() == 2))
	{
		LineSegment lsgEdge((*pListVertices1)[0], (*pListVertices2)[0]);
		TPoint3<Real> ptClosesEdgePoint;
		lsgEdge.DistanceFromPoint(boundingOrientedBox1.Centre, ptClosesEdgePoint);

		contactPoint.Position = (*pListVertices1)(0);
		contactPoint.Normal = ptClosesEdgePoint - boundingOrientedBox1.Centre;
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
		LineSegment lsgEdge1((*pListVertices1)[0], (*pListVertices1)[1]);
		LineSegment lsgEdge2((*pListVertices2)[0], (*pListVertices2)[1]);
		TPoint3<Real> ptClosest1, ptClosest2;
		lsgEdge1.ClosestPointsToSegment(lsgEdge2, ptClosest1, ptClosest2);

		contactPoint.Position = ptClosest1 + (ptClosest2 - ptClosest1) * (Real) 0.5;
		p_contactManifold.ContactPoints.Add(contactPoint);
		return;
	}

	// edge/face - face contact
	for (size_t unIndex = 0; unIndex < pListVertices1->Size(); unIndex++)
	{
		TPoint3<Real>& ptVertex = (*pListVertices1)[unIndex];
		if (Intersects(boundingOrientedBox1, ptVertex)
			&& Intersects(boundingOrientedBox2, ptVertex))
		{
			contactPoint.Position = ptVertex;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
	}
	for (size_t unIndex = 0; unIndex < pListVertices2->Size(); unIndex++)
	{
		TPoint3<Real>& ptVertex = (*pListVertices2)[unIndex];
		if (Intersects(boundingOrientedBox1, ptVertex)
			&& Intersects(boundingOrientedBox2, ptVertex))
		{
			contactPoint.Position = ptVertex;
			p_contactManifold.ContactPoints.Add(contactPoint);
		}
	}
	if (p_contactManifold.ContactPoints.Size() > 0)
		return;

	// hack in case no contacts detected
	TVector3<Real> vecAverageContact = TVector3<Real>::Zero;
	for (size_t unIndex = 0; unIndex < pListVertices1->Size(); unIndex++)
		vecAverageContact += (*pListVertices1)(unIndex).ToVector();
	for (size_t unIndex = 0; unIndex < pListVertices2->Size(); unIndex++)
		vecAverageContact += (*pListVertices2)(unIndex).ToVector();
	vecAverageContact /= (pListVertices1->Size() + pListVertices2->Size());
	contactPoint.Position = TPoint3<Real>::Origin + vecAverageContact;
	p_contactManifold.ContactPoints.Add(contactPoint);
}
