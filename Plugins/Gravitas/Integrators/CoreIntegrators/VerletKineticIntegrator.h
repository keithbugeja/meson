#pragma once

#include "GrvKineticIntegrator.h"

Meson_Gravitas_Kinetics_BEGIN

class VerletKineticIntegrator
	: public IKineticIntegrator
{
private: // private methods
	size_t m_unSubdivisions;
	Real m_rSubdivisionsInverse;

public: // public methods
	VerletKineticIntegrator(void);
	~VerletKineticIntegrator(void);

	// IGravitasEntity interface
	void EnumerateProperties(
		Meson::Common::Collections::TMap<Meson::Common::Text::String, PropertyDescriptor>&
			p_mapProperties);
	void GetProperty(const Meson::Common::Text::String& p_strName, int &p_nValue);
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
