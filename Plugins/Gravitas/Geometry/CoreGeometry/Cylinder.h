#pragma once

#include "GrvGeometry.h"

Meson_Gravitas_Geometry_BEGIN

class Cylinder :
	public IGeometry
{
public: // public variables
	short m_nId;
	Real Height;
	Real HeightSquared;
	Real Radius;
	Real RadiusSquared;

public: // public static methods
	static void ProjectToInterval(
		const Meson::Gravitas::Geometry::LineSegment &p_lnsShaft, Real p_rRadius,
		const Meson::Common::Maths::TVector3<Real>& p_vecAxis, 
		Meson::Common::Maths::TInterval<Real>& p_interval);
	static void EnumerateMaximalVertices(
		const Meson::Gravitas::Geometry::LineSegment &p_lnsShaft, Real p_rRadius,
		const Meson::Common::Maths::TVector3<Real>& p_vecAxis,
		Meson::Gravitas::PointList& p_listVertices);
	static bool IntersectsPoint(
		const Meson::Gravitas::Geometry::LineSegment &p_lnsShaft, Real p_rRadius,
		const Meson::Common::Maths::TPoint3<Real>& p_ptPoint);

public: // public methods
	Cylinder(void);
	Cylinder(const Meson::Common::Text::String& p_strId);
	~Cylinder(void);

	// IConfigurable methods
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String,
			PropertyDescriptor>& p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Real& p_rValue) const;
	void SetProperty(const Meson::Common::Text::String& p_vecName,
		Real p_vecValue);

	// IGeometry methods
	const Meson::Common::Text::String& GetTypeName(void) const;
	short GetTypeId(void) const;
	void SetTypeId(short p_nId);
	Real GetVolume(void) const;
	Meson::Common::Maths::TMatrix3<Real> GetDistributionTensor(void) const;

	bool IsBounded(void) const;
	void ComputeBoundingVolume(BoundingSphere &p_boundingSphere) const;
	void ComputeBoundingVolume(BoundingAxisAlignedBox &p_boundingAxisAlignedBox) const;
	void ComputeBoundingVolume(BoundingOrientedBox &p_boundingOrientedBox) const;

	bool IntersectsRay(const Ray& p_ray) const;
	bool IntersectsRay(const Ray& p_ray,
		Meson::Common::Maths::TPoint3<Real>& p_ptIntersectionPoint) const;
};

Meson_Gravitas_Geometry_END
