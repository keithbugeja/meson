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

Real IndexedTriangle::GetArea(const PointList* p_plistVertices) const
{
	TVector3<Real> vecEdge0 = (*p_plistVertices)[VertexIndices[1]] - (*p_plistVertices)[VertexIndices[0]];
	TVector3<Real> vecEdge1 = (*p_plistVertices)[VertexIndices[2]] - (*p_plistVertices)[VertexIndices[0]];
	return (vecEdge0 ^ vecEdge1).Length() * (Real) 0.5;
}

TPoint3<Real> IndexedTriangle::GetCentroid(const PointList* p_plistVertices) const
{
	TPoint3<Real> ptCentroid = TPoint3<Real>::Origin;
	ptCentroid += (*p_plistVertices)[VertexIndices[0]].ToVector();
	ptCentroid += (*p_plistVertices)[VertexIndices[1]].ToVector();
	ptCentroid += (*p_plistVertices)[VertexIndices[2]].ToVector();
	ptCentroid.ToVector() *= (Real) 1.0 / (Real) 3.0;
	return ptCentroid;
}

bool IndexedTriangle::Intersects(
	const PointList* p_plistVertices,
	const BoundingAxisAlignedBox& boundingAxisAlignedBox) const
{
	return boundingAxisAlignedBox.Contains((*p_plistVertices)[VertexIndices[0]])
		|| boundingAxisAlignedBox.Contains((*p_plistVertices)[VertexIndices[1]])
		|| boundingAxisAlignedBox.Contains((*p_plistVertices)[VertexIndices[2]]);
}

bool IndexedTriangle::IntersectsRay(
	const PointList* p_plistVertices, const Ray& p_ray) const
{
	static TPoint3<Real> s_ptIntersectionPoint;
	return IntersectsRay(p_plistVertices, p_ray, s_ptIntersectionPoint);
}

bool IndexedTriangle::IntersectsRay(
	const PointList* p_plistVertices,
	const Ray& p_ray, TPoint3<Real>& p_ptIntersectionPoint) const
{
	TPoint3<Real> ptVertices[3];
	ptVertices[0] = (*p_plistVertices)[VertexIndices[0]];
	ptVertices[1] = (*p_plistVertices)[VertexIndices[1]];
	ptVertices[2] = (*p_plistVertices)[VertexIndices[2]];

	Real rSourceSignedPlaneDistance((p_ray.Source - ptVertices[0]) * Normal);
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
	TPoint3<Real> ptPlaneIntersection(p_ray.Source + p_ray.Direction * rRayDistance);

	// if ray intersection point with plane is not on face, no intersection
	TVector3<Real> vecPerp;
	vecPerp = (ptVertices[0] - ptPlaneIntersection) ^ (ptVertices[1] - ptPlaneIntersection);
	if (vecPerp * Normal > (Real) 0.0)
		return false;
	vecPerp = (ptVertices[1] - ptPlaneIntersection) ^ (ptVertices[2] - ptPlaneIntersection);
	if (vecPerp * Normal > (Real) 0.0)
		return false;
	vecPerp = (ptVertices[2] - ptPlaneIntersection) ^ (ptVertices[0] - ptPlaneIntersection);
	if (vecPerp * Normal > (Real) 0.0)
		return false;

	// otherwise ray intersects face
	p_ptIntersectionPoint = ptPlaneIntersection;
	return true;
}

void IndexedTriangle::UpdateNormal(const PointList* p_plistVertices)
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
		Real rOffset = Triangles[unIndex].GetCentroid(Vertices).ToVector() * p_vecAxis;
		rMomentOfInertia += Triangles[unIndex].GetArea(Vertices) * rOffset * rOffset;
	}

	if (Child[0] != NULL)
		rMomentOfInertia += Child[0]->GetMomentOfInertia(p_vecAxis);

	if (Child[1] != NULL)
		rMomentOfInertia += Child[1]->GetMomentOfInertia(p_vecAxis);

	return rMomentOfInertia;
}

TPoint3<Real> TriangleMeshNode::GetCentroid(void) const
{
	TPoint3<Real> ptCentroid = TPoint3<Real>::Origin;
	Real rVolume = (Real) 0.0;

	for (size_t unIndex = 0; unIndex < Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle =  Triangles[unIndex];
		rVolume += indexedTriangle.GetArea(Vertices);
		ptCentroid += indexedTriangle.GetCentroid(Vertices).ToVector() * indexedTriangle.GetArea(Vertices);
	}

	if (Child[0] != NULL)
	{
		rVolume += Child[0]->GetVolume();
		ptCentroid += Child[0]->GetCentroid().ToVector() * Child[0]->GetVolume();
	}

	if (Child[1] != NULL)
	{
		rVolume += Child[1]->GetVolume();
		ptCentroid += Child[1]->GetCentroid().ToVector() * Child[1]->GetVolume();
	}

	if (TMaths<Real>::Equals(rVolume, (Real) 0.0))
		return ptCentroid;

	ptCentroid.ToVector() *= (Real) 1.0 / rVolume;

	return ptCentroid;
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
	TPoint3<Real>& p_ptIntersectionPoint) const
{
	// test with bounding volume first
	if (!BoundingVolume.Intersects(p_ray))
		return false;

	// prepare variables for closest intersection point
	// due to potentially concave nature of geometry
	Real rClosestSquared = TMaths<Real>::Maximum;
	TPoint3<Real> ptClosestIntersectionPoint;

	// test contained triangles
	for (size_t unIndex = 0; unIndex < Triangles.Size(); unIndex++)
	{
		IndexedTriangle& indexedTriangle = Triangles[unIndex];
		if (!indexedTriangle.IntersectsRay(Vertices, p_ray, p_ptIntersectionPoint))
			continue;

		// keep closest intersection
		Real rDistanceSquared = (p_ptIntersectionPoint - p_ray.Source).LengthSquared();
		if (rClosestSquared > rDistanceSquared)
		{
			ptClosestIntersectionPoint = p_ptIntersectionPoint;
			rClosestSquared = rDistanceSquared;
		}
	}

	// test negative child if available
	if (Child[0] != NULL
		&& Child[0]->IntersectsRay(p_ray, p_ptIntersectionPoint))
	{
		// keep closest intersection
		Real rDistanceSquared = (p_ptIntersectionPoint - p_ray.Source).LengthSquared();
		if (rClosestSquared > rDistanceSquared)
		{
			ptClosestIntersectionPoint = p_ptIntersectionPoint;
			rClosestSquared = rDistanceSquared;
		}
	}

	// test positive child if available
	if (Child[1] != NULL
		&& Child[1]->IntersectsRay(p_ray, p_ptIntersectionPoint))
	{
		// keep closest intersection
		Real rDistanceSquared = (p_ptIntersectionPoint - p_ray.Source).LengthSquared();
		if (rClosestSquared > rDistanceSquared)
		{
			ptClosestIntersectionPoint = p_ptIntersectionPoint;
			rClosestSquared = rDistanceSquared;
		}
	}

	// intersection if at least one point found
	if (rClosestSquared < TMaths<Real>::Maximum)
	{
		p_ptIntersectionPoint = ptClosestIntersectionPoint;
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
