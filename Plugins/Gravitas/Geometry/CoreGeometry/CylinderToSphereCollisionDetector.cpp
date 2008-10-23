#include "CylinderToSphereCollisionDetector.h"

#include "Cylinder.h"
#include "Sphere.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

CylinderToSphereCollisionDetector::CylinderToSphereCollisionDetector(void)
	: m_strGeometryType1("Cylinder")
	, m_strGeometryType2("Sphere")
{
}

CylinderToSphereCollisionDetector::~CylinderToSphereCollisionDetector(void)
{
}

const String& CylinderToSphereCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& CylinderToSphereCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool CylinderToSphereCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Cylinder", "Expected box geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Expected sphere geometry.")

	const Cylinder& cylinder = (Cylinder &) p_geometry1;
	const Sphere& sphere = (Sphere &) p_geometry2;

	Real rHalfHeight = cylinder.Height * (Real) 0.5;

	// compute relative sphere centre
	TPoint3<Real> ptSphereCentre(TPoint3<Real>::Origin + p_trnRelativePlacement.Translation);

	// early out (fast separation test along shaft)
	if (TMaths<Real>::Abs(ptSphereCentre.Y) > TMaths<Real>::Abs(rHalfHeight + sphere.Radius))
		return false;

	// assume closest point on cylinder to sphere centre is centre itself
	TPoint3<Real> ptClosest(ptSphereCentre);

	// clamp within end disks
	ptClosest.Y = TMaths<Real>::Min(rHalfHeight, ptClosest.Y);
	ptClosest.Y = TMaths<Real>::Max(-rHalfHeight, ptClosest.Y);

	// clamp within sides
	Real rPerpDistSquared = ptClosest.X * ptClosest.X + ptClosest.Z * ptClosest.Z;
	if (rPerpDistSquared > cylinder.RadiusSquared)
	{
		Real rPerpDistCorrection = cylinder.Radius / TMaths<Real>::Sqrt(rPerpDistSquared);
		ptClosest.X *= rPerpDistCorrection;
		ptClosest.Z *= rPerpDistCorrection;
	}

	return (ptClosest - ptSphereCentre).LengthSquared() <= sphere.RadiusSquared;
}

bool CylinderToSphereCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for Cylinder and Sphere not implemented yet.",
		__FILE__, __LINE__);
}

void CylinderToSphereCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Cylinder", "Expected box geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Expected sphere geometry.")

	const Cylinder& cylinder = (Cylinder &) p_geometry1;
	const Sphere& sphere = (Sphere &) p_geometry2;

	Real rHalfHeight = cylinder.Height * (Real) 0.5;

	// compute relative sphere centre
	TPoint3<Real> ptSphereCentre(TPoint3<Real>::Origin + p_trnRelativePlacement.Translation);

	p_contactManifold.ContactPoints.Clear();

	// early out (fast separation test along shaft)
	if (TMaths<Real>::Abs(ptSphereCentre.Y) > TMaths<Real>::Abs(rHalfHeight + sphere.Radius))
		return;

	// assume closest point on cylinder to sphere centre is centre itself
	TPoint3<Real> ptClosest(ptSphereCentre);

	// clamp within end disks
	ptClosest.Y = TMaths<Real>::Min(rHalfHeight, ptClosest.Y);
	ptClosest.Y = TMaths<Real>::Max(-rHalfHeight, ptClosest.Y);

	// clamp within sides
	Real rPerpDistSquared = ptClosest.X * ptClosest.X + ptClosest.Z * ptClosest.Z;
	if (rPerpDistSquared > cylinder.RadiusSquared)
	{
		Real rPerpDistCorrection = cylinder.Radius / TMaths<Real>::Sqrt(rPerpDistSquared);
		ptClosest.X *= rPerpDistCorrection;
		ptClosest.Z *= rPerpDistCorrection;
	}

	TVector3<Real> vecClosestOffset = ptSphereCentre - ptClosest;
	Real rDistanceClosest = vecClosestOffset.Length();
	if (rDistanceClosest > sphere.Radius)
		return;

	// handle case where box overlaps sphere centre
	TVector3<Real> vecNormal
		= vecClosestOffset.IsZero()
			? ptSphereCentre.ToVector().NormaliseCopy()
			: vecClosestOffset.NormaliseCopy();

	p_contactManifold.ContactPoints.Add(
		ContactPoint(ptClosest, vecNormal, sphere.Radius - rDistanceClosest));
}
