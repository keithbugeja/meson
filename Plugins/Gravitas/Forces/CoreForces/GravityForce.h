#pragma once

#include "GrvForce.h"

Meson_Gravitas_Force_BEGIN

class GravityForce
	: public IForce
{
private: // private variables
	Meson::Common::Maths::TVector3<Real> m_vecAcceleration;
	
public: // public methods
	GravityForce(void);
	GravityForce(const Meson::Common::Text::String& p_strId);
	~GravityForce(void);

	// IGravitasEntity interface
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>&
			p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Maths::TVector3<Real>& p_vecValue) const;
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Maths::TVector3<Real>& p_vecValue);

	// IForce interface
	Meson::Common::Maths::TVector3<Real> GetForceValue(
		Meson::Gravitas::Body::MassProperties* p_pMassProperties,
		Meson::Gravitas::Kinetics::KineticProperties* p_pKineticProperties,
		Meson::Gravitas::Geometry::GeometryPtr p_pGeometry,
		Real p_rTime);

	Meson::Common::Maths::TVector3<Real> GetApplicationPoint(
		Meson::Gravitas::Body::MassProperties* p_pMassProperties,
		Meson::Gravitas::Kinetics::KineticProperties* p_pKineticProperties,
		Meson::Gravitas::Geometry::GeometryPtr p_pGeometry,
		Real p_rTime);

	bool IsInstantaneous(void);
};

Meson_Gravitas_Force_END
