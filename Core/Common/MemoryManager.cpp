#include "MemoryManager.h"

#include "MesonException.h"

using namespace Meson::Common;

#pragma warning( disable : 4996 )
#pragma warning( disable : 4748 )

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Meson_Common_Memory_BEGIN

	const int MEMORY_SIGNATURE = 0xCAFEF00D;

	//------------------------------------------------------------------------------------------
	//  Attempts to get a memory pool size for a given pool name from a configuration file.
	//  Returns -1 if the file is not found, is in incorrect format or an appropriate setting
	//  is not found.
	//------------------------------------------------------------------------------------------
	void GetMemoryPoolConfiguration(const char *p_pszPoolName,
		int &p_nPoolSize, int &p_nObjectSize)
	{
		p_nPoolSize = p_nObjectSize = -1;

		// open file, return -1 in case of failure
		FILE *pFile = fopen("Memory.config", "r");
		if (pFile == NULL) return;

		// process file
		while (!feof(pFile))
		{
			// read next line
			char szLine[1024];
			fgets(szLine, 1023, pFile);

			// skip line if name does not match
			if (strncmp(szLine, p_pszPoolName, strlen(p_pszPoolName)))
				continue;

			// expect '=' after name
			if (szLine[strlen(p_pszPoolName)] != '=')
				continue;

			// locate delimiting comma
			int nCommaIndex = (int) strlen(p_pszPoolName) + 1;
			while (szLine[nCommaIndex] != ','
				&& szLine[nCommaIndex] != '\0')
				++nCommaIndex;

			// prepare string pointer to pool size
			char *szPoolSize = szLine + strlen(p_pszPoolName) + 1;

			// if comma not found, extract pool size only
			if (szLine[nCommaIndex] == '\0')
			{
				p_nPoolSize = atoi(szPoolSize);
				fclose(pFile);
				return;
			}
		
			// terminate string at comma to extract pool size
			szLine[nCommaIndex] = '\0';
			p_nPoolSize = atoi(szPoolSize);

			// prepare string pointer to object size
			char *szObjectSize = szLine + nCommaIndex + 1;

			// extract object size and return
			p_nObjectSize = atoi(szObjectSize);
			fclose(pFile);
			return;
		}

		// close file if no matching parameter found
		fclose(pFile);
	}

	//------------------------------------------------------------------------------------------
	//  Memory buffer header block
	//------------------------------------------------------------------------------------------
	struct MemoryHeader
	{
		int m_nSignature;
		bool m_bPooled;
		void *m_pMemoryManager;
		size_t m_unSize;
		MemoryHeader *m_pNext;
	};

	//- Private Methods ------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	//  Frees the memory buffer using the ad-hoc deallocation scheme.
	//------------------------------------------------------------------------------------------
	void MemoryManager::FreeAdhoc(void *p_pData)
	{
		// get the memory header prefix
		MemoryHeader *pMemoryHeader = (MemoryHeader *) p_pData - 1;

		// decrease used memory count and instance
		m_unUsedMemoryAdhoc -= pMemoryHeader->m_unSize;
		--m_unUsedCountAdhoc;

		// free the buffer including header
		free(pMemoryHeader);
	}

	//------------------------------------------------------------------------------------------
	//  Frees the memory buffer using the pooled deallocation scheme.
	//------------------------------------------------------------------------------------------
	void MemoryManager::FreePooled(void *p_pData)
	{
		// get the memory header prefix
		MemoryHeader *pMemoryHeader = (MemoryHeader *) p_pData - 1;

		// place relinquished buffer as the head of the free buffer list
		if (m_pFreeHead == NULL)
			m_pFreeHead = pMemoryHeader;
		else
		{
			MemoryHeader *pMemoryHeaderHead = (MemoryHeader *) m_pFreeHead;
			pMemoryHeader->m_pNext = pMemoryHeaderHead;
			m_pFreeHead = pMemoryHeader;
		}

		// decrease used memory count and instance
		m_unUsedMemoryPooled -= pMemoryHeader->m_unSize;
		--m_unUsedCountPooled;
		pMemoryHeader->m_unSize = 0;
	}

	//- Public Static Methods ------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	/// Returns a memory manager instance associated with the given memory pointer. An exception
	/// is thrown if the pointer does not belong to any memory manager.
	/// \param p_pData Memory pointer for which to retrieve the manager.
	/// \returns Pointer to the associated memory manager.
	//------------------------------------------------------------------------------------------
	MemoryManager *MemoryManager::FromPointer(void *p_pData)
	{
		// get the memory header prefix
		MemoryHeader *pMemoryHeader = (MemoryHeader *) p_pData - 1;

		// check if signature matches - if not memory corrupt or buffer not created by this
		// memory system
		if (pMemoryHeader->m_nSignature != MEMORY_SIGNATURE)
			throw new MesonException(
				"Cannot determine memory manager from the given pointer",
				__FILE__, __LINE__);

		// return the manager associated with this buffer
		return (MemoryManager *) pMemoryHeader->m_pMemoryManager;
	}

	//- Public Methods -------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	/// Constructs a memory manager with the given name, parent manager, object size and pool
	/// size. If no parent manager is specified, the manager is not placed in a hierarchy.
	/// \param p_pszName Memory manager name.
	/// \param p_pParent Optional parent memory manager.
	/// \param p_unObjectSize Size of the allocated objects.
	/// \param p_unPoolSize Size of the memory pool.
	//------------------------------------------------------------------------------------------
	MemoryManager::MemoryManager(
		const char *p_pszName, MemoryManager *p_pParent,
		size_t p_unObjectSize, size_t p_unPoolSize)
		: m_unObjectSize(p_unObjectSize)
		, m_unPoolSize(p_unPoolSize)
		, m_unUsedMemoryPooled(0)
		, m_unUsedMemoryAdhoc(0)
		, m_unUsedCountPooled(0)
		, m_unUsedCountAdhoc(0)
		, m_pFreeHead(NULL)
		, m_pMemoryPool(NULL)
		, m_pNextSibling(NULL)
		, m_pFirstChild(NULL)
	{
		// assign name
		strcpy(m_szName, p_pszName); 

		// update pool configuration if available from config file
		int nPoolSize = -1;
		int nObjectSize = -1;
		GetMemoryPoolConfiguration(m_szName, nPoolSize, nObjectSize);
		if (nPoolSize >= 0)
			m_unPoolSize = (size_t) nPoolSize;
		if (nObjectSize > (int) p_unObjectSize)
			p_unObjectSize = (size_t) nObjectSize;

		if (p_pParent != NULL)
		{
			// if parent given, add as new child to parent
			if (p_pParent->m_pFirstChild == NULL)
				p_pParent->m_pFirstChild = this;
			else
			{
				MemoryManager *pLastChild = p_pParent->m_pFirstChild;
				while (pLastChild->m_pNextSibling != NULL)
					pLastChild = pLastChild->m_pNextSibling;
				pLastChild->m_pNextSibling = this;
			}
		}


		// do nothing else if pool size is zero
		if (m_unPoolSize == 0) return;

		// allocate contigous pool buffer
		size_t unGrossSize = sizeof(MemoryHeader) + p_unObjectSize;
		m_pMemoryPool = malloc(m_unPoolSize * unGrossSize);

		// place all pool slots on free list
		m_pFreeHead = m_pMemoryPool;
		char *pHeader = (char *) m_pMemoryPool;
		MemoryHeader *pMemoryHeaderPrevious = NULL;
		for (size_t unIndex = 0; unIndex < m_unPoolSize; unIndex++)
		{
			// initialise memory header for each slot
			MemoryHeader *pMemoryHeader = (MemoryHeader *) pHeader;
			pMemoryHeader->m_nSignature = MEMORY_SIGNATURE;
			pMemoryHeader->m_bPooled = true;
			pMemoryHeader->m_pMemoryManager = this;
			pMemoryHeader->m_pNext = NULL;

			// link to previous for subsequent slots
			if (pMemoryHeaderPrevious)
				pMemoryHeaderPrevious->m_pNext = pMemoryHeader;

			// keep track of previous node on free list
			pMemoryHeaderPrevious = pMemoryHeader;

			// advance slot pointer
			pHeader += unGrossSize;
		}
	}

	//------------------------------------------------------------------------------------------
	/// Constructs an uninitialised memory manager.
	//------------------------------------------------------------------------------------------
	MemoryManager::MemoryManager(void)
	{
		m_unObjectSize = 0;
		m_unPoolSize = 0;
		m_unUsedCountPooled = 0;
		m_unUsedCountAdhoc = 0;
		m_pFreeHead = NULL;
		m_pMemoryPool = NULL;
		m_pNextSibling = NULL;
		m_pFirstChild = NULL;
		strcpy(m_szName, "(not initialised)"); 
	}

	//------------------------------------------------------------------------------------------
	/// Destroys the memory manager.
	//------------------------------------------------------------------------------------------
	MemoryManager::~MemoryManager(void)
	{
		// do nothing else if pool size is zero
		if (m_unPoolSize == 0) return;

		// otherwise, clear pool buffer
		free(m_pMemoryPool);
	}

	//------------------------------------------------------------------------------------------
	/// Returns the name of the memory manager.
	/// \returns the name of the memory manager.
	//------------------------------------------------------------------------------------------
	const char *MemoryManager::GetName(void) const
	{
		return m_szName;
	}


	//------------------------------------------------------------------------------------------
	/// Returns true if the memory manager has a non-zero pool size, or false otherwise.
	/// \returns true if the memory manager has a non-zero pool size, or false otherwise.
	//------------------------------------------------------------------------------------------
	bool MemoryManager::IsPooled(void) const
	{
		return m_unPoolSize > 0;
	}


	//------------------------------------------------------------------------------------------
	/// Returns the total memory pre-allocated and currently allocated, optionally including
	/// the memory of any child managers.
	/// \param p_bIncludeChildren Flag to include child manager memory or otherwise.
	/// \returns the memory allocated by the manager and optionally its children.
	//------------------------------------------------------------------------------------------
	size_t MemoryManager::GetAllocatedMemory(bool p_bIncludeChildren) const
	{
		// calculate locally allocated memory
		size_t unCount
			= m_unPoolSize * (sizeof(MemoryHeader) + m_unObjectSize)
				+ m_unUsedMemoryAdhoc;

		// if children to be excluded return current value
		if (!p_bIncludeChildren) return unCount;

		// otherwise, recursively add values from children
		MemoryManager *pChild = m_pFirstChild;
		while (pChild != NULL)
		{
			unCount += pChild->GetAllocatedMemory(true);
			pChild = pChild->m_pNextSibling;
		}

		// return recursively calculated value
		return unCount;
	}


	//------------------------------------------------------------------------------------------
	/// Returns the total memory currently in use, optionally includin the memory of any child
	/// managers.
	/// \param p_bIncludeChildren Flag to include child manager memory or otherwise.
	/// \returns the memory currently in use by the manager and optionally its children.
	//------------------------------------------------------------------------------------------
	size_t MemoryManager::GetUsedMemory(bool p_bIncludeChildren) const
	{
		size_t unCount = m_unUsedMemoryPooled + m_unUsedMemoryAdhoc;

		// if children to be excluded return current value
		if (!p_bIncludeChildren) return unCount;

		// otherwise, recursively add values from children
		MemoryManager *pChild = m_pFirstChild;
		while (pChild != NULL)
		{
			unCount += pChild->GetUsedMemory(true);
			pChild = pChild->m_pNextSibling;
		}

		// return recursively calculated value
		return unCount;
	}

	//------------------------------------------------------------------------------------------
	/// Returns the total pre-allocated and currently allocated object instances, optionally
	/// including the memory of any child managers.
	/// \param p_bIncludeChildren Flag to include child manager instances or otherwise.
	/// \returns a count of instances allocated by the manager and optionally its children.
	//------------------------------------------------------------------------------------------
	size_t MemoryManager::GetAllocatedInstances(bool p_bIncludeChildren) const
	{
		size_t unCount = m_unPoolSize + m_unUsedCountAdhoc;

		// if children to be excluded return current value
		if (!p_bIncludeChildren) return unCount;

		// otherwise, recursively add values from children
		MemoryManager *pChild = m_pFirstChild;
		while (pChild != NULL)
		{
			unCount += pChild->GetAllocatedInstances(true);
			pChild = pChild->m_pNextSibling;
		}

		// return recursively calculated value
		return unCount;
	}


	//------------------------------------------------------------------------------------------
	/// Returns a count of object instances currently in use, optionally including counts of any
	/// child managers.
	/// \param p_bIncludeChildren Flag to include child manager instances or otherwise.
	/// \returns a count of instances in use by the manager and optionally its children.
	//------------------------------------------------------------------------------------------
	size_t MemoryManager::GetUsedInstances(bool p_bIncludeChildren) const
	{
		size_t unCount = m_unUsedCountPooled
			+ m_unUsedCountAdhoc;

		// if children to be excluded return current value
		if (!p_bIncludeChildren) return unCount;

		// otherwise, recursively add values from children
		MemoryManager *pChild = m_pFirstChild;
		while (pChild != NULL)
		{
			unCount += pChild->GetUsedInstances(true);
			pChild = pChild->m_pNextSibling;
		}

		// return recursively calculated value
		return unCount;
	}


	//------------------------------------------------------------------------------------------
	/// Returns true if the memory referenced by the given pointer is owned by the manager, or
	/// false otherwise;
	/// \param p_pData Memory reference for which to check ownership.
	/// \returns true if the memory referenced by the given pointer is owned by the manager, or
	/// false otherwise;
	//------------------------------------------------------------------------------------------
	bool MemoryManager::IsOwnedMemory(void *p_pData) const
	{
		// get memory header
		MemoryHeader *pMemoryHeader = (MemoryHeader *) p_pData - 1;

		// check memory signature
		if (pMemoryHeader->m_nSignature != MEMORY_SIGNATURE)
			return false;

		// check if manager pointer refers to this instance
		if (pMemoryHeader->m_pMemoryManager != this)
			return false;

		// if all tests pass, buffer is owned by this manager
		return true;
	}


	//------------------------------------------------------------------------------------------
	/// Allocates a memory buffer of the given size using the ad-hoc allocation scheme.
	/// \param p_unSize Size of the required memory buffer.
	/// \returns Pointer to the allocated memory buffer.
	//------------------------------------------------------------------------------------------
	void *MemoryManager::AllocateAdhoc(size_t p_unSize)
	{
		// calculate buffer size including header
		size_t unGrossSize = sizeof(MemoryHeader) + p_unSize;

		// allocate buffer using standard C function
		void *pData = malloc(unGrossSize);

		// throw exception if allocation fails
		if (pData == NULL)
		{
			char szMessage[2048], szSize[64];
			strcpy(szMessage, "Cannot allocate the requested ad-hoc buffer. Manager: ");
			strcat(szMessage, GetName());
			strcat(szMessage, " Size: ");
			itoa((int) p_unSize, szSize, 10);
			strcat(szMessage, szSize);
			throw new MesonException(
				szMessage,
				__FILE__, __LINE__);
		}

		// format buffer header
		MemoryHeader *pMemoryHeader = (MemoryHeader *) pData;
		pMemoryHeader->m_nSignature = MEMORY_SIGNATURE;
		pMemoryHeader->m_bPooled = false;
		pMemoryHeader->m_pMemoryManager = this;
		pMemoryHeader->m_unSize = p_unSize;
		pMemoryHeader->m_pNext = NULL;

		// track memory usage
		m_unUsedMemoryAdhoc += unGrossSize;
		++m_unUsedCountAdhoc;

		// return buffer offset from header
		return (MemoryHeader *) pData + 1;
	}


	//------------------------------------------------------------------------------------------
	/// Allocates a memory buffer of the given size using the pooled allocation scheme. If the
	/// required buffer size is greater than configured object size or if the pool is full, the
	/// request is delegated to the ad-hoc allocation scheme.
	/// \param p_unSize Size of the required memory buffer.
	/// \returns Pointer to the allocated memory buffer.
	//------------------------------------------------------------------------------------------
	void *MemoryManager::AllocatePooled(size_t p_unSize)
	{
		// if requested buffer size greater than pooled object size, or if
		// pool is used up, delegate request to ad-hoc system
		if ((p_unSize > m_unObjectSize) || (m_unUsedCountPooled >= m_unPoolSize))
			return AllocateAdhoc(p_unSize);
		// otherwise, remove first entry in free list and use it for allocation
		MemoryHeader *pMemoryHeader = (MemoryHeader *) m_pFreeHead;
		m_pFreeHead = pMemoryHeader->m_pNext;
		pMemoryHeader->m_pNext = NULL;

		// format memory header
		pMemoryHeader->m_unSize = p_unSize;

		// track memory usage
		m_unUsedMemoryPooled += sizeof(MemoryHeader) + p_unSize;
		++m_unUsedCountPooled;

		// return buffer offset from header
		return pMemoryHeader + 1;
	}


	//------------------------------------------------------------------------------------------
	/// Frees the memory buffer given by the pointer using the appropriate ad-hoc or pooled
	/// deallocation scheme. An exception is thrown if the memory header is corrupted or if the
	/// buffer does not belong to the manager.
	/// \param p_pData Pointer to buffer to be deallocated.
	//------------------------------------------------------------------------------------------
	void MemoryManager::Free(void *p_pData)
	{
		// locate leading memory header
		MemoryHeader *pMemoryHeader = (MemoryHeader *) p_pData - 1;

		// verify signature
		if (pMemoryHeader->m_nSignature != MEMORY_SIGNATURE)
			throw new MesonException(
				"The given buffer cannot be freed because it is corrupted or allocated by another system.",
				__FILE__, __LINE__);

		// verify that referenced manager points to this instance
		if (pMemoryHeader->m_pMemoryManager != this)
			throw new MesonException(
				"The given buffer cannot be freed because it does not belong to this memory manager.",
				__FILE__, __LINE__);

		// free using pooled or adhoc scheme depending on flag
		if (pMemoryHeader->m_bPooled)
			FreePooled(p_pData);
		else
			FreeAdhoc(p_pData);
	}


	//------------------------------------------------------------------------------------------
	/// Returns the next sibling in a memory manager hierarchy.
	/// \returns the next sibling in a memory manager hierarchy.
	//------------------------------------------------------------------------------------------
	MemoryManager *MemoryManager::GetNextSibling(void) const
	{
		return m_pNextSibling;
	}


	//------------------------------------------------------------------------------------------
	/// Returns the first child in a memory manager hierarchy.
	/// \returns the first child in a memory manager hierarchy.
	//------------------------------------------------------------------------------------------
	MemoryManager *MemoryManager::GetFirstChild(void) const
	{
		return m_pFirstChild;
	}

Meson_Common_Memory_END
