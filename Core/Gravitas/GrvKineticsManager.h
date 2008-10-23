//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Kinetics:KineticsManager
//	Gravitas kinetics manager.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		20/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Singleton.h"
#include "LookaheadInputStream.h"

#include "GrvNamespaces.h"
#include "GrvKineticIntegrator.h"

Meson_Gravitas_Kinetics_BEGIN

struct KineticsManagerState;

//----------------------------------------------------------------------------------------------
/// \class KineticsManager
/// The physics engine's kinetics manager module provides registration and query services for
/// kinetic intergrator implementations.
//----------------------------------------------------------------------------------------------
class KineticsManager
	: public Meson::Common::TSingleton<KineticsManager>
{
	friend class Meson::Common::TSingleton<KineticsManager>;

private: // private variables
	KineticsManagerState *m_pKineticsManagerState;

private: // private methods
	KineticsManager(void);
	~KineticsManager(void);

public:
	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of registered kinetic integrator implementations.
	/// \returns an enumerator of registered kinetic integrator implementations.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<KineticIntegratorPtr>& GetKineticIntegrators(void);

	//----------------------------------------------------------------------------------------------
	/// Returns a registered kinetic integrator indexed by the given name. If no such integrator
	/// is registered, an MesonException is thrown.
	/// \param p_strName Name of a kinetic integrator to retrieve
	/// \returns a registered kinetic integrator indexed by the given name.
	//----------------------------------------------------------------------------------------------
	KineticIntegratorPtr GetKineticIntegrator(const Meson::Common::Text::String& p_strName);

	//----------------------------------------------------------------------------------------------
	/// Registers the given kinetic integrator implementation with the manager.
	/// \param p_pKineticIntegrator Managed pointer to a kinetic integrator implementation
	///        to register.
	//----------------------------------------------------------------------------------------------
	void RegisterKineticIntegrator(
		Meson::Gravitas::Kinetics::KineticIntegratorPtr p_pKineticIntegrator);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the given kinetic integrator implementation from the manager.
	/// \param p_pKineticIntegrator Managed pointer to a kinetic integrator implementation
	///        to unregister.
	//----------------------------------------------------------------------------------------------
	void UnregisterKineticIntegrator(
		Meson::Gravitas::Kinetics::KineticIntegratorPtr p_pKineticIntegrator);

	//----------------------------------------------------------------------------------------------
	/// Unregisters all the kinetic integrator implementations from the manager.
	//----------------------------------------------------------------------------------------------
	void UnregisterAllKineticIntegrators(void);

	//----------------------------------------------------------------------------------------------
	/// Loads and configures a kinetic integrator from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        single kinetic integrator definition.
	//----------------------------------------------------------------------------------------------
	void LoadKineticIntegrator(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);

	//----------------------------------------------------------------------------------------------
	/// Loads and configures a collection of kinetic integrators from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        collection of kinetic integrator definitions.
	//----------------------------------------------------------------------------------------------
	void LoadKineticIntegrators(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);

	//----------------------------------------------------------------------------------------------
	/// Loads and configures a collection of kinetic integrators from the given file.
	/// \param p_strFilename Name of a file containing a collection of kinetic integrator
	///        definitions.
	//----------------------------------------------------------------------------------------------
	void LoadKineticIntegrators(const Meson::Common::Text::String& p_strFilename);
};

Meson_Gravitas_Kinetics_END
