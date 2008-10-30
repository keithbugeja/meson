#include "GrvConstraintDiscrepancy.h"

using namespace Meson::Common;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Constraint;

ConstraintDiscrepancy::ConstraintDiscrepancy(void)
	: Point1()
	, Point2()
	, RelativeVelocity(true)
	, CorrectionFactor((Real) 0.8)
{
}

ConstraintDiscrepancy::ConstraintDiscrepancy(const ConstraintDiscrepancy& p_constraintDiscrepancy)
	: Point1(p_constraintDiscrepancy.Point1)
	, Point2(p_constraintDiscrepancy.Point2)
	, RelativeVelocity(p_constraintDiscrepancy.RelativeVelocity)
	, CorrectionFactor(p_constraintDiscrepancy.CorrectionFactor)
{
}

ConstraintDiscrepancy::ConstraintDiscrepancy(
	const TVector3<Real>& p_vecPoint1,
	const TVector3<Real>& p_vecPoint2,
	const TVector3<Real>& p_vecRelativeVelocity,
	Real p_rCorrectionFactor)
	: Point1(p_vecPoint1)
	, Point2(p_vecPoint2)
	, RelativeVelocity(p_vecRelativeVelocity)
	, CorrectionFactor(p_rCorrectionFactor)
{
}

ConstraintDiscrepancy::ConstraintDiscrepancy(
	const ContactPoint& p_contactPoint,
	Real p_rRestitutionCoefficient,
	Real p_rStaticFrictionCoefficient,
	Real p_rDynamicFrictionCoefficient)
	: Point1(p_contactPoint.Position)
	, Point2(p_contactPoint.Position - p_contactPoint.Normal * p_contactPoint.Penetration)
	, CorrectionFactor((Real) 0.8)
{
	// compute normal and tangent inbound velocities
	Real rNormalVelocityIn(p_contactPoint.Normal * p_contactPoint.RelativeVelocity);
	Real rNormalSpeedIn(TMaths<Real>::Abs(rNormalVelocityIn));
	TVector3<Real> vecNormalVelocityIn(p_contactPoint.Normal * rNormalVelocityIn);
	TVector3<Real> vecTangentVelocityIn(p_contactPoint.RelativeVelocity - vecNormalVelocityIn);
	Real rTangentSpeedIn(vecTangentVelocityIn.Length());

	// compute outbound normal velocity using restition coefficient
	TVector3<Real> vecNormalVelocityOut(vecNormalVelocityIn * -p_rRestitutionCoefficient);

	// compute outbound tangent velocity
	TVector3<Real> vecTangentVelocityOut(vecTangentVelocityIn);
	if (!TMaths<Real>::Equals(rTangentSpeedIn, (Real) 0.0))
	{
		// do Coulomb friction
		if (rTangentSpeedIn <= rNormalSpeedIn * p_rStaticFrictionCoefficient)
			vecTangentVelocityOut.MakeZero();
		else
			vecTangentVelocityOut = vecTangentVelocityIn
				- vecTangentVelocityIn.NormaliseCopy()
					*  rNormalSpeedIn * p_rDynamicFrictionCoefficient;
	}

	// compute desired relative velocity
	RelativeVelocity = vecNormalVelocityOut + vecTangentVelocityOut;
}

ConstraintDiscrepancy::~ConstraintDiscrepancy(void)
{
}

ConstraintDiscrepancy& ConstraintDiscrepancy::operator=(const ConstraintDiscrepancy& p_constraintDiscrepancy)
{
	Point1 = p_constraintDiscrepancy.Point1;
	Point2 = p_constraintDiscrepancy.Point2;
	RelativeVelocity = p_constraintDiscrepancy.RelativeVelocity;
	CorrectionFactor = p_constraintDiscrepancy.CorrectionFactor;
	return *this;
}

bool ConstraintDiscrepancy::operator==(const ConstraintDiscrepancy& p_constraintDiscrepancy) const
{
	return Point1 == p_constraintDiscrepancy.Point1
		&& Point2 == p_constraintDiscrepancy.Point2;
}
