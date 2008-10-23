#include "TreeMap.h"

Meson_Common_Collections_BEGIN

using namespace Meson::Common::Memory;

MemoryManager* GetTreeMapMemoryManager(void)
{
	const size_t TREE_MAP_POOL_SIZE = 1024;
	const size_t TREE_MAP_OBJECT_SIZE = 32;

	MemoryManager* pMemoryManagerMaps = GetMapMemoryManager();
	static MemoryManager s_memoryManagerTreeMaps
		= MemoryManager("Tree Maps", pMemoryManagerMaps,
			TREE_MAP_OBJECT_SIZE, TREE_MAP_POOL_SIZE);
	return &s_memoryManagerTreeMaps;
}

MemoryManager* GetTreeMapNodeMemoryManager(void)
{
	const size_t TREE_MAP_NODE_POOL_SIZE = 1024;
	const size_t TREE_MAP_NODE_OBJECT_SIZE = 32;

	MemoryManager* pMemoryManagerTreeMaps = GetTreeMapMemoryManager();
	static MemoryManager s_memoryManagerTreeMapNodes
		= MemoryManager("Tree Map Nodes", &s_memoryManagerTreeMapNodes,
			TREE_MAP_NODE_OBJECT_SIZE, TREE_MAP_NODE_POOL_SIZE);
	return &s_memoryManagerTreeMapNodes;
}

Meson_Common_Collections_END
