#include "GrvContactManifold.h"

#include "MesonException.h"
#include "Sorting.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Kinetics;

Meson_Gravitas_Collision_BEGIN

int DeeperContact(const ContactPoint& p_contactPoint1, const ContactPoint& p_contactPoint2)
{
	if (p_contactPoint1.Penetration < p_contactPoint2.Penetration) return 1;
	if (p_contactPoint2.Penetration < p_contactPoint1.Penetration) return -1;
	return 0;
}

ContactManifold::ContactManifold(void)
	: Body1(NULL)
	, Body2(NULL)
	, ContactPoints()
{
}

ContactManifold::ContactManifold(const ContactManifold& p_contactManifold)
	: Body1(p_contactManifold.Body1)
	, Body2(p_contactManifold.Body2)
	, ContactPoints(p_contactManifold.ContactPoints)
{
}

ContactManifold::ContactManifold(BodyPtr p_pBody1, BodyPtr p_pBody2)
	: Body1(p_pBody1)
	, Body2(p_pBody2)
	, ContactPoints()
{
}

ContactManifold::~ContactManifold(void)
{
}

ContactPoint ContactManifold::GetDeepestContact(void) const
{
	if (ContactPoints.Size() == 0)
		throw new MesonException(
			"No contacts contained in contect set.",
			__FILE__, __LINE__);

	ContactPoint contactPointDeepest = ContactPoints(0);

	int nSameDepth = 1;
	for (size_t unIndex = 1; unIndex < ContactPoints.Size(); unIndex++)
	{
		ContactPoint* pContactPoint = &ContactPoints(unIndex);
		if (TMaths<Real>::Equals(contactPointDeepest.Penetration, pContactPoint->Penetration))
		{
			Real rDenominator = (Real) 1.0 / (nSameDepth + 1);
			contactPointDeepest.Position = TPoint3<Real>::Origin + (contactPointDeepest.Position.ToVector() * (Real) nSameDepth + pContactPoint->Position.ToVector()) * rDenominator;
			contactPointDeepest.Normal = (contactPointDeepest.Normal * nSameDepth + pContactPoint->Normal) * rDenominator;
			++nSameDepth;
		}

		if (contactPointDeepest.Penetration + TMaths<Real>::Epsilon < pContactPoint->Penetration) 
		{
			nSameDepth = 1;
			contactPointDeepest = *pContactPoint; 
		}
	}

	if (nSameDepth > 1)
	{
		if (contactPointDeepest.Normal.IsZero())
			contactPointDeepest.Normal = TVector3<Real>::Right;
		else
			contactPointDeepest.Normal.Normalise();
	}

	return contactPointDeepest;
}

void ContactManifold::UpdateContactVelocities(void)
{
	if (Body1 == NULL) return;
	if (Body2 == NULL) return;

	for (size_t nIndex = 0; nIndex < ContactPoints.Size(); nIndex++)
	{
		ContactPoint& contactPoint = ContactPoints(nIndex);
		KineticProperties& kineticProperties1 = Body1->GetKineticProperties();
		KineticProperties& kineticProperties2 = Body2->GetKineticProperties();

		TVector3<Real> vecVelocity1, vecVelocity2;
		kineticProperties1.ComputeWorldPointVelocity(contactPoint.Position, vecVelocity1);
		kineticProperties2.ComputeWorldPointVelocity(contactPoint.Position, vecVelocity2);

		contactPoint.RelativeVelocity = vecVelocity2 - vecVelocity1;
	}
}

void ContactManifold::SortContacts(void)
{
	TSorting<ContactPoint>::QuickSort(ContactPoints, DeeperContact);
}

void ContactManifold::Transform(const Meson::Gravitas::Geometry::Transform& p_transform)
{
	for (size_t nIndex = 0; nIndex < ContactPoints.Size(); nIndex++)
		ContactPoints(nIndex).Transform(p_transform);
}

void ContactManifold::PerturbContactNormals(void)
{
	Real rPerturbationCoefficient
		= Body1->GetMaterial()->SurfacePerturbationCoefficient
		+ Body2->GetMaterial()->SurfacePerturbationCoefficient;

	if (TMaths<Real>::Equals(rPerturbationCoefficient, (Real) 0.0))
		return;

	for (size_t nIndex = 0; nIndex < ContactPoints.Size(); nIndex++)
		ContactPoints(nIndex).PerturbNormal(rPerturbationCoefficient);
}

bool ContactManifold::SharesContactWith(const ContactManifold& p_contactManifold) const
{
	if (Body1 == p_contactManifold.Body1) return true;
	if (Body1 == p_contactManifold.Body2) return true;
	if (Body2 == p_contactManifold.Body1) return true;
	if (Body2 == p_contactManifold.Body2) return true;
	return false;
}

bool ContactManifold::operator==(const ContactManifold& p_contactManifold) const
{
	if (Body1 != p_contactManifold.Body1) return false;
	if (Body2 != p_contactManifold.Body2) return false;

	return ContactPoints == p_contactManifold.ContactPoints;
}

bool ContactManifold::operator<(const ContactManifold& p_contactManifold) const
{
	if (Body1 < p_contactManifold.Body1) return true;
	if (Body1 > p_contactManifold.Body1) return false;
	return Body2 < p_contactManifold.Body2;
}

ContactManifold& ContactManifold::operator=(const ContactManifold& p_contactManifold)
{
	Body1 = p_contactManifold.Body1;
	Body2 = p_contactManifold.Body2;
	ContactPoints = p_contactManifold.ContactPoints;
	return *this;
}

Meson_Gravitas_Collision_END
