#pragma once

#include "String8.h"

#include "GrvNamespaces.h"
#include "GrvSimulator.h"

Meson_Gravitas_Simulation_BEGIN

//----------------------------------------------------------------------------------------------
/// \class ISimulatorFactory
/// Abstract factory interface for simulator instances. Implementations of this interface
/// generate specific simulator implementations and are normally loaded through the engine's
/// plugin mechanism.
//----------------------------------------------------------------------------------------------
class ISimulatorFactory
{
public:
	virtual ~ISimulatorFactory(void) {};

	//----------------------------------------------------------------------------------------------
	/// Returns the name of the simulator factory.
	/// \returns the name of the simulator factory.
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetName(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a managed pointer to a newly created instance of a simulator associated with this
	/// factory.
	/// \returns a managed pointer to a newly created instance of a simulator.
	//----------------------------------------------------------------------------------------------
	virtual SimulatorPtr CreateSimulator(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Creates and returns a new simulator instance with the given ID.
	/// \param p_strId ID to assign to the simulator instance.
	/// \returns a new simulator instance.
	//----------------------------------------------------------------------------------------------
	virtual SimulatorPtr CreateSimulator(const Meson::Common::Text::String& p_strId) = 0;
};

Meson_Gravitas_Simulation_END
