#include "TriangleMeshToCylinderCollisionDetector.h"

#include "ArrayList.h"

#include "GrvTriangle.h"

#include "TriangleMesh.h"
#include "Cylinder.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

bool TriangleMeshToCylinderCollisionDetector::TestIntersection(
	const Triangle& p_triangle,
	const LineSegment& p_lnsCylinderShaft, Real p_rCylinderRadius) const
{
	static TInterval<Real> itvTriangleProjection, itvCylinderProjection;
	static TVector3<Real> vecCurrentAxis;

	// triangle normal SAT
	vecCurrentAxis = p_triangle.Normal();
	p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);
	Cylinder::ProjectToInterval(p_lnsCylinderShaft, p_rCylinderRadius,
		vecCurrentAxis, itvCylinderProjection);
	if (!itvTriangleProjection.Intersects(itvCylinderProjection))
		return false;

	TVector3<Real> vecCylinderAxis(p_lnsCylinderShaft.Direction());

	// cylinder shaft SAT
	vecCurrentAxis = vecCylinderAxis;
	p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);
	Cylinder::ProjectToInterval(p_lnsCylinderShaft, p_rCylinderRadius,
		vecCurrentAxis, itvCylinderProjection);
	if (!itvTriangleProjection.Intersects(itvCylinderProjection))
		return false;

	// shaft and triangle edge closest line SATs
	TPoint3<Real> ptClosest1, ptClosest2;
	for (size_t unIndexTriangle = 0; unIndexTriangle < 3; unIndexTriangle++)
	{
		LineSegment lnsEdge(
			p_triangle.Vertices[unIndexTriangle],
			p_triangle.Vertices[(unIndexTriangle + 1) % 3]);

		lnsEdge.ClosestPointsToSegment(p_lnsCylinderShaft, ptClosest1, ptClosest2);
		if (ptClosest1 == ptClosest2)
			return true;

		vecCurrentAxis = ptClosest2 - ptClosest1;

		Cylinder::ProjectToInterval(p_lnsCylinderShaft, p_rCylinderRadius,
			vecCurrentAxis, itvCylinderProjection);
		p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);

		if (!itvTriangleProjection.Intersects(itvCylinderProjection))
			return false;
	}

	// all SATs failed, hence intersecting
	return true;
}

bool TriangleMeshToCylinderCollisionDetector::TestIntersection(
	const TriangleMeshNode* pTriangleMeshNode,
	const LineSegment& p_lnsCylinderShaft, Real p_rCylinderRadius,
	const BoundingAxisAlignedBox& p_boundingVolumeCylinder) const
{
	if (!pTriangleMeshNode->BoundingVolume.Intersects(p_boundingVolumeCylinder))
		return false;

	static Triangle triangle;
	for (size_t unIndex = 0; unIndex < pTriangleMeshNode->Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = pTriangleMeshNode->Triangles[unIndex];
		triangle.Vertices[0] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[0]];
		triangle.Vertices[1] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[1]];
		triangle.Vertices[2] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[2]];

		// test triangle against cylinder BV
		if (!p_boundingVolumeCylinder.Intersects(triangle))
			continue;

		// do actual test
		if (TestIntersection(triangle, p_lnsCylinderShaft, p_rCylinderRadius))
			return true;
	}

	if (pTriangleMeshNode->Child[0] != NULL
		&& TestIntersection(pTriangleMeshNode->Child[0],
			p_lnsCylinderShaft, p_rCylinderRadius, p_boundingVolumeCylinder))
		return true;

	if (pTriangleMeshNode->Child[1] != NULL
		&& TestIntersection(pTriangleMeshNode->Child[1],
			p_lnsCylinderShaft, p_rCylinderRadius, p_boundingVolumeCylinder))
		return true;

	return false;
}

void TriangleMeshToCylinderCollisionDetector::ComputeContactManifold(
	const Triangle& p_triangle,
	const LineSegment& p_lnsCylinderShaft, Real p_rCylinderRadius,
	ContactManifold& p_contactManifold) const
{
	static TInterval<Real> itvTriangleProjection, itvCylinderProjection, itvPenetration;
	static TVector3<Real> vecPenetrationNormal, vecCurrentAxis;

	Real rCurrentPenetration = (Real) 0.0;
	Real rPenetration = TMaths<Real>::Maximum;

	// triangle normal SAT
	vecCurrentAxis = p_triangle.Normal();
	p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);

	Cylinder::ProjectToInterval(p_lnsCylinderShaft, p_rCylinderRadius,
		vecCurrentAxis, itvCylinderProjection);
	itvPenetration = itvTriangleProjection.IntersectCopy(itvCylinderProjection);
	if (itvPenetration.IsEmpty())
		return;
	else
	{
		rCurrentPenetration = itvPenetration.Span();
		if (TMaths<Real>::Equals(itvTriangleProjection.Span(), (Real) 0.0))
			rCurrentPenetration = TMaths<Real>::Min(
				itvTriangleProjection.Median() - itvCylinderProjection.Min,
				itvCylinderProjection.Max - itvTriangleProjection.Median());

		if (rPenetration > rCurrentPenetration)
		{
			rPenetration = rCurrentPenetration;
			vecPenetrationNormal = vecCurrentAxis;
		}
	}

	TVector3<Real> vecCylinderAxis(p_lnsCylinderShaft.Direction());

	// cylinder shaft SAT
	vecCurrentAxis = vecCylinderAxis;
	p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);
	Cylinder::ProjectToInterval(p_lnsCylinderShaft, p_rCylinderRadius,
		vecCurrentAxis, itvCylinderProjection);
	itvPenetration = itvTriangleProjection.IntersectCopy(itvCylinderProjection);
	if (itvPenetration.IsEmpty())
		return;
	else
	{
		rCurrentPenetration = itvPenetration.Span();
		if (TMaths<Real>::Equals(itvTriangleProjection.Span(), (Real) 0.0))
			rCurrentPenetration = TMaths<Real>::Min(
				itvTriangleProjection.Median() - itvCylinderProjection.Min,
				itvCylinderProjection.Max - itvTriangleProjection.Median());

		if (rPenetration > rCurrentPenetration)
		{
			rPenetration = rCurrentPenetration;
			vecPenetrationNormal = vecCurrentAxis;
		}
	}

	// cylinder and triangle edge cross product SATs
	TPoint3<Real> ptClosest1, ptClosest2;
	for (size_t unIndexTriangle = 0; unIndexTriangle < 3; unIndexTriangle++)
	{
		LineSegment lnsEdge(
			p_triangle.Vertices[unIndexTriangle],
			p_triangle.Vertices[(unIndexTriangle + 1) % 3]);

		lnsEdge.ClosestPointsToSegment(p_lnsCylinderShaft, ptClosest1, ptClosest2);
		if (ptClosest1 == ptClosest2)
			continue;

		vecCurrentAxis = ptClosest2 - ptClosest1;

		vecCurrentAxis.Normalise();

		Cylinder::ProjectToInterval(p_lnsCylinderShaft, p_rCylinderRadius,
			vecCurrentAxis, itvCylinderProjection);
		p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);

		itvPenetration = itvTriangleProjection.IntersectCopy(itvCylinderProjection);
		if (itvPenetration.IsEmpty())
			return;
		else
		{
			rCurrentPenetration = itvPenetration.Span();
			if (TMaths<Real>::Equals(itvTriangleProjection.Span(), (Real) 0.0))
				rCurrentPenetration = TMaths<Real>::Min(
					itvTriangleProjection.Median() - itvCylinderProjection.Min,
					itvCylinderProjection.Max - itvTriangleProjection.Median());

			if (rPenetration > rCurrentPenetration)
			{
				rPenetration = rCurrentPenetration;
				vecPenetrationNormal = vecCurrentAxis;
			}
		}
	}

	// all SATs failed, hence intersecting

	// ensure normal pointing from triangle to cylinder
	if ((p_lnsCylinderShaft.Midpoint() - p_triangle.Centroid()) * vecPenetrationNormal < (Real) 0.0)
		vecPenetrationNormal = -vecPenetrationNormal;

	// add contact points from triangle
	static PointArrayList listVertices;
	p_triangle.EnumerateMaximalVertices(vecPenetrationNormal, listVertices);
	for (size_t unIndex = 0; unIndex < listVertices.Size(); unIndex++)
	{
		TPoint3<Real>& ptVertex = listVertices[unIndex];
		if (Cylinder::IntersectsPoint(p_lnsCylinderShaft, p_rCylinderRadius, ptVertex))
			p_contactManifold.ContactPoints.Add(
				ContactPoint(ptVertex, vecPenetrationNormal, rPenetration));
	}

	// add contact points from cylinder
	Cylinder::EnumerateMaximalVertices(p_lnsCylinderShaft, p_rCylinderRadius,
		-vecPenetrationNormal, listVertices);
	for (size_t unIndex = 0; unIndex < listVertices.Size(); unIndex++)
	{
		TPoint3<Real>& ptVertex = listVertices[unIndex];
		if (p_triangle.ContainsPointProjection(ptVertex)
			&& !p_triangle.PointAbovePlane(ptVertex))
		{
			//rPenetration = TMaths<Real>::Abs(p_triangle.SignedDistanceFromPlane(ptVertex));
			p_contactManifold.ContactPoints.Add(
				ContactPoint(ptVertex, vecPenetrationNormal, rPenetration));
		}
	}
}

void TriangleMeshToCylinderCollisionDetector::ComputeContactManifold(
	const TriangleMeshNode* pTriangleMeshNode,
	const LineSegment& p_lnsCylinderShaft, Real p_rCylinderRadius,
	const BoundingAxisAlignedBox& p_boundingVolumeCylinder,
	ContactManifold& p_contactManifold) const
{
	if (!pTriangleMeshNode->BoundingVolume.Intersects(p_boundingVolumeCylinder))
		return;

	static Triangle triangle;
	for (size_t unIndex = 0; unIndex < pTriangleMeshNode->Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = pTriangleMeshNode->Triangles[unIndex];
		triangle.Vertices[0] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[0]];
		triangle.Vertices[1] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[1]];
		triangle.Vertices[2] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[2]];

		ComputeContactManifold(triangle, p_lnsCylinderShaft,
			p_rCylinderRadius, p_contactManifold);
	}

	if (pTriangleMeshNode->Child[0] != NULL)
		ComputeContactManifold(
			pTriangleMeshNode->Child[0],
			p_lnsCylinderShaft, p_rCylinderRadius,
			p_boundingVolumeCylinder, p_contactManifold);

	if (pTriangleMeshNode->Child[1] != NULL)
		ComputeContactManifold(
			pTriangleMeshNode->Child[1],
			p_lnsCylinderShaft, p_rCylinderRadius,
			p_boundingVolumeCylinder, p_contactManifold);
}

TriangleMeshToCylinderCollisionDetector::TriangleMeshToCylinderCollisionDetector(void)
	: m_strGeometryType1("TriangleMesh")
	, m_strGeometryType2("Cylinder")
{
}

TriangleMeshToCylinderCollisionDetector::~TriangleMeshToCylinderCollisionDetector(void)
{
}

const String& TriangleMeshToCylinderCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& TriangleMeshToCylinderCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool TriangleMeshToCylinderCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Cylinder", "Expected cylinder geometry.")

	const TriangleMesh& triangleMesh = (TriangleMesh &) p_geometry1;

	if (triangleMesh.Root == NULL)
		return false;
	else
	{
		const Cylinder& cylinder = (Cylinder &) p_geometry2;

		static LineSegment lnsCylinderShaft;
		Real rHalfHeight = cylinder.Height * (Real) 0.5;
		lnsCylinderShaft.Start = TPoint3<Real>((Real) 0.0, -rHalfHeight, (Real) 0.0);
		lnsCylinderShaft.End = TPoint3<Real>((Real) 0.0, rHalfHeight, (Real) 0.0);
		lnsCylinderShaft.Transform(p_trnRelativePlacement);

		static BoundingAxisAlignedBox boundingAxisAlignedBox;
		cylinder.ComputeBoundingVolume(boundingAxisAlignedBox);
		boundingAxisAlignedBox.Transform(p_trnRelativePlacement);

		return TestIntersection(triangleMesh.Root, lnsCylinderShaft, cylinder.Radius, boundingAxisAlignedBox);
	}
}

bool TriangleMeshToCylinderCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for triangle mesh and cylinder not implemented yet.",
		__FILE__, __LINE__);
}

void TriangleMeshToCylinderCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Cylinder", "Expected cylinder geometry.")

	const TriangleMesh& triangleMesh = (TriangleMesh &) p_geometry1;

	p_contactManifold.ContactPoints.Clear();

	if (triangleMesh.Root == NULL)
		return;
	else
	{
		const Cylinder& cylinder = (Cylinder &) p_geometry2;

		static LineSegment lnsCylinderShaft;
		Real rHalfHeight = cylinder.Height * (Real) 0.5;
		lnsCylinderShaft.Start = TPoint3<Real>((Real) 0.0, -rHalfHeight, (Real) 0.0);
		lnsCylinderShaft.End = TPoint3<Real>((Real) 0.0, rHalfHeight, (Real) 0.0);
		lnsCylinderShaft.Transform(p_trnRelativePlacement);

		static BoundingAxisAlignedBox boundingAxisAlignedBox;
		cylinder.ComputeBoundingVolume(boundingAxisAlignedBox);
		boundingAxisAlignedBox.Transform(p_trnRelativePlacement);

		ComputeContactManifold(triangleMesh.Root,
			lnsCylinderShaft, cylinder.Radius,
			boundingAxisAlignedBox, p_contactManifold);
	}
}
