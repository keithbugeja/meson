#pragma once

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvEnvironment.h"

Meson_Gravitas_Simulation_BEGIN

//----------------------------------------------------------------------------------------------
/// \class ISimulator
/// Interface for a simulator instance. The interface provides methods for initialising,
/// stepping through, and terminating the simulator instance.
//----------------------------------------------------------------------------------------------
class ISimulator
	: public Meson::Gravitas::IGravitasEntity
{
public: // public methods
	ISimulator(void) : IGravitasEntity() {};
	ISimulator(const Meson::Common::Text::String& p_strId) : IGravitasEntity(p_strId) {};
	virtual ~ISimulator(void) {};

	//----------------------------------------------------------------------------------------------
	/// Initialises the simulator with the given simulation environment.
	/// \param p_pEnvironment A managed pointer to a simulation environment for which to initialise
	///        the simulator.
	//----------------------------------------------------------------------------------------------
	virtual void Initialise(Meson::Gravitas::EnvironmentPtr p_pEnvironment) = 0;

	//----------------------------------------------------------------------------------------------
	/// Terminates the simulator. Any data required for termination are assumed to be given at the
	/// initialisation stage.
	//----------------------------------------------------------------------------------------------
	virtual void Terminate(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Steps through the simulator for the given current time and time delta.
	/// \param p_rCurrentTime Current time of the simulator expressed relative to the initialisation
	///        time.
	/// \param p_rDeltaTime Time interval through which to step the simulator.
	//----------------------------------------------------------------------------------------------
	virtual void Step(Real p_rCurrentTime, Real p_rDeltaTime) = 0;
};

//----------------------------------------------------------------------------------------------
/// Managed pointer to a simulator.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<ISimulator> SimulatorPtr;

Meson_Gravitas_Simulation_END
