//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Simulation::SimulationManager
//	Gravitas simulation manager.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		25/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Singleton.h"
#include "LookaheadInputStream.h"

#include "GrvNamespaces.h"
#include "GrvSimulatorFactory.h"

Meson_Gravitas_Simulation_BEGIN

struct SimulationManagerState;

//----------------------------------------------------------------------------------------------
/// \class SimulationManager
/// The physics engine's simulation manager provides registry and query services for simulation
/// factory implementations.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API SimulationManager
	: public Meson::Common::TSingleton<SimulationManager>
{
	friend class Meson::Common::TSingleton<SimulationManager>;

private: // private variables
	SimulationManagerState *m_pSimulationManagerState;

private: // private methods
	SimulationManager(void);
	~SimulationManager(void);

public:
	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of simulator factories registered with the manager.
	/// \returns an enumerator of simulator factories registered with the manager.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<ISimulatorFactory*>& GetSimulatorFactories(void);

	//----------------------------------------------------------------------------------------------
	/// Returns a simulator factory indexed by the given name. If the factory is not found a
	/// MesonException is thrown.
	/// \param p_strName Name of the simulator factory to retrieve.
	/// \returns a pointer to the simulator factory implementation.
	//----------------------------------------------------------------------------------------------
	ISimulatorFactory* GetSimulatorFactory(const Meson::Common::Text::String& p_strName);

	//----------------------------------------------------------------------------------------------
	/// Registers the given simulator factory implementation with the manager.
	/// \param p_pSimulatorFactory Pointer to a simulator factory to registor.
	//----------------------------------------------------------------------------------------------
	void RegisterSimulatorFactory(Meson::Gravitas::Simulation::ISimulatorFactory* p_pSimulatorFactory);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the given simulator factory implementation from the manager.
	/// \param p_pSimulatorFactory Pointer to a simulator factory to unregistor.
	//----------------------------------------------------------------------------------------------
	void UnregisterSimulatorFactory(Meson::Gravitas::Simulation::ISimulatorFactory* p_pSimulatorFactory);

	//----------------------------------------------------------------------------------------------
	/// Unregisters all the simulator factory implementations from the manager.
	//----------------------------------------------------------------------------------------------
	void UnregisterAllSimulatorFactories(void);

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of simulator instances registered with the manager.
	/// \returns an enumerator of simulator instances registered with the manager.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<SimulatorPtr>& GetSimulators(void);

	//----------------------------------------------------------------------------------------------
	/// Returns a simulator instance indexed by the given ID. If the simulator is not found a
	/// MesonException is thrown.
	/// \param p_strId ID of the simulator instance to retrieve.
	/// \returns a managed pointer to the simulator instance.
	//----------------------------------------------------------------------------------------------
	SimulatorPtr GetSimulator(const Meson::Common::Text::String& p_strId);

	//----------------------------------------------------------------------------------------------
	/// Registers the given simulator instance with the manager.
	/// \param p_pSimulator Managed pointer to a simulator instance to registor.
	//----------------------------------------------------------------------------------------------
	void RegisterSimulator(Meson::Gravitas::Simulation::SimulatorPtr p_pSimulator);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the given simulator instance from the manager.
	/// \param p_pSimulator Managed pointer to a simulator instance to unregistor.
	//----------------------------------------------------------------------------------------------
	void UnregisterSimulator(Meson::Gravitas::Simulation::SimulatorPtr p_pSimulator);

	//----------------------------------------------------------------------------------------------
	/// Unregisters all the simulator instances from the manager.
	//----------------------------------------------------------------------------------------------
	void UnregisterAllSimulators(void);

	//----------------------------------------------------------------------------------------------
	/// Loads and registers a simulator instance from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        single simulator definition.
	/// \returns Managed pointer to a simulator instance.
	//----------------------------------------------------------------------------------------------
	SimulatorPtr LoadSimulator(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of environment instances registered with the manager.
	/// \returns an enumerator of environment instances registered with the manager.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<EnvironmentPtr>& GetEnvironments(void);

	//----------------------------------------------------------------------------------------------
	/// Returns an environment instance indexed by the given ID. If the environment is not found a
	/// MesonException is thrown.
	/// \param p_strId ID of the environment instance to retrieve.
	/// \returns a managed pointer to the environment instance.
	//----------------------------------------------------------------------------------------------
	EnvironmentPtr GetEnvironment(const Meson::Common::Text::String& p_strId);

	//----------------------------------------------------------------------------------------------
	/// Registers the given environment instance with the manager.
	/// \param p_pEnvironment Managed pointer to an environment instance to registor.
	//----------------------------------------------------------------------------------------------
	void RegisterEnvironment(Meson::Gravitas::EnvironmentPtr p_pEnvironment);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the given environment instance from the manager.
	/// \param p_pEnvironment Managed pointer to an environment instance to unregistor.
	//----------------------------------------------------------------------------------------------
	void UnregisterEnvironment(Meson::Gravitas::EnvironmentPtr p_pEnvironment);

	//----------------------------------------------------------------------------------------------
	/// Unregisters all the environment instances from the manager.
	//----------------------------------------------------------------------------------------------
	void UnregisterAllEnvironments(void);

	//----------------------------------------------------------------------------------------------
	/// Loads and registers an environment instance from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        single environment definition.
	/// \returns Managed pointer to an environment instance.
	//----------------------------------------------------------------------------------------------
	EnvironmentPtr LoadEnvironment(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);
};

Meson_Gravitas_Simulation_END
