//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Body::Material
//	Class for defining the material properties of a body.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		22/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Pointer.h"
#include "Matrix3.h"

#include "GrvNamespaces.h"
#include "GrvTypes.h"
#include "GrvGravitasEntity.h"

Meson_Gravitas_Materials_BEGIN

//----------------------------------------------------------------------------------------------
/// \class Material
/// Class for defining the material properties of a body.
//----------------------------------------------------------------------------------------------
class Material
	: public Meson::Gravitas::IGravitasEntity
{
public: // public variables
	//----------------------------------------------------------------------------------------------
	/// Coefficient of collision restitution. A value of zero results in a completely inelastic
	/// collision. A value of one creates a perfectly elastic collision. A value higher than one
	/// induces energy in a collison. Realistic values lie between zero and one exclusive.
	//----------------------------------------------------------------------------------------------
	Real RestitutionCoefficient;

	//----------------------------------------------------------------------------------------------
	/// Coefficient of static friction. A ratio defining a Coulomb friction cone within which
	/// static friction applies. 
	//----------------------------------------------------------------------------------------------
	Real StaticFrictionCoefficient;

	//----------------------------------------------------------------------------------------------
	/// Coefficient of dynamic friction. A ratio of sliding friction with respect to the theoretical
	/// reaction force induced between any two bodies in contact.
	//----------------------------------------------------------------------------------------------
	Real DynamicFrictionCoefficient;

	//----------------------------------------------------------------------------------------------
	/// Perturbation coefficient for surface normals. This coefficient allows for simulation of
	/// rough materials by enabling a simple surface microstructure model that applies a random
	/// perturbation to the contact normal. The perturbation is effected by adding a random offset
	/// to the X, Y and Z normal vector components, using a uniform probability distribution [-E, E]
	/// where E is the coefficient. The normal vector is renormalised to preserve unit length.
	/// Realistic values for the coefficient lie in the range [0, 0.1].
	//----------------------------------------------------------------------------------------------
	Real SurfacePerturbationCoefficient;

public: // public methods
	//----------------------------------------------------------------------------------------------
	/// Constructs a material.
	//----------------------------------------------------------------------------------------------
	Material(void);

	//----------------------------------------------------------------------------------------------
	/// Constructs a material with the given ID.
	//----------------------------------------------------------------------------------------------
	Material(const Meson::Common::Text::String& p_strId);

	//----------------------------------------------------------------------------------------------
	/// Copy-constructs a material from the given source.
	/// \param p_material source material instance to copy
	//----------------------------------------------------------------------------------------------
	Material(const Material& p_material);

	//----------------------------------------------------------------------------------------------
	/// Constructs a material properties class with the given pararmenters.
	/// \param p_rRestitutionCoefficient Coefficient of collision restitution
	/// \param p_rStaticFrictionCoefficient Coefficient of static friction
	/// \param p_rDynamicFrictionCoefficient Coefficient of dynamic friction
	/// \param p_rSurfacePerturbationCoefficient Degree of perturbation of the collision normal to
	///        simulate rough materials
	//----------------------------------------------------------------------------------------------
	Material(
		Real p_rRestitutionCoefficient,
		Real p_rStaticFrictionCoefficient,
		Real p_rDynamicFrictionCoefficient,
		Real p_rSurfacePerturbationCoefficient);

	//----------------------------------------------------------------------------------------------
	/// Destroys a material instance.
	//----------------------------------------------------------------------------------------------
	~Material(void);

	//----------------------------------------------------------------------------------------------
	/// Tests if the material has a perturbed surface (a non-zero surface perturbation coefficient).
	/// \returns true if the material has a perturbed surface.
	//----------------------------------------------------------------------------------------------
	bool HasPerturbedSurface(void) const;

	//----------------------------------------------------------------------------------------------
	/// Returns a material instance computed as a combination of this instance with the
	/// given material properties parameter.
	/// \param p_material Material properties to combine with this instance
	/// \returns A combined material properties instance
	//----------------------------------------------------------------------------------------------
	Material CombineWith(const Material& p_material);

	//----------------------------------------------------------------------------------------------
	/// Assigns the given material properties instance to this instance and returns a reference.
	/// \param p_material source material properties instance to copy
	//----------------------------------------------------------------------------------------------
	Material& operator=(const Material& p_material);
};

//----------------------------------------------------------------------------------------------
/// Managed reference type to a material.
//----------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<Material> MaterialPtr;

Meson_Gravitas_Materials_END
