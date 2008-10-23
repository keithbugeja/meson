#include "CylinderFactory.h"
#include "Cylinder.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

CylinderFactory::CylinderFactory(void)
	: m_strName("Cylinder")
{
}

CylinderFactory:: ~CylinderFactory(void)
{
}

const String& CylinderFactory::GetName(void)
{
	return m_strName;
}

GeometryPtr CylinderFactory::CreateGeometry(void)
{
	Cylinder* pCylinder = new Cylinder();
	GravitasEngine::GetInstance()->Logger().Out << "Cylinder geometry created.\n";
	return GeometryPtr(pCylinder);
}

GeometryPtr CylinderFactory::CreateGeometry(const String& p_strId)
{
	Cylinder* pCylinder = new Cylinder(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "Cylinder geometry created with ID '" + p_strId + "'.\n";
	return GeometryPtr(pCylinder);
}
