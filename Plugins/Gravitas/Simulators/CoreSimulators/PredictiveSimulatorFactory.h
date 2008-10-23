#pragma once

#include "GrvSimulatorFactory.h"

Meson_Gravitas_Simulation_BEGIN

class PredictiveSimulatorFactory
	: public Meson::Gravitas::Simulation::ISimulatorFactory
{
private: // private variables
	Meson::Common::Text::String m_strName;

public: // public methods
	PredictiveSimulatorFactory(void);
	~PredictiveSimulatorFactory(void);
	const Meson::Common::Text::String &GetName(void);
	SimulatorPtr CreateSimulator(void);
};

Meson_Gravitas_Simulation_END
