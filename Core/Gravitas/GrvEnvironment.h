#pragma once

#include "Pointer.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvBody.h"
#include "GrvSpace.h"
#include "GrvCollisionFilter.h"
#include "GrvKineticIntegrator.h"
#include "GrvConstraintSolver.h"

Meson_Gravitas_BEGIN

//----------------------------------------------------------------------------------------------
/// \struct Environment
/// A representation of a physical environment to be simulated, consisting of the spatial
/// representation containing the bodies, inter-body constraints, an optional collision filter,
/// a kinetic integrator and a constraint solver.
//----------------------------------------------------------------------------------------------
struct Environment
	: public Meson::Common::TReferenceCounter<Environment>
{
	Meson::Common::Text::String Id;

	//----------------------------------------------------------------------------------------------
	/// Managed pointer to a spatial representation containing the bodies involved in the
	/// simulation.
	//----------------------------------------------------------------------------------------------
	Meson::Gravitas::Space::SpacePtr Space;

	//----------------------------------------------------------------------------------------------
	/// Managed pointer to a list of constraints imposed on the simulation.
	//----------------------------------------------------------------------------------------------
	Meson::Gravitas::Constraint::ConstraintListPtr Constraints;

	//----------------------------------------------------------------------------------------------
	/// Managed pointer to an optional collision filter to restrict collisions by geometry types,
	//// or specific body groups.
	//----------------------------------------------------------------------------------------------
	Meson::Gravitas::Collision::CollisionFilterPtr CollisionFilter;

	//----------------------------------------------------------------------------------------------
	/// Managed pointer to a kinetic integrator used to drive the motion of the bodies.
	//----------------------------------------------------------------------------------------------
	Meson::Gravitas::Kinetics::KineticIntegratorPtr KineticIntegrator;

	//----------------------------------------------------------------------------------------------
	/// Pointer to a solver implementation for the constraints in the simulation.
	//----------------------------------------------------------------------------------------------
	Meson::Gravitas::Constraint::IConstraintSolver* ConstraintSolver;

	//----------------------------------------------------------------------------------------------
	/// Constructs an empty environment.
	//----------------------------------------------------------------------------------------------
	Environment(void);

	Environment(const Meson::Common::Text::String& p_strId);

	//----------------------------------------------------------------------------------------------
	/// Constructs an environment from the given source.
	/// \param p_environment Source environment to copy.
	//----------------------------------------------------------------------------------------------
	Environment(const Environment& p_environment);

	//----------------------------------------------------------------------------------------------
	/// Destroys the environment.
	//----------------------------------------------------------------------------------------------
	~Environment(void);
};

//----------------------------------------------------------------------------------------------
/// Managed pointer to an environment structure.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<Environment> EnvironmentPtr;

Meson_Gravitas_END
