#include "GrvRay.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

Ray::Ray(void)
	: Source(TVector3<Real>::Zero)
	, Direction(TVector3<Real>::Right)
{
}

Ray::Ray(const Ray& p_ray)
	: Source(p_ray.Source)
	, Direction(p_ray.Direction)
{
}

Ray::Ray(const TVector3<Real>& p_vecSource, const TVector3<Real>& p_vecDirection)
	: Source(p_vecSource)
	, Direction(p_vecDirection)
{
}

Ray::~Ray(void)
{
}

void Ray::OrientTo(const Meson::Common::Maths::TVector3<Real>& p_vecPassThrough)
{
	Direction = p_vecPassThrough - Source;
	if (Direction.IsZero())
		Direction = TVector3<Real>::Right;
	else
		Direction.Normalise();
}

void Ray::Transform(const TMatrix3<Real>& p_matrix)
{
	Source = p_matrix * Source;
	Direction = p_matrix * Direction;
}

void Ray::Transform(const Meson::Gravitas::Geometry::Transform& p_transform)
{
	p_transform.Rotation.Transform(Direction);
	p_transform.Apply(Source);
}

Ray Ray::TransformCopy(const Meson::Gravitas::Geometry::Transform& p_transform) const
{
	return Ray(
		p_transform.ApplyCopy(Source),
		p_transform.Rotation.TransformCopy(Direction));
}

Ray Ray::TransformCopy(const TMatrix3<Real>& p_matrix)
{
	return Ray(
		p_matrix * Source,
		p_matrix * Direction);
}

TVector3<Real> Ray::ClosestPointTo(const TVector3<Real>& p_vecPoint) const
{
	Real rRayDistance((p_vecPoint - Source) * Direction);
	if (rRayDistance < (Real) 0.0)
		return Source;
	else
		return Source + Direction * rRayDistance;
}

Real Ray::DistanceTo(const TVector3<Real>& p_vecPoint) const
{
	Real rRayDistance((p_vecPoint - Source) * Direction);
	TVector3<Real> vecClosesPoint(rRayDistance < (Real) 0.0
		? Source
		: Source + Direction * rRayDistance);
	return (vecClosesPoint - p_vecPoint).Length();
}

Real Ray::DistanceSquaredTo(const TVector3<Real>& p_vecPoint) const
{
	Real rRayDistance((p_vecPoint - Source) * Direction);
	TVector3<Real> vecClosesPoint(rRayDistance < (Real) 0.0
		? Source
		: Source + Direction * rRayDistance);
	return (vecClosesPoint - p_vecPoint).LengthSquared();
}

TVector3<Real> Ray::ComputePosition(Real p_rDistance) const
{
	return Source + Direction * p_rDistance;
}
