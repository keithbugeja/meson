#pragma once

#include "Vector3.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvTransform.h"

Meson_Gravitas_Collision_BEGIN

//----------------------------------------------------------------------------------------------
/// \struct ContactPoint
/// Represents a contact point characterised by a position in world space, a contact normal
/// vector pointing from the first body to the second body involved in the collision, and a
/// penetration depth in scalar form.
//----------------------------------------------------------------------------------------------
struct GRAVITAS_API ContactPoint
{
	//----------------------------------------------------------------------------------------------
	/// Position of the contact point in world coordinates.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Position;

	//----------------------------------------------------------------------------------------------
	/// Normal vector perpendicular to the plane containing the contact.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Normal;


	//----------------------------------------------------------------------------------------------
	/// Scalar value for the interpenetration of the two bodies involved in the contact.
	//----------------------------------------------------------------------------------------------
	Real Penetration;

	//----------------------------------------------------------------------------------------------
	/// Relative velocity of the bodies at the point of contact computed with respect to the first
	/// body.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> RelativeVelocity;

	//----------------------------------------------------------------------------------------------
	/// Constructs a contact point.
	//----------------------------------------------------------------------------------------------
	ContactPoint(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs a contact point from the given source.
	/// \param p_contactPoint Contact point to copy.
	//----------------------------------------------------------------------------------------------
	ContactPoint(const ContactPoint& p_contactPoint);

	//----------------------------------------------------------------------------------------------
	/// Constructs a contact point from the given position, contact normal vector and penetration.
	/// \param p_vecPosition Position of the contact point in world coordinates.
	/// \param p_vecNormal Normal vector describing the plane of contact.
	/// \param p_rPenetration Scalar value for the interpenetration between the bodies involved in
	///        the contact.
	//----------------------------------------------------------------------------------------------
	ContactPoint(
		const Meson::Common::Maths::TVector3<Real> &p_vecPosition,
		const Meson::Common::Maths::TVector3<Real> &p_vecNormal,
		Real p_rPenetration);

	//----------------------------------------------------------------------------------------------
	/// Destroys the contact point.
	//----------------------------------------------------------------------------------------------
	~ContactPoint(void);

	//----------------------------------------------------------------------------------------------
	/// Returns true if the relative contact velocity indicates colliding bodies at the point of
	/// contact.
	/// \returns true if the relative contact velocity indicates colliding bodies at the point of
	///          contact.
	//----------------------------------------------------------------------------------------------
	bool IsColliding(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the relative contact velocity indicates that the bodies are at rest or
	/// sliding with respect to each other.
	/// \returns true if the relative contact velocity indicates that the bodies are at rest or
	///          sliding with respect to each other.
	//----------------------------------------------------------------------------------------------
	bool IsInContact(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the relative contact velocity indicates separating bodies at the point of
	/// contact.
	/// \returns true if the relative contact velocity indicates separating bodies at the point of
	///          contact.
	//----------------------------------------------------------------------------------------------
	bool IsSeparating(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the relative contact velocity indicates that the bodies are at rest at the
	/// point of contact.
	/// \returns true if the relative contact velocity indicates that the bodies are at rest at the
	///          point of contact.
	//----------------------------------------------------------------------------------------------
	bool IsResting(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the relative contact velocity indicates that the bodies are in contact and
	/// sliding past each other.
	/// \returns true if the relative contact velocity indicates that the bodies are in contact and
	///          sliding past each other.
	//----------------------------------------------------------------------------------------------
	bool IsSliding(void) const;

	//----------------------------------------------------------------------------------------------
	/// Transforms the contact point using the given transform. The contact point is transformed
	/// using the translation component of the transform, while the contact normal is transformed
	/// using the rotation component of the transform. The penetration depth is unaffected.
	/// \param p_transform A transform object to apply to the contact point
	//----------------------------------------------------------------------------------------------
	void Transform(const Meson::Gravitas::Geometry::Transform& p_transform);

	//----------------------------------------------------------------------------------------------
	/// Perturbs the contact normal using the given coefficient. Each of the three components of the
	/// normal vector are updated by a random offset from a uniform probability distribution in the
	/// range [-p_rCoefficient, +p_rCoefficient]. The vector is then renormalised to preserve unit
	/// length.
	/// p_rCoefficient Coefficient used to perturb normal vector.
	//----------------------------------------------------------------------------------------------
	void PerturbNormal(Real p_rCoefficient);

	//----------------------------------------------------------------------------------------------
	/// Assigns the given contact point to this contact point.
	/// \param p_contactPoint Contact point structure to copy.
	/// \returns A reference to this contact point.
	//----------------------------------------------------------------------------------------------
	ContactPoint& operator=(const ContactPoint& p_contactPoint);

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given contact point is equal to this contact point.
	/// \param p_contactPoint Contact point structure to compare.
	/// \returns true if the given contact point is equal to this contact point.
	//----------------------------------------------------------------------------------------------
	bool operator==(const ContactPoint& p_contactPoint) const;

	//----------------------------------------------------------------------------------------------
	/// Returns true if the given contact point is not equal to this contact point.
	/// \param p_contactPoint Contact point structure to compare.
	/// \returns true if the given contact point is not equal to this contact point.
	//----------------------------------------------------------------------------------------------
	bool operator!=(const ContactPoint& p_contactPoint) const;
};

Meson_Gravitas_Collision_END
