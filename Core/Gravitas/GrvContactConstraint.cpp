#include "GrvContactConstraint.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Memory;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Event;
using namespace Meson::Gravitas::Materials;

Meson_Gravitas_Constraint_BEGIN

MemoryManager* GetContactConstraintMemoryManager(void)
{
	MemoryManager* pMemoryManagerConstraints = GetConstraintMemoryManager();
	static MemoryManager s_memoryManagerContactConstraints
		= MemoryManager("Contact Constraints", pMemoryManagerConstraints, sizeof(ContactConstraint), 1024);
	return &s_memoryManagerContactConstraints;
}

Meson_Gravitas_Constraint_END

const String ContactConstraint::s_strType("Contact");

ContactConstraint::ContactConstraint(void)
	: Body1()
	, Body2()
	, ConstraintDiscrepancies()
{
}

ContactConstraint::ContactConstraint(const ContactManifold& p_contactManifold)
	: Body1(p_contactManifold.Body1)
	, Body2(p_contactManifold.Body2)
	, ConstraintDiscrepancies()
{
	// determine how material is combined
	EventManager* pEventManager = GravitasEngine::GetInstance()->GetEventManager();
	Material materialCombined;
	if (pEventManager->IsEnabled(EventFlags::Material))
	{
		// if the material event is enabled, allow for custimised combination
		MaterialEvent* pMaterialEvent = new MaterialEvent(
			Body1->GetMaterial(), Body2->GetMaterial(), &materialCombined);
		pEventManager->Notify(MaterialEventPtr(pMaterialEvent));
	}
	else
		// otherwise, combine the material automatically
		materialCombined
			= Body1->GetMaterial()->CombineWith(*Body2->GetMaterial());

	ConstraintDiscrepancies.Clear();
	size_t unCount = p_contactManifold.ContactPoints.Size();
	for (size_t unIndex = 0; unIndex < unCount; unIndex++)
	{
		ContactPoint& contactPoint = p_contactManifold.ContactPoints[unIndex];
		if (contactPoint.IsSeparating())
			continue;
		ConstraintDiscrepancies.Add(
			ConstraintDiscrepancy(contactPoint,
				materialCombined.RestitutionCoefficient,
				materialCombined.StaticFrictionCoefficient,
				materialCombined.DynamicFrictionCoefficient));
	}
}

ContactConstraint::~ContactConstraint(void)
{
}

void* ContactConstraint::operator new(size_t p_unSize)
{
	return GetContactConstraintMemoryManager()->AllocatePooled(p_unSize);
}

void ContactConstraint::operator delete(void* p_pObject, size_t p_unSize)
{
	::operator delete(p_pObject);
}							

const String& ContactConstraint::GetType(void) const
{
	return s_strType;
}

BodyPtr ContactConstraint::GetBody1(void)
{
	return Body1;
}

void ContactConstraint::SetBody1(Meson::Gravitas::Body::BodyPtr p_pBody1)
{
	Body1 = p_pBody1;
}

BodyPtr ContactConstraint::GetBody2(void)
{
	return Body2;
}

void ContactConstraint::SetBody2(Meson::Gravitas::Body::BodyPtr p_pBody2)
{
	Body2 = p_pBody2;
}

void ContactConstraint::Bind(void)
{
}

void ContactConstraint::Unbind(void)
{
}

bool ContactConstraint::IsInstantaneous(void) const
{
	return true;
}

bool ContactConstraint::IsBroken(void) const
{
	return false;
}

void ContactConstraint::SetBroken(bool p_bBroken)
{
}

Real ContactConstraint::GetBreakingThreshold(void) const
{
	return TMaths<Real>::Maximum;
}

void ContactConstraint::SetBreakingThreshold(Real p_rValue)
{
}

bool ContactConstraint::CanRecomputeDiscrepancies(void)
{
	return false;
}

void ContactConstraint::ComputeDiscrepancies(void)
{
}

const ConstraintDiscrepancyList& ContactConstraint::GetDiscrepancies(void)
{
	return ConstraintDiscrepancies;
}
