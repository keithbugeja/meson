//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Geometry::BoundingOrientedBox
//	Oriented bounding box implementation.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		15/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "GrvBoundingOrientedBox.h"

#include "ArrayList.h"

using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

void BoundingOrientedBox::QuickSweep(
	const TVector3<Real> &p_vecSweep,
	BoundingOrientedBox &p_boundingOrientedBox) const
{
	p_boundingOrientedBox = *this;

	p_boundingOrientedBox.Extent.X += TMaths<Real>::Abs(p_vecSweep
		* p_boundingOrientedBox.Axis[0]) * (Real) 0.5;
	p_boundingOrientedBox.Extent.Y += TMaths<Real>::Abs(p_vecSweep
		* p_boundingOrientedBox.Axis[1]) * (Real) 0.5;
	p_boundingOrientedBox.Extent.Z += TMaths<Real>::Abs(p_vecSweep
		* p_boundingOrientedBox.Axis[2]) * (Real) 0.5;

	p_boundingOrientedBox.Centre += p_vecSweep * (Real) 0.5;
}

void BoundingOrientedBox::NormaliseAxes(void)
{
	// normalise first axis
	Axis[0].Normalise();

	// eliminate component from second axis perpendicular to first
	// and normalise it
	Axis[1] -= (Axis[1] * Axis[0]) * Axis[0];
	Axis[1].Normalise();

	// compute third axis by cross-product
	Axis[2] = Axis[0] ^ Axis[1];
}

BoundingOrientedBox::BoundingOrientedBox(void)
	: Centre(TPoint3<Real>::Origin)
	, Extent((Real) 1.0, (Real) 1.0, (Real) 1.0)
{
	Axis[0] = TVector3<Real>::Right;
	Axis[1] = TVector3<Real>::Up;
	Axis[2] = TVector3<Real>::Out;
}

BoundingOrientedBox::BoundingOrientedBox(
	const BoundingOrientedBox &p_boundingOrientedBox)
	: Centre(p_boundingOrientedBox.Centre)
	, Extent(p_boundingOrientedBox.Extent)
{
	Axis[0] = p_boundingOrientedBox.Axis[0];
	Axis[1] = p_boundingOrientedBox.Axis[1];
	Axis[2] = p_boundingOrientedBox.Axis[2];
}

BoundingOrientedBox::BoundingOrientedBox(
	const BoundingAxisAlignedBox &p_boundingAxisAlignedBox)
	: Centre(p_boundingAxisAlignedBox.Centre())
	, Extent(p_boundingAxisAlignedBox.Extent())
{
	Axis[0] = TVector3<Real>::Right;
	Axis[1] = TVector3<Real>::Up;
	Axis[2] = TVector3<Real>::Out;
}

BoundingOrientedBox::BoundingOrientedBox(
	const TPoint3<Real> &p_ptCentre,
	const TQuaternion<Real> &p_qtnOrientation,
	const TVector3<Real> &p_vecExtent)
	: Centre(p_ptCentre)
	, Extent(p_vecExtent)
{
	TQuaternion<Real> qtnConjugate = p_qtnOrientation.ConjugateCopy();
	Axis[0] = (p_qtnOrientation * TQuaternion<Real>(TVector3<Real>::Right) * qtnConjugate).Vector;
	Axis[1] = (p_qtnOrientation * TQuaternion<Real>(TVector3<Real>::Up) * qtnConjugate).Vector;
	Axis[2] = (p_qtnOrientation * TQuaternion<Real>(TVector3<Real>::Out) * qtnConjugate).Vector;
}

BoundingOrientedBox::BoundingOrientedBox(
	const TPoint3<Real> &p_ptCentre,
	const TVector3<Real> &p_vecExtent)
	: Centre(p_ptCentre)
	, Extent(p_vecExtent)
{
	Axis[0] = TVector3<Real>::Right;
	Axis[1] = TVector3<Real>::Up;
	Axis[2] = TVector3<Real>::Out;
}

BoundingOrientedBox::~BoundingOrientedBox(void)
{
}

BoundingVolumeType::BoundingVolumeType BoundingOrientedBox::GetType(void) const
{
	return BoundingVolumeType::OrientedBox;
}

bool BoundingOrientedBox::Contains(const Meson::Common::Maths::TPoint3<Real>& p_ptPoint) const
{
	TVector3<Real> vecPointOffset(p_ptPoint - Centre);

	Real rProjectionX(vecPointOffset * Axis[0]);
	if (rProjectionX < -Extent.X) return false;
	if (rProjectionX >  Extent.X) return false;

	Real rProjectionY(vecPointOffset * Axis[1]);
	if (rProjectionY < -Extent.Y) return false;
	if (rProjectionY >  Extent.Y) return false;

	Real rProjectionZ(vecPointOffset * Axis[2]);
	if (rProjectionZ < -Extent.Z) return false;
	if (rProjectionZ >  Extent.Z) return false;

	return true;
}

bool BoundingOrientedBox::Intersects(const Ray& p_ray) const
{
	static TPoint3<Real> s_ptIntersction;
	return Intersects(p_ray, s_ptIntersction);
}

bool BoundingOrientedBox::Intersects(const Ray& p_ray,
	Meson::Common::Maths::TPoint3<Real>& p_ptIntersectionPoint) const
{
	// compute rotation matrix from OBB axes
	TMatrix3<Real> matWorld(false);
	for (int nIndex = 0; nIndex < 3; nIndex++)
		matWorld.SetColumn(nIndex, Axis[nIndex]);

	// prepare local and world transforms
	Meson::Gravitas::Geometry::Transform trnWorld;
	trnWorld.Rotation.MakeRotation(matWorld);
	trnWorld.Translation = Centre.ToVector();
	Meson::Gravitas::Geometry::Transform trnLocal(trnWorld.InvertCopy());

	/// transform ray to local space
	Ray rayLocal(p_ray.TransformCopy(trnLocal));

	// prepare AABB version of OBB
	BoundingAxisAlignedBox boundingAxisAlignedBox(TPoint3<Real>::Origin - Extent, TPoint3<Real>::Origin + Extent);

	// do test with local ray on AABB
	if (!boundingAxisAlignedBox.Intersects(rayLocal, p_ptIntersectionPoint))
		return false;

	// transform the local intersection point to world
	trnWorld.Apply(p_ptIntersectionPoint);

	return true;
}

bool BoundingOrientedBox::Intersects(const Triangle& p_triangle) const
{
	TInterval<Real> itvBoxProjection, itvTriangleProjection;
	
	// OBB axes SAT
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		ProjectToInterval(Axis[unIndex], itvBoxProjection);
		p_triangle.ProjectToInterval(Axis[unIndex], itvTriangleProjection);
		if (!itvBoxProjection.Intersects(itvTriangleProjection))
			return false;
	}

	// triangle normal SAT
	TVector3<Real> vecNormal = p_triangle.Normal();
	ProjectToInterval(vecNormal, itvBoxProjection);
	p_triangle.ProjectToInterval(vecNormal, itvTriangleProjection);
	if (!itvBoxProjection.Intersects(itvTriangleProjection))
		return false;

	// edges cross-product SAT
	for (size_t unTriangleIndex = 0; unTriangleIndex < 3; unTriangleIndex++)
	{
		TVector3<Real> vecTriangleEdge(p_triangle.Edge(unTriangleIndex));

		if (vecTriangleEdge.IsZero())
			continue;

		for (size_t unBoxIndex = 0; unBoxIndex < 3; unBoxIndex++)
		{
			vecNormal = vecTriangleEdge ^ Axis[unBoxIndex];
			if (vecNormal.IsZero())
				continue;
			ProjectToInterval(vecNormal, itvBoxProjection);
			p_triangle.ProjectToInterval(vecNormal, itvTriangleProjection);
			if (!itvBoxProjection.Intersects(itvTriangleProjection))
				return false;
		}
	}

	// all SATs failed, hence intersecting
	return true;}

bool BoundingOrientedBox::Intersects(const IBoundingVolume &p_boundingVolume) const
{
	switch (p_boundingVolume.GetType())
	{
	case BoundingVolumeType::Sphere:
		return Intersects((BoundingSphere &) p_boundingVolume);
	case BoundingVolumeType::AxisAlignedBox:
		return Intersects((BoundingAxisAlignedBox &) p_boundingVolume);
	case BoundingVolumeType::OrientedBox:
		return Intersects((BoundingOrientedBox &) p_boundingVolume);
	default:
		return false;
	}
}

bool BoundingOrientedBox::IntersectsDynamic(const IBoundingVolume &p_boundingVolume,
	const Meson::Common::Maths::TVector3<Real> &p_vecSweep) const
{
	BoundingOrientedBox boundingOrientedBoxSwept;
	QuickSweep(-p_vecSweep, boundingOrientedBoxSwept);
	return Intersects(p_boundingVolume);
}

BoundingVolumePtr BoundingOrientedBox::Duplicate(void)
{
	return BoundingVolumePtr(new BoundingOrientedBox(*this));
}

void BoundingOrientedBox::AssignFrom(BoundingVolumePtr p_pBoundingVolume)
{
	IBoundingVolume* pBoundingVolume = p_pBoundingVolume;
	MESON_ASSERT(pBoundingVolume->GetType() == BoundingVolumeType::OrientedBox, "Bounding volume must be an oriented box.");
	BoundingOrientedBox* pBoundingOrientedBox = (BoundingOrientedBox*) pBoundingVolume;
	*this = *pBoundingOrientedBox;
}

void BoundingOrientedBox::Transform(
	const Meson::Gravitas::Geometry::Transform& p_transform)
{
	// rotate axes

	// compute first and second axis
	Axis[0] = p_transform.Rotation.TransformCopy(Axis[0]);
	Axis[1] = p_transform.Rotation.TransformCopy(Axis[1]);

	// normalise axes (computes third axis for free)
	NormaliseAxes();

	// translate centre
	Centre = p_transform.ApplyCopy(Centre);
}

void BoundingOrientedBox::ClosestPointTo(
	const TPoint3<Real> &p_ptPoint, TPoint3<Real> &p_ptClosestPoint) const
{
	TVector3<Real> vecOffset = p_ptPoint - Centre;

	// start closest point at box centre
	p_ptClosestPoint = Centre;

	// process all box axes
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		// project point offset onto axis
		Real rDistance = vecOffset * Axis[unIndex];

		// clamp distance to extent
		rDistance = TMaths<Real>::Clamp(rDistance,
			-Extent.Element[unIndex], Extent.Element[unIndex]);

		// shift closest point along axis by clamped distance
		p_ptClosestPoint += rDistance * Axis[unIndex];
	}
}

void BoundingOrientedBox::ProjectToInterval(
	const Meson::Common::Maths::TVector3<Real> &p_vecAxis,
	Meson::Common::Maths::TInterval<Real> &p_interval) const
{
	Real rCentre = Centre.ToVector() * p_vecAxis;
	Real rRadius
		= Extent.X * TMaths<Real>::Abs(Axis[0] * p_vecAxis)
		+ Extent.Y * TMaths<Real>::Abs(Axis[1] * p_vecAxis)
		+ Extent.Z * TMaths<Real>::Abs(Axis[2] * p_vecAxis);
	p_interval.Min = rCentre - rRadius;
	p_interval.Max = rCentre + rRadius;
}

bool BoundingOrientedBox::operator==(
	const BoundingOrientedBox &p_boundingOrientedBox) const
{
	if (Centre != p_boundingOrientedBox.Centre) return false;
	if (Axis[0] != p_boundingOrientedBox.Axis[0]) return false;
	if (Axis[1] != p_boundingOrientedBox.Axis[1]) return false;
	if (Axis[2] != p_boundingOrientedBox.Axis[2]) return false;
	if (Extent != p_boundingOrientedBox.Extent) return false;
	return true;
}

BoundingOrientedBox &BoundingOrientedBox::operator=(
	const BoundingOrientedBox &p_boundingOrientedBox)
{
	Centre = p_boundingOrientedBox.Centre;
	Axis[0] = p_boundingOrientedBox.Axis[0];
	Axis[1] = p_boundingOrientedBox.Axis[1];
	Axis[2] = p_boundingOrientedBox.Axis[2];
	Extent = p_boundingOrientedBox.Extent;
	return *this;
}

BoundingOrientedBox &BoundingOrientedBox::operator=(
	const BoundingAxisAlignedBox &p_boundingAxisAlignedBox)
{
	Centre = p_boundingAxisAlignedBox.Centre();
	Axis[0] = TVector3<Real>::Right;
	Axis[1] = TVector3<Real>::Up;
	Axis[2] = TVector3<Real>::Out;
	Extent = p_boundingAxisAlignedBox.Extent();
	return *this;
}

TMatrix3<Real> BoundingOrientedBox::Orientation(void) const
{
	TMatrix3<Real> matOrientation;
	matOrientation.SetColumn(0, Axis[0]);
	matOrientation.SetColumn(1, Axis[1]);
	matOrientation.SetColumn(2, Axis[2]);
	return matOrientation;
}

bool BoundingOrientedBox::Intersects(const BoundingSphere &p_boundingSphere) const
{
	TPoint3<Real> ptClosestPoint;
	ClosestPointTo(p_boundingSphere.Centre, ptClosestPoint);

	return (p_boundingSphere.Centre - ptClosestPoint).LengthSquared()
		<= p_boundingSphere.RadiusSquared;
}

bool BoundingOrientedBox::Intersects(const BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const
{
	TInterval<Real> interval1, interval2;

	// try to find separating axis along line connecting centroids
	TVector3<Real> vecAxisCentroids(Centre - p_boundingAxisAlignedBox.Centre());
	// project boxes onto intervals
	ProjectToInterval(vecAxisCentroids, interval1);
	p_boundingAxisAlignedBox.ProjectToInterval(vecAxisCentroids, interval2);
	// no intersection if intervals disjoint
	if (!interval1.Intersects(interval2))
		return false;

	// try to find separating axis along first box face normals
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		// project boxes onto intervals
		ProjectToInterval(Axis[unIndex], interval1);
		p_boundingAxisAlignedBox.ProjectToInterval(Axis[unIndex], interval2);

		// no intersection if intervals disjoint
		if (!interval1.Intersects(interval2)) return false;
	}

	// compute second box face normals
	TVector3<Real> vecSecondNormals[3];
	vecSecondNormals[0] = TVector3<Real>::Right;
	vecSecondNormals[1] = TVector3<Real>::Up;
	vecSecondNormals[2] = TVector3<Real>::Out;

	// try to find separating axis along second box face normals
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		// project boxes onto intervals
		ProjectToInterval(vecSecondNormals[unIndex], interval1);
		p_boundingAxisAlignedBox.ProjectToInterval(vecSecondNormals[unIndex], interval2);

		// no intersection if intervals disjoint
		if (!interval1.Intersects(interval2)) return false;
	}

	// try to find separating axis from edge cross-products
	// note: all edges parallel to face normals
	for (size_t unIndex1 = 0; unIndex1 < 3; unIndex1++)
	{
		for (size_t unIndex2 = 0; unIndex2 < 3; unIndex2++)
		{
			// compute axis from cross-products of edges
			TVector3<Real> vecAxis
				= Axis[unIndex1] ^ vecSecondNormals[unIndex2];

			// skip if edges parallel
			if (vecAxis.IsZero()) continue;

			// project boxes onto intervals
			ProjectToInterval(vecAxis, interval1);
			p_boundingAxisAlignedBox.ProjectToInterval(vecAxis, interval2);

			// no intersection if intervals disjoint
			if (!interval1.Intersects(interval2)) return false;
		}
	}

	// if no separating axis found, boxes must be intersecting
	return true;
}

bool BoundingOrientedBox::Intersects(const BoundingOrientedBox &p_boundingOrientedBox) const
{
	TInterval<Real> interval1, interval2;

	// try to find separating axis along line connecting centroids
	TVector3<Real> vecAxisCentroids(Centre - p_boundingOrientedBox.Centre);
	// project boxes onto intervals
	ProjectToInterval(vecAxisCentroids, interval1);
	p_boundingOrientedBox.ProjectToInterval(vecAxisCentroids, interval2);
	// no intersection if intervals disjoint
	if (!interval1.Intersects(interval2))
		return false;

	// try to find separating axis along first box face normals
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		// project boxes onto intervals
		ProjectToInterval(Axis[unIndex], interval1);
		p_boundingOrientedBox.ProjectToInterval(Axis[unIndex], interval2);

		// no intersection if intervals disjoint
		if (!interval1.Intersects(interval2)) return false;
	}

	// try to find separating axis along second box face normals
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		// project boxes onto intervals
		ProjectToInterval(p_boundingOrientedBox.Axis[unIndex], interval1);
		p_boundingOrientedBox.ProjectToInterval(p_boundingOrientedBox.Axis[unIndex], interval2);

		// no intersection if intervals disjoint
		if (!interval1.Intersects(interval2)) return false;
	}

	// try to find separating axis from edge cross-products
	// note: all edges parallel to face normals
	for (size_t unIndex1 = 0; unIndex1 < 3; unIndex1++)
	{
		for (size_t unIndex2 = 0; unIndex2 < 3; unIndex2++)
		{
			// compute axis from cross-products of edges
			TVector3<Real> vecAxis
				= Axis[unIndex1] ^ p_boundingOrientedBox.Axis[unIndex2];

			// skip if edges parallel
			if (vecAxis.IsZero()) continue;

			// project boxes onto intervals
			ProjectToInterval(vecAxis, interval1);
			p_boundingOrientedBox.ProjectToInterval(vecAxis, interval2);

			// no intersection if intervals disjoint
			if (!interval1.Intersects(interval2)) return false;
		}
	}

	// if no separating axis found, boxes must be intersecting
	return true;
}

void BoundingOrientedBox::EnumerateFeatures(
	PointList& p_listVertices) const
{
	// compute transformed extents
	TVector3<Real> vecExtent[3];
	for(size_t unIndex = 0; unIndex < 3; unIndex++)
		vecExtent[unIndex] = Extent.Element[unIndex]
			* Axis[unIndex];

	p_listVertices.Clear();

	TPoint3<Real> ptCME0 = Centre - vecExtent[0];
	TPoint3<Real> ptCPE0 = Centre + vecExtent[0];
	TPoint3<Real> ptCME0ME1 = ptCME0 - vecExtent[1];
	TPoint3<Real> ptCME0PE1 = ptCME0 + vecExtent[1];
	TPoint3<Real> ptCPE0ME1 = ptCPE0 - vecExtent[1];
	TPoint3<Real> ptCPE0PE1 = ptCPE0 + vecExtent[1];

	p_listVertices.Add(ptCME0ME1 - vecExtent[2]);
	p_listVertices.Add(ptCME0ME1 + vecExtent[2]);
	p_listVertices.Add(ptCME0PE1 - vecExtent[2]);
	p_listVertices.Add(ptCME0PE1 + vecExtent[2]);
	p_listVertices.Add(ptCPE0ME1 - vecExtent[2]);
	p_listVertices.Add(ptCPE0ME1 + vecExtent[2]);
	p_listVertices.Add(ptCPE0PE1 - vecExtent[2]);
	p_listVertices.Add(ptCPE0PE1 + vecExtent[2]);
}

void BoundingOrientedBox::EnumerateFeatures(
	LineSegmentList& p_listEdges) const
{
	PointArrayList listVertices(8);
	EnumerateFeatures(listVertices);

	p_listEdges.Clear();
	p_listEdges.Add(LineSegment(listVertices(0), listVertices(1)));
	p_listEdges.Add(LineSegment(listVertices(0), listVertices(2)));
	p_listEdges.Add(LineSegment(listVertices(1), listVertices(3)));
	p_listEdges.Add(LineSegment(listVertices(2), listVertices(3)));
	p_listEdges.Add(LineSegment(listVertices(4), listVertices(5)));
	p_listEdges.Add(LineSegment(listVertices(4), listVertices(6)));
	p_listEdges.Add(LineSegment(listVertices(5), listVertices(7)));
	p_listEdges.Add(LineSegment(listVertices(6), listVertices(7)));
	p_listEdges.Add(LineSegment(listVertices(0), listVertices(4)));
	p_listEdges.Add(LineSegment(listVertices(1), listVertices(5)));
	p_listEdges.Add(LineSegment(listVertices(2), listVertices(6)));
	p_listEdges.Add(LineSegment(listVertices(3), listVertices(7)));
}

void BoundingOrientedBox::EnumerateFeatures(
	PointList& p_listVertices, LineSegmentList& p_listEdges) const
{
	EnumerateFeatures(p_listVertices);

	p_listEdges.Clear();
	p_listEdges.Add(LineSegment(p_listVertices(0), p_listVertices(1)));
	p_listEdges.Add(LineSegment(p_listVertices(0), p_listVertices(2)));
	p_listEdges.Add(LineSegment(p_listVertices(1), p_listVertices(3)));
	p_listEdges.Add(LineSegment(p_listVertices(2), p_listVertices(3)));
	p_listEdges.Add(LineSegment(p_listVertices(4), p_listVertices(5)));
	p_listEdges.Add(LineSegment(p_listVertices(4), p_listVertices(6)));
	p_listEdges.Add(LineSegment(p_listVertices(5), p_listVertices(7)));
	p_listEdges.Add(LineSegment(p_listVertices(6), p_listVertices(7)));
	p_listEdges.Add(LineSegment(p_listVertices(0), p_listVertices(4)));
	p_listEdges.Add(LineSegment(p_listVertices(1), p_listVertices(5)));
	p_listEdges.Add(LineSegment(p_listVertices(2), p_listVertices(6)));
	p_listEdges.Add(LineSegment(p_listVertices(3), p_listVertices(7)));
}

void BoundingOrientedBox::EnumerateMaximalVertices(
	const TVector3<Real>& p_vecDirection, PointList& p_listVertices) const
{
	// clear vertex list
	p_listVertices.Clear();

	// enumerate 8 OBB vertices
	TArrayList< TPoint3<Real> > listAllVertices(8);
	Real rMaximumProjection = -TMaths<Real>::Maximum;
	EnumerateFeatures(listAllVertices);

	// determine maximal vertices in terms of projection
	for (size_t unIndex = 0; unIndex < 8; unIndex++)
	{
		TPoint3<Real>& ptVertex = listAllVertices(unIndex);
		Real rProjection = p_vecDirection * ptVertex.ToVector();

		// ignore vertices that are definitely less extreme than current
		if (rProjection + TMaths<Real>::Epsilon < rMaximumProjection) continue;

		// if new projection definitely greater than current maximum, discard
		// current maximal vertices
		if (rProjection - TMaths<Real>::Epsilon > rMaximumProjection)
			p_listVertices.Clear();

		rMaximumProjection = rProjection;
		p_listVertices.Add(ptVertex);
	}
}
