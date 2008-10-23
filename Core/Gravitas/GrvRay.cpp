#include "GrvRay.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

Ray::Ray(void)
	: Source(TPoint3<Real>::Origin)
	, Direction(TVector3<Real>::Right)
{
}

Ray::Ray(const Ray& p_ray)
	: Source(p_ray.Source)
	, Direction(p_ray.Direction)
{
}

Ray::Ray(const TPoint3<Real>& p_ptSource, const TVector3<Real>& p_vecDirection)
	: Source(p_ptSource)
	, Direction(p_vecDirection)
{
}

Ray::Ray(const TPoint3<Real>& p_ptSource, const TPoint3<Real>& p_ptPassThrough)
	: Source(p_ptSource)
	, Direction(p_ptPassThrough - p_ptSource)
{
	if (Direction.IsZero())
		Direction = TVector3<Real>::Right;
	else
		Direction.Normalise();
}

Ray::~Ray(void)
{
}

void Ray::OrientTo(const Meson::Common::Maths::TPoint3<Real>& p_ptPassThrough)
{
	Direction = p_ptPassThrough - Source;
	if (Direction.IsZero())
		Direction = TVector3<Real>::Right;
	else
		Direction.Normalise();
}

void Ray::Transform(const TMatrix3<Real>& p_matrix)
{
	Source = TPoint3<Real>::Origin + p_matrix * Source.ToVector();
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
		TPoint3<Real>::Origin + p_matrix * Source.ToVector(),
		p_matrix * Direction);
}
TPoint3<Real> Ray::ClosestPointTo(const TPoint3<Real>& p_ptPoint) const
{
	Real rRayDistance((p_ptPoint - Source) * Direction);
	if (rRayDistance < (Real) 0.0)
		return Source;
	else
		return Source + Direction * rRayDistance;
}

Real Ray::DistanceTo(const TPoint3<Real>& p_ptPoint) const
{
	Real rRayDistance((p_ptPoint - Source) * Direction);
	TPoint3<Real> ptClosesPoint(rRayDistance < (Real) 0.0
		? Source
		: Source + Direction * rRayDistance);
	return (ptClosesPoint - p_ptPoint).Length();
}

Real Ray::DistanceSquaredTo(const TPoint3<Real>& p_ptPoint) const
{
	Real rRayDistance((p_ptPoint - Source) * Direction);
	TPoint3<Real> ptClosesPoint(rRayDistance < (Real) 0.0
		? Source
		: Source + Direction * rRayDistance);
	return (ptClosesPoint - p_ptPoint).LengthSquared();
}

TPoint3<Real> Ray::ComputePosition(Real p_rDistance) const
{
	return Source + Direction * p_rDistance;
}
