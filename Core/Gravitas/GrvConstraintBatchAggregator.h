#pragma once

#include "List.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"

#include "GrvConstraintBatch.h"

Meson_Gravitas_Constraint_BEGIN

struct ConstraintBatchAggregatorState;

//------------------------------------------------------------------------------------------
/// \class ConstraintBatchAggregator
/// This class provides functionality for partitioning a list of constraints into disjoint
/// constraint batches (body island processing). This is achieved by constructing undirected 
/// graphs using the constraints as links and the referred bodies as nodes, resulting in one
/// or more disjoint graphs. Each constraint batch consists of the constraints contained in
/// each graph.
//------------------------------------------------------------------------------------------
class GRAVITAS_API ConstraintBatchAggregator
{
private:
	ConstraintBatchAggregatorState* m_pConstraintBatchAggregatorState;

public:

	//------------------------------------------------------------------------------------------
	/// Constructs a constraint batch aggregator.
	//------------------------------------------------------------------------------------------
	ConstraintBatchAggregator(void);

	//------------------------------------------------------------------------------------------
	/// Destroys the constraint batch aggregator.
	//------------------------------------------------------------------------------------------
	~ConstraintBatchAggregator(void);

	//------------------------------------------------------------------------------------------
	/// Partitions the given list of constraints into a number of disjoint constraint batches
	/// and stores the batches in the given list of constraint batches. The destination list is
	/// cleared before being populated.
	/// \param p_listConstraints List of constraints to process.
	/// \param p_rTime Current simulator time.
	/// \param p_listConstraintBatches Destination list of constraint batches.
	//------------------------------------------------------------------------------------------
	void GenerateBatches(
		const ConstraintList& p_listConstraints,
		Real p_rTime, ConstraintBatchList& p_listConstraintBatches);
};

Meson_Gravitas_Constraint_END
