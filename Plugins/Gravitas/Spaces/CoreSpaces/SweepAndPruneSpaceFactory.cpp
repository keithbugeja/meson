#include "SweepAndPruneSpaceFactory.h"

#include "SweepAndPruneSpace.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Space;

SweepAndPruneSpaceFactory::SweepAndPruneSpaceFactory(void)
	: m_strName("Sweep-And-Prune")
{
}

SweepAndPruneSpaceFactory:: ~SweepAndPruneSpaceFactory(void)
{
}

const String &SweepAndPruneSpaceFactory::GetName(void)
{
	return m_strName;
}

SpacePtr SweepAndPruneSpaceFactory::CreateSpace(void)
{
	SweepAndPruneSpace* pSweepAndPruneSpace = new SweepAndPruneSpace();
	GravitasEngine::GetInstance()->Logger().Out << "Sweep-and-prune space created.\n";
	return SpacePtr(pSweepAndPruneSpace);
}

SpacePtr SweepAndPruneSpaceFactory::CreateSpace(const String& p_strId)
{
	SweepAndPruneSpace* pSweepAndPruneSpace = new SweepAndPruneSpace(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "Sweep-and-prune space created with ID '" + p_strId + "'.\n";
	return SpacePtr(pSweepAndPruneSpace);
}
