#pragma once

#include "List.h"

#include "GrvNamespaces.h"
#include "GrvBody.h"
#include "GrvCollisionGroup.h"

Meson_Gravitas_Collision_BEGIN

struct CollisionFilterState;

//----------------------------------------------------------------------------------------------
/// \class CollisionFilter
/// A collision filter for restricting collisions between specific bodies or collision groups.
/// The filtering behaviour is defined through a number of positive criteria (rules) and
/// negative criteria (exceptions). If none of the rules apply to a given pair of bodies,
/// the default acceptance or rejection flag is applied.
//----------------------------------------------------------------------------------------------
class CollisionFilter
	: public Meson::Common::TReferenceCounter<CollisionFilter>
{
private: // private variables
	CollisionFilterState *m_pCollisionFilterState;

public: // public methods
	//----------------------------------------------------------------------------------------------
	/// Constructs the collision filter.
	//----------------------------------------------------------------------------------------------
	CollisionFilter(void);

	//----------------------------------------------------------------------------------------------
	/// Destroys the collision filter.
	//----------------------------------------------------------------------------------------------
	~CollisionFilter(void);

	//----------------------------------------------------------------------------------------------
	/// Returns true if collisons are allowed by default, or false if rejected by default.
	/// \returns true if collisons are allowed by default, or false if rejected by default.
	//----------------------------------------------------------------------------------------------
	bool IsAllowedByDefault(void);

	//----------------------------------------------------------------------------------------------
	/// Sets the flag for allowing or rejecting collisions by default.
	/// p_bAllowedByDefault default acceptance / rejection flag.
	//----------------------------------------------------------------------------------------------
	void SetAllowedByDefault(bool p_bAllowedByDefault);

	//----------------------------------------------------------------------------------------------
	/// Clear all rules and exceptions.
	//----------------------------------------------------------------------------------------------
	void Clear(void);

	// rules (positive criteria)

	//----------------------------------------------------------------------------------------------
	/// Adds a rule that allows any collision with the given body. Conflicting body-specific rules
	/// and exceptions are automatically removed to maintain consistency.
	/// \param p_pBody Body to specify for the rule.
	//----------------------------------------------------------------------------------------------
	void AddRule(Meson::Gravitas::Body::BodyPtr p_pBody);

	//----------------------------------------------------------------------------------------------
	/// Removes a rule, if set, that allows collisions with the given body.
	/// \param p_pBody Body to specify for the rule.
	//----------------------------------------------------------------------------------------------
	void RemoveRule(Meson::Gravitas::Body::BodyPtr p_pBody);

	//----------------------------------------------------------------------------------------------
	/// Adds a rule that allows collisions between two specific bodies. The order of the given
	/// bodies is unimportant. Conflicting body-specific rules and exceptions are automatically
	/// removed to maintain consistency.
	/// \param p_pBody1 First body involved in the collision.
	/// \param p_pBody2 Second body involved in the collision.
	//----------------------------------------------------------------------------------------------
	void AddRule(Meson::Gravitas::Body::BodyPtr p_pBody1, Meson::Gravitas::Body::BodyPtr p_pBody2);

	//----------------------------------------------------------------------------------------------
	/// Removes a rule, if set, that allows collisions between two specific bodies. The order of the
	/// given bodies is unimportant.
	/// \param p_pBody1 First body involved in the collision.
	/// \param p_pBody2 Second body involved in the collision.
	//----------------------------------------------------------------------------------------------
	void RemoveRule(Meson::Gravitas::Body::BodyPtr p_pBody1, Meson::Gravitas::Body::BodyPtr p_pBody2);

	//----------------------------------------------------------------------------------------------
	/// Adds a rule that allows any collision involving at least one body that is a member of the
	/// given collision group. Conflicting collision group-specific rules and exceptions are
	/// automatically removed to maintain consistency.
	/// \param p_pCollisionGroup Collision group to specify for the rule.
	//----------------------------------------------------------------------------------------------
	void AddRule(Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup);

	//----------------------------------------------------------------------------------------------
	/// Removes a rule, if set, that allows collisions involving any bodies that are members of the
	/// given collision group.
	/// \param p_pCollisionGroup Collision group to specify for the rule.
	//----------------------------------------------------------------------------------------------
	void RemoveRule(Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup);

	//----------------------------------------------------------------------------------------------
	/// Adds a rule that allows collisions between bodies belonging to the two given collision
	/// groups. The order of the given groups is unimportant. Conflicting collison group-specific
	/// rules and exceptions are automatically removed to maintain consistency.
	/// \param p_pCollisionGroup1 First collision group for the rule.
	/// \param p_pCollisionGroup2 Second collision group for the rule.
	//----------------------------------------------------------------------------------------------
	void AddRule(
		Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup1,
		Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup2);

	//----------------------------------------------------------------------------------------------
	/// Removes a rule, if set, that allows collisions between bodies belonging to the given
	/// collision groups. The order of the given groups is unimportant.
	/// \param p_pCollisionGroup1 First collision group for the rule.
	/// \param p_pCollisionGroup2 Second collision group for the rule.
	//----------------------------------------------------------------------------------------------
	void RemoveRule(
		Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup1,
		Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup2);

	// exceptions (negative criteria)

	//----------------------------------------------------------------------------------------------
	/// Adds an exception that rejects any collision with the given body. Conflicting body-specific
	/// rules and exceptions are automatically removed to maintain consistency.
	/// \param p_pBody Body to specify for the exception.
	//----------------------------------------------------------------------------------------------
	void AddException(Meson::Gravitas::Body::BodyPtr p_pBody);

	//----------------------------------------------------------------------------------------------
	/// Removes an exception, if set, that rejects collisions with the given body.
	/// \param p_pBody Body to specify for the exception.
	//----------------------------------------------------------------------------------------------
	void RemoveException(Meson::Gravitas::Body::BodyPtr p_pBody);

	//----------------------------------------------------------------------------------------------
	/// Adds an exception that rejects collisions between two specific bodies. The order of the
	/// given bodies is unimportant. Conflicting body-specific rules and exceptions are automatically
	/// removed to maintain consistency.
	/// \param p_pBody1 First body involved in the collision.
	/// \param p_pBody2 Second body involved in the collision.
	//----------------------------------------------------------------------------------------------
	void AddException(Meson::Gravitas::Body::BodyPtr p_pBody1,
		Meson::Gravitas::Body::BodyPtr p_pBody2);

	//----------------------------------------------------------------------------------------------
	/// Removes an exception, if set, that rejects collisions between two specific bodies. The order
	/// of the given bodies is unimportant.
	/// \param p_pBody1 First body involved in the collision.
	/// \param p_pBody2 Second body involved in the collision.
	//----------------------------------------------------------------------------------------------
	void RemoveException(Meson::Gravitas::Body::BodyPtr p_pBody1,
		Meson::Gravitas::Body::BodyPtr p_pBody2);

	//----------------------------------------------------------------------------------------------
	/// Adds an exception that rejects any collision involving at least one body that is a member of
	/// the given collision group. Conflicting collision group-specific rules and exceptions are
	/// automatically removed to maintain consistency.
	/// \param p_pCollisionGroup Collision group to specify for the exception.
	//----------------------------------------------------------------------------------------------
	void AddException(Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup);

	//----------------------------------------------------------------------------------------------
	/// Removes an exception, if set, that rejects collisions involving any bodies that are members
	/// of the given collision group.
	/// \param p_pCollisionGroup Collision group to specify for the exception.
	//----------------------------------------------------------------------------------------------
	void RemoveException(Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup);

	//----------------------------------------------------------------------------------------------
	/// Adds an exception that rejects collisions between bodies belonging to the two given
	/// collision groups. The order of the given groups is unimportant. Conflicting collison group-
	/// specific rules and exceptions are automatically removed to maintain consistency.
	/// \param p_pCollisionGroup1 First collision group for the exception.
	/// \param p_pCollisionGroup2 Second collision group for the exception.
	//----------------------------------------------------------------------------------------------
	void AddException(
		Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup1,
		Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup2);

	//----------------------------------------------------------------------------------------------
	/// Removes an exception, if set, that rejects collisions between bodies belonging to the given
	/// collision groups. The order of the given groups is unimportant.
	/// \param p_pCollisionGroup1 First collision group for the exception.
	/// \param p_pCollisionGroup2 Second collision group for the exception.
	//----------------------------------------------------------------------------------------------
	void RemoveException(
		Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup1,
		Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup2);

	// filter queries and functions

	//----------------------------------------------------------------------------------------------
	/// Returns true if the two given bodies are allowed by the filter. The filter operates by first
	/// applying positive criteria (the rules) followed by the negative criteria (the exceptions).
	/// If none of the criteria match the given bodies, the default filtering flag defined by
	/// SetAllowedByDefault() is applied. The rules are applied in the order: specific bodies,
	/// specific body pairs, specific collison groups and specific group pairs. The exceptions are
	/// applied in the same order. If collision filter events are enabled, any subscribed event
	/// listeners may alter the filter result by modifying a property within the collision filter
	/// event.
	/// \param p_pBody1 First body specified for the filtering test.
	/// \param p_pBody2 second body specified for the filtering test.
	/// \returns true if the two given bodies are allowed by the filter, or false otherwise.
	//----------------------------------------------------------------------------------------------
	bool IsAllowed(
		Meson::Gravitas::Body::BodyPtr p_pBody1,
		Meson::Gravitas::Body::BodyPtr p_pBody2);

	//----------------------------------------------------------------------------------------------
	/// Returns true if the two given bodies are rejected by the filter. This function returns true
	/// whenever IsAllowed() returns false, and vice-versa.
	/// \param p_pBody1 First body specified for the filtering test.
	/// \param p_pBody2 second body specified for the filtering test.
	/// \returns true if the two given bodies are rejected by the filter, or false otherwise.
	//----------------------------------------------------------------------------------------------
	bool IsRejected(
		Meson::Gravitas::Body::BodyPtr p_pBody1,
		Meson::Gravitas::Body::BodyPtr p_pBody2);

	//----------------------------------------------------------------------------------------------
	/// Filters the given list of body pairs by removing pairs that fail the filtering criteria. On
	/// termination of the method, the list contains only body pairs that are allowed by the filter,
	/// as defined by the IsAllowed() predicate method.
	//----------------------------------------------------------------------------------------------
	void FilterList(Meson::Gravitas::Body::BodyPairList& p_listBodyPairs);
};

typedef Meson::Common::TPointer<CollisionFilter> CollisionFilterPtr;

Meson_Gravitas_Collision_END
