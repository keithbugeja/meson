#include "HashSet.h"

Meson_Common_Collections_BEGIN

using namespace Meson::Common::Memory;

MemoryManager* GetHashSetMemoryManager(void)
{
	const size_t HASH_SET_POOL_SIZE = 256;
	const size_t HASH_SET_OBJECT_SIZE = 32;

	MemoryManager* pMemoryManagerSets = GetSetMemoryManager();
	static MemoryManager s_memoryManagerHashSets
		= MemoryManager("Hash Sets", pMemoryManagerSets,
			HASH_SET_OBJECT_SIZE, HASH_SET_POOL_SIZE);
	return &s_memoryManagerHashSets;
}

Meson_Common_Collections_END
