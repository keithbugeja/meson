#include "AngularMotorConstraintFactory.h"

#include "AngularMotorConstraint.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Space;

using namespace Meson::Gravitas::Constraint;

AngularMotorConstraintFactory::AngularMotorConstraintFactory(void)
	: m_strName("AngularMotor")
{
}

AngularMotorConstraintFactory:: ~AngularMotorConstraintFactory(void)
{
}

const String &AngularMotorConstraintFactory::GetName(void)
{
	return m_strName;
}

ConstraintPtr AngularMotorConstraintFactory::CreateConstraint(void)
{
	AngularMotorConstraint* pAngularMotorConstraint = new AngularMotorConstraint();
	GravitasEngine::GetInstance()->Logger().Out << "AngularMotor constraint created.\n";
	return ConstraintPtr(pAngularMotorConstraint);
}

ConstraintPtr AngularMotorConstraintFactory::CreateConstraint(const String& p_strId)
{
	AngularMotorConstraint* pAngularMotorConstraint = new AngularMotorConstraint(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "AngularMotor constraint created with ID '" + p_strId + "'.\n";
	return ConstraintPtr(pAngularMotorConstraint);
}
