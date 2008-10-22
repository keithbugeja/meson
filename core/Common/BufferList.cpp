
#include "BufferList.h"

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	16/02/2008	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

Meson_Common_Collections_BEGIN

using namespace Meson::Common::Memory;

MemoryManager *GetBufferListMemoryManager(void)
{
	const size_t BUFFER_LIST_POOL_SIZE = 1024;
	const size_t BUFFER_LIST_OBJECT_SIZE = sizeof( TBufferList<int> );

	MemoryManager *pMemoryManagerLists = GetListMemoryManager();
	static MemoryManager s_memoryManagerBufferListNodes
		= MemoryManager("Buffer Lists", pMemoryManagerLists,
			BUFFER_LIST_OBJECT_SIZE, BUFFER_LIST_POOL_SIZE);
	return &s_memoryManagerBufferListNodes;
}

Meson_Common_Collections_END
