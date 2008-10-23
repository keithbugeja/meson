#pragma once

#include "GrvSimulator.h"

#include "CollisionTimeEstimator.h"

Meson_Gravitas_Simulation_BEGIN

class PredictiveSimulator
	: public ISimulator
{
private: // private variables
	Meson::Gravitas::EnvironmentPtr m_pEnvironment;
	CollisionTimeEstimator m_collisionTimeEstimator;

private: // private methods
	void IntegrateKinetics(Real p_rCurrentTime, Real p_rDeltaTime);

public: // public methods
	PredictiveSimulator(void);
	~PredictiveSimulator(void);

	// ISimulator interface
	void Initialise(Meson::Gravitas::EnvironmentPtr p_pEnvironment);
	void Terminate(void);

	void Step(Real p_rCurrentTime, Real p_rDeltaTime);
};

Meson_Gravitas_Simulation_END
