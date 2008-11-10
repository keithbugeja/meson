#pragma once

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvGravitasEntity.h"
#include "GrvBody.h"
#include "GrvRay.h"

Meson_Gravitas_Space_BEGIN

//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for Gravitas events.
/// \returns a pointer to the memory manager for Gravitas events.
//------------------------------------------------------------------------------------------
GRAVITAS_API Meson::Common::Memory::MemoryManager* GetSpaceMemoryManager(void);

//------------------------------------------------------------------------------------------
/// \struct BodyRayIntersection
/// Represents an instance of a ray intersecting a body at a given point in world space.
//------------------------------------------------------------------------------------------
struct GRAVITAS_API BodyRayIntersection
{
	//------------------------------------------------------------------------------------------
	/// Pointer to the ray intersecting the body.
	//------------------------------------------------------------------------------------------
	const Meson::Gravitas::Geometry::Ray* Ray;

	//------------------------------------------------------------------------------------------
	/// Managed pointer to the body intersected by the ray.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Body::BodyPtr Body;

	//------------------------------------------------------------------------------------------
	/// The point of intersection between the ray and body in world coordinates.
	//------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Position;

	//------------------------------------------------------------------------------------------
	/// Constructs a body ray intersection structure.
	//------------------------------------------------------------------------------------------
	BodyRayIntersection(void);

	//------------------------------------------------------------------------------------------
	/// Constructs a body ray intersection structure using the given ray reference, managed
	/// pointer to a body and position.
	/// \param p_ray A reference to the ray intersecting the body.
	/// \param p_pBody A managed pointer to the body intersected by the ray.
	/// \param p_vecPosition A point of intersection in world space.
	//------------------------------------------------------------------------------------------
	BodyRayIntersection(
		const Meson::Gravitas::Geometry::Ray& p_ray,
		Meson::Gravitas::Body::BodyPtr p_pBody,
		const Meson::Common::Maths::TVector3<Real>& p_vecPosition);

	//------------------------------------------------------------------------------------------
	/// Constructs a body ray intersection structure using the given source structure.
	/// \param p_bodyRayIntersection Source body ray intersection structure to copy.
	//------------------------------------------------------------------------------------------
	BodyRayIntersection(const BodyRayIntersection& p_bodyRayIntersection);

	//------------------------------------------------------------------------------------------
	/// Assigns the given source body ray intersection structure to this structure.
	/// \param p_bodyRayIntersection Source structure to copy.
	/// \returns A reference to this body ray intersection structure.
	//------------------------------------------------------------------------------------------
	BodyRayIntersection& operator=(const BodyRayIntersection& p_bodyRayIntersection);

	//------------------------------------------------------------------------------------------
	/// Returns true if the given body ray intersection structure is equal to this structure.
	/// The comparision involves matching of the ray, body and position.
	/// \returns true if the given body ray intersection structure is equal to this structure.
	//------------------------------------------------------------------------------------------
	bool operator==(const BodyRayIntersection& p_bodyRayIntersection) const;

	//------------------------------------------------------------------------------------------
	/// Returns true if the ray distance of this body ray intersection structure is less than
	/// that of the given structure. This comparison operator is used to order intersections
	/// by distance.
	/// \returns true if the ray distance of this body ray intersection structure is less than
	///          that of the given structure.
	//------------------------------------------------------------------------------------------
	bool operator<(const BodyRayIntersection& p_bodyRayIntersection) const;

	//------------------------------------------------------------------------------------------
	/// Returns true if the ray distance of this body ray intersection structure is greater than
	/// that of the given structure. This comparison operator is used to order intersections
	/// by distance.
	/// \returns true if the ray distance of this body ray intersection structure is greater 
	///          than that of the given structure.
	//------------------------------------------------------------------------------------------
	bool operator>(const BodyRayIntersection& p_bodyRayIntersection) const;

	//------------------------------------------------------------------------------------------
	/// Returns true if the ray distance of this body ray intersection structure is less than or 
	/// equal to that of the given structure. This comparison operator is used to order
	/// intersections by distance.
	/// \returns true if the ray distance of this body ray intersection structure is less than 
	///          or equal to that of the given structure.
	//------------------------------------------------------------------------------------------
	bool operator<=(const BodyRayIntersection& p_bodyRayIntersection) const;

	//------------------------------------------------------------------------------------------
	/// Returns true if the ray distance of this body ray intersection structure is greater than
	/// or equal to that of the given structure. This comparison operator is used to order
	/// intersections by distance.
	/// \returns true if the ray distance of this body ray intersection structure is greater 
	///          than or equal to that of the given structure.
	//------------------------------------------------------------------------------------------
	bool operator>=(const BodyRayIntersection& p_bodyRayIntersection) const;
};

//------------------------------------------------------------------------------------------
/// Abstract list of body ray intersections.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TList< BodyRayIntersection >
	BodyRayIntersectionList;

//------------------------------------------------------------------------------------------
/// Array list of body ray intersections.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TArrayList< BodyRayIntersection >
	BodyRayIntersectionArrayList;

//------------------------------------------------------------------------------------------
/// Linked list of body ray intersections.
//------------------------------------------------------------------------------------------
typedef Meson::Common::Collections::TLinkedList< BodyRayIntersection >
	BodyRayIntersectionLinkedList;

//------------------------------------------------------------------------------------------
/// \class ISpace
/// Abstract interface for the spatial representation of bodies in a simulation environment.
/// Implementations of this interface consist of broad-phase collision detection algorithms
/// that generatate sets of potentially colliding body pairs from the set of all possible
/// pairs. Typical implementations may entail spatial partitioning, grids and coordinate
/// sorting. Within a collision detection pipeline, candidate collision pairs generated by a
/// space typically undergo a mid-phase collision detection step entailing testing the
/// bodies' bounding volumes in order to further prune the candidate collision list. The
/// actual geometry of the bodies are then tested for actual intersection, and subsequently
/// to obtain contact manifolds between bodies in contact.
//------------------------------------------------------------------------------------------
class GRAVITAS_API ISpace
	: public Meson::Gravitas::IGravitasEntity
{
public:
	ISpace(void) : IGravitasEntity() {};
	ISpace(const Meson::Common::Text::String& p_strId) : IGravitasEntity(p_strId) {};
	virtual ~ISpace(void) {};

	//------------------------------------------------------------------------------------------
	/// Returns an enumerator of all the bodies in the simulation space.
	/// \returns an enumerator of all the bodies in the simulation space.
	//------------------------------------------------------------------------------------------
	virtual Meson::Common::Collections::TEnumerator<Meson::Gravitas::Body::BodyPtr>&
		GetBodies(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Returns a body using the given ID. A MesonException is thrown if no body with the given
	/// ID is contained in the space.
	/// \param p_strId ID of the body to return.
	/// \returns a body using the given ID.
	//------------------------------------------------------------------------------------------
	virtual Meson::Gravitas::Body::BodyPtr GetBody(
		const Meson::Common::Text::String& p_strId) = 0;

	//------------------------------------------------------------------------------------------
	/// Inserts the given body in the simulation space.
	/// \param p_pBody A managed pointer to a body to insert in the space.
	//------------------------------------------------------------------------------------------
	virtual void AddBody(Meson::Gravitas::Body::BodyPtr p_pBody) = 0;

	//------------------------------------------------------------------------------------------
	/// Removes the given body from the simulation space.
	/// \param p_pBody A managed pointer to a body to remove from the space.
	//------------------------------------------------------------------------------------------
	virtual void RemoveBody(Meson::Gravitas::Body::BodyPtr p_pBody) = 0;

	//------------------------------------------------------------------------------------------
	/// Rebuilds the internal data structures for maintaining the spatial representation. This
	/// method is assumed to be computationally expensive and should result in a near optimal
	/// configuration of the space. This method is typically involved at setup and, possibly
	/// during the course of the simulation on an infrequent basis. In the case of a
	/// hierarchical space representation, this may entail a complete rebuild of the associated
	/// tree structure, to ensure balanced queries.
	//------------------------------------------------------------------------------------------
	virtual void Rebuild(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Updates the internal data structures for maintaining the spatial representation. This
	/// method is assumed to be computationally cheap but with potentially sub-obtimal
	/// configuration of the internal structures. This method is intended for frequent
	/// invocation during the course of a simulation in order to maintain consistency. In the
	/// case of a hierarchical space representation, this may entail relocating bodies to more
	/// appropriate nodes in the existing tree.
	//------------------------------------------------------------------------------------------
	virtual void Update(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Computes a list of candidate collision pairs using the underlying spatial algorithm and
	/// internal data structures. The list should contain body pairs of objects considered as
	/// potentially colliding. An effective algorithm will typically prune away pairs that are
	/// too far apart and only generate pairs for bodies in relatively close proximity. The
	/// algorithm should operate on the bodies' bounding volumes for performance reasons.
	/// \param p_listCandidateCollisions List of candidate collision pairs to populate.
	//------------------------------------------------------------------------------------------
	virtual void ComputeCandidateCollisions(
		Meson::Gravitas::Body::BodyPairList& p_listCandidateCollisions) = 0;

	// ray casting

	//------------------------------------------------------------------------------------------
	/// Performs a ray cast using the given ray and populating the given body ray intersection
	/// list with bodies intersected by the ray. The list is ordered by ray intersection
	/// distance. The implementation of this ray test may benefit from the underlying data
	/// structures used by the space.
	/// \param p_ray Ray to intersect with the space.
	/// \param p_listBodyRayIntersections List of body ray intersections, ordered by closest
	///        intersection first.
	//------------------------------------------------------------------------------------------
	virtual void IntersectRay(const Meson::Gravitas::Geometry::Ray& p_ray,
		BodyRayIntersectionList& p_listBodyRayIntersections) const = 0;
};

//------------------------------------------------------------------------------------------
/// Managed pointer to a space.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<ISpace> SpacePtr;

Meson_Gravitas_Space_END
