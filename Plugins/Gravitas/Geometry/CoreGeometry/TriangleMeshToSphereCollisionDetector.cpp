#include "TriangleMeshToSphereCollisionDetector.h"

#include "ArrayList.h"

#include "GrvTriangle.h"

#include "TriangleMesh.h"
#include "Sphere.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

bool TriangleMeshToSphereCollisionDetector::TestIntersection(
	const TriangleMeshNode* pTriangleMeshNode,
	const BoundingSphere& p_boundingSphere) const
{
	if (!pTriangleMeshNode->BoundingVolume.Intersects(p_boundingSphere))
		return false;

	static Triangle triangle;
	for (size_t unIndex = 0; unIndex < pTriangleMeshNode->Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = pTriangleMeshNode->Triangles[unIndex];
		triangle.Vertices[0] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[0]];
		triangle.Vertices[1] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[1]];
		triangle.Vertices[2] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[2]];
		TPoint3<Real> ptClosestPoint(triangle.ClosestPoint(p_boundingSphere.Centre));
		if ((ptClosestPoint - p_boundingSphere.Centre).LengthSquared() <= p_boundingSphere.RadiusSquared)
			return true;
	}

	if (pTriangleMeshNode->Child[0] != NULL
		&& TestIntersection(pTriangleMeshNode->Child[0], p_boundingSphere))
		return true;

	if (pTriangleMeshNode->Child[1] != NULL
		&& TestIntersection(pTriangleMeshNode->Child[1], p_boundingSphere))
		return true;

	return false;
}

void TriangleMeshToSphereCollisionDetector::ComputeContactManifold(
	const TriangleMeshNode* pTriangleMeshNode,
	const BoundingSphere& p_boundingSphere,
	ContactManifold& p_contactManifold) const
{
	if (!pTriangleMeshNode->BoundingVolume.Intersects(p_boundingSphere))
		return;

	static Triangle triangle;
	for (size_t unIndex = 0; unIndex < pTriangleMeshNode->Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = pTriangleMeshNode->Triangles[unIndex];
		triangle.Vertices[0] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[0]];
		triangle.Vertices[1] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[1]];
		triangle.Vertices[2] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[2]];
		TPoint3<Real> ptClosestPoint(triangle.ClosestPoint(p_boundingSphere.Centre));
		TVector3<Real> vecOffset(ptClosestPoint - p_boundingSphere.Centre);
		if (vecOffset.LengthSquared() <= p_boundingSphere.RadiusSquared)
		{
			p_contactManifold.ContactPoints.Add(
				ContactPoint(ptClosestPoint, -vecOffset.NormaliseCopy(),
					p_boundingSphere.Radius - vecOffset.Length()));
		}
	}

	if (pTriangleMeshNode->Child[0] != NULL)
		ComputeContactManifold(
			pTriangleMeshNode->Child[0], p_boundingSphere, p_contactManifold);

	if (pTriangleMeshNode->Child[1] != NULL)
		ComputeContactManifold(
			pTriangleMeshNode->Child[1], p_boundingSphere, p_contactManifold);
}

TriangleMeshToSphereCollisionDetector::TriangleMeshToSphereCollisionDetector(void)
	: m_strGeometryType1("TriangleMesh")
	, m_strGeometryType2("Sphere")
{
}

TriangleMeshToSphereCollisionDetector::~TriangleMeshToSphereCollisionDetector(void)
{
}

const String& TriangleMeshToSphereCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& TriangleMeshToSphereCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool TriangleMeshToSphereCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Expected sphere geometry.")

	const TriangleMesh& triangleMesh = (TriangleMesh &) p_geometry1;

	if (triangleMesh.Root == NULL)
		return false;
	else
	{
		// transform sphere centre to polyhedron space
		const Sphere& sphere = (Sphere &) p_geometry2;
		TPoint3<Real> ptSphereCentre(TPoint3<Real>::Origin + p_trnRelativePlacement.Translation);
		BoundingSphere boundingSphere(ptSphereCentre, sphere.Radius);
		return TestIntersection(triangleMesh.Root, boundingSphere);
	}
}

bool TriangleMeshToSphereCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for triangle mesh and sphere not implemented yet.",
		__FILE__, __LINE__);
}

void TriangleMeshToSphereCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Expected sphere geometry.")

	const TriangleMesh& triangleMesh = (TriangleMesh &) p_geometry1;

	p_contactManifold.ContactPoints.Clear();

	if (triangleMesh.Root == NULL)
		return;
	else
	{
		// transform sphere centre to polyhedron space
		const Sphere& sphere = (Sphere &) p_geometry2;
		TPoint3<Real> ptSphereCentre(TPoint3<Real>::Origin + p_trnRelativePlacement.Translation);
		BoundingSphere boundingSphere(ptSphereCentre, sphere.Radius);
		ComputeContactManifold(triangleMesh.Root, boundingSphere, p_contactManifold);
	}
}
