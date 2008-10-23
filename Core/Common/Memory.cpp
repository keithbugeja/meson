#include "Memory.h"

#include <stdlib.h>

using namespace Meson::Common::Memory;

MemoryManager *GetRootMemoryManager(void)
{
	static MemoryManager s_memoryManagerRoot
		= MemoryManager("Root", NULL, 0, 0);
	return &s_memoryManagerRoot;
}

MemoryManager *GetArrayMemoryManager(void)
{
	MemoryManager *pMemoryManagerRoot = GetRootMemoryManager();
	static MemoryManager s_memoryManagerArray
		= MemoryManager("Array Allocations", pMemoryManagerRoot, 0, 0);
	return &s_memoryManagerArray;
}

void *operator new(size_t unSize)
{
	return GetRootMemoryManager()->AllocateAdhoc(unSize);
}

void *operator new[](size_t p_unSize)
{
	return GetArrayMemoryManager()->AllocateAdhoc(p_unSize);
}

void operator delete(void *p_pData)
{
	MemoryManager *pMemoryManager = MemoryManager::FromPointer(p_pData);
	if (pMemoryManager != NULL)
		pMemoryManager->Free(p_pData);
	else
		free(p_pData);
}

void operator delete[](void *p_pData)
{
	MemoryManager *pMemoryManager = MemoryManager::FromPointer(p_pData);
	if (pMemoryManager != NULL)
		pMemoryManager->Free(p_pData);
	else
		free(p_pData);
}
