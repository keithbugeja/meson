#include "Particle.h"

#include "Maths.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Gravitas::Geometry;

Meson_Gravitas_Geometry_BEGIN

const String g_strGeometryTypeParticle = "Particle";

Particle::Particle(void)
{
}

Particle::Particle(const String& p_strId)
	: IGeometry(p_strId)
{
}

Particle::~Particle(void)
{
}

const String& Particle::GetTypeName(void) const
{
	return g_strGeometryTypeParticle;
}

short Particle::GetTypeId(void) const
{
	return m_nId;
}

void Particle::SetTypeId(short p_nId)
{
	m_nId = p_nId;
}

Real Particle::GetVolume(void) const
{
	return (Real) 0.0;
}

TMatrix3<Real> Particle::GetDistributionTensor(void) const
{
	TMatrix3<Real> matDistributionTensor = TMatrix3<Real>::Zero;
	return matDistributionTensor;
}

bool Particle::IsBounded(void) const
{
	return true;
}

void Particle::ComputeBoundingVolume(BoundingSphere &p_boundingSphere) const
{
	p_boundingSphere.Centre = TVector3<Real>::Zero;
	p_boundingSphere.Radius = TMaths<Real>::Epsilon;
	p_boundingSphere.RadiusSquared = TMaths<Real>::Epsilon;
}

void Particle::ComputeBoundingVolume(BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const
{
	p_boundingAxisAlignedBox.Min.X = p_boundingAxisAlignedBox.Min.Y
		= p_boundingAxisAlignedBox.Min.Z = -TMaths<Real>::Epsilon;
	p_boundingAxisAlignedBox.Max.X = p_boundingAxisAlignedBox.Max.Y
		= p_boundingAxisAlignedBox.Max.Z = TMaths<Real>::Epsilon;
}

void Particle::ComputeBoundingVolume(BoundingOrientedBox &p_boundingOrientedBox) const
{
	p_boundingOrientedBox.Centre = TVector3<Real>::Zero;
	p_boundingOrientedBox.Axis[0] = TVector3<Real>::Right;
	p_boundingOrientedBox.Axis[1] = TVector3<Real>::Up;
	p_boundingOrientedBox.Axis[2] = TVector3<Real>::Out;
	p_boundingOrientedBox.Extent.X = p_boundingOrientedBox.Extent.Y
		= p_boundingOrientedBox.Extent.Z = TMaths<Real>::Epsilon;
}

bool Particle::IntersectsRay(const Ray& p_ray) const
{
	return false;
}

bool Particle::IntersectsRay(const Ray& p_ray, TVector3<Real>& p_vecIntersectionPoint) const
{
	return false;
}

Meson_Gravitas_Geometry_END
