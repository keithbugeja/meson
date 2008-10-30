#include "TriangleMeshToBoxCollisionDetector.h"

#include "ArrayList.h"

#include "GrvTriangle.h"

#include "TriangleMesh.h"
#include "Box.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

bool TriangleMeshToBoxCollisionDetector::TestIntersection(
	const TriangleMeshNode* pTriangleMeshNode,
	const BoundingOrientedBox& p_boundingOrientedBox) const
{
	if (!pTriangleMeshNode->BoundingVolume.Intersects(p_boundingOrientedBox))
		return false;

	static Triangle triangle;
	for (size_t unIndex = 0; unIndex < pTriangleMeshNode->Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = pTriangleMeshNode->Triangles[unIndex];
		triangle.Vertices[0] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[0]];
		triangle.Vertices[1] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[1]];
		triangle.Vertices[2] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[2]];
		if (p_boundingOrientedBox.Intersects(triangle))
			return true;
	}

	if (pTriangleMeshNode->Child[0] != NULL
		&& TestIntersection(pTriangleMeshNode->Child[0], p_boundingOrientedBox))
		return true;

	if (pTriangleMeshNode->Child[1] != NULL
		&& TestIntersection(pTriangleMeshNode->Child[1], p_boundingOrientedBox))
		return true;

	return false;
}

void TriangleMeshToBoxCollisionDetector::ComputeContactManifold(
	const Triangle& p_triangle,
	const BoundingOrientedBox& p_boundingOrientedBox,
	ContactManifold& p_contactManifold) const
{
	static TInterval<Real> itvTriangleProjection, itvBoxProjection, itvPenetration;
	static TVector3<Real> vecPenetrationNormal, vecCurrentAxis;

	Real rCurrentPenetration = (Real) 0.0;
	Real rPenetration = TMaths<Real>::Maximum;

	// triangle normal SAT
	vecCurrentAxis = p_triangle.Normal();
	p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);
	p_boundingOrientedBox.ProjectToInterval(vecCurrentAxis, itvBoxProjection);
	itvPenetration = itvTriangleProjection.IntersectCopy(itvBoxProjection);
	if (itvPenetration.IsEmpty())
		return;
	else
	{
		rCurrentPenetration = itvPenetration.Span();
		if (TMaths<Real>::Equals(rCurrentPenetration, (Real) 0.0))
			rCurrentPenetration = TMaths<Real>::Min(
				itvPenetration.Median() - itvBoxProjection.Min,
				itvBoxProjection.Max - itvPenetration.Median());

		if (rPenetration > rCurrentPenetration)
		{
			rPenetration = rCurrentPenetration;
			vecPenetrationNormal = vecCurrentAxis;
		}
	}

	// OBB axes SATs
	for (size_t unAxisIndex = 0; unAxisIndex < 3; unAxisIndex++)
	{
		vecCurrentAxis = p_boundingOrientedBox.Axis[unAxisIndex];
		p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);
		p_boundingOrientedBox.ProjectToInterval(vecCurrentAxis, itvBoxProjection);
		itvPenetration = itvTriangleProjection.IntersectCopy(itvBoxProjection);
		if (itvPenetration.IsEmpty())
			return;
		else
		{
			rCurrentPenetration = itvPenetration.Span();
			if (TMaths<Real>::Equals(rCurrentPenetration, (Real) 0.0))
				rCurrentPenetration = TMaths<Real>::Min(
					itvPenetration.Median() - itvBoxProjection.Min,
					itvBoxProjection.Max - itvPenetration.Median());

			if (rPenetration > rCurrentPenetration)
			{
				rPenetration = rCurrentPenetration;
				vecPenetrationNormal = vecCurrentAxis;
			}
		}
	}

	// OBB and triangle edge cross products SATs
	for (size_t unIndexBox = 0; unIndexBox < 3; unIndexBox++)
	{
		const TVector3<Real>& vecAxisBox = p_boundingOrientedBox.Axis[unIndexBox];

		for (size_t unIndexTriangle = 0; unIndexTriangle < 3; unIndexTriangle++)
		{
			TVector3<Real>& vecAxisTriangle = p_triangle.Edge(unIndexTriangle);

			if (vecAxisTriangle.IsZero())
				continue;

			vecCurrentAxis = vecAxisBox ^ vecAxisTriangle;

			// ignore parallel axes pairs
			if (vecCurrentAxis.IsZero())
				continue;

			vecCurrentAxis.Normalise();

			p_boundingOrientedBox.ProjectToInterval(vecCurrentAxis, itvBoxProjection);
			p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);

			itvPenetration = itvTriangleProjection.IntersectCopy(itvBoxProjection);
			if (itvPenetration.IsEmpty())
				return;
			else
			{
				rCurrentPenetration = itvPenetration.Span();
				if (TMaths<Real>::Equals(rCurrentPenetration, (Real) 0.0))
					rCurrentPenetration = TMaths<Real>::Min(
						itvPenetration.Median() - itvBoxProjection.Min,
						itvBoxProjection.Max - itvPenetration.Median());

				if (rPenetration > rCurrentPenetration)
				{
					rPenetration = rCurrentPenetration;
					vecPenetrationNormal = vecCurrentAxis;
				}
			}
		}
	}

	// all SATs failed, hence intersecting

	// ensure normal pointing from triangle to box
	if ((p_boundingOrientedBox.Centre - p_triangle.Centroid()) * vecPenetrationNormal < (Real) 0.0)
		vecPenetrationNormal = -vecPenetrationNormal;

	// add contact points from triangle
	static VectorArrayList listVertices;
	p_triangle.EnumerateMaximalVertices(vecPenetrationNormal, listVertices);
	for (size_t unIndex = 0; unIndex < listVertices.Size(); unIndex++)
	{
		TVector3<Real>& vecVertex = listVertices[unIndex];
		if (p_boundingOrientedBox.Contains(vecVertex))
			p_contactManifold.ContactPoints.Add(
				ContactPoint(vecVertex, vecPenetrationNormal, rPenetration));
	}

	// add contact points from box
	p_boundingOrientedBox.EnumerateMaximalVertices(-vecPenetrationNormal, listVertices);
	for (size_t unIndex = 0; unIndex < listVertices.Size(); unIndex++)
	{
		TVector3<Real>& vecVertex = listVertices[unIndex];
		if (p_triangle.ContainsPointProjection(vecVertex))
		{
			rPenetration = TMaths<Real>::Abs(p_triangle.SignedDistanceFromPlane(vecVertex));
			p_contactManifold.ContactPoints.Add(
				ContactPoint(vecVertex, vecPenetrationNormal, rPenetration));
		}
	}

	// hack in case no contacts generated
	if (p_contactManifold.ContactPoints.Size() == 0)
	{
		p_contactManifold.ContactPoints.Add(
			ContactPoint(p_triangle.Centroid(), vecPenetrationNormal, rPenetration));
	}
}

void TriangleMeshToBoxCollisionDetector::ComputeContactManifold(
	const TriangleMeshNode* pTriangleMeshNode,
	const BoundingOrientedBox& p_boundingOrientedBox,
	ContactManifold& p_contactManifold) const
{
	if (!pTriangleMeshNode->BoundingVolume.Intersects(p_boundingOrientedBox))
		return;

	static Triangle triangle;

	for (size_t unIndex = 0; unIndex < pTriangleMeshNode->Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = pTriangleMeshNode->Triangles[unIndex];
		triangle.Vertices[0] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[0]];
		triangle.Vertices[1] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[1]];
		triangle.Vertices[2] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[2]];

		ComputeContactManifold(triangle, p_boundingOrientedBox, p_contactManifold);
	}

	if (pTriangleMeshNode->Child[0] != NULL)
		ComputeContactManifold(
			pTriangleMeshNode->Child[0], p_boundingOrientedBox, p_contactManifold);

	if (pTriangleMeshNode->Child[1] != NULL)
		ComputeContactManifold(
			pTriangleMeshNode->Child[1], p_boundingOrientedBox, p_contactManifold);
}

TriangleMeshToBoxCollisionDetector::TriangleMeshToBoxCollisionDetector(void)
	: m_strGeometryType1("TriangleMesh")
	, m_strGeometryType2("Box")
{
}

TriangleMeshToBoxCollisionDetector::~TriangleMeshToBoxCollisionDetector(void)
{
}

const String& TriangleMeshToBoxCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& TriangleMeshToBoxCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool TriangleMeshToBoxCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Box", "Expected box geometry.")

	const TriangleMesh& triangleMesh = (TriangleMesh &) p_geometry1;

	if (triangleMesh.Root == NULL)
		return false;
	else
	{
		const Box& box = (Box &) p_geometry2;
		BoundingOrientedBox boundingOrientedBox(TVector3<Real>::Zero, box.Extent);
		boundingOrientedBox.Transform(p_trnRelativePlacement);
		return TestIntersection(triangleMesh.Root, boundingOrientedBox);
	}
}

bool TriangleMeshToBoxCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for triangle mesh and box not implemented yet.",
		__FILE__, __LINE__);
}

void TriangleMeshToBoxCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Box", "Expected box geometry.")

	const TriangleMesh& triangleMesh = (TriangleMesh &) p_geometry1;

	p_contactManifold.ContactPoints.Clear();

	if (triangleMesh.Root == NULL)
		return;
	else
	{
		const Box& box = (Box &) p_geometry2;
		BoundingOrientedBox boundingOrientedBox(TVector3<Real>::Zero, box.Extent);
		boundingOrientedBox.Transform(p_trnRelativePlacement);
		ComputeContactManifold(triangleMesh.Root, boundingOrientedBox, p_contactManifold);
	}
}
