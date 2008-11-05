#pragma once

#include "Pointer.h"
#include "String8.h"
#include "Enumerator.h"
#include "Pair.h"

#include "GrvNamespaces.h"
#include "GrvBody.h"

Meson_Gravitas_Collision_BEGIN

struct CollisionGroupState;

//----------------------------------------------------------------------------------------------
/// \class CollisionGroup
/// A body grouping mechanism for use with collision filtering. Bodies may be assigned to
/// multiple groups.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API CollisionGroup
	: public Meson::Common::TReferenceCounter<CollisionGroup>
{
private: // private variables
	CollisionGroupState* m_pCollisionGroupState;

public: // public methods
	//----------------------------------------------------------------------------------------------
	/// Constructs a collision group.
	//----------------------------------------------------------------------------------------------
	CollisionGroup(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs a collision group with the given name.
	/// \param p_strName Name for the collision group.
	//----------------------------------------------------------------------------------------------
	CollisionGroup(const Meson::Common::Text::String& p_strName);

	//----------------------------------------------------------------------------------------------
	/// Destroys the collision group.
	//----------------------------------------------------------------------------------------------
	~CollisionGroup(void);

	//----------------------------------------------------------------------------------------------
	/// Returns the name of the collision group.
	/// \returns the name of the collision group.
	//----------------------------------------------------------------------------------------------
	const Meson::Common::Text::String& GetName(void) const;

	//----------------------------------------------------------------------------------------------
	/// Assigns a name to the collision group.
	/// \param p_strName Name to assign to the collision group.
	//----------------------------------------------------------------------------------------------
	void SetName(const Meson::Common::Text::String& p_strName);

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of bodies assigned to the collision group.
	/// \returns an enumerator of bodies assigned to the collision group.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<Meson::Gravitas::Body::BodyPtr>&
		GetAssignedBodies(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given body belongs to the collision group, or false otherwise.
	/// \returns true if the given body belongs to the collision group, or false otherwise.
	//----------------------------------------------------------------------------------------------
	bool Contains(Meson::Gravitas::Body::BodyPtr p_pBody) const;

	//----------------------------------------------------------------------------------------------
	/// Adds the given body to the collision group.
	/// \param p_pBody Body to add to the group.
	//----------------------------------------------------------------------------------------------
	void Add(Meson::Gravitas::Body::BodyPtr p_pBody);

	//----------------------------------------------------------------------------------------------
	/// Removes the given body from the collision group.
	/// \param p_pBody Body to remove from the group.
	//----------------------------------------------------------------------------------------------
	void Remove(Meson::Gravitas::Body::BodyPtr p_pBody);
};

//----------------------------------------------------------------------------------------------
/// Managed reference to a collision group.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<CollisionGroup> CollisionGroupPtr;

//----------------------------------------------------------------------------------------------
/// A pair of managed collision group references.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TPair<
	CollisionGroupPtr, CollisionGroupPtr> CollisionGroupPair;

Meson_Gravitas_Collision_END
