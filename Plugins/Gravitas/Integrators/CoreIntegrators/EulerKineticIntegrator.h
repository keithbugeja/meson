#pragma once

#include "GrvKineticIntegrator.h"

Meson_Gravitas_Kinetics_BEGIN

class EulerKineticIntegrator
	: public IKineticIntegrator
{
private: // private methods
	size_t m_unSubdivisions;
	Real m_rSubdivisionsInverse;

public: // public methods
	EulerKineticIntegrator(void);
	~EulerKineticIntegrator(void);

	// IGravitasEntity interface
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>&
			p_mapProperties) const;
	void GetProperty(const Meson::Common::Text::String& p_strName, int &p_nValue) const;
	void SetProperty(const Meson::Common::Text::String& p_strName, int p_nValue);

	// IKineticIntegrator interface
	const Meson::Common::Text::String& GetName(void) const;
	void IntegratePosition(
		Real p_rCurrentTime, Real p_rDeltaTime,
		IVectorTimeFunction& p_linearAcceleration,
		IVectorTimeFunction& p_angularAcceleration,
		KineticProperties& p_kineticProperties);
	void IntegrateVelocity(
		Real p_rCurrentTime, Real p_rDeltaTime,
		IVectorTimeFunction& p_linearAcceleration,
		IVectorTimeFunction& p_angularAcceleration,
		KineticProperties& p_kineticProperties);
};

Meson_Gravitas_Kinetics_END
