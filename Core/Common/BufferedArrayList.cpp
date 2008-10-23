
//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "BufferedArrayList.h"

Meson_Common_Collections_BEGIN

using namespace Meson::Common::Memory;

MemoryManager *GetBufferedArrayListMemoryManager(void)
{
	const size_t ARRAY_LIST_POOL_SIZE = 1024;
	const size_t ARRAY_LIST_OBJECT_SIZE = sizeof( TBufferedArrayList<int> );

	MemoryManager *pMemoryManagerLists = GetListMemoryManager();
	static MemoryManager s_memoryManagerBufferedArrayListNodes
		= MemoryManager("Buffered Array Lists", pMemoryManagerLists,
			ARRAY_LIST_OBJECT_SIZE, ARRAY_LIST_POOL_SIZE);
	return &s_memoryManagerBufferedArrayListNodes;
}

Meson_Common_Collections_END
