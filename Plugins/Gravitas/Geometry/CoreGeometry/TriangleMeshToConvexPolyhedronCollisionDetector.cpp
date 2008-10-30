#include "TriangleMeshToConvexPolyhedronCollisionDetector.h"

#include "ArrayList.h"

#include "GrvTriangle.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

bool TriangleMeshToConvexPolyhedronCollisionDetector::TestIntersection(
	const Triangle& p_triangle,
	const ConvexPolyhedron& p_convexPolyhedron) const
{
	static TInterval<Real> itvTriangleProjection, itvPolyhedronProjection;
	static TVector3<Real> vecCurrentAxis;

	// triangle normal SAT
	vecCurrentAxis = p_triangle.Normal();
	p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);
	p_convexPolyhedron.ProjectToInterval(vecCurrentAxis, itvPolyhedronProjection);
	if (!itvTriangleProjection.Intersects(itvPolyhedronProjection))
		return false;

	// polyhedron face normal SATs
	for (size_t unIndex = 0; unIndex < p_convexPolyhedron.Faces.Size(); unIndex++)
	{
		ConvexPolyhedronFace& face = p_convexPolyhedron.Faces[unIndex];
		vecCurrentAxis = face.Normal;
		p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);
		p_convexPolyhedron.ProjectToInterval(vecCurrentAxis, itvPolyhedronProjection);

		if (!itvTriangleProjection.Intersects(itvPolyhedronProjection))
			return false;
	}

	// polyhedron and triangle edge cross products SATs
	for (size_t unIndexPolyhedron = 0; unIndexPolyhedron < p_convexPolyhedron.Edges.Size(); unIndexPolyhedron++)
	{
		const TVector3<Real>& vecPolyhedronEdge
			= p_convexPolyhedron.Edges[unIndexPolyhedron].Displacement;

		for (size_t unIndexTriangle = 0; unIndexTriangle < 3; unIndexTriangle++)
		{
			TVector3<Real> vecTriangleEdge(p_triangle.Edge(unIndexTriangle));

			if (vecTriangleEdge.IsZero())
				continue;

			vecCurrentAxis = vecPolyhedronEdge ^ vecTriangleEdge;

			// ignore parallel axes pairs
			if (vecCurrentAxis.IsZero())
				continue;

			p_convexPolyhedron.ProjectToInterval(vecCurrentAxis, itvPolyhedronProjection);
			p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);

			if (!itvTriangleProjection.Intersects(itvPolyhedronProjection))
				return false;
		}
	}

	// all SATs failed, hence intersecting
	return true;
}

bool TriangleMeshToConvexPolyhedronCollisionDetector::TestIntersection(
	const TriangleMeshNode* pTriangleMeshNode,
	const ConvexPolyhedron& p_convexPolyhedron,
	const BoundingAxisAlignedBox& p_boundingVolumePolyhedron) const
{
	if (!pTriangleMeshNode->BoundingVolume.Intersects(p_boundingVolumePolyhedron))
		return false;

	static Triangle triangle;
	for (size_t unIndex = 0; unIndex < pTriangleMeshNode->Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = pTriangleMeshNode->Triangles[unIndex];
		triangle.Vertices[0] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[0]];
		triangle.Vertices[1] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[1]];
		triangle.Vertices[2] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[2]];

		// test triangle against polyhedron AABB
		if (!p_boundingVolumePolyhedron.Intersects(triangle))
			continue;

		// do actual test
		if (TestIntersection(triangle, p_convexPolyhedron))
			return true;
	}

	if (pTriangleMeshNode->Child[0] != NULL
		&& TestIntersection(pTriangleMeshNode->Child[0], p_convexPolyhedron, p_boundingVolumePolyhedron))
		return true;

	if (pTriangleMeshNode->Child[1] != NULL
		&& TestIntersection(pTriangleMeshNode->Child[1], p_convexPolyhedron, p_boundingVolumePolyhedron))
		return true;

	return false;
}

void TriangleMeshToConvexPolyhedronCollisionDetector::ComputeContactManifold(
	const Triangle& p_triangle,
	const ConvexPolyhedron& p_convexPolyhedron,
	ContactManifold& p_contactManifold) const
{
	static TInterval<Real> itvTriangleProjection, itvPolyhedronProjection, itvPenetration;
	static TVector3<Real> vecPenetrationNormal, vecCurrentAxis;

	Real rCurrentPenetration = (Real) 0.0;
	Real rPenetration = TMaths<Real>::Maximum;

	// triangle normal SAT
	vecCurrentAxis = p_triangle.Normal();
	p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);
	p_convexPolyhedron.ProjectToInterval(vecCurrentAxis, itvPolyhedronProjection);
	itvPenetration = itvTriangleProjection.IntersectCopy(itvPolyhedronProjection);
	if (itvPenetration.IsEmpty())
		return;
	else
	{
		rCurrentPenetration = itvPenetration.Span();
		if (TMaths<Real>::Equals(rCurrentPenetration, (Real) 0.0))
			rCurrentPenetration = TMaths<Real>::Min(
				itvPenetration.Median() - itvPolyhedronProjection.Min,
				itvPolyhedronProjection.Max - itvPenetration.Median());

		if (rPenetration > rCurrentPenetration)
		{
			rPenetration = rCurrentPenetration;
			vecPenetrationNormal = vecCurrentAxis;
		}
	}

	// polyhedron face normals SATs
	for (size_t unIndex = 0; unIndex < p_convexPolyhedron.Faces.Size(); unIndex++)
	{
		ConvexPolyhedronFace& face = p_convexPolyhedron.Faces[unIndex];
		vecCurrentAxis = face.Normal;
		p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);
		p_convexPolyhedron.ProjectToInterval(vecCurrentAxis, itvPolyhedronProjection);
		itvPenetration = itvTriangleProjection.IntersectCopy(itvPolyhedronProjection);
		if (itvPenetration.IsEmpty())
			return;
		else
		{
			rCurrentPenetration = itvPenetration.Span();
			if (TMaths<Real>::Equals(rCurrentPenetration, (Real) 0.0))
				rCurrentPenetration = TMaths<Real>::Min(
					itvPenetration.Median() - itvPolyhedronProjection.Min,
					itvPolyhedronProjection.Max - itvPenetration.Median());

			if (rPenetration > rCurrentPenetration)
			{
				rPenetration = rCurrentPenetration;
				vecPenetrationNormal = vecCurrentAxis;
			}
		}
	}

	// polyhedron and triangle edge cross products SATs
	for (size_t unIndexPolyhedron = 0; unIndexPolyhedron < p_convexPolyhedron.Edges.Size(); unIndexPolyhedron++)
	{
		const TVector3<Real>& vecPolyhedronEdge = p_convexPolyhedron.Edges[unIndexPolyhedron].Displacement;

		for (size_t unIndexTriangle = 0; unIndexTriangle < 3; unIndexTriangle++)
		{
			TVector3<Real> vecTriangleEdge(p_triangle.Edge(unIndexTriangle));

			if (vecTriangleEdge.IsZero())
				continue;

			vecCurrentAxis = vecPolyhedronEdge ^ vecTriangleEdge;

			// ignore parallel axes pairs
			if (vecCurrentAxis.IsZero())
				continue;

			vecCurrentAxis.Normalise();

			p_convexPolyhedron.ProjectToInterval(vecCurrentAxis, itvPolyhedronProjection);
			p_triangle.ProjectToInterval(vecCurrentAxis, itvTriangleProjection);

			itvPenetration = itvTriangleProjection.IntersectCopy(itvPolyhedronProjection);
			if (itvPenetration.IsEmpty())
				return;
			else
			{
				rCurrentPenetration = itvPenetration.Span();
				if (TMaths<Real>::Equals(rCurrentPenetration, (Real) 0.0))
					rCurrentPenetration = TMaths<Real>::Min(
						itvPenetration.Median() - itvPolyhedronProjection.Min,
						itvPolyhedronProjection.Max - itvPenetration.Median());

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
	if ((p_convexPolyhedron.GetCentroid() - p_triangle.Centroid()) * vecPenetrationNormal < (Real) 0.0)
		vecPenetrationNormal = -vecPenetrationNormal;

	// add contact points from triangle
	static VectorArrayList listVertices;
	p_triangle.EnumerateMaximalVertices(vecPenetrationNormal, listVertices);
	for (size_t unIndex = 0; unIndex < listVertices.Size(); unIndex++)
	{
		TVector3<Real>& vecVertex = listVertices[unIndex];
		if (p_convexPolyhedron.ContainsPoint(vecVertex))
			p_contactManifold.ContactPoints.Add(
				ContactPoint(vecVertex, vecPenetrationNormal, rPenetration));
	}

	// add contact points from polyhedron
	p_convexPolyhedron.EnumerateMaximalVertices(-vecPenetrationNormal, listVertices);
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

void TriangleMeshToConvexPolyhedronCollisionDetector::ComputeContactManifold(
	const TriangleMeshNode* pTriangleMeshNode,
	const ConvexPolyhedron& p_convexPolyhedron,
	const BoundingAxisAlignedBox& p_boundingVolumePolyhedron,
	ContactManifold& p_contactManifold) const
{
	if (!pTriangleMeshNode->BoundingVolume.Intersects(p_boundingVolumePolyhedron))
		return;

	static Triangle triangle;
	for (size_t unIndex = 0; unIndex < pTriangleMeshNode->Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = pTriangleMeshNode->Triangles[unIndex];
		triangle.Vertices[0] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[0]];
		triangle.Vertices[1] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[1]];
		triangle.Vertices[2] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[2]];

		ComputeContactManifold(triangle, p_convexPolyhedron, p_contactManifold);
	}

	if (pTriangleMeshNode->Child[0] != NULL)
		ComputeContactManifold(
			pTriangleMeshNode->Child[0], p_convexPolyhedron,
			p_boundingVolumePolyhedron, p_contactManifold);

	if (pTriangleMeshNode->Child[1] != NULL)
		ComputeContactManifold(
			pTriangleMeshNode->Child[1], p_convexPolyhedron,
			p_boundingVolumePolyhedron, p_contactManifold);
}

TriangleMeshToConvexPolyhedronCollisionDetector::TriangleMeshToConvexPolyhedronCollisionDetector(void)
	: m_strGeometryType1("TriangleMesh")
	, m_strGeometryType2("ConvexPolyhedron")
{
}

TriangleMeshToConvexPolyhedronCollisionDetector::~TriangleMeshToConvexPolyhedronCollisionDetector(void)
{
}

const String& TriangleMeshToConvexPolyhedronCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& TriangleMeshToConvexPolyhedronCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool TriangleMeshToConvexPolyhedronCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "ConvexPolyhedron", "Expected convex polyhedron geometry.")

	const TriangleMesh& triangleMesh = (TriangleMesh &) p_geometry1;

	if (triangleMesh.Root == NULL)
		return false;
	else
	{
		const ConvexPolyhedron& convexPolyhedron = (ConvexPolyhedron &) p_geometry2;

		static ConvexPolyhedron convexPolyhedronWorld;
		convexPolyhedronWorld = convexPolyhedron.TransformCopy(p_trnRelativePlacement);

		static BoundingAxisAlignedBox boundingAxisAlignedBox;
		convexPolyhedronWorld.ComputeBoundingVolume(boundingAxisAlignedBox);

		return TestIntersection(triangleMesh.Root, convexPolyhedronWorld, boundingAxisAlignedBox);
	}
}

bool TriangleMeshToConvexPolyhedronCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for triangle mesh and convex polyhedron not implemented yet.",
		__FILE__, __LINE__);
}

void TriangleMeshToConvexPolyhedronCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "ConvexPolyhedron", "Expected convex polyhedron geometry.")

	const TriangleMesh& triangleMesh = (TriangleMesh &) p_geometry1;

	p_contactManifold.ContactPoints.Clear();

	if (triangleMesh.Root == NULL)
		return;
	else
	{
		const ConvexPolyhedron& convexPolyhedron = (ConvexPolyhedron &) p_geometry2;

		static ConvexPolyhedron convexPolyhedronWorld;
		convexPolyhedronWorld = convexPolyhedron.TransformCopy(p_trnRelativePlacement);

		static BoundingAxisAlignedBox boundingAxisAlignedBox;
		convexPolyhedronWorld.ComputeBoundingVolume(boundingAxisAlignedBox);

		ComputeContactManifold(triangleMesh.Root, convexPolyhedronWorld,
			boundingAxisAlignedBox, p_contactManifold);
	}
}
