#include "GrvConstraintBatch.h"

#include "Maths.h"
#include "Sorting.h"
#include "TreeSet.h"

using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Constraint;

ConstraintBatch::ConstraintBatch(void)
	: Time((Real) 0.0)
	, Constraints()
{
}

ConstraintBatch::ConstraintBatch(const ConstraintBatch& p_constraintBatch)
	: Time(p_constraintBatch.Time)
	, Constraints(p_constraintBatch.Constraints)
{
}


ConstraintBatch::~ConstraintBatch(void)
{
}

ConstraintBatch& ConstraintBatch::operator=(const ConstraintBatch& p_constraintBatch)
{
	Time = p_constraintBatch.Time;
	Constraints = p_constraintBatch.Constraints;
	return *this;
}

bool ConstraintBatch::operator==(const ConstraintBatch& p_constraintBatch) const
{
	if (!TMaths<Real>::Equals(Time, p_constraintBatch.Time)) return false;
	if (Constraints != p_constraintBatch.Constraints) return false;
	return true;
}