#pragma once

#include "GrvGeometry.h"

Meson_Gravitas_Geometry_BEGIN

class Box :
	public IGeometry
{
private: // private variables
	short m_nId;

public: // public variables
	Meson::Common::Maths::TVector3<Real> Extent;

public: // public methods
	Box(void);
	Box(const Meson::Common::Text::String& p_strId);
	Box(const Meson::Common::Maths::TVector3<Real>& p_vecExtent);
	~Box(void);

	// IConfigurable methods
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String,
			PropertyDescriptor>& p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Maths::TVector3<Real>& p_ptValue) const;
	void SetProperty(const Meson::Common::Text::String& p_vecName,
		const Meson::Common::Maths::TVector3<Real>& p_vecValue);

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
