#include "GrvConstraintBatchAggregator.h"

#include "TreeSet.h"
#include "ArrayList.h"

using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Constraint;

Meson_Gravitas_Constraint_BEGIN

struct ConstraintBatchAggregatorState
{
	TArrayList< TTreeSet<ConstraintPtr> > m_listPartitions;
	TArrayList< size_t > m_listMatchingsPartitions;

	ConstraintBatchAggregatorState(void)
		: m_listPartitions(), m_listMatchingsPartitions() { }

	bool ConstraintsShareBody(
		ConstraintPtr pConstraint1, 
		ConstraintPtr pConstraint2)
	{
		return pConstraint1->GetBody1() == pConstraint2->GetBody1()
			|| pConstraint1->GetBody1() == pConstraint2->GetBody2()
			|| pConstraint1->GetBody2() == pConstraint2->GetBody1()
			|| pConstraint1->GetBody2() == pConstraint2->GetBody2();
	}
};

Meson_Gravitas_Constraint_END

ConstraintBatchAggregator::ConstraintBatchAggregator(void)
	: m_pConstraintBatchAggregatorState(NULL)
{
	m_pConstraintBatchAggregatorState = new ConstraintBatchAggregatorState();
}

ConstraintBatchAggregator::~ConstraintBatchAggregator(void)
{
	delete m_pConstraintBatchAggregatorState;
}

void ConstraintBatchAggregator::GenerateBatches(
	const ConstraintList& p_listConstraints,
	Real p_rTime, ConstraintBatchList& p_listConstraintBatches)
{
	p_listConstraintBatches.Clear();

	size_t unConstraintCount = p_listConstraints.Size(); 
	if (unConstraintCount == 0) return;

	// short-hand references
	TArrayList< TTreeSet<ConstraintPtr> >& listPartitions = m_pConstraintBatchAggregatorState->m_listPartitions;
	TArrayList< size_t >& listMatchingsPartitions = m_pConstraintBatchAggregatorState->m_listMatchingsPartitions;

	listPartitions.Clear();

	// process all input contact sets
	for (size_t unConstraintIndex = 0; unConstraintIndex < unConstraintCount; unConstraintIndex++)
	{
		// get current constraint
		ConstraintPtr pConstraint = p_listConstraints(unConstraintIndex);

		// initialise list of indices for matching partitions
		listMatchingsPartitions.Clear();

		size_t unPartitionCount = listPartitions.Size();
		for (size_t unPartitionIndex = 0; unPartitionIndex < unPartitionCount; unPartitionIndex++)
		{
			// check if current contact set matches current partition
			TTreeSet<ConstraintPtr>& setPartition = listPartitions(unPartitionIndex);
			for (TEnumerator<ConstraintPtr>& enumConstraints = setPartition.GetEnumerator();
				enumConstraints.HasMoreElements();)
			{
				ConstraintPtr pConstraintPartitioned = enumConstraints.NextElement();
				if (m_pConstraintBatchAggregatorState->ConstraintsShareBody(
					pConstraintPartitioned, pConstraint))
				{
					// add constraint to this partition
					setPartition += pConstraint;

					// keep track of matched partitions
					listMatchingsPartitions.Add(unPartitionIndex);
					break;
				}
			}
		}

		// special case - new partition
		if (listMatchingsPartitions.Size() == 0)
		{
			TTreeSet<ConstraintPtr> setNewPartition;
			setNewPartition += pConstraint;
			listPartitions += setNewPartition;
			continue;
		}

		// matches exactly 1 partition - no merges
		if (listMatchingsPartitions.Size() == 1)
			continue;

		// matches multiple partitions - merge secondary, tertiary etc. partitions with
		// first matched partition
		TTreeSet<ConstraintPtr>& setFirstPartitionMatched
			= listPartitions[listMatchingsPartitions[0]];
		for (int nMatchedPartitionIndex = (int) listMatchingsPartitions.Size() - 1;
			nMatchedPartitionIndex > 0; nMatchedPartitionIndex--)
		{
			size_t unPartitionToMergeIndex
				= listMatchingsPartitions[nMatchedPartitionIndex];
			TTreeSet<ConstraintPtr>& setPartitionToMerge
				= listPartitions[unPartitionToMergeIndex];
			setFirstPartitionMatched.InsertSet(setPartitionToMerge);
			listPartitions.RemoveAt(unPartitionToMergeIndex);
		}
	}

	// prepare batches

	ConstraintBatch constraintBatch;
	constraintBatch.Time = p_rTime;

	size_t unPartitionCount = listPartitions.Size();
	for (size_t unPartitionIndex = 0; unPartitionIndex < unPartitionCount; unPartitionIndex++)
	{
		constraintBatch.Constraints.Clear();

		TTreeSet<ConstraintPtr>& setPartition = listPartitions(unPartitionIndex);
		constraintBatch.Constraints.InsertEnumerator(0, setPartition.GetEnumerator());

		p_listConstraintBatches += constraintBatch;
	}
}
