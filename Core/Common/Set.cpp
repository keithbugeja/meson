#include "Set.h"

Meson_Common_Collections_BEGIN

using namespace Meson::Common::Memory;

MemoryManager* GetSetMemoryManager(void)
{
	MemoryManager* pMemoryManagerRoot = GetRootMemoryManager();
	static MemoryManager s_memoryManagerSets
		= MemoryManager("Sets", pMemoryManagerRoot, 0, 0);
	return &s_memoryManagerSets;
}

Meson_Common_Collections_END
