#include "TrieMap.h"

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

Meson_Common_Collections_BEGIN

using namespace Meson::Common::Memory;

MemoryManager* GetTrieMapMemoryManager(void)
{
	const size_t TRIE_MAP_POOL_SIZE = 256;
	const size_t TRIE_MAP_OBJECT_SIZE = 128;

	MemoryManager* pMemoryManagerMaps = GetMapMemoryManager();
	static MemoryManager s_memoryManagerTrieMaps
		= MemoryManager("Trie Maps", pMemoryManagerMaps,
			TRIE_MAP_OBJECT_SIZE, TRIE_MAP_POOL_SIZE);
	return &s_memoryManagerTrieMaps;
}

MemoryManager* GetTrieMapNodeMemoryManager(void)
{
	const size_t TRIE_MAP_NODE_POOL_SIZE = 1024;
	const size_t TRIE_MAP_NODE_OBJECT_SIZE = sizeof( TTrieNode<int,int> );

	MemoryManager* pMemoryManagerTrieMaps = GetTrieMapMemoryManager();
	static MemoryManager s_memoryManagerTrieMapNodes
		= MemoryManager("Trie Map Nodes", &s_memoryManagerTrieMapNodes,
			TRIE_MAP_NODE_OBJECT_SIZE, TRIE_MAP_NODE_POOL_SIZE);
	return &s_memoryManagerTrieMapNodes;
}

MemoryManager* GetTrieMapPairMemoryManager(void)
{
	const size_t TRIE_MAP_PAIR_POOL_SIZE = 1024;
	const size_t TRIE_MAP_PAIR_OBJECT_SIZE = 512;

	MemoryManager* pMemoryManagerTrieMaps = GetTrieMapMemoryManager();
	static MemoryManager s_memoryManagerTrieMapPairs
		= MemoryManager("Trie Map Pairs", &s_memoryManagerTrieMapPairs,
			TRIE_MAP_PAIR_OBJECT_SIZE, TRIE_MAP_PAIR_POOL_SIZE);
	return &s_memoryManagerTrieMapPairs;
}

Meson_Common_Collections_END