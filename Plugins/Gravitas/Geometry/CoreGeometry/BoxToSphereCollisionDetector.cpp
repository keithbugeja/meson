#include "BoxToSphereCollisionDetector.h"

#include "Box.h"
#include "Sphere.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

BoxToSphereCollisionDetector::BoxToSphereCollisionDetector(void)
	: m_strGeometryType1("Box")
	, m_strGeometryType2("Sphere")
{
}

BoxToSphereCollisionDetector::~BoxToSphereCollisionDetector(void)
{
}

const String& BoxToSphereCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& BoxToSphereCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool BoxToSphereCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Box", "Expected box geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Expected sphere geometry.")

	const Box& box = (Box &) p_geometry1;
	const Sphere& sphere = (Sphere &) p_geometry2;

	BoundingAxisAlignedBox boundingAxisAlignedBox(
		TPoint3<Real>::Origin - box.Extent,
		TPoint3<Real>::Origin + box.Extent);

	BoundingSphere boundingSphere(
		TPoint3<Real>::Origin + p_trnRelativePlacement.Translation,
		sphere.Radius);

	return boundingAxisAlignedBox.Intersects(boundingSphere);
}

bool BoxToSphereCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for Box and Sphere not implemented yet.",
		__FILE__, __LINE__);
}

void BoxToSphereCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "Box", "Expected box geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Expected sphere geometry.")

	const Box& box = (Box &) p_geometry1;
	const Sphere& sphere = (Sphere &) p_geometry2;

	BoundingAxisAlignedBox boundingAxisAlignedBox(
		TPoint3<Real>::Origin - box.Extent,
		TPoint3<Real>::Origin + box.Extent);

	TPoint3<Real> ptSphereOrigin
		= TPoint3<Real>::Origin + p_trnRelativePlacement.Translation;
	TPoint3<Real> ptClosestPoint;
	
	boundingAxisAlignedBox.ClosestPointTo(ptSphereOrigin, ptClosestPoint);

	p_contactManifold.ContactPoints.Clear();

	TVector3<Real> vecOffset = ptSphereOrigin - ptClosestPoint;
	Real rOffset = vecOffset.Length();
	if (rOffset > sphere.Radius)
		return;

	// handle case where box overlaps sphere centre
	TVector3<Real> vecNormal(vecOffset);
	if (vecNormal.IsZero())
	{
		if (ptSphereOrigin.IsOrigin())
			vecNormal = TVector3<Real>::Right;
		else
			vecNormal = ptSphereOrigin.ToVector().NormaliseCopy();
	}
	else
		vecNormal.Normalise();

	p_contactManifold.ContactPoints.Add(
		ContactPoint(ptClosestPoint, vecNormal, sphere.Radius - rOffset));
}
