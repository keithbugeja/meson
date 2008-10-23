#pragma once

#include "ArrayList.h"
#include "LinkedList.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvConstraint.h"

Meson_Gravitas_Constraint_BEGIN

//------------------------------------------------------------------------------------------
/// \struct ConstraintBatch
/// A structure for grouping a number of constraints. The batching process is usually the
/// result of a body island processing phase that partitions constraints into disjoint sets
/// for processing by a constraint solver. The ConstraintBatchAggregator class provides this
/// functinality.
//------------------------------------------------------------------------------------------
struct ConstraintBatch
{
	//------------------------------------------------------------------------------------------
	/// Current time of the simulator.
	//------------------------------------------------------------------------------------------
	Real Time;

	//------------------------------------------------------------------------------------------
	/// List of constraints contained in the batch.
	//------------------------------------------------------------------------------------------
	Meson::Common::Collections::TArrayList<ConstraintPtr> Constraints;

	//------------------------------------------------------------------------------------------
	/// Constructs a constraint batch.
	//------------------------------------------------------------------------------------------
	ConstraintBatch(void);

	//------------------------------------------------------------------------------------------
	/// Constructs a constraint batch from the given source.
	/// \param p_constraintBatch Constraint batch structure to copy.
	//------------------------------------------------------------------------------------------
	ConstraintBatch(const ConstraintBatch& p_constraintBatch);

	//------------------------------------------------------------------------------------------
	/// Destroys the constraint batch.
	//------------------------------------------------------------------------------------------
	~ConstraintBatch(void);

	//------------------------------------------------------------------------------------------
	/// Assigns the given constraint batch to this batch.
	/// \param p_constraintBatch Constraint batch structure to copy.
	/// \returns a reference to this constraint batch.
	//------------------------------------------------------------------------------------------
	ConstraintBatch& operator=(const ConstraintBatch& p_constraintBatch);

	//------------------------------------------------------------------------------------------
	/// Returns true if the given constraint batch is equal to this batch.
	/// \returns true if the given constraint batch is equal to this batch.
	//------------------------------------------------------------------------------------------
	bool operator==(const ConstraintBatch& p_constraintBatch) const;
};

//------------------------------------------------------------------------------------------
/// Abstract list of constraint batches.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TList<ConstraintBatch> ConstraintBatchList;

//------------------------------------------------------------------------------------------
/// Array list of constraint batches.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TArrayList<ConstraintBatch> ConstraintBatchArrayList;

//------------------------------------------------------------------------------------------
/// Linked list of constraint batches.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TLinkedList<ConstraintBatch> ConstraintBatchLinkedList;

Meson_Gravitas_Constraint_END
