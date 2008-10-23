#include "SphereFactory.h"
#include "Sphere.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

SphereFactory::SphereFactory(void)
	: m_strName("Sphere")
{
}

SphereFactory:: ~SphereFactory(void)
{
}

const String& SphereFactory::GetName(void)
{
	return m_strName;
}

GeometryPtr SphereFactory::CreateGeometry(void)
{
	Sphere* pSphere = new Sphere();
	GravitasEngine::GetInstance()->Logger().Out << "Sphere geometry created.\n";
	return GeometryPtr(pSphere);
}

GeometryPtr SphereFactory::CreateGeometry(const String& p_strId)
{
	Sphere* pSphere = new Sphere(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "Sphere geometry created with ID '" + p_strId + "'.\n";
	return GeometryPtr(pSphere);
}
