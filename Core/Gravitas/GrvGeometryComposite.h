#pragma once

#include "GrvGeometry.h"

Meson_Gravitas_Geometry_BEGIN

struct GRAVITAS_API GeometryComponent
{
	Meson::Gravitas::Geometry::Transform Transform;
	GeometryPtr Geometry;

	GeometryComponent(void);
	GeometryComponent(
		const Meson::Gravitas::Geometry::Transform& p_pTransform,
		GeometryPtr p_pGeometry);

	GeometryComponent& operator=(const GeometryComponent& p_geometryComponent);
	bool operator==(const GeometryComponent& p_geometryComponent);
};

struct GeometryCompositeState;

//----------------------------------------------------------------------------------------------
/// \class GeometryComposite
/// A composite geometry implementation for building unions of component geometries. The
/// composite consists of a number of simpler geometry instances, each positioned with respect
/// to the composite geometry frame using a transform instance. All the geometry functionality
/// provided by the composite is implemented in terms of its component geometry instances. The
/// composite geometry is considered bounded only if all its constituents are bounded. The
/// bounding volume is computed using conservative algorithms for combining the constituent
/// volumes. Ray intersection tests are performed against the constituent geomety where the
/// poiint of intersection is taken as the closest point found.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API GeometryComposite
	: public IGeometry
{
private: // private variables
	GeometryCompositeState *m_pGeometryCompositeState;

private: // private methods
	Meson::Common::Maths::TVector3<Real> GetCentroid(void) const;

public: // public methods

	// constructors

	//----------------------------------------------------------------------------------------------
	/// Constructs an empty geometry composite.
	//----------------------------------------------------------------------------------------------
	GeometryComposite(void);

	//----------------------------------------------------------------------------------------------
	/// Destorys the geometry composite.
	//----------------------------------------------------------------------------------------------
	~GeometryComposite(void);

	// IGeometry interface
	const Meson::Common::Text::String& GetTypeName(void) const;
	short GetTypeId(void) const;
	void SetTypeId(short p_nTypeId);

	Real GetVolume(void) const;
	Meson::Common::Maths::TMatrix3<Real> GetDistributionTensor(void) const;

	bool IsBounded(void) const;
	void ComputeBoundingVolume(BoundingSphere& p_boundingSphere) const;
	void ComputeBoundingVolume(BoundingAxisAlignedBox& p_boundingAxisAlignedBox) const;
	void ComputeBoundingVolume(BoundingOrientedBox& p_boundingOrientedBox) const;

	bool IntersectsRay(const Ray& p_ray) const;
	bool IntersectsRay(const Ray& p_ray,
		Meson::Common::Maths::TVector3<Real>& p_vecIntersectionPoint) const;

	// custom interface

	//----------------------------------------------------------------------------------------------
	/// Returns a list of the constituent geometry instances.
	/// \returns a list of the constituent geometry instances.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TImmutableList<GeometryComponent>& GetComponentGeometries(void);

	//----------------------------------------------------------------------------------------------
	/// Adds the given geometry as a component of the composite geometry.
	/// \param p_transform Transform representing the relative position of the component geometry
	///        with respect to the composite geometry frame.
	/// \param p_pGeometry Component geometry instance to add to this composite.
	//----------------------------------------------------------------------------------------------
	void AddGeometry(
		const Meson::Gravitas::Geometry::Transform& p_transform,
		GeometryPtr p_pGeometry);

	//----------------------------------------------------------------------------------------------
	/// Removes the given geometry component from the composite geometry. The transform is required
	/// to discriminate between multiple instances of the same component geometry added at different
	/// positions.
	/// \param p_transform Transform representing the relative position of the component geometry
	///        with respect to the composite geometry frame.
	/// \param p_pGeometry Component geometry instance to add to this composite.
	//----------------------------------------------------------------------------------------------
	void RemoveGeometry(
		const Meson::Gravitas::Geometry::Transform& p_transform,
		GeometryPtr p_pGeometry);

	//----------------------------------------------------------------------------------------------
	/// Removes all instances of the given component geometry from the composite.
	/// \param p_pGeometry Component geometry instance to remove.
	//----------------------------------------------------------------------------------------------
	void RemoveGeometry(
		GeometryPtr p_pGeometry);

	//----------------------------------------------------------------------------------------------
	/// Applies a translation to all component geometry transforms to reallign the compount centroid
	/// with the composite geometry frame origin.
	//----------------------------------------------------------------------------------------------
	void RealignCentroid(void);
};

Meson_Gravitas_Geometry_END
