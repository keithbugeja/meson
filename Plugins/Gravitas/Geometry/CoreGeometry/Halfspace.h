#pragma once

#include "GrvGeometry.h"

Meson_Gravitas_Geometry_BEGIN

class Halfspace :
	public IGeometry
{
private: // private variables
	static Meson::Common::Text::String s_strTypeName;
	static Meson::Common::Maths::TMatrix3<Real> s_matDistributionTensor;
	short m_nTypeId;

public: // public variables

public: // public methods
	Halfspace(void);
	Halfspace(const Meson::Common::Text::String& p_strId);
	~Halfspace(void);

	// IConfigurable methods
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>
		&p_mapProperties) const;

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
		Meson::Common::Maths::TVector3<Real>& p_vecIntersectionPoint) const;
};

Meson_Gravitas_Geometry_END
