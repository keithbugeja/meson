#include "LinkedList.h"

Meson_Common_Collections_BEGIN

using namespace Meson::Common::Memory;

MemoryManager* GetLinkedListMemoryManager(void)
{
	const size_t LINKED_LIST_POOL_SIZE = 1024;
	const size_t LINKED_LIST_OBJECT_SIZE = 36;

	MemoryManager* pMemoryManagerLists = GetListMemoryManager();
	static MemoryManager s_memoryManagerLinkedLists
		= MemoryManager("Linked Lists", pMemoryManagerLists,
			LINKED_LIST_OBJECT_SIZE, LINKED_LIST_POOL_SIZE);
	return &s_memoryManagerLinkedLists;
}

MemoryManager* GetLinkedListNodeMemoryManager(void)
{
	const size_t LINKED_LIST_NODE_POOL_SIZE = 2048;
	const size_t LINKED_LIST_NODE_OBJECT_SIZE = 16;

	MemoryManager* pMemoryManagerLinkedLists = GetLinkedListMemoryManager();
	static MemoryManager s_memoryManagerLinkedListNodes
		= MemoryManager("Linked List Nodes", pMemoryManagerLinkedLists,
			LINKED_LIST_NODE_OBJECT_SIZE, LINKED_LIST_NODE_POOL_SIZE);
	return &s_memoryManagerLinkedListNodes;
}

Meson_Common_Collections_END
