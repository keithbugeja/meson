#pragma once

#include "String8.h"
#include "Matrix3.h"
#include "Point3.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvGravitasEntity.h"
#include "GrvBoundingSphere.h"
#include "GrvBoundingAxisAlignedBox.h"
#include "GrvBoundingOrientedBox.h"
#include "GrvRay.h"

Meson_Gravitas_Geometry_BEGIN

//----------------------------------------------------------------------------------------------
/// \class IGeometry
/// Abstract interface for body geometry. Implementations of the interface provide computations
/// of density-independent mass properties, functions for computing bounding volumes and ray
/// intersection operations. The goemetry is assumed in local body coordinates and may represent
/// both soft and rigid bodies. For boundary-only implementations (non-solid), the mass-related
/// methods should be computed on the surface area or should at least return non-zero values to
/// avoid overflows when computing mass and inertia tensor inverses.
//----------------------------------------------------------------------------------------------
class IGeometry
	: public Meson::Gravitas::IGravitasEntity
{
public:
	IGeometry(void) { };
	IGeometry(const Meson::Common::Text::String& p_strId) : IGravitasEntity(p_strId) { };
	virtual ~IGeometry(void) { };

	//----------------------------------------------------------------------------------------------
	/// Returns the type name for the implementation of this geometry.
	/// \returns the type name for the implementation of this geometry.
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetTypeName(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a type identifier for this geometry. This numeric identifier is assigned through
	/// a type name mapping maintained by the physics engine's collision manager.
	/// \returns a type identifier for this geometry
	//----------------------------------------------------------------------------------------------
	virtual short GetTypeId(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Assigns the given type identifier to the geometry. This numeric identifier is assigned
	/// through a type name mapping maintained by the physics engine's collision manager.
	/// \param p_nTypeId Type identifier to assign.
	//----------------------------------------------------------------------------------------------
	virtual void SetTypeId(short p_nTypeId) = 0;

	// mass properties

	//----------------------------------------------------------------------------------------------
	/// Returns the volume of the geometry. This is often multiplied by a constant density value to
	/// yield a mass for the body.
	/// \returns the volume of the geometry.
	//----------------------------------------------------------------------------------------------
	virtual Real GetVolume(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a 3x3 distribution tensor matrix for the geometry. This is often multiplied by a
	/// constant density value to yield a mass inertia tensor for the body.
	/// \returns the distribution tensor of the geometry.
	//----------------------------------------------------------------------------------------------
	virtual Meson::Common::Maths::TMatrix3<Real> GetDistributionTensor(void) const = 0;

	// bounding volume

	//----------------------------------------------------------------------------------------------
	/// Returns true if the geometry implementation can be contained in a bounding volume of finite
	/// size. An example of an unabounded geometry implementation is a plane boundary or halfspace.
	/// \returns true if the geometry is bounded.
	//----------------------------------------------------------------------------------------------
	virtual bool IsBounded(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Computes a bounding sphere for the geometry. The method should throw an excpetion if the
	/// geometry is unbounded.
	/// \param p_boundingSphere Bounding sphere to populate.
	//----------------------------------------------------------------------------------------------
	virtual void ComputeBoundingVolume(BoundingSphere& p_boundingSphere) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Computes an axis-aligned bounding box for the geometry. The method should throw an excpetion
	/// if the geometry is unbounded.
	/// \param p_boundingAxisAlignedBox Axis-aligned bounding box to populate.
	//----------------------------------------------------------------------------------------------
	virtual void ComputeBoundingVolume(BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Computes an oriented bounding box for the geometry. The method should throw an excpetion if
	/// the geometry is unbounded.
	/// \param p_boundingOrientedBox Oriented bounding box to populate.
	//----------------------------------------------------------------------------------------------
	virtual void ComputeBoundingVolume(BoundingOrientedBox& p_boundingOrientedBox) const = 0;

	// ray casting

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given ray intersects the geometry.
	/// \param p_ray Ray to test.
	/// \returns true if the given ray intersects the geometry.
	//----------------------------------------------------------------------------------------------
	virtual bool IntersectsRay(const Ray& p_ray) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given ray intersects the geometry. If the intersection test is positive,
	/// the given intersection point is updated.
	/// \param p_ray Ray to test.
	/// \param p_ptIntersectionPoint Computed intersection point.
	/// \returns true if the given ray intersects the geometry.
	//----------------------------------------------------------------------------------------------
	virtual bool IntersectsRay(const Ray& p_ray,
		Meson::Common::Maths::TPoint3<Real>& p_ptIntersectionPoint) const = 0;
};

//----------------------------------------------------------------------------------------------
/// Managed pointer to a geometry instance.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IGeometry> GeometryPtr;

Meson_Gravitas_Geometry_END
