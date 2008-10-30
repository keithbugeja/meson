#pragma once

#include "List.h"
#include "ArrayList.h"
#include "LinkedList.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvContactPoint.h"

Meson_Gravitas_Constraint_BEGIN

//------------------------------------------------------------------------------------------
/// \struct ConstraintDiscrepancy
/// Represents a discrepancy with respect to a constraint imposed on two bodies. The 
/// discrepancy is defined by two points in world coordinates, each belonging to the 
/// respective body, that must be aligned to satisfy the constraint. In addition, the
/// discrepancy specifies a relative velocity that the points must satisfy. This is usually
/// zero for joint constraints but may be non-zero for contacts or motors.
//------------------------------------------------------------------------------------------
struct ConstraintDiscrepancy
{
	//------------------------------------------------------------------------------------------
	/// First discrepancy point.
	//------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Point1;

	//------------------------------------------------------------------------------------------
	/// Second discrepancy point.
	//------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Point2;

	//------------------------------------------------------------------------------------------
	/// Relative velocity to be satisfied by the two points.
	//------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> RelativeVelocity;

	//------------------------------------------------------------------------------------------
	/// A value between zero and one that specifies a degree of severity for this particular
	/// discrepancy. This factor is used to control the rate of convergence in the case of
	/// iterative solvers and also to support a prioritisation scheme for potentially
	/// conflicting discrepancies.
	//------------------------------------------------------------------------------------------
	Real CorrectionFactor;

	//------------------------------------------------------------------------------------------
	/// Constructs a constraint discrepancy.
	//------------------------------------------------------------------------------------------
	ConstraintDiscrepancy(void);

	//------------------------------------------------------------------------------------------
	/// Constructs a constraint discrepancy from the given source.
	/// \param p_constraintDiscrepancy Constraint discrepancy structure to copy.
	//------------------------------------------------------------------------------------------
	ConstraintDiscrepancy(const ConstraintDiscrepancy& p_constraintDiscrepancy);

	//------------------------------------------------------------------------------------------
	/// Constructs a constraint discrepancy for a collision or contact point using the given
	/// contact point and material properties. The contact point structure is used to specify
	/// penetration discrepancy via the discrepancy points. The coefficients of restitution
	/// and friction are used to compute a desired post-collision velocity using the Coulomb
	/// friction law.
	/// \param p_contactPoint Contact point structure from which to build the discrepancy.
	/// \param p_rRestitutionCoefficient Combined coefficient of collision restitution between
	///        the two bodies involved.
	/// \param p_rStaticFrictionCoefficient Combined coefficient of static friction between the
	///        two bodies involved.
	/// \param p_rDynamicFrictionCoefficient Combined coefficient of dynamic friction between
	///        the two bodies involved.
	//------------------------------------------------------------------------------------------
	ConstraintDiscrepancy(
		const Meson::Gravitas::Collision::ContactPoint& p_contactPoint,
		Real p_rRestitutionCoefficient,
		Real p_rStaticFrictionCoefficient,
		Real p_rDynamicFrictionCoefficient);

	//------------------------------------------------------------------------------------------
	/// Constructs a constraint discrepancy using the given points, relative velocity and
	/// correction factor. If not specified, the relative velocity and correction factor are
	/// respectively assumed zero and 0.8.
	/// \param p_vecPoint1 First discrepancy point.
	/// \param p_vecPoint2 Second discrepancy point.
	/// \param p_vecRelativeVelocity Relative velocity to be satisfied by the two points.
	/// \param p_rCorrectionFactor Degree of correction required for this discrepancy.
	//------------------------------------------------------------------------------------------
	ConstraintDiscrepancy(
		const Meson::Common::Maths::TVector3<Real>& p_vecPoint1,
		const Meson::Common::Maths::TVector3<Real>& p_vecPoint2,
		const Meson::Common::Maths::TVector3<Real>& p_vecRelativeVelocity
			= Meson::Common::Maths::TVector3<Real>::Zero,
		Real p_rCorrectionFactor = (Real) 0.8);

	//------------------------------------------------------------------------------------------
	/// Destroys the constraint discrepancy.
	//------------------------------------------------------------------------------------------
	~ConstraintDiscrepancy(void);

	//------------------------------------------------------------------------------------------
	/// Assigns the given constraint discrepancy to this discrepancy.
	/// \param p_constraintDiscrepancy Constraint discrepancy structure to copy.
	/// \returns a reference to this constraint discrepancy.
	//------------------------------------------------------------------------------------------
	ConstraintDiscrepancy& operator=(const ConstraintDiscrepancy& p_constraintDiscrepancy);

	//------------------------------------------------------------------------------------------
	/// Returns true if the given constraint discrepancy is equal to this discrepancy.
	/// \returns true if the given constraint discrepancy is equal to this discrepancy.
	//------------------------------------------------------------------------------------------
	bool operator==(const ConstraintDiscrepancy& p_constraintDiscrepancy) const;
};

//------------------------------------------------------------------------------------------
/// Abstract list of constraint discrepancies.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TList<ConstraintDiscrepancy> ConstraintDiscrepancyList;

//------------------------------------------------------------------------------------------
/// Array list of constraint discrepancies.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TArrayList<ConstraintDiscrepancy> ConstraintDiscrepancyArrayList;

//------------------------------------------------------------------------------------------
/// Linked list of constraint discrepancies.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TLinkedList<ConstraintDiscrepancy> ConstraintDiscrepancyLinkedList;

Meson_Gravitas_Constraint_END
