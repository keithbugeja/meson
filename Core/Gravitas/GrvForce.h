#pragma once

#include "Pair.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvGravitasEntity.h"
#include "GrvMassProperties.h"
#include "GrvKineticProperties.h"
#include "GrvGeometry.h"

Meson_Gravitas_Body_BEGIN
class IBody;
Meson_Gravitas_Body_END

Meson_Gravitas_Force_BEGIN

//------------------------------------------------------------------------------------------
/// \class IForceGenerator
/// Abstract interface for a force generator. Implementations of this interface may be used
/// to implement a variety of force fields, ranging from simple constant forces such as
/// gravity, to complex time and position varying forces such as explosions and vortices.
/// The force generator has access to mass, kinetic and geometric properties and hence it
/// is also possible to implement aerodynamic or fluid forces, if required. A force
/// generator instance is not bound to a specific body but may be shared by an arbitrary
/// number of bodies.
//------------------------------------------------------------------------------------------
class IForce
	: public Meson::Gravitas::IGravitasEntity
{
public:
	IForce(void) : IGravitasEntity() {};
	IForce(const Meson::Common::Text::String& p_strId) : IGravitasEntity(p_strId) {};
	virtual ~IForce(void) {};

	//------------------------------------------------------------------------------------------
	/// Returns a force vector for the given mass, kinetic and geometric properties, and the
	/// given simulation time.
	/// \param p_pMassProperties Mass properties of the affected body.
	/// \param p_pKineticProperties Kinetic properties of the affected body.
	/// \param p_pGeometry Geometry of the affected body.
	/// \param p_rTime Current simulator time.
	/// \returns a force vector for the given mass, kinetic and geometric properties, and the
	///          given simulation time.
	//------------------------------------------------------------------------------------------
	virtual Meson::Common::Maths::TVector3<Real> GetForceValue(
		Meson::Gravitas::Body::MassProperties* p_pMassProperties,
		Meson::Gravitas::Kinetics::KineticProperties* p_pKineticProperties,
		Meson::Gravitas::Geometry::GeometryPtr p_pGeometry,
		Real p_rTime) = 0;

	//------------------------------------------------------------------------------------------
	/// Returns a force application point, in body coordinates, for the given mass, kinetic and
	/// geometric properties, and the given simulation time.
	/// \param p_pMassProperties Mass properties of the affected body.
	/// \param p_pKineticProperties Kinetic properties of the affected body.
	/// \param p_pGeometry Geometry of the affected body.
	/// \param p_rTime Current simulator time.
	/// \returns a force application point, in body coordinates, for the given mass, kinetic
	///          and geometric properties, and the given simulation time.
	//------------------------------------------------------------------------------------------
	virtual Meson::Common::Maths::TPoint3<Real> GetApplicationPoint(
		Meson::Gravitas::Body::MassProperties* p_pMassProperties,
		Meson::Gravitas::Kinetics::KineticProperties* p_pKineticProperties,
		Meson::Gravitas::Geometry::GeometryPtr p_pGeometry,
		Real p_rTime) = 0;

	//------------------------------------------------------------------------------------------
	/// Returns true if this force generator is instantaneous. Examples of such forces include
	/// reaction and friction forces.
	/// \returns true if this force generator is instantaneous.
	//------------------------------------------------------------------------------------------
	virtual bool IsInstantaneous(void) = 0;
};

//------------------------------------------------------------------------------------------
/// Managed pointer to a force generator.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<IForce> ForcePtr;

Meson_Gravitas_Force_END
