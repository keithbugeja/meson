#include "RigidJointConstraintFactory.h"

#include "RigidJointConstraint.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Space;

using namespace Meson::Gravitas::Constraint;

RigidJointConstraintFactory::RigidJointConstraintFactory(void)
	: m_strName("RigidJoint")
{
}

RigidJointConstraintFactory:: ~RigidJointConstraintFactory(void)
{
}

const String &RigidJointConstraintFactory::GetName(void)
{
	return m_strName;
}

ConstraintPtr RigidJointConstraintFactory::CreateConstraint(void)
{
	RigidJointConstraint* pRigidJointConstraint = new RigidJointConstraint();
	GravitasEngine::GetInstance()->Logger().Out << "RigidJoint constraint created.\n";
	return ConstraintPtr(pRigidJointConstraint);
}

ConstraintPtr RigidJointConstraintFactory::CreateConstraint(const String& p_strId)
{
	RigidJointConstraint* pRigidJointConstraint = new RigidJointConstraint(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "RigidJoint constraint created with ID '" + p_strId + "'.\n";
	return ConstraintPtr(pRigidJointConstraint);
}
