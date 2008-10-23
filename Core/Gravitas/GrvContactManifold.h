#pragma once

#include "ArrayList.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvBody.h"
#include "GrvContactPoint.h"

Meson_Gravitas_Collision_BEGIN

//----------------------------------------------------------------------------------------------
/// \struct ContactManifold
/// Represents a contact manifold consisting of contact points. This structure is generated
/// on collision between two bodies and provides information about the goemetry of the
/// collision. The contact points describing the manifold are representative point samples of
/// the manifold geometry which may consist of a single points, line segments or areas. For the
/// line and area cases, the points typically describe, respectively, the end points and the
/// vertices of the manifold geometry.
//----------------------------------------------------------------------------------------------
struct ContactManifold
{
	//----------------------------------------------------------------------------------------------
	/// First body involved in the contact.
	//----------------------------------------------------------------------------------------------
	Meson::Gravitas::Body::BodyPtr Body1;

	//----------------------------------------------------------------------------------------------
	/// Second body involved in the contact.
	//----------------------------------------------------------------------------------------------
	Meson::Gravitas::Body::BodyPtr Body2;

	//----------------------------------------------------------------------------------------------
	/// A list of contact points describing the geometry of the collision.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TArrayList<ContactPoint> ContactPoints;

	//----------------------------------------------------------------------------------------------
	/// Constructs a contact manifold.
	//----------------------------------------------------------------------------------------------
	ContactManifold(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs a contact manifold from the given source manifold.
	/// \param p_contactManifold A contact manifold to copy.
	//----------------------------------------------------------------------------------------------
	ContactManifold(const ContactManifold& p_contactManifold);

	//----------------------------------------------------------------------------------------------
	/// Constructs a contact manifold for the given bodies.
	/// \param p_pBody1 First body involved in the contact.
	/// \param p_pBody2 Second body involved in the contact.
	//----------------------------------------------------------------------------------------------
	ContactManifold(
		Meson::Gravitas::Body::BodyPtr p_pBody1,
		Meson::Gravitas::Body::BodyPtr p_pBody2);

	//----------------------------------------------------------------------------------------------
	/// Destroys the contact manifold.
	//----------------------------------------------------------------------------------------------
	~ContactManifold(void);

	//----------------------------------------------------------------------------------------------
	/// Updates the relative velocity of each contact point. Relative velocities are computed with
	/// respect to the first body.
	//----------------------------------------------------------------------------------------------
	void UpdateContactVelocities(void);

	//----------------------------------------------------------------------------------------------
	/// Sorts the contact points in decreasing order of penetration. This facility is typically
	/// used by single-contact point solvers or iterative solvers.
	//----------------------------------------------------------------------------------------------
	void SortContacts(void);

	//----------------------------------------------------------------------------------------------
	/// Returns the contact point with the greatest penetration.
	/// \returns the contact point with the greatest penetration.
	//----------------------------------------------------------------------------------------------
	ContactPoint GetDeepestContact(void) const;

	//----------------------------------------------------------------------------------------------
	/// Transforms the contact geometry using the given transform.
	/// \param p_transform Transform object used to transform the contact geometry.
	//----------------------------------------------------------------------------------------------
	void Transform(const Meson::Gravitas::Geometry::Transform& p_transform);

	//----------------------------------------------------------------------------------------------
	/// Perturbs the contact normals according the involved bodies' materials. The degree of
	/// perturbation depends on the surface perturbation coefficients of the materials.
	//----------------------------------------------------------------------------------------------
	void PerturbContactNormals(void);

	//----------------------------------------------------------------------------------------------
	/// Returns true of this contact manifold shares either of its bodies with at least one body
	/// of the given manifold. This predicate method is typically used in body island construction.
	/// \param p_contactManifold Contact manifold to test for shared bodies.
	/// \returns true of this contact manifold shares either of its bodies with at least one body
	/// of the given manifold.
	//----------------------------------------------------------------------------------------------
	bool SharesContactWith(const ContactManifold& p_contactManifold) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if this contact manifold is equal to the given manifold.
	/// \param p_contactManifold Contact manifold to compare.
	/// \returns true if this contact manifold is equal to the given manifold.
	//----------------------------------------------------------------------------------------------
	bool operator==(const ContactManifold& p_contactManifold) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if this contact manifold is less than the given manifold. This predicate
	/// compares body pointers and is typically used for manifold sorting purposes.
	/// \param p_contactManifold Contact manifold to compare.
	/// \returns true if this contact manifold is less than the given manifold.
	//----------------------------------------------------------------------------------------------
	bool operator<(const ContactManifold& p_contactManifold) const;

	//----------------------------------------------------------------------------------------------
	/// Assigns the given contact manifold to this manifold.
	/// \param p_contactManifold Source contact manifold to copy.
	//----------------------------------------------------------------------------------------------
	ContactManifold& operator=(const ContactManifold& p_contactManifold);
};

Meson_Gravitas_Collision_END
