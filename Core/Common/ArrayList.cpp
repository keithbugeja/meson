
#include "ArrayList.h"

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

MemoryManager *GetArrayListMemoryManager(void)
{
	const size_t ARRAY_LIST_POOL_SIZE = 1024;
	const size_t ARRAY_LIST_OBJECT_SIZE = sizeof( TArrayList<int> );

	MemoryManager *pMemoryManagerLists = GetListMemoryManager();
	static MemoryManager s_memoryManagerArrayListNodes
		= MemoryManager("Array Lists", pMemoryManagerLists,
			ARRAY_LIST_OBJECT_SIZE, ARRAY_LIST_POOL_SIZE);
	return &s_memoryManagerArrayListNodes;
}

Meson_Common_Collections_END
