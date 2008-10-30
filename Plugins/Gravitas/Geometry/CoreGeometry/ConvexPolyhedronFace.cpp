#include "ConvexPolyhedronFace.h"

#include "MesonException.h"
#include "DebugSupport.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

ConvexPolyhedronFace::ConvexPolyhedronFace(void)
	: Vertices(NULL)
	, VertexIndices()
	, Normal(TVector3<Real>::Up)
{
}

ConvexPolyhedronFace::ConvexPolyhedronFace(VectorList* p_pVertices)
	: Vertices(p_pVertices)
	, VertexIndices()
	, Normal(TVector3<Real>::Up)
{
}

ConvexPolyhedronFace::ConvexPolyhedronFace(const ConvexPolyhedronFace& p_convexPolyhedronFace)
	: Vertices(p_convexPolyhedronFace.Vertices)
	, VertexIndices(p_convexPolyhedronFace.VertexIndices)
	, Normal(p_convexPolyhedronFace.Normal)
{
}

ConvexPolyhedronFace::~ConvexPolyhedronFace(void)
{
}

ConvexPolyhedronFace& ConvexPolyhedronFace::operator=(const ConvexPolyhedronFace& p_convexPolyhedronFace)
{
	Vertices = p_convexPolyhedronFace.Vertices;
	VertexIndices = p_convexPolyhedronFace.VertexIndices;
	Normal = p_convexPolyhedronFace.Normal;
	return *this;
}

bool ConvexPolyhedronFace::operator==(const ConvexPolyhedronFace& p_convexPolyhedronFace) const
{
	return Vertices == p_convexPolyhedronFace.Vertices
		&& VertexIndices == p_convexPolyhedronFace.VertexIndices;
}

Real ConvexPolyhedronFace::ComputeArea(void) const
{
	if (Vertices == NULL)
		throw new MesonException("Vertices reference not set.",
			__FILE__, __LINE__);

	size_t unCount(VertexIndices.Size());

	if (unCount < 3)
		return (Real) 0.0;

	Real rDoubleArea((Real) 0.0);
	const TVector3<Real>& vecFirst = (*Vertices)[VertexIndices[0]];
	for (size_t unIndex1 = 1, unIndex2 = 2; unIndex1 < unCount - 1; unIndex1++, unIndex2++)
	{
		TVector3<Real> vecOffset01((*Vertices)[VertexIndices[unIndex1]] - vecFirst);
		TVector3<Real> vecOffset02((*Vertices)[VertexIndices[unIndex2]] - vecFirst);
		rDoubleArea += TMaths<Real>::Sqrt((vecOffset01 ^ vecOffset02).Length());
	}

	return rDoubleArea * (Real) 0.5;
}

Real ConvexPolyhedronFace::ComputeSignedVolume(
	const TVector3<Real>& p_vecOrigin) const
{
	if (Vertices == NULL)
		throw new MesonException("Vertices reference not set.",
			__FILE__, __LINE__);

	if (VertexIndices.Size() < 3)
		return (Real) 0.0;

	TVector3<Real>& vecFirst = (*Vertices)[VertexIndices[0]];
	Real rSignedHeight((vecFirst - p_vecOrigin) * Normal);

	// cone-like volume is 1/3 A h
	return ComputeArea() * rSignedHeight * (Real) (1.0 / 3.0);
}

void ConvexPolyhedronFace::UpdateDerivativeData(void)
{
	if (Vertices == NULL)
		throw new MesonException("Vertices reference not set.",
			__FILE__, __LINE__);

	// update normal

	size_t unCount(VertexIndices.Size());
	if (unCount < 3)
		return;

	TVector3<Real>& vecVertex0 = (*Vertices)[VertexIndices[0]];
	TVector3<Real>& vecVertex1 = (*Vertices)[VertexIndices[1]];

	TVector3<Real> vecOffset1(vecVertex1 - vecVertex0);

	// compute most representative normal by using max x product
	size_t unIndex(2);
	Real rMaxSquared((Real) 0.0);
	Normal.MakeZero();
	for(size_t unIndex = 2; unIndex < unCount; unIndex++)
	{
		TVector3<Real>& vecVertex2 = (*Vertices)[VertexIndices[unIndex]];
		TVector3<Real> vecOffset2(vecVertex2 - vecVertex0);
		TVector3<Real> vecNormal(vecOffset2 ^ vecOffset1);

		if (Normal * vecNormal < (Real) 0.0)
			throw new MesonException("Inconsistent face winding.", __FILE__, __LINE__);

		Real rNormalSquared(vecNormal.LengthSquared());
		if (rMaxSquared >= rNormalSquared)
			continue;
		Normal = vecNormal.NormaliseCopy();
		rMaxSquared = rNormalSquared;
	}
}

bool ConvexPolyhedronFace::HasVertexIndex(ushort p_usVertexIndex) const
{
	return VertexIndices.IndexOf(p_usVertexIndex) >= 0;
}

Real ConvexPolyhedronFace::GetSignedDistanceFromPlane(const TVector3<Real>& p_vecPoint) const
{
	MESON_ASSERT(Vertices != NULL, "Vertices reference not set.")

	TVector3<Real>& vecFirst = (*Vertices)[VertexIndices[0]];
	return (p_vecPoint - vecFirst) * Normal;
}

bool ConvexPolyhedronFace::IsPointOnPlane(const TVector3<Real>& p_vecPoint) const
{
	return TMaths<Real>::Equals(GetSignedDistanceFromPlane(p_vecPoint), (Real) 0.0);
}

bool ConvexPolyhedronFace::IsPointAbovePlane(const TVector3<Real>& p_vecPoint) const
{
	Real rSignedDistanceFromPlane = GetSignedDistanceFromPlane(p_vecPoint);
	return rSignedDistanceFromPlane > TMaths<Real>::Epsilon;
}

bool ConvexPolyhedronFace::IsPointBelowPlane(const TVector3<Real>& p_vecPoint) const
{
	return GetSignedDistanceFromPlane(p_vecPoint) < -TMaths<Real>::Epsilon;
}

bool ConvexPolyhedronFace::IsPointProjectableOnFace(const TVector3<Real>& p_vecPoint) const
{
	// test if point interior to face using winding
	size_t unIndex0(VertexIndices.Size() - 1);
	for (size_t unIndex1 = 0; unIndex1 < VertexIndices.Size(); unIndex1++)
	{
		TVector3<Real>& vecVertex0 = (*Vertices)[VertexIndices[unIndex0]];
		TVector3<Real>& vecVertex1 = (*Vertices)[VertexIndices[unIndex1]];
		TVector3<Real> vecPerp((vecVertex0 - p_vecPoint) ^ (vecVertex1 - p_vecPoint));

		if (vecPerp * Normal > (Real) 0.0)
			return false;

		unIndex0 = unIndex1;
	}

	return true;
}

bool ConvexPolyhedronFace::IntersectsRay(const Ray& p_ray) const
{
	static TVector3<Real> s_vecIntersectionPoint;
	return IntersectsRay(p_ray, s_vecIntersectionPoint);
}

bool ConvexPolyhedronFace::IntersectsRay(const Ray& p_ray,
	TVector3<Real>& p_vecIntersectionPoint) const
{
	Real rSignedDistanceFromPlane(GetSignedDistanceFromPlane(p_ray.Source));
	Real rDirectionProjection = Normal * p_ray.Direction;

	bool bRayParallelToFace(TMaths<Real>::Equals(rDirectionProjection, (Real) 0.0));

	// if ray parallel, assume no intersection (even if on plane)
	if (bRayParallelToFace)
		return false;

	Real rRayDistance(-rSignedDistanceFromPlane / rDirectionProjection);

	// no intersection if ray pointing away
	if (rRayDistance < (Real) 0.0)
		return false;

	// compute ray intersection with plane
	TVector3<Real> vecPlaneIntersection(p_ray.Source + p_ray.Direction * rRayDistance);

	// if ray intersection point with plane is not on face, no intersection
	if (!IsPointProjectableOnFace(vecPlaneIntersection))
		return false;

	// otherwise ray intersects face
	p_vecIntersectionPoint = vecPlaneIntersection;
	return true;
}
