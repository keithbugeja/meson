#include "TreeSet.h"

Meson_Common_Collections_BEGIN

using namespace Meson::Common::Memory;

MemoryManager* GetTreeSetMemoryManager(void)
{
	const size_t TREE_SET_POOL_SIZE = 1024;
	const size_t TREE_SET_OBJECT_SIZE = 32;

	MemoryManager* pMemoryManagerSets = GetSetMemoryManager();
	static MemoryManager s_memoryManagerTreeSets
		= MemoryManager("Tree Sets", pMemoryManagerSets,
			TREE_SET_OBJECT_SIZE, TREE_SET_POOL_SIZE);
	return &s_memoryManagerTreeSets;
}

Meson_Common_Collections_END
