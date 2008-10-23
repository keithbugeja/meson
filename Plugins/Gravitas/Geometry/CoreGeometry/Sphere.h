#pragma once

#include "GrvGeometry.h"

Meson_Gravitas_Geometry_BEGIN

class Sphere :
	public IGeometry
{
public: // public variables
	short m_nId;
	Real Radius;
	Real RadiusSquared;

public: // public methods
	Sphere(void);
	Sphere(const Meson::Common::Text::String& p_strId);
	~Sphere(void);

	// IConfigurable methods
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>
		&p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String& p_strName, Real& p_rValue) const;
	void SetProperty(const Meson::Common::Text::String& p_strName, Real p_rValue);

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

	// custom methods
	Real GetRadius(void);
	Real GetRadiusSquared(void);
	void SetRadius(Real p_rRadius);
};

Meson_Gravitas_Geometry_END
