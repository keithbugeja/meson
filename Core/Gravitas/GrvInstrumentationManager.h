#pragma once

//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Instrumentation::InstrumentationManager
//	Gravitas instrumentation manager.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		11/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Singleton.h"
#include "Enumerator.h"

#include "GrvNamespaces.h"
#include "GrvInstrumentationDevice.h"
#include "GrvEnvironment.h"

Meson_Gravitas_Instrumentation_BEGIN

namespace InstrumentationFlags
{
	//----------------------------------------------------------------------------------------------
	/// Enumeration for the instrumentation flags.
	//----------------------------------------------------------------------------------------------
	enum InstrumentationFlags
	{
		None              = 0,
		Space             = 1,
		Bodies            = 2,
		Geometry          = 4,
		Forces            = 8,
		Constraints       = 16,
		KineticIntegrator = 32,
		ConstraintSolver  = 64,
		All               = Space | Bodies | Geometry | Forces | Constraints 
							| KineticIntegrator | ConstraintSolver
	};
}

//----------------------------------------------------------------------------------------------
/// Returns a OR combination the given instrumentation flags.
/// \param p_instrumentationFlags1 First set of flags to OR.
/// \param p_instrumentationFlags2 Second set of flags to OR.
/// \returns a OR combination the given instrumentation flags.
//----------------------------------------------------------------------------------------------
InstrumentationFlags::InstrumentationFlags operator|(
	InstrumentationFlags::InstrumentationFlags p_instrumentationFlags1,
	InstrumentationFlags::InstrumentationFlags p_instrumentationFlags2);

struct InstrumentationManagerState;

//----------------------------------------------------------------------------------------------
/// \class InstrumentationManager
/// The physics engine's instrumentation manager module provides a means to render
/// instrumentation graphics related to a simulation. The instrumentation is used to illustrate
/// profiling or debugging text and graphics, usually within the simulator visuals or a separate
/// console. The manager operates through the use of an IInstrumentationDevice interface that
/// is usually implemented in the same rendering technology used for the simulator.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API InstrumentationManager
	: public Meson::Common::TSingleton<InstrumentationManager>
{
	friend class Meson::Common::TSingleton<InstrumentationManager>;

private: // private variables
	InstrumentationManagerState* m_pInstrumentationManagerState;

private: // private methods
	InstrumentationManager(void);
	~InstrumentationManager(void);

public: // public methods
	//----------------------------------------------------------------------------------------------
	/// Sets the instrumentation device used by the manager.
	/// \param p_pInstrumentationDevice Managed pointer to an instrumentation device.
	//----------------------------------------------------------------------------------------------
	void SetDevice(InstrumentationDevicePtr p_pInstrumentationDevice);

	//----------------------------------------------------------------------------------------------
	/// Returns true if all the given instrumentation flags are enabled.
	/// \param p_instrumentationFlags Instrumentation flags to test.
	//----------------------------------------------------------------------------------------------
	bool IsEnabled(InstrumentationFlags::InstrumentationFlags p_instrumentationFlags) const;

	//----------------------------------------------------------------------------------------------
	/// Enables the given instrumentation flags.
	/// \param p_instrumentationFlags Instrumentation flags to enable.
	//----------------------------------------------------------------------------------------------
	void Enable(InstrumentationFlags::InstrumentationFlags p_instrumentationFlags);

	//----------------------------------------------------------------------------------------------
	/// Disables the given instrumentation flags.
	/// \param p_instrumentationFlags Instrumentation flags to disable.
	//----------------------------------------------------------------------------------------------
	void Disable(InstrumentationFlags::InstrumentationFlags p_instrumentationFlags);

	//----------------------------------------------------------------------------------------------
	/// Renders the instrumentation of the given simulation environment at the given simulation
	/// time using the instrumentation device assigned to the manager.
	/// \param p_pEnvironment Simulation environment for which to render the instrumentation.
	/// \param p_rCurrentTime Current simulation time.
	//----------------------------------------------------------------------------------------------
	void RenderInstrumentation(Meson::Gravitas::Environment* p_pEnvironment, Real p_rCurrentTime);
};

Meson_Gravitas_Instrumentation_END
