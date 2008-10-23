#include "SphericalJointConstraintFactory.h"

#include "SphericalJointConstraint.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Space;

using namespace Meson::Gravitas::Constraint;

SphericalJointConstraintFactory::SphericalJointConstraintFactory(void)
	: m_strName("SphericalJoint")
{
}

SphericalJointConstraintFactory:: ~SphericalJointConstraintFactory(void)
{
}

const String &SphericalJointConstraintFactory::GetName(void)
{
	return m_strName;
}

ConstraintPtr SphericalJointConstraintFactory::CreateConstraint(void)
{
	SphericalJointConstraint* pSphericalJointConstraint = new SphericalJointConstraint();
	GravitasEngine::GetInstance()->Logger().Out << "SphericalJoint constraint created.\n";
	return ConstraintPtr(pSphericalJointConstraint);
}

ConstraintPtr SphericalJointConstraintFactory::CreateConstraint(const String& p_strId)
{
	SphericalJointConstraint* pSphericalJointConstraint = new SphericalJointConstraint(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "SphericalJoint constraint created with ID '" + p_strId + "'.\n";
	return ConstraintPtr(pSphericalJointConstraint);
}
