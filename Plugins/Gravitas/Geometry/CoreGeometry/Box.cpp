#include "Box.h"

#include "Maths.h"
#include "DebugSupport.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas::Geometry;

Meson_Gravitas_Geometry_BEGIN

const String g_strGeometryTypeBox = "Box";

Box::Box(void)
	: m_nId(-1)
	, Extent((Real) 0.5, (Real) 0.5, (Real) 0.5)
{
}

Box::Box(const String& p_strId)
	: IGeometry(p_strId)
	, m_nId(-1)
	, Extent((Real) 0.5, (Real) 0.5, (Real) 0.5)
{
}

Box::Box(const TVector3<Real>& p_vecExtent)
	: Extent(p_vecExtent)
{
}

Box::~Box(void)
{
}

void Box::EnumerateProperties(
	TMap<String, PropertyDescriptor>& p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties["Extent"] = PropertyDescriptor("Extent", PropertyType::Vector, false);
}

void Box::GetProperty(const String& p_strName,
	TVector3<Real>& p_vecValue) const
{
	if (p_strName == "Extent")
		p_vecValue = Extent;
	else
		MESON_ASSERT(false, "Box geometry: Invalid property specified.")
}

void Box::SetProperty(const String &p_strName, const TVector3<Real>& p_vecValue)
{
	if (p_strName == "Extent")
		Extent = p_vecValue;
	else
		MESON_ASSERT(false, "Box geometry: Invalid property specified.")
}

const String& Box::GetTypeName(void) const
{
	return g_strGeometryTypeBox;
}

short Box::GetTypeId(void) const
{
	return m_nId;
}

void Box::SetTypeId(short p_nId)
{
	m_nId = p_nId;
}

Real Box::GetVolume(void) const
{
	return Extent.X * Extent.Y * Extent.Z * (Real) 8.0;
}

TMatrix3<Real> Box::GetDistributionTensor(void) const
{
	Real rCommonTerm = Extent.X * Extent.Y * Extent.Z * (Real) 8.0 / (Real) 3.0;
	Real rXSquared = Extent.X * Extent.X;
	Real rYSquared = Extent.Y * Extent.Y;
	Real rZSquared = Extent.Z * Extent.Z;

	TMatrix3<Real> matDistributionTensor = TMatrix3<Real>::Zero;
	matDistributionTensor._11 = (rYSquared + rZSquared) * rCommonTerm;
	matDistributionTensor._22 = (rXSquared + rZSquared) * rCommonTerm;
	matDistributionTensor._33 = (rXSquared + rYSquared) * rCommonTerm;

	return matDistributionTensor;
}

bool Box::IsBounded(void) const
{
	return true;
}

void Box::ComputeBoundingVolume(BoundingSphere &p_boundingSphere) const
{
	p_boundingSphere.Centre = TVector3<Real>::Zero;
	p_boundingSphere.RadiusSquared = Extent.LengthSquared();
	p_boundingSphere.Radius = TMaths<Real>::Sqrt(p_boundingSphere.RadiusSquared);
}

void Box::ComputeBoundingVolume(BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const
{
	MESON_ASSERT(Extent.X >= (Real) 0.0, "Box extents must be positive.");
	MESON_ASSERT(Extent.Y >= (Real) 0.0, "Box extents must be positive.");
	MESON_ASSERT(Extent.Z >= (Real) 0.0, "Box extents must be positive.");

	p_boundingAxisAlignedBox.Min = -Extent;
	p_boundingAxisAlignedBox.Max =  Extent;
}

void Box::ComputeBoundingVolume(BoundingOrientedBox &p_boundingOrientedBox) const
{
	MESON_ASSERT(Extent.X >= (Real) 0.0, "Box extents must be positive.");
	MESON_ASSERT(Extent.Y >= (Real) 0.0, "Box extents must be positive.");
	MESON_ASSERT(Extent.Z >= (Real) 0.0, "Box extents must be positive.");

	p_boundingOrientedBox = BoundingOrientedBox(TVector3<Real>::Zero, Extent);
}

bool Box::IntersectsRay(const Ray& p_ray) const
{
	static TVector3<Real> s_vecIntersctionPoint;
	return IntersectsRay(p_ray, s_vecIntersctionPoint);
}

bool Box::IntersectsRay(const Ray& p_ray, TVector3<Real>& p_vecIntersectionPoint) const
{
	Real rTMin((Real) 0.0);
	Real rTMax(TMaths<Real>::Maximum);
	for (size_t unIndex = 0; unIndex < 3; unIndex++)
	{
		if (TMaths<Real>::Equals(p_ray.Direction.Element[unIndex], (Real) 0.0))
		{
			// ray parallel to this slab i.e. definitely no intersection if ray source outside
			if (p_ray.Source.Element[unIndex] < -Extent.Element[unIndex]
				|| p_ray.Source.Element[unIndex] > Extent.Element[unIndex])
					return false;
		}
		else
		{
			// ray not parallel to slab - compute slab t min / max
			Real rDirectionInverse((Real) 1.0 / p_ray.Direction.Element[unIndex]);
			Real rTMinSlab((-Extent.Element[unIndex] - p_ray.Source.Element[unIndex]) * rDirectionInverse);
			Real rTMaxSlab((Extent.Element[unIndex] - p_ray.Source.Element[unIndex]) * rDirectionInverse);
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

Meson_Gravitas_Geometry_END
