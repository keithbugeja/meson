#include "HalfspaceFactory.h"
#include "Halfspace.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

HalfspaceFactory::HalfspaceFactory(void)
	: m_strName("Halfspace")
{
}

HalfspaceFactory:: ~HalfspaceFactory(void)
{
}

const String& HalfspaceFactory::GetName(void)
{
	return m_strName;
}

GeometryPtr HalfspaceFactory::CreateGeometry(void)
{
	Halfspace* pHalfspace = new Halfspace();
	GravitasEngine::GetInstance()->Logger().Out << "Halfspace geometry created.\n";
	return GeometryPtr(pHalfspace);
}

GeometryPtr HalfspaceFactory::CreateGeometry(const String& p_strId)
{
	Halfspace* pHalfspace = new Halfspace(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "Halfspace geometry created with ID '" + p_strId + "'.\n";
	return GeometryPtr(pHalfspace);
}
