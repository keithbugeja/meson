//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Geometry::BoundingAxisAlignedBox
//	Axis-aligned bounding box implementation.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		15/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "GrvBoundingAxisAlignedBox.h"

using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

void BoundingAxisAlignedBox::QuickSweep(
	const TVector3<Real> &p_vecSweep,
	BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const
{
	p_boundingAxisAlignedBox = *this;

	if (p_vecSweep.X > (Real) 0.0)
		p_boundingAxisAlignedBox.Max.X += p_vecSweep.X;
	if (p_vecSweep.X < (Real) 0.0)
		p_boundingAxisAlignedBox.Min.X += p_vecSweep.X;

	if (p_vecSweep.Y > (Real) 0.0)
		p_boundingAxisAlignedBox.Max.Y += p_vecSweep.Y;
	if (p_vecSweep.Y < (Real) 0.0)
		p_boundingAxisAlignedBox.Min.Y += p_vecSweep.Y;

	if (p_vecSweep.Z > (Real) 0.0)
		p_boundingAxisAlignedBox.Max.Z += p_vecSweep.Z;
	if (p_vecSweep.Z < (Real) 0.0)
		p_boundingAxisAlignedBox.Min.Z += p_vecSweep.Z;
}

BoundingAxisAlignedBox::BoundingAxisAlignedBox(void)
	: Min(TVector3<Real>::Zero)
	, Max(TVector3<Real>::Zero)
{
}

BoundingAxisAlignedBox::BoundingAxisAlignedBox(
	const BoundingAxisAlignedBox &p_boundingAxisAlignedBox)
	: Min(p_boundingAxisAlignedBox.Min)
	, Max(p_boundingAxisAlignedBox.Max)
{
}

BoundingAxisAlignedBox::BoundingAxisAlignedBox(
	const TVector3<Real> &p_vecMin, const TVector3<Real> &p_vecMax)
	: Min(p_vecMin)
	, Max(p_vecMax)
{
}

BoundingAxisAlignedBox::~BoundingAxisAlignedBox(void)
{
}

BoundingVolumeType::BoundingVolumeType BoundingAxisAlignedBox::GetType(void) const
{
	return BoundingVolumeType::AxisAlignedBox;
}

bool BoundingAxisAlignedBox::Contains(const Meson::Common::Maths::TVector3<Real>& p_vecPoint) const
{
	if (p_vecPoint.X < Min.X) return false;
	if (p_vecPoint.X > Max.X) return false;
	if (p_vecPoint.Y < Min.Y) return false;
	if (p_vecPoint.Y > Max.Y) return false;
	if (p_vecPoint.Z < Min.Z) return false;
	if (p_vecPoint.Z > Max.Z) return false;
	return true;
}

bool BoundingAxisAlignedBox::Intersects(const Ray& p_ray) const
{
	static TVector3<Real> s_vecIntersction;
	return Intersects(p_ray, s_vecIntersction);
}

bool BoundingAxisAlignedBox::Intersects(const Ray& p_ray,
	Meson::Common::Maths::TVector3<Real>& p_vecIntersectionPoint) const
{
	Real rTMin((Real) 0.0);
	Real rTMax(TMaths<Real>::Maximum);
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		if (TMaths<Real>::Equals(p_ray.Direction.Element[unIndex], (Real) 0.0))
		{
			// ray parallel to this slab i.e. definitely no intersection if ray source outside
			if (p_ray.Source.Element[unIndex] < Min.Element[unIndex]
				|| p_ray.Source.Element[unIndex] > Max.Element[unIndex])
					return false;
		}
		else
		{
			// ray not parallel to slab - compute slab t min / max
			Real rDirectionInverse((Real) 1.0 / p_ray.Direction.Element[unIndex]);
			Real rTMinSlab((Min.Element[unIndex] - p_ray.Source.Element[unIndex]) * rDirectionInverse);
			Real rTMaxSlab((Max.Element[unIndex] - p_ray.Source.Element[unIndex]) * rDirectionInverse);
			if (rTMinSlab > rTMaxSlab)
			{
				Real rTemp(rTMinSlab);
				rTMinSlab = rTMaxSlab;
				rTMaxSlab = rTemp;
			}

			// restrict best t min and max using current slab t min/max
			rTMin = TMaths<Real>::Max(rTMin, rTMinSlab);
			rTMax = TMaths<Real>::Min(rTMax, rTMaxSlab);

			// no intersection if ray looking away from this slab
			if (rTMin > rTMax)
				return false;
		}
	}

	// compute intersection point
	p_vecIntersectionPoint = p_ray.Source + p_ray.Direction * rTMin;

	// if this point reached - intersection occured
	return true;
}

bool BoundingAxisAlignedBox::Intersects(const Triangle& p_triangle) const
{
	TInterval<Real> itvBoxProjection, itvTriangleProjection;
	
	// AABB X axis SAT
	itvBoxProjection.Min = Min.X;
	itvBoxProjection.Max = Max.X;
	p_triangle.ProjectToInterval(TVector3<Real>::Right, itvTriangleProjection);
	if (!itvBoxProjection.Intersects(itvTriangleProjection))
		return false;

	// AABB Y axis SAT
	itvBoxProjection.Min = Min.Y;
	itvBoxProjection.Max = Max.Y;
	p_triangle.ProjectToInterval(TVector3<Real>::Up, itvTriangleProjection);
	if (!itvBoxProjection.Intersects(itvTriangleProjection))
		return false;

	// AABB Z axis SAT
	itvBoxProjection.Min = Min.Z;
	itvBoxProjection.Max = Max.Z;
	p_triangle.ProjectToInterval(TVector3<Real>::Out, itvTriangleProjection);
	if (!itvBoxProjection.Intersects(itvTriangleProjection))
		return false;

	// triangle normal SAT
	TVector3<Real> vecNormal = p_triangle.Normal();
	ProjectToInterval(vecNormal, itvBoxProjection);
	p_triangle.ProjectToInterval(vecNormal, itvTriangleProjection);
	if (!itvBoxProjection.Intersects(itvTriangleProjection))
		return false;

	// edges cross-product SAT
	static TVector3<Real> s_vecAxis[3]
		= { TVector3<Real>::Right, TVector3<Real>::Up, TVector3<Real>::Out};

	for (size_t unTriangleIndex = 0; unTriangleIndex < 3; unTriangleIndex++)
	{
		TVector3<Real> vecTriangleEdge(p_triangle.Edge(unTriangleIndex));

		if (vecTriangleEdge.IsZero())
			continue;

		for (size_t unBoxIndex = 0; unBoxIndex < 3; unBoxIndex++)
		{
			vecNormal = vecTriangleEdge ^ s_vecAxis[unBoxIndex];
			if (vecNormal.IsZero())
				continue;
			ProjectToInterval(vecNormal, itvBoxProjection);
			p_triangle.ProjectToInterval(vecNormal, itvTriangleProjection);
			if (!itvBoxProjection.Intersects(itvTriangleProjection))
				return false;
		}
	}

	// all SATs failed, hence intersecting
	return true;
}

bool BoundingAxisAlignedBox::Intersects(const IBoundingVolume &p_boundingVolume) const
{
	switch (p_boundingVolume.GetType())
	{
	case BoundingVolumeType::Sphere:
		return Intersects((BoundingSphere &) p_boundingVolume);
	case BoundingVolumeType::AxisAlignedBox:
		return Intersects((BoundingAxisAlignedBox &) p_boundingVolume);
	case BoundingVolumeType::OrientedBox:
		return p_boundingVolume.Intersects(*this);
	default:
		return false;
	}
}

bool BoundingAxisAlignedBox::IntersectsDynamic(
	const IBoundingVolume &p_boundingVolume,
	const TVector3<Real> &p_vecSweep) const
{
	BoundingAxisAlignedBox boundingAxisAlignedBoxSwept;
	QuickSweep(-p_vecSweep, boundingAxisAlignedBoxSwept);
	return Intersects(p_boundingVolume);
}

BoundingVolumePtr BoundingAxisAlignedBox::Duplicate(void)
{
	return BoundingVolumePtr(new BoundingAxisAlignedBox(*this));
}

void BoundingAxisAlignedBox::AssignFrom(BoundingVolumePtr p_pBoundingVolume)
{
	IBoundingVolume* pBoundingVolume = p_pBoundingVolume;
	MESON_ASSERT(pBoundingVolume->GetType() == BoundingVolumeType::AxisAlignedBox, "Bounding volume must be an axis aligned box.");
	BoundingAxisAlignedBox* pBoundingAxisAlignedBox = (BoundingAxisAlignedBox*) pBoundingVolume;
	*this = *pBoundingAxisAlignedBox;
}

void BoundingAxisAlignedBox::Transform(
	const Meson::Gravitas::Geometry::Transform& p_transform)
{
	// rotate coord axes (inverse orientation)
	TQuaternion<Real> qtnRotationInverse = p_transform.Rotation.ConjugateCopy();
	TVector3<Real> vecAxis1 = qtnRotationInverse.TransformCopy(TVector3<Real>::Right);
	TVector3<Real> vecAxis2 = qtnRotationInverse.TransformCopy(TVector3<Real>::Up);
	TVector3<Real> vecAxis3 = vecAxis1 ^ vecAxis2;

	// project box on inverse axes
	TInterval<Real> interval1, interval2, interval3;
	ProjectToInterval(vecAxis1, interval1);
	ProjectToInterval(vecAxis2, interval2);
	ProjectToInterval(vecAxis3, interval3);

	// intervals give spans of AABB if it were rotated
	Min.X = interval1.Min;
	Min.Y = interval2.Min;
	Min.Z = interval3.Min;

	Max.X = interval1.Max;
	Max.Y = interval2.Max;
	Max.Z = interval3.Max;

	// translate
	Min += p_transform.Translation;
	Max += p_transform.Translation;
}

void BoundingAxisAlignedBox::ClosestPointTo(
	const TVector3<Real>& p_vecPoint, TVector3<Real>& p_vecClosestPoint) const
{
	p_vecClosestPoint = p_vecPoint;

	// clamp point to within AABB
	if (p_vecClosestPoint.X < Min.X) p_vecClosestPoint.X = Min.X;
	if (p_vecClosestPoint.Y < Min.Y) p_vecClosestPoint.Y = Min.Y;
	if (p_vecClosestPoint.Z < Min.Z) p_vecClosestPoint.Z = Min.Z;

	if (p_vecClosestPoint.X > Max.X) p_vecClosestPoint.X = Max.X;
	if (p_vecClosestPoint.Y > Max.Y) p_vecClosestPoint.Y = Max.Y;
	if (p_vecClosestPoint.Z > Max.Z) p_vecClosestPoint.Z = Max.Z;
}

void BoundingAxisAlignedBox::ProjectToInterval(
	const Meson::Common::Maths::TVector3<Real>& p_vecAxis,
	Meson::Common::Maths::TInterval<Real>& p_interval) const
{
	TVector3<Real> vecCentre = Min + (Max - Min) * (Real) 0.5;
	Real rCentre = vecCentre * p_vecAxis;

	TVector3<Real> vecRadius = Max - vecCentre;
	Real rRadius
		= TMaths<Real>::Abs(vecRadius.X * p_vecAxis.X)
		+ TMaths<Real>::Abs(vecRadius.Y * p_vecAxis.Y)
		+ TMaths<Real>::Abs(vecRadius.Z * p_vecAxis.Z);

	p_interval.Min = rCentre - rRadius;
	p_interval.Max = rCentre + rRadius;
}

TVector3<Real> BoundingAxisAlignedBox::Centre(void) const
{
	return Min + (Max - Min) * (Real) 0.5;
}

TVector3<Real> BoundingAxisAlignedBox::Extent(void) const
{
	return (Max - Min) * (Real) 0.5;
}

BoundingAxisAlignedBox &BoundingAxisAlignedBox::operator=(
	const BoundingAxisAlignedBox &p_boundingAxisAlignedBox)
{
	Min = p_boundingAxisAlignedBox.Min;
	Max = p_boundingAxisAlignedBox.Max;
	return *this;
}

void BoundingAxisAlignedBox::ExtendToPoint(
	const Meson::Common::Maths::TVector3<Real> &p_vecPoint)
{
	Min.X = TMaths<Real>::Min(Min.X, p_vecPoint.X);
	Min.Y = TMaths<Real>::Min(Min.Y, p_vecPoint.Y);
	Min.Z = TMaths<Real>::Min(Min.Z, p_vecPoint.Z);

	Max.X = TMaths<Real>::Max(Max.X, p_vecPoint.X);
	Max.Y = TMaths<Real>::Max(Max.Y, p_vecPoint.Y);
	Max.Z = TMaths<Real>::Max(Max.Z, p_vecPoint.Z);
}

const bool BoundingAxisAlignedBox::operator==(
	const BoundingAxisAlignedBox& p_boundingAxisAlignedBox)const
{
	if (!(Min - p_boundingAxisAlignedBox.Min).IsZero()) return false;
	if (!(Max - p_boundingAxisAlignedBox.Max).IsZero()) return false;
	return true;
}

const bool BoundingAxisAlignedBox::operator!=(
	const BoundingAxisAlignedBox& p_boundingAxisAlignedBox)const
{
	if (!(Min - p_boundingAxisAlignedBox.Min).IsZero()) return true;
	if (!(Max - p_boundingAxisAlignedBox.Max).IsZero()) return true;
	return false;
}

bool BoundingAxisAlignedBox::Intersects(
	const BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const
{
	if (Min.X > p_boundingAxisAlignedBox.Max.X) return false;
	if (Min.Y > p_boundingAxisAlignedBox.Max.Y) return false;
	if (Min.Z > p_boundingAxisAlignedBox.Max.Z) return false;

	if (Max.X < p_boundingAxisAlignedBox.Min.X) return false;
	if (Max.Y < p_boundingAxisAlignedBox.Min.Y) return false;
	if (Max.Z < p_boundingAxisAlignedBox.Min.Z) return false;

	return true;
}

bool BoundingAxisAlignedBox::Intersects(const BoundingSphere &p_boundingSphere) const
{
	TVector3<Real> vecClosestPoint;
	ClosestPointTo(p_boundingSphere.Centre, vecClosestPoint);

	return (p_boundingSphere.Centre - vecClosestPoint).LengthSquared()
		<= p_boundingSphere.RadiusSquared;
}

void BoundingAxisAlignedBox::EnumerateMaximalVertices(
	const TVector3<Real>& p_vecDirection, VectorList& p_listVertices) const
{
	// clear vertex list
	p_listVertices.Clear();

	// enumerate 8 OBB vertices
	Real rMaximumProjection = -TMaths<Real>::Maximum;

	// determine maximal vertices in terms of projection
	TVector3<Real> vecVertex; 
	for (size_t unIndex = 0; unIndex < 8; unIndex++)
	{
		vecVertex.X = unIndex % 2 == 0 ? Min.X : Max.X;
		vecVertex.Y = (unIndex >> 1) % 2 == 0 ? Min.Y : Max.Y;
		vecVertex.Z = (unIndex >> 2) % 2 == 0 ? Min.Z : Max.Z;

		Real rProjection = p_vecDirection * vecVertex;

		// ignore vertices that are definitely less extreme than current
		if (rProjection + TMaths<Real>::Epsilon < rMaximumProjection) continue;

		// if new projection definitely greater than current maximum, discard
		// current maximal vertices
		if (rProjection - TMaths<Real>::Epsilon > rMaximumProjection)
			p_listVertices.Clear();

		rMaximumProjection = rProjection;
		p_listVertices.Add(vecVertex);
	}
}