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

ConvexPolyhedronFace::ConvexPolyhedronFace(PointList* p_pVertices)
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
	const TPoint3<Real>& ptFirst = (*Vertices)[VertexIndices[0]];
	for (size_t unIndex1 = 1, unIndex2 = 2; unIndex1 < unCount - 1; unIndex1++, unIndex2++)
	{
		TVector3<Real> vecOffset01((*Vertices)[VertexIndices[unIndex1]] - ptFirst);
		TVector3<Real> vecOffset02((*Vertices)[VertexIndices[unIndex2]] - ptFirst);
		rDoubleArea += TMaths<Real>::Sqrt((vecOffset01 ^ vecOffset02).Length());
	}

	return rDoubleArea * (Real) 0.5;
}

Real ConvexPolyhedronFace::ComputeSignedVolume(
	const TPoint3<Real>& p_ptOrigin) const
{
	if (Vertices == NULL)
		throw new MesonException("Vertices reference not set.",
			__FILE__, __LINE__);

	if (VertexIndices.Size() < 3)
		return (Real) 0.0;

	TPoint3<Real>& ptFirst = (*Vertices)[VertexIndices[0]];
	Real rSignedHeight((ptFirst - p_ptOrigin) * Normal);

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

	TPoint3<Real>& ptVertex0 = (*Vertices)[VertexIndices[0]];
	TPoint3<Real>& ptVertex1 = (*Vertices)[VertexIndices[1]];

	TVector3<Real> vecOffset1(ptVertex1 - ptVertex0);

	// compute most representative normal by using max x product
	size_t unIndex(2);
	Real rMaxSquared((Real) 0.0);
	Normal.MakeZero();
	for(size_t unIndex = 2; unIndex < unCount; unIndex++)
	{
		TPoint3<Real>& ptVertex2 = (*Vertices)[VertexIndices[unIndex]];
		TVector3<Real> vecOffset2(ptVertex2 - ptVertex0);
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

Real ConvexPolyhedronFace::GetSignedDistanceFromPlane(const TPoint3<Real>& ptPoint) const
{
	MESON_ASSERT(Vertices != NULL, "Vertices reference not set.")

	TPoint3<Real>& ptFirst = (*Vertices)[VertexIndices[0]];
	return (ptPoint - ptFirst) * Normal;
}

bool ConvexPolyhedronFace::IsPointOnPlane(const TPoint3<Real>& ptPoint) const
{
	return TMaths<Real>::Equals(GetSignedDistanceFromPlane(ptPoint), (Real) 0.0);
}

bool ConvexPolyhedronFace::IsPointAbovePlane(const TPoint3<Real>& ptPoint) const
{
	Real rSignedDistanceFromPlane = GetSignedDistanceFromPlane(ptPoint);
	return rSignedDistanceFromPlane > TMaths<Real>::Epsilon;
}

bool ConvexPolyhedronFace::IsPointBelowPlane(const TPoint3<Real>& ptPoint) const
{
	return GetSignedDistanceFromPlane(ptPoint) < -TMaths<Real>::Epsilon;
}

bool ConvexPolyhedronFace::IsPointProjectableOnFace(const TPoint3<Real>& ptPoint) const
{
	// test if point interior to face using winding
	size_t unIndex0(VertexIndices.Size() - 1);
	for (size_t unIndex1 = 0; unIndex1 < VertexIndices.Size(); unIndex1++)
	{
		TPoint3<Real>& ptVertex0 = (*Vertices)[VertexIndices[unIndex0]];
		TPoint3<Real>& ptVertex1 = (*Vertices)[VertexIndices[unIndex1]];
		TVector3<Real> vecPerp((ptVertex0 - ptPoint) ^ (ptVertex1 - ptPoint));

		if (vecPerp * Normal > (Real) 0.0)
			return false;

		unIndex0 = unIndex1;
	}

	return true;
}

bool ConvexPolyhedronFace::IntersectsRay(const Ray& p_ray) const
{
	static TPoint3<Real> s_ptIntersectionPoint;
	return IntersectsRay(p_ray, s_ptIntersectionPoint);
}

bool ConvexPolyhedronFace::IntersectsRay(const Ray& p_ray,
	TPoint3<Real>& p_ptIntersectionPoint) const
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
	TPoint3<Real> ptPlaneIntersection(p_ray.Source + p_ray.Direction * rRayDistance);

	// if ray intersection point with plane is not on face, no intersection
	if (!IsPointProjectableOnFace(ptPlaneIntersection))
		return false;

	// otherwise ray intersects face
	p_ptIntersectionPoint = ptPlaneIntersection;
	return true;
}
