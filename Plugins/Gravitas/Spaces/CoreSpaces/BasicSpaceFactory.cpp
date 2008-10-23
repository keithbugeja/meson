#include "BasicSpaceFactory.h"

#include "BasicSpace.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Space;

BasicSpaceFactory::BasicSpaceFactory(void)
	: m_strName("Basic")
{
}

BasicSpaceFactory:: ~BasicSpaceFactory(void)
{
}

const String &BasicSpaceFactory::GetName(void)
{
	return m_strName;
}

SpacePtr BasicSpaceFactory::CreateSpace(void)
{
	BasicSpace* pBasicSpace = new BasicSpace();
	GravitasEngine::GetInstance()->Logger().Out << "Basic space created.\n";
	return SpacePtr(pBasicSpace);
}

SpacePtr BasicSpaceFactory::CreateSpace(const String& p_strId)
{
	BasicSpace* pBasicSpace = new BasicSpace(p_strId);
	GravitasEngine::GetInstance()->Logger().Out << "Basic space created with ID '" + p_strId + "'.\n";
	return SpacePtr(pBasicSpace);
}
