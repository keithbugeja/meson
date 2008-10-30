#include "TriangleMeshToHalfspaceCollisionDetector.h"

#include "ArrayList.h"

#include "GrvTriangle.h"

#include "TriangleMesh.h"
#include "Sphere.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Random;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

bool TriangleMeshToHalfspaceCollisionDetector::TestIntersection(
	const Meson::Gravitas::Geometry::TriangleMeshNode* pTriangleMeshNode,
	const TVector3<Real>& p_vecPlanePoint,
	const TVector3<Real>& p_vecPlaneNormal) const
{
	static TInterval<Real> itvVolumeProjection;
	pTriangleMeshNode->BoundingVolume.ProjectToInterval(p_vecPlaneNormal, itvVolumeProjection);

	Real rPlaneProjection = p_vecPlanePoint * p_vecPlaneNormal;

	if (!itvVolumeProjection.Contains(rPlaneProjection))
		return false;

	if (pTriangleMeshNode->Triangles.Size() > 0)
	{
		Triangle triangle;
		TInterval<Real> itvTriangleProjection;
		for (size_t unIndex = 0; unIndex < pTriangleMeshNode->Triangles.Size(); unIndex++)
		{
			IndexedTriangle& indexedTriangle = pTriangleMeshNode->Triangles[unIndex];
			triangle.Vertices[0] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[0]];
			triangle.Vertices[1] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[1]];
			triangle.Vertices[2] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[2]];

			triangle.ProjectToInterval(p_vecPlaneNormal, itvTriangleProjection);
			if (itvTriangleProjection.Contains(rPlaneProjection))
				return true;
		}
	}

	if (pTriangleMeshNode->Child[0] != NULL
		&& TestIntersection(pTriangleMeshNode->Child[0], p_vecPlanePoint, p_vecPlaneNormal))
		return true;

	if (pTriangleMeshNode->Child[1] != NULL
		&& TestIntersection(pTriangleMeshNode->Child[1], p_vecPlanePoint, p_vecPlaneNormal))
		return true;

	return false;
}

void TriangleMeshToHalfspaceCollisionDetector::ComputeContactManifold(
	const TriangleMeshNode* pTriangleMeshNode,
	const Meson::Common::Maths::TVector3<Real>& p_vecPlanePoint,
	const Meson::Common::Maths::TVector3<Real>& p_vecPlaneNormal,
	ContactManifold& p_contactManifold) const
{
	static TInterval<Real> itvVolumeProjection;
	pTriangleMeshNode->BoundingVolume.ProjectToInterval(p_vecPlaneNormal, itvVolumeProjection);

	Real rPlaneProjection = p_vecPlanePoint * p_vecPlaneNormal;

	if (!itvVolumeProjection.Contains(rPlaneProjection))
		return;

	if (pTriangleMeshNode->Triangles.Size() > 0)
	{
		Triangle triangle;
		TInterval<Real> itvTriangleProjection;
		for (size_t unIndex = 0; unIndex < pTriangleMeshNode->Triangles.Size(); unIndex++)
		{
			IndexedTriangle& indexedTriangle = pTriangleMeshNode->Triangles[unIndex];
			triangle.Vertices[0] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[0]];
			triangle.Vertices[1] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[1]];
			triangle.Vertices[2] = (*pTriangleMeshNode->Vertices)[indexedTriangle.VertexIndices[2]];

			for (size_t unVertexIndex = 0; unVertexIndex < 3; unVertexIndex++)
			{
				Real rVertexProjection = triangle.Vertices[unVertexIndex] * p_vecPlaneNormal;
				if (rVertexProjection < rPlaneProjection)
					p_contactManifold.ContactPoints.Add(ContactPoint(
						triangle.Vertices[unVertexIndex],
						-p_vecPlaneNormal,
						rPlaneProjection - rVertexProjection));			
			}
		}
	}

	if (pTriangleMeshNode->Child[0] != NULL)
		ComputeContactManifold(
			pTriangleMeshNode->Child[0], p_vecPlanePoint, p_vecPlaneNormal, p_contactManifold);

	if (pTriangleMeshNode->Child[1] != NULL)
		ComputeContactManifold(
			pTriangleMeshNode->Child[1], p_vecPlanePoint, p_vecPlaneNormal, p_contactManifold);
}

TriangleMeshToHalfspaceCollisionDetector::TriangleMeshToHalfspaceCollisionDetector(void)
	: m_strGeometryType1("TriangleMesh")
	, m_strGeometryType2("Halfspace")
{
}

TriangleMeshToHalfspaceCollisionDetector::~TriangleMeshToHalfspaceCollisionDetector(void)
{
}

const String& TriangleMeshToHalfspaceCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& TriangleMeshToHalfspaceCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool TriangleMeshToHalfspaceCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Halfspace", "Expected halfspace geometry.")

	const TriangleMesh& triangleMesh = (TriangleMesh &) p_geometry1;

	if (triangleMesh.Root == NULL)
		return false;
	else
	{
		// transform plane point and normal
		const TVector3<Real>& vecPlanePoint = p_trnRelativePlacement.Translation;
		TVector3<Real> vecPlaneNormal
			= (p_trnRelativePlacement.Rotation * TVector3<Real>::Up * p_trnRelativePlacement.Rotation.ConjugateCopy()).Vector;

		return TestIntersection(triangleMesh.Root, vecPlanePoint, vecPlaneNormal);
	}
}

bool TriangleMeshToHalfspaceCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for triangle mesh and halfspace not implemented yet.",
		__FILE__, __LINE__);
}

void TriangleMeshToHalfspaceCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Halfspace", "Expected halfspace geometry.")

	const TriangleMesh& triangleMesh = (TriangleMesh &) p_geometry1;

	p_contactManifold.ContactPoints.Clear();

	if (triangleMesh.Root == NULL)
		return;
	else
	{
		// transform plane point and normal
		const TVector3<Real>& vecPlanePoint = p_trnRelativePlacement.Translation;
		TVector3<Real> vecPlaneNormal
			= (p_trnRelativePlacement.Rotation * TVector3<Real>::Up
				* p_trnRelativePlacement.Rotation.ConjugateCopy()).Vector;

		ComputeContactManifold(triangleMesh.Root, vecPlanePoint, vecPlaneNormal, p_contactManifold);

		// reduce excessive contacts
		RandomGeneric& randomGeneric = GravitasEngine::GetInstance()->Random();
		while (p_contactManifold.ContactPoints.Size() > 8)
		{
			p_contactManifold.ContactPoints.RemoveAt(
				(size_t) randomGeneric.Next((uint) p_contactManifold.ContactPoints.Size()));
		}
	}
}
