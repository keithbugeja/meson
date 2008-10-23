//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Body::IBody
//	Abstract interface for a generic body.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		21/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Set.h"
#include "Pair.h"
#include "List.h"
#include "ArrayList.h"
#include "LinkedList.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvGravitasEntity.h"
#include "GrvGeometry.h"
#include "GrvMassProperties.h"
#include "GrvMaterial.h"
#include "GrvKineticProperties.h"
#include "GrvForceAccumulator.h"

Meson_Gravitas_Collision_BEGIN

class CollisionGroup;
typedef Meson::Common::TPointer<CollisionGroup> CollisionGroupPtr;

Meson_Gravitas_Collision_END

Meson_Gravitas_Body_BEGIN

namespace BodyType
{
	//----------------------------------------------------------------------------------------------
	/// Body type enumeration. Currently includes rigid and deformable body types.
	//----------------------------------------------------------------------------------------------
	enum BodyType
	{
		Rigid,
		Deformable
	};
}

//----------------------------------------------------------------------------------------------
/// \class IBody
/// Abstract interface for a physical body.
//----------------------------------------------------------------------------------------------
class IBody
	: public Meson::Gravitas::IGravitasEntity
{
public:
	IBody(void) {};
	IBody(const Meson::Common::Text::String& p_strId) : IGravitasEntity(p_strId) {};
	virtual ~IBody(void) {};

	virtual BodyType::BodyType GetType(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a set reference to the body's associated collision groups.
	/// \returns a set reference to the body's associated collision groups.
	//----------------------------------------------------------------------------------------------
	virtual Meson::Common::Collections::TSet<Meson::Gravitas::Collision::CollisionGroupPtr>& GetCollisionGroups() = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns true of the body is a member of the given collision group, or false otherwise.
	/// \returns true of the body is a member of the given collision group, or false otherwise.
	//----------------------------------------------------------------------------------------------
	virtual bool IsMemberOfCollisionGroup(Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Adds the body to the given collision group. Bilateral links are maintained by both entities.
	/// \param p_pCollisionGroup A collision group to which the body is to be associated.
	//----------------------------------------------------------------------------------------------
	virtual void AddToCollisionGroup(Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup) = 0;

	//----------------------------------------------------------------------------------------------
	/// Removes the body from the given collision group.
	/// \param p_pCollisionGroup A collision group from which the body is to be disassociated.
	//----------------------------------------------------------------------------------------------
	virtual void RemoveFromCollisionGroup(Meson::Gravitas::Collision::CollisionGroupPtr p_pCollisionGroup) = 0;

	//----------------------------------------------------------------------------------------------
	/// Removes the body from all the collision groups to which it is currently associated.
	//----------------------------------------------------------------------------------------------
	virtual void RemoveFromAllCollisionGroups(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a managed reference to the body's geometry. A geometry instance may be shared by
	/// multiple bodies with the same shape and dimensions.
	/// \returns a managed reference to the body's geometry.
	//----------------------------------------------------------------------------------------------
	virtual Meson::Gravitas::Geometry::GeometryPtr GetGeometry(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Sets the body's geometry reference.
	/// \param p_pGeometry A managed geometry object reference to assign to the body.
	//----------------------------------------------------------------------------------------------
	virtual void SetGeometry(Meson::Gravitas::Geometry::GeometryPtr p_pGeometry) = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a managed reference to the body's bounding volume. The position and orientation of
	/// the volume changes with that of the body.
	/// \returns a managed reference to the body's bounding volume.
	//----------------------------------------------------------------------------------------------
	virtual Meson::Gravitas::Geometry::BoundingVolumePtr GetBoundingVolume(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a reference to the body's mass properties.
	/// \returns a reference to the body's mass properties.
	//----------------------------------------------------------------------------------------------
	virtual MassProperties& GetMassProperties(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a managed reference to a material assigned to the body.
	/// \returns a managed reference to a material assigned to the body.
	//----------------------------------------------------------------------------------------------
	virtual Meson::Gravitas::Materials::MaterialPtr GetMaterial(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a reference to the kinetic properties of the body.
	/// \returns a reference to the kinetic properties of the body.
	//----------------------------------------------------------------------------------------------
	virtual Meson::Gravitas::Kinetics::KineticProperties& GetKineticProperties(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a reference to the force accumulator of the body.
	/// \returns a reference to the force accumulator of the body.
	//----------------------------------------------------------------------------------------------
	virtual Meson::Gravitas::Force::ForceAccumulator& GetForceAccumulator(void) = 0;
};

//----------------------------------------------------------------------------------------------
/// Managed reference to an IBody.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IBody> BodyPtr;

//----------------------------------------------------------------------------------------------
/// Pair of IBody managed references.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TPair<BodyPtr, BodyPtr> BodyPair;

//----------------------------------------------------------------------------------------------
/// Generic list of IBody managed references.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TList<BodyPtr> BodyList;

//----------------------------------------------------------------------------------------------
/// Managed reference to generic list of IBody managed references.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<BodyList> BodyListPtr;

//----------------------------------------------------------------------------------------------
/// Array list of IBody managed references.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TArrayList<BodyPtr> BodyArrayList;

//----------------------------------------------------------------------------------------------
/// Managed reference to array list of IBody managed references.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<BodyArrayList> BodyArrayListPtr;

//----------------------------------------------------------------------------------------------
/// Linked list of IBody managed references.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TLinkedList<BodyPtr> BodyLinkedList;

//----------------------------------------------------------------------------------------------
/// Managed reference to linked list of IBody managed references.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<BodyLinkedList> BodyLinkedListPtr;

//----------------------------------------------------------------------------------------------
/// Generic list of IBody managed reference pairs.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TList<BodyPair> BodyPairList;

//----------------------------------------------------------------------------------------------
/// Array list of IBody managed reference pairs.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TArrayList<BodyPair> BodyPairArrayList;

//----------------------------------------------------------------------------------------------
/// Linked list of IBody managed reference pairs.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TLinkedList<BodyPair> BodyPairLinkedList;

Meson_Gravitas_Body_END
