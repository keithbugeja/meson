#include "HashMap.h"

//#include "stdint.h"

Meson_Common_Collections_BEGIN

using namespace Meson::Common::Memory;
using namespace Meson::Common::Text;

MemoryManager* GetHashMapMemoryManager(void)
{
	const size_t HASH_MAP_POOL_SIZE = 256;
	const size_t HASH_MAP_OBJECT_SIZE = 32;

	MemoryManager* pMemoryManagerMaps = GetMapMemoryManager();
	static MemoryManager s_memoryManagerHashMaps
		= MemoryManager("Hash Maps", pMemoryManagerMaps,
			HASH_MAP_OBJECT_SIZE, HASH_MAP_POOL_SIZE);
	return &s_memoryManagerHashMaps;
}

MemoryManager* GetHashMapNodeMemoryManager(void)
{
	const size_t HASH_MAP_NODE_POOL_SIZE = 2048;
	const size_t HASH_MAP_NODE_OBJECT_SIZE = 32;

	MemoryManager* pMemoryManagerHashMaps = GetHashMapMemoryManager();
	static MemoryManager s_memoryManagerHashMapNodes
		= MemoryManager("Hash Map Nodes", &s_memoryManagerHashMapNodes,
			HASH_MAP_NODE_OBJECT_SIZE, HASH_MAP_NODE_POOL_SIZE);
	return &s_memoryManagerHashMapNodes;
}

#undef get16bits

#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)

#define GET_LOW_WORD(d) (*((const short *) (d)))

#else
#define GET_LOW_WORD(d) ((((uint32_t)(((const uchar *)(d))[1])) << 8)\
                       +(uint32_t)(((const uchar *)(d))[0]) )
#endif

// adapted from Paul Hsieh's SuperFastHash algorithm
ulong HashDataGeneric(const void* p_pData, size_t p_unLength)
{
	ulong ulHash = (ulong) p_unLength;

	if (p_unLength == 0 || p_pData == NULL) return 0;

	const char* pCharData = (const char *) p_pData;

	// get remainder for end cases
	size_t unRemainder = p_unLength & 3;

	// process data
	p_unLength >>= 2;
	while(p_unLength-- > 0)
	{
		ulHash += GET_LOW_WORD(pCharData);
		ulong ulTemp
			= (GET_LOW_WORD(pCharData + 2) << 11) ^ ulHash;
		ulHash = (ulHash << 16) ^ ulTemp;
		pCharData  += 2 * sizeof (ushort);
		ulHash += ulHash >> 11;
	}

	// handle end cases
	switch (unRemainder)
	{
		case 3: ulHash += GET_LOW_WORD(pCharData);
				ulHash ^= ulHash << 16;
				ulHash ^= pCharData[sizeof (ushort)] << 18;
				ulHash += ulHash >> 11;
				break;
		case 2: ulHash += GET_LOW_WORD(pCharData);
				ulHash ^= ulHash << 11;
				ulHash += ulHash >> 17;
				break;
		case 1: ulHash += *pCharData;
				ulHash ^= ulHash << 10;
				ulHash += ulHash >> 1;
	}

	// force "avalanching" of final 127 bits
	ulHash ^= ulHash << 3;
	ulHash += ulHash >> 5;
	ulHash ^= ulHash << 4;
	ulHash += ulHash >> 17;
	ulHash ^= ulHash << 25;
	ulHash += ulHash >> 6;

	return ulHash;
}

//ulong HashData(const String& p_strKey)
//{
//		const char* pszKey = (const char *) p_strKey;
//		return HashDataGeneric(pszKey, p_strKey.Length());
//};

ulong HashData(const String& p_strKey)
{
		ulong hash = 5381;

		size_t unLen = p_strKey.Length();
		for( size_t un=0; un < unLen; un++ )
		{
			hash = ((hash << 5) + hash) + p_strKey(un); /* hash * 33 + c */
		}

        return hash;
};

Meson_Common_Collections_END
