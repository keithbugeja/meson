#pragma once

#include "GrvForce.h"

Meson_Gravitas_Force_BEGIN

struct BasicForceState;

//------------------------------------------------------------------------------------------
/// \class BasicForce
/// A simple force generator implementation provided by the physics engine for applying
/// constant forces.
//------------------------------------------------------------------------------------------
class BasicForce
	: public IForce
{
private: // private variables
	Meson::Common::Maths::TVector3<Real> m_vecForceValue;
	Meson::Common::Maths::TPoint3<Real> m_ptApplicationPoint;
	bool m_bInstantaneous;
	
public: // public methods
	BasicForce(void);
	BasicForce(const Meson::Common::Text::String& p_strId);
	BasicForce(
		const Meson::Common::Maths::TVector3<Real>& p_vecForceValue,
		const Meson::Common::Maths::TPoint3<Real>& p_ptApplicationPoint
			= Meson::Common::Maths::TPoint3<Real>::Origin,
		bool p_bInstantaneous = false);
	~BasicForce(void);

	// IGravitasEntity interface
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>&
			p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String& p_strName, bool& p_bValue) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Maths::TVector3<Real>& p_vecValue) const;
	void GetProperty(const Meson::Common::Text::String& p_strName,
		Meson::Common::Maths::TPoint3<Real>& p_ptValue) const;
	void SetProperty(const Meson::Common::Text::String& p_strName, bool p_bValue);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Maths::TVector3<Real>& p_vecValue);
	void SetProperty(const Meson::Common::Text::String& p_strName,
		const Meson::Common::Maths::TPoint3<Real>& p_ptValue);

	// IForce interface
	Meson::Common::Maths::TVector3<Real> GetForceValue(
		Meson::Gravitas::Body::MassProperties* p_pMassProperties,
		Meson::Gravitas::Kinetics::KineticProperties* p_pKineticProperties,
		Meson::Gravitas::Geometry::GeometryPtr p_pGeometry,
		Real p_rTime);

	Meson::Common::Maths::TPoint3<Real> GetApplicationPoint(
		Meson::Gravitas::Body::MassProperties* p_pMassProperties,
		Meson::Gravitas::Kinetics::KineticProperties* p_pKineticProperties,
		Meson::Gravitas::Geometry::GeometryPtr p_pGeometry,
		Real p_rTime);

	bool IsInstantaneous(void);
};

Meson_Gravitas_Force_END
