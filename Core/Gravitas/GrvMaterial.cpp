#include "GrvMaterial.h"

using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Materials;

Material::Material(void)
	: RestitutionCoefficient((Real) 0.5)
	, StaticFrictionCoefficient((Real) 1.0)
	, DynamicFrictionCoefficient((Real) 0.5)
	, SurfacePerturbationCoefficient((Real) 0.0)
{
}

Material::Material(const String& p_strId)
	: IGravitasEntity(p_strId)
	, RestitutionCoefficient((Real) 0.5)
	, StaticFrictionCoefficient((Real) 1.0)
	, DynamicFrictionCoefficient((Real) 0.5)
	, SurfacePerturbationCoefficient((Real) 0.0)
{
}

Material::Material(const Material& p_material)
	: RestitutionCoefficient(p_material.RestitutionCoefficient)
	, StaticFrictionCoefficient(p_material.StaticFrictionCoefficient)
	, DynamicFrictionCoefficient(p_material.DynamicFrictionCoefficient)
	, SurfacePerturbationCoefficient(p_material.SurfacePerturbationCoefficient)
{
}

Material::Material(
	Real p_rRestitutionCoefficient,
	Real p_rStaticFrictionCoefficient,
	Real p_rDynamicFrictionCoefficient,
	Real p_rSurfacePerturbationCoefficient)
	: RestitutionCoefficient(p_rRestitutionCoefficient)
	, StaticFrictionCoefficient(p_rStaticFrictionCoefficient)
	, DynamicFrictionCoefficient(p_rDynamicFrictionCoefficient)
	, SurfacePerturbationCoefficient(p_rSurfacePerturbationCoefficient)
{
}

Material::~Material(void)
{
}

bool Material::HasPerturbedSurface(void) const
{
	return !TMaths<Real>::Equals(SurfacePerturbationCoefficient, (Real) 0.0);
}

Material Material::CombineWith(const Material& p_material)
{
	return Material(
		(RestitutionCoefficient + p_material.RestitutionCoefficient) * (Real) 0.5,
		(StaticFrictionCoefficient + p_material.StaticFrictionCoefficient) * (Real) 0.5,
		(DynamicFrictionCoefficient + p_material.DynamicFrictionCoefficient) * (Real) 0.5,
		SurfacePerturbationCoefficient + p_material.SurfacePerturbationCoefficient);
}


Material& Material::operator=(const Material &p_material)
{
	RestitutionCoefficient = p_material.RestitutionCoefficient;
	StaticFrictionCoefficient = p_material.StaticFrictionCoefficient;
	DynamicFrictionCoefficient = p_material.DynamicFrictionCoefficient;
	SurfacePerturbationCoefficient = p_material.SurfacePerturbationCoefficient;
	return *this;
}
