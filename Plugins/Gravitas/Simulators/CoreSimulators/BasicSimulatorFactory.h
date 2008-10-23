#pragma once

#include "GrvSimulatorFactory.h"

Meson_Gravitas_Simulation_BEGIN

class BasicSimulatorFactory
	: public Meson::Gravitas::Simulation::ISimulatorFactory
{
private: // private variables
	Meson::Common::Text::String m_strName;

public: // public methods
	BasicSimulatorFactory(void);
	~BasicSimulatorFactory(void);
	const Meson::Common::Text::String &GetName(void);
	SimulatorPtr CreateSimulator(void);
	SimulatorPtr CreateSimulator(const Meson::Common::Text::String& p_strId);
};

Meson_Gravitas_Simulation_END
