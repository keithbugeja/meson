#pragma once

#include "GrvGeometry.h"

Meson_Gravitas_Geometry_BEGIN

class Particle :
	public IGeometry
{
private: // private variables
	short m_nId;

public:
	Particle(void);
	Particle(const Meson::Common::Text::String& p_strId);
	~Particle(void);

	// IGeometry interface
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
