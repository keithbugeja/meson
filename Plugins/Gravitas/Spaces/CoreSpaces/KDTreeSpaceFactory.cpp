#include "KDTreeSpaceFactory.h"

#include "KDTreeSpace.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Space;

KDTreeSpaceFactory::KDTreeSpaceFactory(void)
	: m_strName("KD-Tree")
{
}

KDTreeSpaceFactory:: ~KDTreeSpaceFactory(void)
{
}

const String &KDTreeSpaceFactory::GetName(void)
{
	return m_strName;
}

SpacePtr KDTreeSpaceFactory::CreateSpace(void)
{
	KDTreeSpace* pKDTreeSpace = new KDTreeSpace();
	GravitasEngine::GetInstance()->Logger().Out << "KD-Tree space created.\n";
	return SpacePtr(pKDTreeSpace);
}

SpacePtr KDTreeSpaceFactory::CreateSpace(const String& p_strId)
{
	KDTreeSpace* pKDTreeSpace = new KDTreeSpace(p_strId);
	GravitasEngine::GetInstance()->Logger().Out << "KD-Tree space created with ID '" + p_strId + "'.\n";
	return SpacePtr(pKDTreeSpace);
}
