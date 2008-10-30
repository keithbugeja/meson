#include "TriangleMeshNode.h"

#include "Maths.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Instrumentation;

bool IndexedTriangle::operator==(const IndexedTriangle& p_indexedTriangle) const
{
	return VertexIndices[0] == p_indexedTriangle.VertexIndices[0]
		&& VertexIndices[1] == p_indexedTriangle.VertexIndices[1]
		&& VertexIndices[2] == p_indexedTriangle.VertexIndices[2];
}

Real IndexedTriangle::GetArea(const VectorList* p_plistVertices) const
{
	TVector3<Real> vecEdge0 = (*p_plistVertices)[VertexIndices[1]] - (*p_plistVertices)[VertexIndices[0]];
	TVector3<Real> vecEdge1 = (*p_plistVertices)[VertexIndices[2]] - (*p_plistVertices)[VertexIndices[0]];
	return (vecEdge0 ^ vecEdge1).Length() * (Real) 0.5;
}

TVector3<Real> IndexedTriangle::GetCentroid(const VectorList* p_plistVertices) const
{
	TVector3<Real> vecCentroid = TVector3<Real>::Zero;
	vecCentroid += (*p_plistVertices)[VertexIndices[0]];
	vecCentroid += (*p_plistVertices)[VertexIndices[1]];
	vecCentroid += (*p_plistVertices)[VertexIndices[2]];
	vecCentroid *= ((Real) 1.0 / (Real) 3.0);
	return vecCentroid;
}

bool IndexedTriangle::Intersects(
	const VectorList* p_plistVertices,
	const BoundingAxisAlignedBox& boundingAxisAlignedBox) const
{
	return boundingAxisAlignedBox.Contains((*p_plistVertices)[VertexIndices[0]])
		|| boundingAxisAlignedBox.Contains((*p_plistVertices)[VertexIndices[1]])
		|| boundingAxisAlignedBox.Contains((*p_plistVertices)[VertexIndices[2]]);
}

bool IndexedTriangle::IntersectsRay(
	const VectorList* p_plistVertices, const Ray& p_ray) const
{
	static TVector3<Real> s_vecIntersectionPoint;
	return IntersectsRay(p_plistVertices, p_ray, s_vecIntersectionPoint);
}

bool IndexedTriangle::IntersectsRay(
	const VectorList* p_plistVertices,
	const Ray& p_ray, TVector3<Real>& p_vecIntersectionPoint) const
{
	TVector3<Real> vecVertices[3];
	vecVertices[0] = (*p_plistVertices)[VertexIndices[0]];
	vecVertices[1] = (*p_plistVertices)[VertexIndices[1]];
	vecVertices[2] = (*p_plistVertices)[VertexIndices[2]];

	Real rSourceSignedPlaneDistance((p_ray.Source - vecVertices[0]) * Normal);
	Real rDirectionProjection = Normal * p_ray.Direction;

	bool bRayParallelToFace(TMaths<Real>::Equals(rDirectionProjection, (Real) 0.0));

	// if ray parallel, assume no intersection (even if on plane)
	if (bRayParallelToFace)
		return false;

	Real rRayDistance(-rSourceSignedPlaneDistance / rDirectionProjection);

	// no intersection if ray pointing away
	if (rRayDistance < (Real) 0.0)
		return false;

	// compute ray intersection with plane
	TVector3<Real> vecPlaneIntersection(p_ray.Source + p_ray.Direction * rRayDistance);

	// if ray intersection point with plane is not on face, no intersection
	TVector3<Real> vecPerp;
	vecPerp = (vecVertices[0] - vecPlaneIntersection) ^ (vecVertices[1] - vecPlaneIntersection);
	if (vecPerp * Normal > (Real) 0.0)
		return false;
	vecPerp = (vecVertices[1] - vecPlaneIntersection) ^ (vecVertices[2] - vecPlaneIntersection);
	if (vecPerp * Normal > (Real) 0.0)
		return false;
	vecPerp = (vecVertices[2] - vecPlaneIntersection) ^ (vecVertices[0] - vecPlaneIntersection);
	if (vecPerp * Normal > (Real) 0.0)
		return false;

	// otherwise ray intersects face
	p_vecIntersectionPoint = vecPlaneIntersection;
	return true;
}

void IndexedTriangle::UpdateNormal(const VectorList* p_plistVertices)
{
	TVector3<Real> vecEdge0 = (*p_plistVertices)[VertexIndices[2]] - (*p_plistVertices)[VertexIndices[0]];
	TVector3<Real> vecEdge1 = (*p_plistVertices)[VertexIndices[1]] - (*p_plistVertices)[VertexIndices[0]];
	Normal = vecEdge0 ^ vecEdge1;
	if (Normal.IsZero())
		Normal = TVector3<Real>::Up;
	else
		Normal.Normalise();
}

TriangleMeshNode::TriangleMeshNode(void)
	: Vertices(NULL)
	, BoundingVolume()
	, Triangles()
{
	Child[0] = Child[1] = NULL;
}

TriangleMeshNode::~TriangleMeshNode(void)
{
	if (Child[0] != NULL)
		delete Child[0];
	if (Child[1] != NULL)
		delete Child[1];
}

size_t TriangleMeshNode::GetTreeDepth(void) const
{
	size_t unDepth0 = Child[0] != NULL ? Child[0]->GetTreeDepth() : 0;
	size_t unDepth1 = Child[1] != NULL ? Child[1]->GetTreeDepth() : 0;
	return 1 + (unDepth0 > unDepth1 ? unDepth0 : unDepth1);
}

size_t TriangleMeshNode::GetNodeCount(void) const
{
	size_t unCount = 1;
	if (Child[0] != NULL)
		unCount += Child[0]->GetNodeCount();
	if (Child[1] != NULL)
		unCount += Child[1]->GetNodeCount();
	return unCount;
}

size_t TriangleMeshNode::GetTriangleCount(void) const
{
	size_t unCount = Triangles.Size();
	if (Child[0] != NULL)
		unCount += Child[0]->GetTriangleCount();
	if (Child[1] != NULL)
		unCount += Child[1]->GetTriangleCount();
	return unCount;
}

Real TriangleMeshNode::GetVolume(void) const
{
	// compute volume as area
	Real rVolume = (Real) 0.0;
	for (size_t unIndex = 0; unIndex < Triangles.Size(); unIndex++)
		rVolume += Triangles[unIndex].GetArea(Vertices);

	if (Child[0] != NULL)
		rVolume += Child[0]->GetVolume();

	if (Child[1] != NULL)
		rVolume += Child[1]->GetVolume();

	return rVolume;
}

Real TriangleMeshNode::GetMomentOfInertia(const TVector3<Real>& p_vecAxis) const
{
	Real rMomentOfInertia = (Real) 0.0;

	for (size_t unIndex = 0; unIndex < Triangles.Size(); unIndex++)
	{
		Real rOffset = Triangles[unIndex].GetCentroid(Vertices) * p_vecAxis;
		rMomentOfInertia += Triangles[unIndex].GetArea(Vertices) * rOffset * rOffset;
	}

	if (Child[0] != NULL)
		rMomentOfInertia += Child[0]->GetMomentOfInertia(p_vecAxis);

	if (Child[1] != NULL)
		rMomentOfInertia += Child[1]->GetMomentOfInertia(p_vecAxis);

	return rMomentOfInertia;
}

TVector3<Real> TriangleMeshNode::GetCentroid(void) const
{
	TVector3<Real> vecCentroid = TVector3<Real>::Zero;
	Real rVolume = (Real) 0.0;

	for (size_t unIndex = 0; unIndex < Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle =  Triangles[unIndex];
		rVolume += indexedTriangle.GetArea(Vertices);
		vecCentroid += indexedTriangle.GetCentroid(Vertices) * indexedTriangle.GetArea(Vertices);
	}

	if (Child[0] != NULL)
	{
		rVolume += Child[0]->GetVolume();
		vecCentroid += Child[0]->GetCentroid() * Child[0]->GetVolume();
	}

	if (Child[1] != NULL)
	{
		rVolume += Child[1]->GetVolume();
		vecCentroid += Child[1]->GetCentroid() * Child[1]->GetVolume();
	}

	if (TMaths<Real>::Equals(rVolume, (Real) 0.0))
		return vecCentroid;

	vecCentroid *= ((Real) 1.0 / rVolume);

	return vecCentroid;
}

bool TriangleMeshNode::IntersectsRay(const Ray& p_ray) const
{
	// test with bounding volume first
	if (!BoundingVolume.Intersects(p_ray))
		return false;

	// test contained triangles
	for (size_t unIndex = 0; unIndex < Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = Triangles[unIndex];
		if (indexedTriangle.IntersectsRay(Vertices, p_ray))
			return true;
	}

	// test negative child if available
	if (Child[0] != NULL && Child[0]->IntersectsRay(p_ray))
		return true;

	// test positive child if available
	if (Child[1] != NULL && Child[1]->IntersectsRay(p_ray))
		return true;

	// no intersection if this point reached
	return false;
}

bool TriangleMeshNode::IntersectsRay(const Ray& p_ray,
	TVector3<Real>& p_vecIntersectionPoint) const
{
	// test with bounding volume first
	if (!BoundingVolume.Intersects(p_ray))
		return false;

	// prepare variables for closest intersection point
	// due to potentially concave nature of geometry
	Real rClosestSquared = TMaths<Real>::Maximum;
	TVector3<Real> vecClosestIntersectionPoint;

	// test contained triangles
	for (size_t unIndex = 0; unIndex < Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = Triangles[unIndex];
		if (!indexedTriangle.IntersectsRay(Vertices, p_ray, p_vecIntersectionPoint))
			continue;

		// keep closest intersection
		Real rDistanceSquared = (p_vecIntersectionPoint - p_ray.Source).LengthSquared();
		if (rClosestSquared > rDistanceSquared)
		{
			vecClosestIntersectionPoint = p_vecIntersectionPoint;
			rClosestSquared = rDistanceSquared;
		}
	}

	// test negative child if available
	if (Child[0] != NULL
		&& Child[0]->IntersectsRay(p_ray, p_vecIntersectionPoint))
	{
		// keep closest intersection
		Real rDistanceSquared = (p_vecIntersectionPoint - p_ray.Source).LengthSquared();
		if (rClosestSquared > rDistanceSquared)
		{
			vecClosestIntersectionPoint = p_vecIntersectionPoint;
			rClosestSquared = rDistanceSquared;
		}
	}

	// test positive child if available
	if (Child[1] != NULL
		&& Child[1]->IntersectsRay(p_ray, p_vecIntersectionPoint))
	{
		// keep closest intersection
		Real rDistanceSquared = (p_vecIntersectionPoint - p_ray.Source).LengthSquared();
		if (rClosestSquared > rDistanceSquared)
		{
			vecClosestIntersectionPoint = p_vecIntersectionPoint;
			rClosestSquared = rDistanceSquared;
		}
	}

	// intersection if at least one point found
	if (rClosestSquared < TMaths<Real>::Maximum)
	{
		p_vecIntersectionPoint = vecClosestIntersectionPoint;
		return true;
	}
	else
		return false;
}

void TriangleMeshNode::RenderInstrumentation(
	IInstrumentationDevice* p_pInstrumentationDevice)
{
	p_pInstrumentationDevice->DrawBox(BoundingVolume.Centre(), BoundingVolume.Extent());

	if (Child[0] != NULL)
		Child[0]->RenderInstrumentation(p_pInstrumentationDevice);
	if (Child[1] != NULL)
		Child[1]->RenderInstrumentation(p_pInstrumentationDevice);
}
