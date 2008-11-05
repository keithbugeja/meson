//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Body::MassProperties
//	Structure for defining the mass properties of a body.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		22/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Matrix3.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"

Meson_Gravitas_Body_BEGIN


//----------------------------------------------------------------------------------------------
/// \struct MassProperties
/// Structure for defining the mass properties of a body.
//----------------------------------------------------------------------------------------------
struct GRAVITAS_API MassProperties
{
	/// \var mass value (linear inertia)
	Real Mass;

	/// \var mass inverse value for dynamics computations (zero for immovable objects)
	Real MassInverse;

	/// \var moment of inertia tensor matrix (angular inertia) in local body coordinates
	Meson::Common::Maths::TMatrix3<Real> InertiaTensor;

	/// \var inverse matrix for the moment of inertia tensor in local body coordinates
	Meson::Common::Maths::TMatrix3<Real> InertiaTensorInverse;

	/// \var inverse matrix for the moment of inertia tensor in world coordinates
	Meson::Common::Maths::TMatrix3<Real> InertiaTensorWorldInverse;

	//----------------------------------------------------------------------------------------------
	/// Constructs a mass properties structure.
	//----------------------------------------------------------------------------------------------
	MassProperties(void);

	//----------------------------------------------------------------------------------------------
	/// Copy-constructs a mass properties structure from the given source structure.
	/// \param p_massProperties A mass properties structure to copy.
	//----------------------------------------------------------------------------------------------
	MassProperties(const MassProperties &p_massProperties);

	//----------------------------------------------------------------------------------------------
	/// Destroys a mass properties structure.
	//----------------------------------------------------------------------------------------------
	~MassProperties(void);

	//----------------------------------------------------------------------------------------------
	/// Assigns the given mass properties structure to this structure.
	/// \param p_massProperties A mass properties structure to copy.
	/// \returns a reference to this structure
	//----------------------------------------------------------------------------------------------
	MassProperties &operator=(const MassProperties &p_massProperties);

	//----------------------------------------------------------------------------------------------
	/// Configures the mass properties structure for an immovable object of infinite mass.
	//----------------------------------------------------------------------------------------------
	void SetInfiniteMass(void);

	//----------------------------------------------------------------------------------------------
	/// Tests if the mass properties structure is configured as an infinite mass.
	/// \returns true of the structure is configured as an infinite mass.
	//----------------------------------------------------------------------------------------------
	bool HasInfiniteMass(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns a real value between 0 and 1 giving the mass ratio of this structure with respect
	/// to the given structure. The computed value is 0 or 1 if exactly one or the other structure
	/// is an infinite mass. For the case that both are infinite masses, the value 0.5 is returned.
	/// \param p_massProperties A mass properties structure with whitch to compute the ratio.
	/// \returns mass ratio of this structure with respect to the given structure.
	//----------------------------------------------------------------------------------------------
	Meson::Gravitas::Real GetMassRatio(const MassProperties &p_massProperties) const;
};

Meson_Gravitas_Body_END
