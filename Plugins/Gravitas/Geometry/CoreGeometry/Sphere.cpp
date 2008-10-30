#include "Sphere.h"

#include "Maths.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Gravitas::Geometry;

Meson_Gravitas_Geometry_BEGIN

const String g_strGeometryTypeSphere = "Sphere";

Sphere::Sphere(void)
	: m_nId(-1)
	, Radius(1.0)
	, RadiusSquared(1.0)
{
}

Sphere::Sphere(const String& p_strId)
	: IGeometry(p_strId)
	, m_nId(-1)
	, Radius(1.0)
	, RadiusSquared(1.0)
{
}

Sphere::~Sphere(void)
{
}

void Sphere::EnumerateProperties(
	Meson::Common::Collections::TMap<String, PropertyDescriptor>
		&p_mapProperties) const
{
	p_mapProperties.Clear();
	p_mapProperties["Radius"] = PropertyDescriptor("Radius", PropertyType::Real, false);
	p_mapProperties["RadiusSquared"] = PropertyDescriptor("RadiusSquared", PropertyType::Real, true);
}

void Sphere::GetProperty(const String &p_strName, Real &p_rValue) const
{
	if (p_strName == "Radius")
		p_rValue = Radius;
	else if (p_strName == "RadiusSquared")
		p_rValue = RadiusSquared;		
	else
		MESON_ASSERT(false, "Sphere geometry: Invalid property specified.")
}

void Sphere::SetProperty(const String &p_strName, Real p_rValue)
{
	if (p_strName == "Radius")
		SetRadius(p_rValue);
	else
		MESON_ASSERT(false, "Sphere geometry: Invalid property specified.")
}

const String& Sphere::GetTypeName(void) const
{
	return g_strGeometryTypeSphere;
}

short Sphere::GetTypeId(void) const
{
	return m_nId;
}

void Sphere::SetTypeId(short p_nId)
{
	m_nId = p_nId;
}

Real Sphere::GetVolume(void) const
{
	// 4/3 PI r^3
	return 4.0 * TMaths<Real>::Pi * RadiusSquared * Radius / 3.0;
}

TMatrix3<Real> Sphere::GetDistributionTensor(void) const
{
	// diagonal element = 2/5 m r^2
	Real rMomentOfInertia = (Real) 0.4 * GetVolume() * RadiusSquared;

	TMatrix3<Real> matDistributionTensor = TMatrix3<Real>::Zero;
	matDistributionTensor._11 = matDistributionTensor._22
		= matDistributionTensor._33 = rMomentOfInertia;

	return matDistributionTensor;
}

bool Sphere::IsBounded(void) const
{
	return true;
}

void Sphere::ComputeBoundingVolume(BoundingSphere &p_boundingSphere) const
{
	p_boundingSphere.Centre = TVector3<Real>::Zero;
	p_boundingSphere.Radius = Radius;
	p_boundingSphere.RadiusSquared = RadiusSquared;
}

void Sphere::ComputeBoundingVolume(BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const
{
	p_boundingAxisAlignedBox.Min.X = p_boundingAxisAlignedBox.Min.Y
		= p_boundingAxisAlignedBox.Min.Z = -Radius;
	p_boundingAxisAlignedBox.Max = -p_boundingAxisAlignedBox.Min;
}

void Sphere::ComputeBoundingVolume(BoundingOrientedBox &p_boundingOrientedBox) const
{
	p_boundingOrientedBox.Centre = TVector3<Real>::Zero;
	p_boundingOrientedBox.Axis[0] = TVector3<Real>::Right;
	p_boundingOrientedBox.Axis[1] = TVector3<Real>::Up;
	p_boundingOrientedBox.Axis[2] = TVector3<Real>::Out;
	p_boundingOrientedBox.Extent.X = p_boundingOrientedBox.Extent.Y
		= p_boundingOrientedBox.Extent.Z = Radius;
}

bool Sphere::IntersectsRay(const Ray& p_ray) const
{
	return p_ray.DistanceSquaredTo(TVector3<Real>::Zero) <= RadiusSquared;
}

bool Sphere::IntersectsRay(const Ray& p_ray, TVector3<Real>& p_vecIntersectionPoint) const
{
	TVector3<Real> vecClosestPoint(p_ray.ClosestPointTo(TVector3<Real>::Zero));
	Real rDistanceSquared = vecClosestPoint.LengthSquared();
	if (rDistanceSquared > RadiusSquared)
		return false;

	Real rClosestToSurface(TMaths<Real>::Sqrt(RadiusSquared - rDistanceSquared));

	p_vecIntersectionPoint = vecClosestPoint - p_ray.Direction * rClosestToSurface;

	return true;
}

Real Sphere::GetRadius(void)
{
	return Radius;
}

Real Sphere::GetRadiusSquared(void)
{
	return RadiusSquared;
}

void Sphere::SetRadius(Real p_rRadius)
{
	Radius = p_rRadius;
	RadiusSquared = p_rRadius * p_rRadius;
}

Meson_Gravitas_Geometry_END
