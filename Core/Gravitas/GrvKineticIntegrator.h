#pragma once

#include "String8.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvGravitasEntity.h"
#include "GrvVectorTimeFunction.h"
#include "GrvKineticProperties.h"

Meson_Gravitas_Kinetics_BEGIN

//----------------------------------------------------------------------------------------------
/// \class IKineticIntegrator
/// Abstract interface for a kinetic numerical integrator. The interface defines methods for
/// integrating the linear and angular components of velocity and position of a body using the
/// corresponding vector time functions for the linear and angular accelerations over a
/// specified time delta.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API IKineticIntegrator
	: public Meson::Gravitas::IGravitasEntity
{
public:
	virtual ~IKineticIntegrator(void) {};

	//----------------------------------------------------------------------------------------------
	/// Returns the name of the integrator implementation.
	/// \returns the name of the integrator implementation.
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetName(void) const = 0;

	//----------------------------------------------------------------------------------------------
	/// Integrates the linear and angular velocity of the given kinetic properties for the time
	/// interval defined by the given current time as the starting point and the given delta time as
	/// the interval span. The linear and angular accelerations are given as vector time functor
	/// objects. The new velocity components are stored in the give kinetic properties structure.
	/// \param p_rCurrentTime Current simulation time.
	/// \param p_rDeltaTime Interval over which to integrate the velocity components.
	/// \param p_linearAcceleration Vector time functor for the linear acceleration.
	/// \param p_angularAcceleration Vector time functor for the angular acceleration.
	/// \param p_kineticProperties Kinetic properties structure for which to update the velocity
	///        components.
	//----------------------------------------------------------------------------------------------
	virtual void IntegrateVelocity(
		Real p_rCurrentTime, Real p_rDeltaTime,
		IVectorTimeFunction& p_linearAcceleration,
		IVectorTimeFunction& p_angularAcceleration,
		KineticProperties& p_kineticProperties) = 0;

	//----------------------------------------------------------------------------------------------
	/// Integrates the position and orientation of the given kinetic properties for the time
	/// interval defined by the given current time as the starting point and the given delta time as
	/// the interval span. The linear and angular accelerations are given as vector time functor
	/// objects. The new positional components are stored in the give kinetic properties structure.
	/// \param p_rCurrentTime Current simulation time.
	/// \param p_rDeltaTime Interval over which to integrate the velocity components.
	/// \param p_linearAcceleration Vector time functor for the linear acceleration.
	/// \param p_angularAcceleration Vector time functor for the angular acceleration.
	/// \param p_kineticProperties Kinetic properties structure for which to update the positional
	///        components.
	//----------------------------------------------------------------------------------------------
	virtual void IntegratePosition(
		Real p_rCurrentTime, Real p_rDeltaTime,
		IVectorTimeFunction& p_linearAcceleration,
		IVectorTimeFunction& p_angularAcceleration,
		KineticProperties& p_kineticProperties) = 0;
};

//----------------------------------------------------------------------------------------------
/// Managed pointer to a kinetic integrator.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IKineticIntegrator> KineticIntegratorPtr;

Meson_Gravitas_Kinetics_END
