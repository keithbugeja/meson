#include "Map.h"

Meson_Common_Collections_BEGIN

using namespace Meson::Common::Memory;

MemoryManager* GetMapMemoryManager(void)
{
	MemoryManager* pMemoryManagerRoot = GetRootMemoryManager();
	static MemoryManager s_memoryManagerMaps
		= MemoryManager("Maps", pMemoryManagerRoot, 0, 0);
	return &s_memoryManagerMaps;
}

Meson_Common_Collections_END
