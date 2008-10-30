#pragma once

#include "Quaternion.h"
#include "Vector3.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"

Meson_Gravitas_Kinetics_BEGIN

//----------------------------------------------------------------------------------------------
/// \struct KineticProperties
/// A representation of the combined kinetic properties of a body. The properties are tailored
/// mainly for rigid bodies although they may be adapted as an aggregate of the kinetic state
/// of a soft body around the centre of mass. The kinetic properties structure includes
/// position, orientation, linear and angular velocities, a damping coefficient to allow for
/// a simple motion resistance model, and activity information for supporting sleep states.
//----------------------------------------------------------------------------------------------
struct KineticProperties
{
	///----------------------------------------------------------------------------------------------
	/// Boolean flag indicating if the associated body is in a kinetically active state or otherwise.
	///----------------------------------------------------------------------------------------------
	bool Active;

	///----------------------------------------------------------------------------------------------
	/// Boolean flag indicating if the body is allowed to switch to a sleeping state.
	///----------------------------------------------------------------------------------------------
	bool CanSleep;

	///----------------------------------------------------------------------------------------------
	/// Time elapsed since the last time the body was in actual motion (active). This value is used
	/// to determine a period of relative inactivity in order to deactivate a body from the
	/// simulation.
	///----------------------------------------------------------------------------------------------
	Real LastActive;

	///----------------------------------------------------------------------------------------------
	/// A point representing the position of body's centre of mass in world coordinates.
	///----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> Position;

	///----------------------------------------------------------------------------------------------
	/// A unit quaternion representing the orientation of the body in world coordinates.
	///----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TQuaternion<Real> Orientation;

	///----------------------------------------------------------------------------------------------
	/// Conjugate of the orientation quoternion. This is a derived quantity used to speed up
	/// computations.
	///----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TQuaternion<Real> OrientationConjugate;

	///----------------------------------------------------------------------------------------------
	/// A vector representing the linear velocity at the body's centre of mass.
	///----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> LinearVelocity;

	///----------------------------------------------------------------------------------------------
	/// A vector representing the angular velocity of the body. The direction of the vector
	/// determines the axis of rotation, while the magnitude represents the angular speed around the
	/// axis. The direction of rotation is determined by the right-hand rule.
	///----------------------------------------------------------------------------------------------
	Meson::Common::Maths::TVector3<Real> AngularVelocity;

	///----------------------------------------------------------------------------------------------
	/// A parameter for a simple motion-resistive friction model, typically applied as an exponential
	/// function of time. It may be used as a first order model for rolling and spinning friction
	/// or fluid drag.
	///----------------------------------------------------------------------------------------------
	Real DampingCoefficient;

	///----------------------------------------------------------------------------------------------
	/// Constructs a kinetic properties structure.
	///----------------------------------------------------------------------------------------------
	KineticProperties(void);

	///----------------------------------------------------------------------------------------------
	/// Constructs a kinetic properties structure from a given source.
	/// \param p_kineticProperties Source structure to copy.
	///----------------------------------------------------------------------------------------------
	KineticProperties(const KineticProperties& p_kineticProperties);

	///----------------------------------------------------------------------------------------------
	/// Destroys the kinetic properties structure.
	///----------------------------------------------------------------------------------------------
	~KineticProperties(void);

	///----------------------------------------------------------------------------------------------
	/// Assigns the given kinetic properties to this structure.
	/// \param p_kineticProperties Source structure to copy.
	/// \returns a reference to this kinetic properties structure.
	///----------------------------------------------------------------------------------------------
	KineticProperties& operator=(const KineticProperties& p_kineticProperties);

	///----------------------------------------------------------------------------------------------
	/// Computes a difference between this kinetic properties structure and the given structure,
	/// returning a new set of kinetic properties for the given structure, but expressed in the
	/// coordinate frame of this structure.
	/// \param p_kineticProperties Structure to transform.
	/// \returns a copy of the given kinetic properties structure transformed into the coordinate
	///          frame of this structure.
	///----------------------------------------------------------------------------------------------
	KineticProperties operator-(const KineticProperties& p_kineticProperties);
	
	///----------------------------------------------------------------------------------------------
	/// Updates all the derivative properties from the corresponding primary properties.
	///----------------------------------------------------------------------------------------------
	void ComputeDerivativeProperties(void);

	///----------------------------------------------------------------------------------------------
	/// Returns a value representing the level of activity, or kinetic energy, present in the
	/// associated body. This value is computed as the sum of the squares of the linear and angular
	/// velocity vectors.
	///----------------------------------------------------------------------------------------------
	Meson::Gravitas::Real GetActivity(void);

	///----------------------------------------------------------------------------------------------
	/// Transforms the given world position into a poisition expressed in the body's frame of
	/// reference. The value is stored in the given local position.
	/// \param p_vecWorldPosition A point in world coordinates to transform.
	/// \param p_vecLocalPosition A reference to point in which to store the position in local body
	///        coordinates.
	///----------------------------------------------------------------------------------------------
	void TransformPointToLocal(
		const Meson::Common::Maths::TVector3<Real>& p_vecWorldPosition, 
		Meson::Common::Maths::TVector3<Real>& p_vecLocalPosition) const;

	///----------------------------------------------------------------------------------------------
	/// Transforms the given point expressed in the body's frame of reference into an absolute world
	/// position.
	/// \param p_vecLocalPosition A point expressed in body coordinates to the transformed.
	/// \param p_vecWorldPosition A reference to a point in which to store the position in world
	///        coordinates.
	///----------------------------------------------------------------------------------------------
	void TransformPointToWorld(
		const Meson::Common::Maths::TVector3<Real>& p_vecLocalPosition, 
		Meson::Common::Maths::TVector3<Real>& p_vecWorldPosition) const;

	///----------------------------------------------------------------------------------------------
	/// Computes the absolute velocity, in the body's frame of reference, of the given point, also
	/// expressed in body coordinates.
	/// \param p_vecLocalPosition Point in body coordinates for which to compute the velocity.
	/// \param p_vecVelocity Vector in which to store the computed velocity.
	///----------------------------------------------------------------------------------------------
	void ComputeLocalPointVelocity(
		const Meson::Common::Maths::TVector3<Real>& p_vecLocalPosition,
		Meson::Common::Maths::TVector3<Real>& p_vecVelocity) const;

	///----------------------------------------------------------------------------------------------
	/// Computes the absolute velocity, in the body's frame of reference, of the given point
	/// expressed in world coordinates. The given point is assumed to be fixed with the body.
	/// \param p_vecWorldPosition Point in world coordinates for which to compute the velocity.
	/// \param p_vecVelocity Vector in which to store the computed velocity.
	///----------------------------------------------------------------------------------------------
	void ComputeWorldPointVelocity(
		const Meson::Common::Maths::TVector3<Real>& p_vecWorldPosition,
		Meson::Common::Maths::TVector3<Real>& p_vecVelocity) const;
};

Meson_Gravitas_Kinetics_END
