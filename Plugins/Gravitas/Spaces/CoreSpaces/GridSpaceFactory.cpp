#include "GridSpaceFactory.h"

#include "GridSpace.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Space;

GridSpaceFactory::GridSpaceFactory(void)
	: m_strName("Grid")
{
}

GridSpaceFactory:: ~GridSpaceFactory(void)
{
}

const String &GridSpaceFactory::GetName(void)
{
	return m_strName;
}

SpacePtr GridSpaceFactory::CreateSpace(void)
{
	GridSpace* pGridSpace = new GridSpace();
	GravitasEngine::GetInstance()->Logger().Out << "Grid space created.\n";
	return SpacePtr(pGridSpace);
}

SpacePtr GridSpaceFactory::CreateSpace(const String& p_strId)
{
	GridSpace* pGridSpace = new GridSpace(p_strId);
	GravitasEngine::GetInstance()->Logger().Out << "Grid space created with ID '" + p_strId + "'.\n";
	return SpacePtr(pGridSpace);
}
