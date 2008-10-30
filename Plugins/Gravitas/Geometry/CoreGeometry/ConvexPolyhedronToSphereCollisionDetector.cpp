#include "ConvexPolyhedronToSphereCollisionDetector.h"

#include "ArrayList.h"

#include "ConvexPolyhedron.h"
#include "Sphere.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Geometry;

ConvexPolyhedronToSphereCollisionDetector::ConvexPolyhedronToSphereCollisionDetector(void)
	: m_strGeometryType1("ConvexPolyhedron")
	, m_strGeometryType2("Sphere")
{
}

ConvexPolyhedronToSphereCollisionDetector::~ConvexPolyhedronToSphereCollisionDetector(void)
{
}

const String& ConvexPolyhedronToSphereCollisionDetector::GetGeometryType1(void)
{
	return m_strGeometryType1;
}

const String& ConvexPolyhedronToSphereCollisionDetector::GetGeometryType2(void)
{
	return m_strGeometryType2;
}

bool ConvexPolyhedronToSphereCollisionDetector::TestIntersection(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "ConvexPolyhedron", "Expected convex polyhedron geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Expected sphere geometry.")

	const ConvexPolyhedron& convexPolyhedron = (ConvexPolyhedron &) p_geometry1;
	const Sphere& sphere = (Sphere &) p_geometry2;

	// transform sphere centre to polyhedron space
	const TVector3<Real>& vecSphereCentre = p_trnRelativePlacement.Translation;

	TVector3<Real> vecClosest = convexPolyhedron.GetClosestPoint(vecSphereCentre);
	TVector3<Real> vecOffset(vecSphereCentre - vecClosest);

	return vecOffset.LengthSquared() <= sphere.RadiusSquared;
}

bool ConvexPolyhedronToSphereCollisionDetector::EstimateImpact(
	const IGeometry& p_geometry1,
	const IGeometry& p_geometry2,
	const Transform& p_trnRelativePlacement,
	const TVector3<Real>& p_vecRelativeLinearVelocity,
	const TVector3<Real>& p_vecRelativeAngularVelocity,
	Real p_rMaxTime,
	Real& p_rTime)
{
	throw new MesonException(
		"EstimateImpact for convex polyhedron and sphere not implemented yet.",
		__FILE__, __LINE__);
}

void ConvexPolyhedronToSphereCollisionDetector::ComputeContactManifold(
	const Meson::Gravitas::Geometry::IGeometry& p_geometry1,
	const Meson::Gravitas::Geometry::IGeometry& p_geometry2,
	const Meson::Gravitas::Geometry::Transform& p_trnRelativePlacement,
	ContactManifold& p_contactManifold)
{
	MESON_ASSERT(p_geometry1.GetTypeName() == "ConvexPolyhedron", "Expected convex polyhedron geometry.")
	MESON_ASSERT(p_geometry2.GetTypeName() == "Sphere", "Expected sphere geometry.")

	const ConvexPolyhedron& convexPolyhedron = (ConvexPolyhedron &) p_geometry1;
	const Sphere& sphere = (Sphere &) p_geometry2;

	// transform sphere centre to polyhedron space
	const TVector3<Real>& vecSphereCentre = p_trnRelativePlacement.Translation;

	// get closest point in polyhedron to sphere centre
	TVector3<Real> vecClosest = convexPolyhedron.GetClosestPoint(vecSphereCentre);

	// compute offset between sphere centre and point
	TVector3<Real> vecOffset(vecSphereCentre - vecClosest);

	// clear manifold
	p_contactManifold.ContactPoints.Clear();

	// compute penetration
	Real rPenetration(sphere.Radius - vecOffset.Length());

	// negative penetration implies non-penetration
	if (rPenetration < (Real) 0.0)
		return;

	// robust computation of normal
	static TVector3<Real> vecNormal;
	if (!vecOffset.IsZero())
		vecNormal = vecOffset.NormaliseCopy();
	else if (!vecSphereCentre.IsZero())
		vecNormal = vecSphereCentre.NormaliseCopy();
	else
		vecNormal = TVector3<Real>::Up;

	p_contactManifold.ContactPoints.Add(ContactPoint(vecClosest, vecNormal, rPenetration));
}
