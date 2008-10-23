#include "GrvGeometryComposite.h"

#include "ArrayList.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas::Geometry;

Meson_Gravitas_Geometry_BEGIN

GeometryComponent::GeometryComponent(void)
	: Transform()
	, Geometry(NULL)
{
}

GeometryComponent::GeometryComponent(
	const Meson::Gravitas::Geometry::Transform& p_transform,
	GeometryPtr p_pGeometry)
	: Transform(p_transform)
	, Geometry(p_pGeometry)
{
}

GeometryComponent& GeometryComponent::operator=(const GeometryComponent &p_geometryComponent)
{
	Transform = p_geometryComponent.Transform;
	Geometry = p_geometryComponent.Geometry;
	return *this;;
}

bool GeometryComponent::operator==(const GeometryComponent &p_geometryComponent)
{
	if (Geometry != p_geometryComponent.Geometry) return false;
	if (Transform != p_geometryComponent.Transform) return false;
	return true;
}

struct GeometryCompositeState
{
	String m_strTypeName;
	short m_nTypeId;
	TArrayList<GeometryComponent> m_listGeometryComponents;

	GeometryCompositeState(void)
		: m_strTypeName("GeometryComposite")
		, m_nTypeId(-1)
		, m_listGeometryComponents() { }
};

TPoint3<Real> GeometryComposite::GetCentroid(void) const
{
	TArrayList<GeometryComponent> &listGeometryComponents
		= m_pGeometryCompositeState->m_listGeometryComponents;

	TPoint3<Real> ptCentroid = TPoint3<Real>::Origin;
	Real rVolumeTotal = (Real) 0.0;
	for (size_t unIndex = 0; unIndex < listGeometryComponents.Size(); unIndex++)
	{
		GeometryComponent &geometryComponent = listGeometryComponents(unIndex);
		Real rVolumeComponent = geometryComponent.Geometry->GetVolume();
		ptCentroid += geometryComponent.Transform.ApplyCopy(TPoint3<Real>::Origin).ToVector()
			* rVolumeComponent;
		rVolumeTotal += rVolumeComponent;
	}
	ptCentroid.ToVector() /= rVolumeTotal;
	return ptCentroid;
}

GeometryComposite::GeometryComposite(void)
	: m_pGeometryCompositeState(NULL)
{
	m_pGeometryCompositeState = new GeometryCompositeState();
}

GeometryComposite::~GeometryComposite(void)
{
	delete m_pGeometryCompositeState;
}

const String &GeometryComposite::GetTypeName(void) const
{
	return m_pGeometryCompositeState->m_strTypeName;
}

short GeometryComposite::GetTypeId(void) const
{
	return m_pGeometryCompositeState->m_nTypeId;
}

void GeometryComposite::SetTypeId(short p_nTypeId)
{
	m_pGeometryCompositeState->m_nTypeId = p_nTypeId;
}

Real GeometryComposite::GetVolume(void) const
{
	TArrayList<GeometryComponent> &listGeometryComponents
		= m_pGeometryCompositeState->m_listGeometryComponents;

	Real rVolumeTotal = (Real) 0.0;
	for (size_t unIndex = 0; unIndex < listGeometryComponents.Size(); unIndex++)
	{
		GeometryComponent &geometryComponent = listGeometryComponents(unIndex);
		rVolumeTotal += geometryComponent.Geometry->GetVolume();
	}
	return rVolumeTotal;
}

TMatrix3<Real> GeometryComposite::GetDistributionTensor(void) const
{
	TArrayList<GeometryComponent> &listGeometryComponents
		= m_pGeometryCompositeState->m_listGeometryComponents;

	// initialise distibution tensore
	TMatrix3<Real> matTensorTotal = TMatrix3<Real>::Zero;

	for (size_t unIndex = 0; unIndex < listGeometryComponents.Size(); unIndex++)
	{
		GeometryComponent &geometryComponent = listGeometryComponents(unIndex);

		TMatrix3<Real> matTensorComponent
			= geometryComponent.Geometry->GetDistributionTensor();

		// rotate tensor component with quaternion
		TVector3<Real> vecRow;
		for (int nRowIndex = 0; nRowIndex < 3; nRowIndex++)
		{
			vecRow = geometryComponent.Transform.Rotation.TransformCopy(
				matTensorComponent.GetRow(nRowIndex));
			matTensorComponent.SetRow(nRowIndex, vecRow);
		}
		
		// compute offset tensor for component using parallel axes theorem

		// (R.R)I
		TVector3<Real> &vecOffset = geometryComponent.Transform.Translation;
		Real rDistanceSquared = vecOffset.LengthSquared();
		TMatrix3<Real> matOffset = TMatrix3<Real>::Zero;
		matOffset._11 = matOffset._22 = matOffset._33 = rDistanceSquared;

		// R (x) R
		TMatrix3<Real> matOuter;
		matOuter._11 = vecOffset.X * vecOffset.X;
		matOuter._12 = vecOffset.X * vecOffset.Y;
		matOuter._13 = vecOffset.X * vecOffset.Z;
		matOuter._21 = matOuter._12;
		matOuter._22 = vecOffset.Y * vecOffset.Y;
		matOuter._23 = vecOffset.Y * vecOffset.Z;
		matOuter._31 = matOuter._13;
		matOuter._32 = matOuter._23;
		matOuter._33 = vecOffset.Z * vecOffset.Z;

		// Io = Ic + M[(R.R)I - R (x) R]
		TMatrix3<Real> matOffsetTensorComponent = matTensorComponent
			+ (matOffset - matOuter) * geometryComponent.Geometry->GetVolume();

		// add offset tensor to total
		matTensorTotal += matOffsetTensorComponent;
	}

	return matTensorTotal;
}

bool GeometryComposite::IsBounded(void) const
{
	TArrayList<GeometryComponent> &listGeometryComponents
		= m_pGeometryCompositeState->m_listGeometryComponents;

	size_t unCount = listGeometryComponents.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
		if (!listGeometryComponents[unIndex].Geometry->IsBounded())
			return false;

	return true;
}

void GeometryComposite::ComputeBoundingVolume(BoundingSphere &p_boundingSphere) const
{
	if (!IsBounded())
		throw new MesonException(
			"Cannot compute a bounding volume for unbounded geometry.",
			__FILE__, __LINE__);

	TArrayList<GeometryComponent> &listGeometryComponents
		= m_pGeometryCompositeState->m_listGeometryComponents;

	// initialise bounding sphere
	p_boundingSphere.Centre = TPoint3<Real>::Origin;
	p_boundingSphere.Radius = p_boundingSphere.RadiusSquared = (Real) 0.0;

	// process all component geometries
	for (size_t unIndex = 0; unIndex < listGeometryComponents.Size(); unIndex++)
	{
		GeometryComponent &geometryComponent = listGeometryComponents(unIndex);

		// compute bounding sphere for component geometry
		BoundingSphere boundingSphereComponent;
		geometryComponent.Geometry->ComputeBoundingVolume(boundingSphereComponent);

		// transform relative to composite
		geometryComponent.Transform.Apply(boundingSphereComponent.Centre);

		// grow sphere to include component sphere
		TVector3<Real> vecOffset = boundingSphereComponent.Centre - p_boundingSphere.Centre;
		TVector3<Real> vecDirection = vecOffset.NormaliseCopy();
		TPoint3<Real> ptExtreme1 = p_boundingSphere.Centre
			- vecDirection * p_boundingSphere.Radius;
		TPoint3<Real> ptExtreme2 = boundingSphereComponent.Centre
			+ vecDirection * boundingSphereComponent.Radius;

		p_boundingSphere.RadiusSquared = (ptExtreme2 - ptExtreme1).LengthSquared();
		p_boundingSphere.Radius = TMaths<Real>::Sqrt(p_boundingSphere.RadiusSquared);
		p_boundingSphere.Centre = ptExtreme1 + (ptExtreme2 - ptExtreme1) * (Real) 0.5;
	}
}

void GeometryComposite::ComputeBoundingVolume(BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const
{
	if (!IsBounded())
		throw new MesonException(
			"Cannot compute a bounding volume for unbounded geometry.",
			__FILE__, __LINE__);

	TArrayList<GeometryComponent> &listGeometryComponents
		= m_pGeometryCompositeState->m_listGeometryComponents;

	// initialise bounding box
	p_boundingAxisAlignedBox.Min = p_boundingAxisAlignedBox.Max = TPoint3<Real>::Origin;

	// process all component geometries
	for (size_t unIndex = 0; unIndex < listGeometryComponents.Size(); unIndex++)
	{
		GeometryComponent &geometryComponent = listGeometryComponents(unIndex);

		// compute AABB for component geometry
		BoundingAxisAlignedBox boundingAxisAlignedBoxComponent;
		geometryComponent.Geometry->ComputeBoundingVolume(boundingAxisAlignedBoxComponent);

		// transform relative to composite
		BoundingAxisAlignedBox boundingAxisAlignedBoxTransformed
			= boundingAxisAlignedBoxComponent;
		boundingAxisAlignedBoxTransformed.Transform(
			geometryComponent.Transform);

		// grow AABB to include component AABB
		p_boundingAxisAlignedBox.ExtendToPoint(boundingAxisAlignedBoxTransformed.Min);
		p_boundingAxisAlignedBox.ExtendToPoint(boundingAxisAlignedBoxTransformed.Max);
	}
}

void GeometryComposite::ComputeBoundingVolume(BoundingOrientedBox &p_boundingOrientedBox) const
{
	if (!IsBounded())
		throw new MesonException(
			"Cannot compute a bounding volume for unbounded geometry.",
			__FILE__, __LINE__);

	TArrayList<GeometryComponent>& listGeometryComponents
		= m_pGeometryCompositeState->m_listGeometryComponents;

	// prepare list of transformed component boxes
	TArrayList<BoundingOrientedBox> listComponentBoxes;

	// process all component geometries
	for (size_t unIndex = 0; unIndex < listGeometryComponents.Size(); unIndex++)
	{
		GeometryComponent& geometryComponent = listGeometryComponents[unIndex];

		// compute OBB for component geometry
		BoundingOrientedBox boundingOrientedBoxComponent;
		geometryComponent.Geometry->ComputeBoundingVolume(boundingOrientedBoxComponent);

		// transform relative to composite
		BoundingOrientedBox boundingOrientedBoxTransformed
			= boundingOrientedBoxComponent;
		boundingOrientedBoxTransformed.Transform(
			geometryComponent.Transform);

		// add to box list
		listComponentBoxes.Add(boundingOrientedBoxTransformed);
	}

	// compute list of candidate axes from axes of component OBBs
	TArrayList< TVector3<Real> > listAxes;
	for (size_t unBoxIndex = 0; unBoxIndex < listComponentBoxes.Size(); unBoxIndex++)
	{
		BoundingOrientedBox &boundingOrientedBox = listComponentBoxes(unBoxIndex);
		for (size_t unAxisIndex = 0; unAxisIndex < 3; unAxisIndex++)
			listAxes.Add(boundingOrientedBox.Axis[unAxisIndex]);
	}

	// process axes list to find first box axis
	Real rMinVolume = TMaths<Real>::Maximum;
	for (size_t unAxisIndex1 = 0; unAxisIndex1 < listAxes.Size(); unAxisIndex1++)
	{
		TVector3<Real> &vecAxis1 = listAxes(unAxisIndex1);

		// project component boxes on first axis and compute composite interval
		TInterval<Real> interval1;
		interval1.MakeEmpty();
		for (size_t unBoxIndex = 0; unBoxIndex < listComponentBoxes.Size(); unBoxIndex++)
		{
			BoundingOrientedBox &boundingOrientedBox = listComponentBoxes(unBoxIndex);
			TInterval<Real> interval;
			boundingOrientedBox.ProjectToInterval(vecAxis1, interval);
			interval1.Extend(interval);
		}

		// process axes list to find second box axis
		for (size_t unAxisIndex2 = 0; unAxisIndex2 < listAxes.Size(); unAxisIndex2++)
		{
			// skip first axis
			if (unAxisIndex2 == unAxisIndex1) continue;

			TVector3<Real> vecAxis2 = listAxes(unAxisIndex2);

			// eliminate component of axis 2 along axis 1
			vecAxis2 -= (vecAxis2 * vecAxis1) * vecAxis1;

			// skip second axis if zero (i.e. was parallel with first)
			if (TMaths<Real>::Equals(vecAxis2.LengthSquared(), (Real) 0.0))
				continue;

			// compute axis 3 from cross-product
			TVector3<Real> vecAxis3 = vecAxis1 ^ vecAxis2;

			// project component boxes on second and third axes and compute
			// corresponding intervals
			TInterval<Real> interval2, interval3;
			interval1.MakeEmpty();
			for (size_t unBoxIndex = 0; unBoxIndex < listComponentBoxes.Size(); unBoxIndex++)
			{
				BoundingOrientedBox &boundingOrientedBox = listComponentBoxes(unBoxIndex);
				TInterval<Real> interval;

				// extend interval 2 using axis 2
				boundingOrientedBox.ProjectToInterval(vecAxis2, interval);
				interval2.Extend(interval);

				// extend interval 3 using axis 3
				boundingOrientedBox.ProjectToInterval(vecAxis3, interval);
				interval3.Extend(interval);

				// compute current volume
				Real rSpan1 = interval1.Span();
				Real rSpan2 = interval2.Span();
				Real rSpan3 = interval3.Span();
				Real rCurrentVolume = rSpan1 * rSpan2 * rSpan3;

				// skip these axes if current min volume is smaller
				if (rMinVolume < rCurrentVolume) continue;

				// otherwise, compute new OBB
				p_boundingOrientedBox.Centre.X = interval1.Median();
				p_boundingOrientedBox.Centre.Y = interval2.Median();
				p_boundingOrientedBox.Centre.Z = interval3.Median();
				p_boundingOrientedBox.Axis[0] = vecAxis1;
				p_boundingOrientedBox.Axis[1] = vecAxis2;
				p_boundingOrientedBox.Axis[2] = vecAxis3;
				p_boundingOrientedBox.Extent.X = rSpan1 * (Real) 0.5;
				p_boundingOrientedBox.Extent.Y = rSpan2 * (Real) 0.5;
				p_boundingOrientedBox.Extent.Z = rSpan3 * (Real) 0.5;

				// keep track of best OBB so far
				rMinVolume = rCurrentVolume;
			}
		}
	}
}

bool GeometryComposite::IntersectsRay(const Ray& p_ray) const
{
	TArrayList<GeometryComponent>& listGeometryComponents
		= m_pGeometryCompositeState->m_listGeometryComponents;

	// prepare list of transformed component boxes
	TArrayList<BoundingOrientedBox> listComponentBoxes;

	// process all component geometries
	for (size_t unIndex = 0; unIndex < listGeometryComponents.Size(); unIndex++)
	{
		GeometryComponent& geometryComponent = listGeometryComponents[unIndex];
		Ray rayComponent = p_ray.TransformCopy(geometryComponent.Transform);
		if (geometryComponent.Geometry->IntersectsRay(rayComponent))
			return true;
	}

	return false;
}

bool GeometryComposite::IntersectsRay(const Ray& p_ray,
	TPoint3<Real>& p_ptIntersectionPoint) const
{
	TArrayList<GeometryComponent>& listGeometryComponents
		= m_pGeometryCompositeState->m_listGeometryComponents;

	// prepare list of transformed component boxes
	TArrayList<BoundingOrientedBox> listComponentBoxes;

	bool bResult = false;
	TPoint3<Real> ptIntersectionPointComponent;
	Real rClosestDistanceSquared = TMaths<Real>::Maximum;

	// process all component geometries
	for (size_t unIndex = 0; unIndex < listGeometryComponents.Size(); unIndex++)
	{
		GeometryComponent& geometryComponent = listGeometryComponents[unIndex];

		// prepare ray transformed in component space
		Ray rayComponent = p_ray.TransformCopy(geometryComponent.Transform.InvertCopy());
		
		if (!geometryComponent.Geometry->IntersectsRay(
			rayComponent, ptIntersectionPointComponent))
			continue;

		bResult = true;

		// transform intersection point back to composite space
		geometryComponent.Transform.Apply(ptIntersectionPointComponent);

		Real rDistanceSquared = (ptIntersectionPointComponent - p_ray.Source).LengthSquared();
		if (rClosestDistanceSquared > rDistanceSquared)
		{
			rClosestDistanceSquared = rDistanceSquared;
			p_ptIntersectionPoint = ptIntersectionPointComponent;
		}
	}

	return bResult;
}

TImmutableList<GeometryComponent>& GeometryComposite::GetComponentGeometries(void)
{
	return m_pGeometryCompositeState->m_listGeometryComponents.AsImmutableList();
}

void GeometryComposite::AddGeometry(
	const Meson::Gravitas::Geometry::Transform& p_transform,
	GeometryPtr p_pGeometry)
{
	m_pGeometryCompositeState->m_listGeometryComponents.Add(
		GeometryComponent(p_transform, p_pGeometry));
}

void GeometryComposite::RemoveGeometry(
	const Meson::Gravitas::Geometry::Transform& p_transform,
	GeometryPtr p_pGeometry)
{
	GeometryComponent geometryComponent(p_transform, p_pGeometry);
	m_pGeometryCompositeState->m_listGeometryComponents.Remove(geometryComponent);
}

void GeometryComposite::RemoveGeometry(
	GeometryPtr p_pGeometry)
{
	TArrayList<GeometryComponent> &listGeometryComponents
		= m_pGeometryCompositeState->m_listGeometryComponents;

	for (int nIndex = (int) listGeometryComponents.Size() - 1; nIndex >= 0; nIndex--)
		if (listGeometryComponents(nIndex).Geometry == p_pGeometry)
			listGeometryComponents.RemoveAt(nIndex);
}

void GeometryComposite::RealignCentroid(void)
{
	TVector3<Real> ptCentroidOffset = GetCentroid().ToVector();

	TArrayList<GeometryComponent>& listGeometryComponents
		= m_pGeometryCompositeState->m_listGeometryComponents;

	for (size_t unIndex = 0; unIndex < listGeometryComponents.Size(); unIndex++)
	{
		GeometryComponent& geometryComponent = listGeometryComponents[unIndex];
		geometryComponent.Transform.Translation -= ptCentroidOffset;
	}
}

Meson_Gravitas_Geometry_END
