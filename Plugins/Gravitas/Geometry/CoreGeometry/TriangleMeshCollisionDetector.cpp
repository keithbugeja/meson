#include "TriangleMeshCollisionDetector.h"

#include "ArrayList.h"

#include "GrvTriangle.h"
#include "GrvGravitasEngine.h"

#include "TriangleMesh.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Random;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

bool TriangleMeshCollisionDetector::TestIntersection(
	const TriangleMeshNode* pTriangleMeshNode1,
	const TriangleMeshNode* pTriangleMeshNode2,
	const Transform& p_trnRelativePlacement) const
{
	BoundingAxisAlignedBox boundingVolumeRelative2(pTriangleMeshNode2->BoundingVolume);
	boundingVolumeRelative2.Transform(p_trnRelativePlacement);

	// early out if the BV of the nodes do not intersect
	if (!pTriangleMeshNode1->BoundingVolume.Intersects(boundingVolumeRelative2))
		return false;

	if (pTriangleMeshNode1->Triangles.Size() == 0)
	{
		// 
		if (pTriangleMeshNode2->Triangles.Size() == 0)
		{
			// both n1 and n2 both have children
			return TestIntersection(pTriangleMeshNode1->Child[0], pTriangleMeshNode2->Child[0], p_trnRelativePlacement)
				|| TestIntersection(pTriangleMeshNode1->Child[0], pTriangleMeshNode2->Child[1], p_trnRelativePlacement)
				|| TestIntersection(pTriangleMeshNode1->Child[1], pTriangleMeshNode2->Child[0], p_trnRelativePlacement)
				|| TestIntersection(pTriangleMeshNode1->Child[1], pTriangleMeshNode2->Child[1], p_trnRelativePlacement);
		}
		else
		{
			// n1 has children, n2 is leaf
			return TestIntersection(pTriangleMeshNode1->Child[0], pTriangleMeshNode2, p_trnRelativePlacement)
				|| TestIntersection(pTriangleMeshNode1->Child[1], pTriangleMeshNode2, p_trnRelativePlacement);
		}
	}
	else
	{
		if (pTriangleMeshNode2->Triangles.Size() == 0)
		{
			// n1 is leaf, n2 has children
			return TestIntersection(pTriangleMeshNode1, pTriangleMeshNode2->Child[0], p_trnRelativePlacement)
				|| TestIntersection(pTriangleMeshNode1, pTriangleMeshNode2->Child[1], p_trnRelativePlacement);
		}
		else
		{
			// both n1 and n2 are leaves

			for (size_t unTriangle2 = 0; unTriangle2 < pTriangleMeshNode2->Triangles.Size(); unTriangle2++)
			{
				IndexedTriangle& indexedTriangle2 = pTriangleMeshNode2->Triangles[unTriangle2];
				Triangle triangle2(
					pTriangleMeshNode2->Vertices->GetElement(indexedTriangle2.VertexIndices[0]),
					pTriangleMeshNode2->Vertices->GetElement(indexedTriangle2.VertexIndices[1]),
					pTriangleMeshNode2->Vertices->GetElement(indexedTriangle2.VertexIndices[2]));

				triangle2.Transform(p_trnRelativePlacement);

				for (size_t unTriangle1 = 0; unTriangle1 < pTriangleMeshNode1->Triangles.Size(); unTriangle1++)
				{
					IndexedTriangle& indexedTriangle1 = pTriangleMeshNode1->Triangles[unTriangle1];
					Triangle triangle1(
						pTriangleMeshNode1->Vertices->GetElement(indexedTriangle1.VertexIndices[0]),
						pTriangleMeshNode1->Vertices->GetElement(indexedTriangle1.VertexIndices[1]),
						pTriangleMeshNode1->Vertices->GetElement(indexedTriangle1.VertexIndices[2]));

					if (triangle1.Intersects(triangle2))
						return true;
				}
			}

			// no contacts therefore false
			return false;
		}
	}
}

void TriangleMeshCollisionDetector::ComputeContactManifold(
	const TriangleMeshNode* pTriangleMeshNode1,
	const TriangleMeshNode* pTriangleMeshNode2,
	const Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold) const
{
	BoundingAxisAlignedBox boundingVolumeRelative2(pTriangleMeshNode2->BoundingVolume);
	boundingVolumeRelative2.Transform(p_trnRelativePlacement);

	// early out if the BV of the nodes do not intersect
	if (!pTriangleMeshNode1->BoundingVolume.Intersects(boundingVolumeRelative2))
		return;

	TInterval<Real> itvProjection1, itvProjection2, itvSpan1, itvSpan2;
	TVector3<Real> vecNormal1, vecNormal2;
	ContactPoint contactPoint;

	if (pTriangleMeshNode1->Triangles.Size() == 0)
	{
		// 
		if (pTriangleMeshNode2->Triangles.Size() == 0)
		{
			// both n1 and n2 both have children
			ComputeContactManifold(pTriangleMeshNode1->Child[0], pTriangleMeshNode2->Child[0],
				p_trnRelativePlacement, p_contactManifold);
			ComputeContactManifold(pTriangleMeshNode1->Child[0], pTriangleMeshNode2->Child[1],
				p_trnRelativePlacement, p_contactManifold);
			ComputeContactManifold(pTriangleMeshNode1->Child[1], pTriangleMeshNode2->Child[0],
				p_trnRelativePlacement, p_contactManifold);
			ComputeContactManifold(pTriangleMeshNode1->Child[1], pTriangleMeshNode2->Child[1],
				p_trnRelativePlacement, p_contactManifold);
		}
		else
		{
			// n1 has children, n2 is leaf
			ComputeContactManifold(pTriangleMeshNode1->Child[0], pTriangleMeshNode2,
				p_trnRelativePlacement, p_contactManifold);
			ComputeContactManifold(pTriangleMeshNode1->Child[1], pTriangleMeshNode2,
				p_trnRelativePlacement, p_contactManifold);
		}
	}
	else
	{
		if (pTriangleMeshNode2->Triangles.Size() == 0)
		{
			// n1 is leaf, n2 has children
			ComputeContactManifold(pTriangleMeshNode1, pTriangleMeshNode2->Child[0],
				p_trnRelativePlacement, p_contactManifold);
			ComputeContactManifold(pTriangleMeshNode1, pTriangleMeshNode2->Child[1],
				p_trnRelativePlacement, p_contactManifold);
		}
		else
		{
			// both n1 and n2 are leaves

			for (size_t unTriangle2 = 0; unTriangle2 < pTriangleMeshNode2->Triangles.Size(); unTriangle2++)
			{
				IndexedTriangle& indexedTriangle2 = pTriangleMeshNode2->Triangles[unTriangle2];
				Triangle triangle2(
					pTriangleMeshNode2->Vertices->GetElement(indexedTriangle2.VertexIndices[0]),
					pTriangleMeshNode2->Vertices->GetElement(indexedTriangle2.VertexIndices[1]),
					pTriangleMeshNode2->Vertices->GetElement(indexedTriangle2.VertexIndices[2]));

				triangle2.Transform(p_trnRelativePlacement);
				vecNormal2 = triangle2.Normal();

				for (size_t unTriangle1 = 0; unTriangle1 < pTriangleMeshNode1->Triangles.Size(); unTriangle1++)
				{
					IndexedTriangle& indexedTriangle1 = pTriangleMeshNode1->Triangles[unTriangle1];
					Triangle triangle1(
						pTriangleMeshNode1->Vertices->GetElement(indexedTriangle1.VertexIndices[0]),
						pTriangleMeshNode1->Vertices->GetElement(indexedTriangle1.VertexIndices[1]),
						pTriangleMeshNode1->Vertices->GetElement(indexedTriangle1.VertexIndices[2]));


					if (!triangle1.Intersects(triangle2))
						continue;

					vecNormal1 = triangle1.Normal();

					if (vecNormal1 * vecNormal2 > 0.0)
						return;

					contactPoint.Penetration = TMaths<Real>::Maximum;
					for (size_t unVertex = 0; unVertex < 3; unVertex++)
					{
						const TVector3<Real>& vecVertex2 = triangle2.Vertices[unVertex];
						Real rSignedDistange = triangle1.SignedDistanceFromPlane(vecVertex2);
						if (rSignedDistange <= (Real) 0.0)
						{
							if (contactPoint.Penetration > -rSignedDistange)
							{
								contactPoint.Penetration = -rSignedDistange;
								contactPoint.Normal = vecNormal1;
								contactPoint.Position = vecVertex2;
							}
						}

						const TVector3<Real>& vecVertex1 = triangle1.Vertices[unVertex];
						rSignedDistange = triangle2.SignedDistanceFromPlane(vecVertex1);
						if (rSignedDistange <= (Real) 0.0)
						{
							if (contactPoint.Penetration > -rSignedDistange)
							{
								contactPoint.Penetration = -rSignedDistange;
								contactPoint.Normal = -vecNormal2;
								contactPoint.Position = vecVertex1;
							}
						}
					}

					p_contactManifold.ContactPoints.Add(contactPoint);
				}
			}
		}
	}
}

TriangleMeshCollisionDetector::TriangleMeshCollisionDetector(void)
	: m_strGeometryType("TriangleMesh")
{
}

TriangleMeshCollisionDetector::~TriangleMeshCollisionDetector(void)
{
}

const String& TriangleMeshCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType;
}

const String& TriangleMeshCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType;
}

bool TriangleMeshCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")

	const TriangleMesh& triangleMesh1 = (TriangleMesh &) p_geometry1;
	const TriangleMesh& triangleMesh2 = (TriangleMesh &) p_geometry2;

	return TestIntersection(triangleMesh1.Root, triangleMesh2.Root, p_trnRelativePlacement);
}

bool TriangleMeshCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for triangle meshes not implemented yet.",
		__FILE__, __LINE__);
}

void TriangleMeshCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "TriangleMesh", "Expected triangle mesh geometry.")

	const TriangleMesh& triangleMesh1 = (TriangleMesh &) p_geometry1;
	const TriangleMesh& triangleMesh2 = (TriangleMesh &) p_geometry2;

	p_contactManifold.ContactPoints.Clear();
	ComputeContactManifold(triangleMesh1.Root, triangleMesh2.Root,
		p_trnRelativePlacement, p_contactManifold);

	// reduce excessive contacts
	RandomGeneric& randomGeneric = GravitasEngine::GetInstance()->Random();
	while (p_contactManifold.ContactPoints.Size() > 8)
	{
		p_contactManifold.ContactPoints.RemoveAt(
			(size_t) randomGeneric.Next((uint) p_contactManifold.ContactPoints.Size()));
	}
}
