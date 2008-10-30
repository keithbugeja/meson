//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Geometry::BoundingSphere
//	Bounding sphere implementation.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		15/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------

#include "GrvBoundingSphere.h"

#include "GrvLineSegment.h"

using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

BoundingSphere::BoundingSphere(void)
	: Centre(TVector3<Real>::Zero)
	, Radius((Real) 1.0)
	, RadiusSquared((Real) 1.0)
{
}

BoundingSphere::BoundingSphere(const BoundingSphere& p_boundingSphere)
	: Centre(p_boundingSphere.Centre)
	, Radius(p_boundingSphere.Radius)
	, RadiusSquared(p_boundingSphere.RadiusSquared)
{
}

BoundingSphere::BoundingSphere(const TVector3<Real>& p_vecCentre, Real p_rRadius)
	: Centre(p_vecCentre)
	, Radius(p_rRadius)
	, RadiusSquared(p_rRadius * p_rRadius)
{
}

BoundingSphere::~BoundingSphere(void)
{
}

BoundingVolumeType::BoundingVolumeType BoundingSphere::GetType(void) const
{
	return BoundingVolumeType::Sphere;
}

bool BoundingSphere::Contains(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const
{
	return (p_vecPoint - Centre).LengthSquared() <= RadiusSquared;
}

bool BoundingSphere::Intersects(const Ray& p_ray) const
{
	return p_ray.DistanceSquaredTo(Centre) <= RadiusSquared;
}

bool BoundingSphere::Intersects(const Ray& p_ray,
	Meson::Common::Maths::TVector3<Real>& p_vecIntersectionPoint) const
{
	TVector3<Real> vecClosestPoint(p_ray.ClosestPointTo(Centre));
	Real rDistanceSquared = (vecClosestPoint - Centre).LengthSquared();
	if (rDistanceSquared > RadiusSquared)
		return false;

	Real rClosestToSurface(TMaths<Real>::Sqrt(RadiusSquared - rDistanceSquared));

	p_vecIntersectionPoint = vecClosestPoint - p_ray.Direction * rClosestToSurface;

	return true;
}

bool BoundingSphere::Intersects(const Triangle& p_triangle) const
{
	TVector3<Real> vecClosest = p_triangle.ClosestPoint(Centre);
	TVector3<Real> vecAxis(vecClosest - Centre);
	if (vecAxis.IsZero())
		return true;

	vecAxis.Normalise();
	TInterval<Real> itvSphereProjection, itvTriangleProjection;
	ProjectToInterval(vecAxis, itvSphereProjection);
	p_triangle.ProjectToInterval(vecAxis, itvTriangleProjection);

	return itvSphereProjection.Intersects(itvTriangleProjection);
}

bool BoundingSphere::Intersects(const IBoundingVolume& p_boundingVolume) const
{
	// for volume type other than sphere, let other volume do test
	if (p_boundingVolume.GetType() != BoundingVolumeType::Sphere)
		return p_boundingVolume.Intersects(*this);

	const BoundingSphere &boundingSphere = (BoundingSphere &) p_boundingVolume;
	Real rRadiusSum = Radius + boundingSphere.Radius;
	return (boundingSphere.Centre - Centre).LengthSquared()
		<= rRadiusSum * rRadiusSum;
}

bool BoundingSphere::IntersectsDynamic(const IBoundingVolume &p_boundingVolume,
	const Meson::Common::Maths::TVector3<Real>& p_vecSweep) const
{
	// for volume type other than sphere, let other volume do test
	if (p_boundingVolume.GetType() != BoundingVolumeType::Sphere)
		return p_boundingVolume.IntersectsDynamic(*this, -p_vecSweep);

	const BoundingSphere &boundingSphere = (BoundingSphere &) p_boundingVolume;

	// construct line segment for sphere sweep
	LineSegment lineSegmentSweep
		(boundingSphere.Centre,
		boundingSphere.Centre + p_vecSweep);

	return lineSegmentSweep.DistanceFromPoint(Centre)
		<= Radius + boundingSphere.Radius;
}

BoundingVolumePtr BoundingSphere::Duplicate(void)
{
	return BoundingVolumePtr(new BoundingSphere(*this));
}

void BoundingSphere::AssignFrom(BoundingVolumePtr p_pBoundingVolume)
{
	IBoundingVolume* pBoundingVolume = p_pBoundingVolume;
	MESON_ASSERT(pBoundingVolume->GetType() == BoundingVolumeType::Sphere, "Bounding volume must be a sphere.");
	BoundingSphere* pBoundingSphere = (BoundingSphere*) pBoundingVolume;
	*this = *pBoundingSphere;
}

void BoundingSphere::Transform(
	const Meson::Gravitas::Geometry::Transform& p_transform)
{
	// transform centre
	Centre = p_transform.ApplyCopy(Centre);
}

void BoundingSphere::ClosestPointTo(
	const TVector3<Real>& p_vecPoint, TVector3<Real>& p_vecClosestPoint) const
{
	TVector3<Real> vecOffset = p_vecPoint - Centre;

	if (vecOffset.LengthSquared() < RadiusSquared)
		p_vecClosestPoint = p_vecPoint;
	else
		p_vecClosestPoint = Centre + vecOffset.NormaliseCopy() * Radius;
}

void BoundingSphere::ProjectToInterval(
	const Meson::Common::Maths::TVector3<Real>& p_vecAxis,
	Meson::Common::Maths::TInterval<Real>& p_interval) const
{
	Real rCentre = Centre * p_vecAxis;

	p_interval.Min = rCentre - Radius;
	p_interval.Max = rCentre + Radius;
}

BoundingSphere& BoundingSphere::operator=(const BoundingSphere& p_boundingSphere)
{
	Centre = p_boundingSphere.Centre;
	Radius = p_boundingSphere.Radius;
	RadiusSquared = p_boundingSphere.RadiusSquared;
	return *this;
}
