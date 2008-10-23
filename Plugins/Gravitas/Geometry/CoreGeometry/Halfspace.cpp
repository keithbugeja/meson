#include "Halfspace.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

String Halfspace::s_strTypeName("Halfspace");

TMatrix3<Real> Halfspace::s_matDistributionTensor = TMatrix3<Real>::Identity * TMaths<Real>::Maximum;

Halfspace::Halfspace(void)
	: m_nTypeId(-1)
{
}

Halfspace::Halfspace(const String& p_strId)
	: IGeometry(p_strId)
	, m_nTypeId(-1)
{
}

Halfspace::~Halfspace(void)
{
}

void Halfspace::EnumerateProperties(
	TMap<String, PropertyDescriptor> &p_mapProperties) const
{
	p_mapProperties.Clear();
}

const String& Halfspace::GetTypeName(void) const
{
	return s_strTypeName;
}

short Halfspace::GetTypeId(void) const
{
	return m_nTypeId;
}

void Halfspace::SetTypeId(short p_nId)
{
	m_nTypeId = p_nId;
}

Real Halfspace::GetVolume(void) const
{
	return TMaths<Real>::Maximum;
}

TMatrix3<Real> Halfspace::GetDistributionTensor(void) const
{
	return s_matDistributionTensor;
}

bool Halfspace::IsBounded(void) const
{
	return false;
}

void Halfspace::ComputeBoundingVolume(BoundingSphere &p_boundingSphere) const
{
	throw new MesonException(
		"Cannot compute a bounding volume for unbounded geometry.",
		__FILE__, __LINE__);
}

void Halfspace::ComputeBoundingVolume(BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const
{
	throw new MesonException(
		"Cannot compute a bounding volume for unbounded geometry.",
		__FILE__, __LINE__);
}

void Halfspace::ComputeBoundingVolume(BoundingOrientedBox &p_boundingOrientedBox) const
{
	throw new MesonException(
		"Cannot compute a bounding volume for unbounded geometry.",
		__FILE__, __LINE__);
}

bool Halfspace::IntersectsRay(const Ray& p_ray) const
{
	// if ray source already in halfspace, trivially true
	if (p_ray.Source.Y <= (Real) 0.0)
		return true;

	// otherwise, ray source outside halfspace

	// check if ray direction towards surface
	return p_ray.Direction.Y < -TMaths<Real>::Epsilon;
}

bool Halfspace::IntersectsRay(const Ray& p_ray,
	TPoint3<Real>& p_ptIntersectionPoint) const
{
	// if ray source already in halfspace, trivially true
	// and intersection is source itself
	if (p_ray.Source.Y <= (Real) 0.0)
	{
		p_ptIntersectionPoint = p_ray.Source;
		return true;
	}

	// otherwise, ray source outside halfspace

	// if ray pointing away from surface, no intersection
	if (p_ray.Direction.Y > TMaths<Real>::Epsilon)
		return false;

	// otherwise, compute intersection point at Ry = 0
	Real rDistance = - p_ray.Source.Y / p_ray.Direction.Y;
	p_ptIntersectionPoint =  p_ray.Source +  p_ray.Direction * rDistance;
	return true;
}
