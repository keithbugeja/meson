#pragma once

#include "GrvKineticIntegrator.h"

Meson_Gravitas_Kinetics_BEGIN

class RK4KineticIntegrator
	: public IKineticIntegrator
{
private: // private methods

public: // public methods
	RK4KineticIntegrator(void);
	~RK4KineticIntegrator(void);

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
