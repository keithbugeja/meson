#pragma once

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvGravitasEntity.h"
#include "GrvBody.h"
#include "GrvConstraintDiscrepancy.h"

Meson_Gravitas_Constraint_BEGIN

//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for Gravitas constraints.
/// \returns a pointer to the memory manager for Gravitas constraints.
//------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager* GetConstraintMemoryManager(void);

//------------------------------------------------------------------------------------------
/// \class IConstraint
/// Abstract interface for a constraint imposed on two bodies. A constraint may be
/// persistent, as in the case of a joint, or instantaneous, as in the case of
/// non-penetration constraints generated on demand after the collision detection phase. A
/// constraint specifies its behaviour by defining a number of constraint discrepancies
/// between the bodies. Each discrepancy typically eliminates a degree of freedom in the
/// relative movement of the two bodies.
//------------------------------------------------------------------------------------------
class GRAVITAS_API IConstraint
	: public Meson::Gravitas::IGravitasEntity
{
public:
	IConstraint(void) {};
	IConstraint(const Meson::Common::Text::String& p_strId) : IGravitasEntity(p_strId) {};
	virtual ~IConstraint(void) {};

	//------------------------------------------------------------------------------------------
	/// Returns the type name of the constraint.
	/// \returns the type name of the constraint.
	//------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetType(void) const = 0;

	//------------------------------------------------------------------------------------------
	/// Returns a managed reference to the first body affected by the constraint.
	/// \returns a managed reference to the first body affected by the constraint.
	//------------------------------------------------------------------------------------------
	virtual Meson::Gravitas::Body::BodyPtr GetBody1(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Sets the first body affected by the constraint, using the given managed reference.
	/// \param p_pBody Managed reference to the first body affected by the constraint.
	//------------------------------------------------------------------------------------------
	virtual void SetBody1(Meson::Gravitas::Body::BodyPtr p_pBody) = 0;

	//------------------------------------------------------------------------------------------
	/// Returns a managed reference to the second body affected by the constraint.
	/// \returns a managed reference to the second body affected by the constraint.
	//------------------------------------------------------------------------------------------
	virtual Meson::Gravitas::Body::BodyPtr GetBody2(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Sets the second body affected by the constraint, using the given managed reference.
	/// \param p_pBody Managed reference to the second body affected by the constraint.
	//------------------------------------------------------------------------------------------
	virtual void SetBody2(Meson::Gravitas::Body::BodyPtr p_pBody) = 0;

	//------------------------------------------------------------------------------------------
	/// Activates the constraint. This method is typically invoked following configuration of
	/// the constraint.
	//------------------------------------------------------------------------------------------
	virtual void Bind(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Deactivates the constraint. After this method is invoked, the affected bodies are no
	/// longer bound by the constraint.
	//------------------------------------------------------------------------------------------
	virtual void Unbind(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Returns true if this constraint implementation describes an instantaneous constraint.
	/// \returns true if this constraint implementation describes an instantaneous constraint.
	//------------------------------------------------------------------------------------------
	virtual bool IsInstantaneous(void) const = 0;

	//------------------------------------------------------------------------------------------
	/// Returns true if this constraint flagged as broken.
	/// \returns true if this constraint flagged as broken.
	//------------------------------------------------------------------------------------------
	virtual bool IsBroken(void) const = 0;

	//------------------------------------------------------------------------------------------
	/// Sets the broken flag of the constraint.
	/// \param p_bBroken Boolean flag.
	//------------------------------------------------------------------------------------------
	virtual void SetBroken(bool p_bBroken) = 0;

	//------------------------------------------------------------------------------------------
	/// Returns the breaking threshold set for this constraint. The threshold typically limits
	/// the maximum relative velocity computed at each discrepancy point within the constraint.
	/// If this limit is exceeded, the constraint is marked as broken and is eliminated from
	/// the simulator's list of constraints in a subsequent pass.
	//------------------------------------------------------------------------------------------
	virtual Real GetBreakingThreshold(void) const = 0;

	//------------------------------------------------------------------------------------------
	/// Sets the breaking threshold of the constraint.
	/// \param p_rValue Value to apply for the breaking threshold.
	//------------------------------------------------------------------------------------------
	virtual void SetBreakingThreshold(Real p_rValue) = 0;

	//------------------------------------------------------------------------------------------
	/// Returns true if the point discrepancies can be recomputed an arbitrary number of times
	/// within the same simulation step. Non-penetration constraints are typically expensive to
	/// compute and hence such constraints are marked as non-recomputable.
	/// \returns true if the point discrepancies can be recomputed.
	//------------------------------------------------------------------------------------------
	virtual bool CanRecomputeDiscrepancies(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Computes an internal list of constraint discrepancies describing the behaviour of the
	/// constraint.
	//------------------------------------------------------------------------------------------
	virtual void ComputeDiscrepancies(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Returns the list of constraint discrepancies describing the behaviour of the constraint.
	/// \returns the list of constraint discrepancies describing the behaviour of the
	///          constraint.
	//------------------------------------------------------------------------------------------
	virtual const ConstraintDiscrepancyList& GetDiscrepancies(void) = 0;
};

//------------------------------------------------------------------------------------------
/// Managed pointer to a constraint.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IConstraint> ConstraintPtr;

//------------------------------------------------------------------------------------------
/// Abstract list of managed constraint pointers.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TList<ConstraintPtr> ConstraintList;

//------------------------------------------------------------------------------------------
/// Managed pointer to abstract list of managed constraint pointers.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<ConstraintList> ConstraintListPtr;

//------------------------------------------------------------------------------------------
/// Array list of managed constraint pointers.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TArrayList<ConstraintPtr> ConstraintArrayList;

//------------------------------------------------------------------------------------------
/// Managed pointer to array list of managed constraint pointers.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<ConstraintArrayList> ConstraintArrayListPtr;

//------------------------------------------------------------------------------------------
/// Linked list of managed constraint pointers.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TLinkedList<ConstraintPtr> ConstraintLinkedList;

//------------------------------------------------------------------------------------------
/// Managed pointer to linked list of managed constraint pointers.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<ConstraintLinkedList> ConstraintLinkedListPtr;

Meson_Gravitas_Constraint_END
