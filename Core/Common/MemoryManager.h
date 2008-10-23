//----------------------------------------------------------------------------------------------
//	Meson::Common::Memory::MemoryManager
//	The memory manager class provides pooled and ad-hoc monitored memory allocation services
//	that can be transparently integrated into the object instancing system of specific classes.
//  In addition, default managers are provided for global instancing operators.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella 	17/07/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"

Meson_Common_Memory_BEGIN

//----------------------------------------------------------------------------------------------
/// Memory management class that provides both ad-hoc and pooled memory allocation mechanisms.
/// The class also supports hierarchical structuring of memory managers.
//----------------------------------------------------------------------------------------------
class MemoryManager
{	
private: // private constants
	static const int MAX_NAME_LENGTH = 128;

private: // private variables
	char m_szName[MAX_NAME_LENGTH];
	size_t m_unObjectSize;
	size_t m_unPoolSize;
	size_t m_unUsedMemoryPooled;
	size_t m_unUsedMemoryAdhoc;
	size_t m_unUsedCountPooled;
	size_t m_unUsedCountAdhoc;
	void* m_pFreeHead;
	void* m_pMemoryPool;
	MemoryManager *m_pNextSibling;
	MemoryManager *m_pFirstChild;

private: // private methods
	void FreeAdhoc(void *p_pData);
	void FreePooled(void *p_pData);

public: // public static methods
	static MemoryManager *FromPointer(void *p_pData);


public: // public methods
	MemoryManager(const char *p_pszName, MemoryManager *p_pParent,
		size_t p_unObjectSize, size_t p_unPoolSize);
	MemoryManager(void);
	~MemoryManager(void);
	const char *GetName(void) const;
	bool IsPooled(void) const;
	size_t GetAllocatedMemory(bool p_bIncludeChildren) const;
	size_t GetUsedMemory(bool p_bIncludeChildren) const;
	size_t GetAllocatedInstances(bool p_bIncludeChildren) const;
	size_t GetUsedInstances(bool p_bIncludeChildren) const;
	bool IsOwnedMemory(void *p_pData) const;
	void *AllocateAdhoc(size_t p_unSize);
	void *AllocatePooled(size_t p_unSize);
	void Free(void *p_pData);
	MemoryManager *GetNextSibling(void) const;
	MemoryManager *GetFirstChild(void) const;
};

Meson_Common_Memory_END
