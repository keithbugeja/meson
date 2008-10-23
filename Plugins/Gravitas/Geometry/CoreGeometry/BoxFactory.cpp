#include "BoxFactory.h"
#include "Box.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

BoxFactory::BoxFactory(void)
	: m_strName("Box")
{
}

BoxFactory:: ~BoxFactory(void)
{
}

const String& BoxFactory::GetName(void)
{
	return m_strName;
}

GeometryPtr BoxFactory::CreateGeometry(void)
{
	Box* pBox = new Box();
	GravitasEngine::GetInstance()->Logger().Out << "Box geometry created.\n";
	return GeometryPtr(pBox);
}

GeometryPtr BoxFactory::CreateGeometry(const String& p_strId)
{
	Box* pBox = new Box(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "Box geometry created with ID '" + p_strId + "'.\n";
	return GeometryPtr(pBox);
}
