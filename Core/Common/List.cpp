#include "List.h"

Meson_Common_Collections_BEGIN

using namespace Meson::Common::Memory;

MemoryManager* GetListMemoryManager(void)
{
	MemoryManager* pMemoryManagerRoot = GetRootMemoryManager();
	static MemoryManager s_memoryManagerLists
		= MemoryManager("Lists", pMemoryManagerRoot, 0, 0);
	return &s_memoryManagerLists;
}

Meson_Common_Collections_END
