#include "RevoluteJointConstraintFactory.h"

#include "RevoluteJointConstraint.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Space;

using namespace Meson::Gravitas::Constraint;

RevoluteJointConstraintFactory::RevoluteJointConstraintFactory(void)
	: m_strName("RevoluteJoint")
{
}

RevoluteJointConstraintFactory:: ~RevoluteJointConstraintFactory(void)
{
}

const String &RevoluteJointConstraintFactory::GetName(void)
{
	return m_strName;
}

ConstraintPtr RevoluteJointConstraintFactory::CreateConstraint(void)
{
	RevoluteJointConstraint* pRevoluteJointConstraint = new RevoluteJointConstraint();
	GravitasEngine::GetInstance()->Logger().Out << "RevoluteJoint constraint created.\n";
	return ConstraintPtr(pRevoluteJointConstraint);
}

ConstraintPtr RevoluteJointConstraintFactory::CreateConstraint(const String& p_strId)
{
	RevoluteJointConstraint* pRevoluteJointConstraint = new RevoluteJointConstraint(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "RevoluteJoint constraint created with ID '" + p_strId + "'.\n";
	return ConstraintPtr(pRevoluteJointConstraint);
}
