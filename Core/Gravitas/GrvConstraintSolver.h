#pragma once

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvGravitasEntity.h"
#include "GrvConstraintBatch.h"

Meson_Gravitas_Constraint_BEGIN

//------------------------------------------------------------------------------------------
/// \class IConstraintSolver
/// Abstract interface for a constraint solving algorithm.
//------------------------------------------------------------------------------------------
class IConstraintSolver
	: public Meson::Gravitas::IGravitasEntity
{
public:
	virtual ~IConstraintSolver(void) {};

	//------------------------------------------------------------------------------------------
	/// Returns the name of the constraint solver.
	/// \returns the name of the constraint solver.
	//------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetName(void) const = 0;

	//------------------------------------------------------------------------------------------
	/// Solves the given batch of constraints. A batch is the smallest unit of input for the
	/// solver and should consist of constraints that interact only with each other.
	/// \param p_constraintBatch Batch of constraints to solve.
	//------------------------------------------------------------------------------------------
	virtual void Solve(ConstraintBatch& p_constraintBatch) = 0;
};

//------------------------------------------------------------------------------------------
/// Managed pointer to a constraint solver.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IConstraintSolver> ConstraintSolverPtr;

Meson_Gravitas_Constraint_END
