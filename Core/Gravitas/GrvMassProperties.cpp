#include "GrvMassProperties.h"

using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;

MassProperties::MassProperties(void)
	: Mass((Real) 1.0)
	, MassInverse((Real) 1.0)
	, InertiaTensor(TMatrix3<Real>::Identity)
	, InertiaTensorInverse(TMatrix3<Real>::Identity)
	, InertiaTensorWorldInverse(TMatrix3<Real>::Identity)
{
}

MassProperties::MassProperties(const MassProperties &p_massProperties)
	: Mass(p_massProperties.Mass)
	, MassInverse(p_massProperties.MassInverse)
	, InertiaTensor(p_massProperties.InertiaTensor)
	, InertiaTensorInverse(p_massProperties.InertiaTensorInverse)
	, InertiaTensorWorldInverse(p_massProperties.InertiaTensorWorldInverse)
{
}

MassProperties::~MassProperties(void)
{
}

MassProperties &MassProperties::operator=(const MassProperties &p_massProperties)
{
	Mass = p_massProperties.Mass;
	MassInverse = p_massProperties.MassInverse;
	InertiaTensor = p_massProperties.InertiaTensor;
	InertiaTensorInverse = p_massProperties.InertiaTensorInverse;
	InertiaTensorWorldInverse = p_massProperties.InertiaTensorWorldInverse;
	return *this;
}

void MassProperties::SetInfiniteMass(void)
{
	Mass = TMaths<Real>::Maximum;
	MassInverse = (Real) 0.0;
	InertiaTensor = TMatrix3<Real>::Identity;
	InertiaTensorInverse = InertiaTensorWorldInverse = TMatrix3<Real>::Zero;
}

bool MassProperties::HasInfiniteMass(void) const
{
	return TMaths<Real>::Equals(MassInverse, (Real) 0.0);
}

Real MassProperties::GetMassRatio(const MassProperties &p_massProperties) const
{
	bool bInfiniteMass1 = TMaths<Real>::Equals(MassInverse, (Real) 0.0);
	bool bInfiniteMass2 = TMaths<Real>::Equals(p_massProperties.MassInverse, (Real) 0.0);

	if (bInfiniteMass1 && bInfiniteMass2)
		return (Real) 0.5;
	else if (bInfiniteMass1)
		return (Real) 1.0;
	else if (bInfiniteMass2)
		return (Real) 0.0;
	else
		return Mass / (Mass + p_massProperties.Mass);
}
