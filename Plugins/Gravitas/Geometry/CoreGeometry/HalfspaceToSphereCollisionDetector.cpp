#include "HalfspaceToSphereCollisionDetector.h"

#include "Halfspace.h"
#include "Sphere.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

HalfspaceToSphereCollisionDetector::HalfspaceToSphereCollisionDetector(void)
	: m_strGeometryType1("Halfspace")
	, m_strGeometryType2("Sphere")
{
}

HalfspaceToSphereCollisionDetector::~HalfspaceToSphereCollisionDetector(void)
{
}

const String& HalfspaceToSphereCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& HalfspaceToSphereCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool HalfspaceToSphereCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Halfspace", "Expected halfspace geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Expected sphere geometry.")

	const Sphere& sphere = (Sphere &) p_geometry2;

	return p_trnRelativePlacement.Translation.Y <= sphere.Radius;
}

bool HalfspaceToSphereCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for halfspace and sphere not implemented yet.",
		__FILE__, __LINE__);
}

void HalfspaceToSphereCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Halfspace", "Expected halfspace geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Expected sphere geometry.")

	const Sphere& sphere = (Sphere &) p_geometry2;

	p_contactManifold.ContactPoints.Clear();

	if (p_trnRelativePlacement.Translation.Y > sphere.Radius)
		return;

	const TVector3<Real>& vecSphereOrigin = p_trnRelativePlacement.Translation;
	Real rPenetration(sphere.Radius - vecSphereOrigin.Y);
	TVector3<Real> vecContactPoint(vecSphereOrigin.X, -rPenetration * (Real) 0.5, vecSphereOrigin.Z);

	p_contactManifold.ContactPoints.Add(
		ContactPoint(vecContactPoint, TVector3<Real>::Up, rPenetration));
}
