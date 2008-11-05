//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::GravitasEngine
//	Gravitas physics engine.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		09/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Engine.h"

#include "GrvNamespaces.h"
#include "GrvCollisionManager.h"
#include "GrvConstraintManager.h"
#include "GrvEventManager.h"
#include "GrvForceManager.h"
#include "GrvGeometryManager.h"
#include "GrvInstrumentationManager.h"
#include "GrvKineticsManager.h"
#include "GrvMaterialManager.h"
#include "GrvSimulationManager.h"
#include "GrvSpaceManager.h"

Meson_Gravitas_BEGIN

//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for  Gravitas entities.
/// \returns a pointer to the memory manager for Gravitas.
//------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager* GetGravitasMemoryManager(void);

struct GravitasEngineState;

//------------------------------------------------------------------------------------------
/// \class GravitasEngine
/// The principal interface of the Gravitas physics engine. It provides access to all the
/// manager modules within the engine, including those for collisions, constraints, events,
/// geometry, instrumentation, kinetics, simulation and spatial representation.
//------------------------------------------------------------------------------------------
class GRAVITAS_API GravitasEngine
	: public Meson::Common::TEngine<GravitasEngine>
{
	friend class Meson::Common::TEngine<GravitasEngine>;

private:
	GravitasEngineState *m_pGravitasEngineState;

protected:
	/** The engine is in itself a singleton; the constructor and destructor 
	    are protected and may only be accessed through the <i>GetInstance</i> or
		<i>Delete</i> functions.
	 */
	GravitasEngine();
	~GravitasEngine();

public:
	/** Methods through which the engine provides identification.
	 */

	const Meson::Common::Text::String& GetName(void);
	const Meson::Common::Version& GetVersion(void);

	/** Managers are retrieved through the interface provided by the engine.
	 */

	//------------------------------------------------------------------------------------------
	/// Returns a pointer to the collision manager.
	/// \returns a pointer to the collision manager.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Collision::CollisionManager* GetCollisionManager(void);

	//------------------------------------------------------------------------------------------
	/// Returns a pointer to the constraint manager.
	/// \returns a pointer to the constraint manager.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Constraint::ConstraintManager* GetConstraintManager(void);

	//------------------------------------------------------------------------------------------
	/// Returns a pointer to the event manager.
	/// \returns a pointer to the event manager.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Event::EventManager* GetEventManager(void);

	//------------------------------------------------------------------------------------------
	/// Returns a pointer to the force manager.
	/// \returns a pointer to the force manager.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Force::ForceManager* GetForceManager(void);

	//------------------------------------------------------------------------------------------
	/// Returns a pointer to the geometry manager.
	/// \returns a pointer to the geometry manager.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Geometry::GeometryManager* GetGeometryManager(void);

	//------------------------------------------------------------------------------------------
	/// Returns a pointer to the instrumentation manager.
	/// \returns a pointer to the instrumentation manager.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Instrumentation::InstrumentationManager* GetInstrumentationManager(void);

	//------------------------------------------------------------------------------------------
	/// Returns a pointer to the kinetics manager.
	/// \returns a pointer to the kinetics manager.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Kinetics::KineticsManager* GetKineticsManager(void);

	//------------------------------------------------------------------------------------------
	/// Returns a pointer to the material manager.
	/// \returns a pointer to the material manager.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Materials::MaterialManager* GetMaterialManager(void);

	//------------------------------------------------------------------------------------------
	/// Returns a pointer to the simulation manager.
	/// \returns a pointer to the simulation manager.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Simulation::SimulationManager* GetSimulationManager(void);

	//------------------------------------------------------------------------------------------
	/// Returns a pointer to the space manager.
	/// \returns a pointer to the space manager.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Space::SpaceManager* GetSpaceManager(void);
};

Meson_Gravitas_END