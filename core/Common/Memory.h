//----------------------------------------------------------------------------------------------
//	Interface for the memory management system. Provides interface declaration and code
//  implementation macros to keep track of memory allocation and to provide transparent support
//  for object pooling.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella 	17/07/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include "MemoryManager.h"

#ifndef NULL
	#define NULL 0
#endif


//----------------------------------------------------------------------------------------------
/// Returns the root memory manager used for non-specialised allocations.
/// \returns Pointer to the root memory manager.
//----------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager *GetRootMemoryManager(void);


//----------------------------------------------------------------------------------------------
/// Allocates object instance memory using the root memory manager.
/// \param p_unSize Allocation size in bytes.
/// \returns Pointer to the allocated memory.
//----------------------------------------------------------------------------------------------
void *operator new(size_t p_unSize);


//----------------------------------------------------------------------------------------------
/// Allocates object array memory using the array memory manager.
/// \param p_unSize Allocation size in bytes.
/// \returns Pointer to the allocated memory.
//----------------------------------------------------------------------------------------------
void *operator new[](size_t p_unSize);


//----------------------------------------------------------------------------------------------
/// Deletes object instance memory using the root memory manager.
/// \param p_pData Pointer to allocated memory.
//----------------------------------------------------------------------------------------------
void operator delete(void *p_pData);


//----------------------------------------------------------------------------------------------
/// Deletes object array memory using the array memory manager.
/// \param p_pData Pointer to allocated memory.
//----------------------------------------------------------------------------------------------
void operator delete[](void *p_pData);


//----------------------------------------------------------------------------------------------
///  Class declaration macro for memory management. The macro must be invoked within a class
///  declaration.
//----------------------------------------------------------------------------------------------
#define DECLARE_MEMORY												\
	public:															\
		static void *operator new(size_t unSize);					\
		static void operator delete(void *pObject,  size_t unSize);	\
		static Meson::Common::Memory::MemoryManager *				\
			GetMemoryManager(void);									\
																	\
	private:														\
		static Meson::Common::Memory::MemoryManager s_memoryManager;


//----------------------------------------------------------------------------------------------
/// Class implementation macro for pooled memory management under the root memory manager.
/// \param P_CLASS Class name.
/// \param P_NAME Memory manager name.
/// \param P_POOLSIZE Object pool size.
//----------------------------------------------------------------------------------------------
#define IMPLEMENT_MEMORY_POOLED(P_CLASS, P_NAME, P_POOLSIZE)		\
	Meson::Common::Memory::MemoryManager P_CLASS::s_memoryManager	\
		= Meson::Common::Memory::MemoryManager(						\
			P_NAME, GetRootMemoryManager(),							\
			sizeof(P_CLASS), P_POOLSIZE);							\
																	\
	IMPLEMENT_MEMORY_COMMON(P_CLASS)


//----------------------------------------------------------------------------------------------
/// Class implementation macro for pooled memory management under a given memory manager.
/// \param P_CLASS Class name.
/// \param P_PARENT Parent class name (must also use memory management).
/// \param P_NAME Memory manager name.
/// \param P_POOLSIZE Object pool size.
//----------------------------------------------------------------------------------------------
#define IMPLEMENT_MEMORY_POOLED_CHILD(P_CLASS, P_PARENT, P_NAME, P_POOLSIZE)	\
	Meson::Common::Memory::MemoryManager P_CLASS::s_memoryManager				\
		= Meson::Common::Memory::MemoryManager(									\
			P_NAME, P_PARENT::GetMemoryManager(),								\
			sizeof(P_CLASS), P_POOLSIZE);										\
																				\
	IMPLEMENT_MEMORY_COMMON(P_CLASS)


//----------------------------------------------------------------------------------------------
/// Class implementation macro for ad-hoc memory management under the root memory manager.
/// \param P_CLASS Class name.
/// \param P_NAME Memory manager name.
//----------------------------------------------------------------------------------------------
#define IMPLEMENT_MEMORY_ADHOC(P_CLASS, P_NAME)	\
	IMPLEMENT_MEMOR_POOLED(P_CLASS, P_NAME, 0)


//----------------------------------------------------------------------------------------------
/// Class implementation macro for ad-hoc memory management under a given memory manager.
/// \param P_CLASS Class name.
/// \param P_PARENT Parent class name (must also use memory management).
/// \param P_NAME Memory manager name.
//----------------------------------------------------------------------------------------------
#define IMPLEMENT_MEMORY_ADHOC_CHILD(P_CLASS, P_PARENT, P_NAME)	\
	IMPLEMENT_MEMOR_POOLED_CHILD(P_CLASS, P_PARENT, P_NAME, 0)


//----------------------------------------------------------------------------------------------
/// Common memory implementation macro.
/// \param P_CLASS Class name.
//----------------------------------------------------------------------------------------------
#define IMPLEMENT_MEMORY_COMMON(P_CLASS)									\
	void *P_CLASS::operator new(size_t unSize)								\
	{																		\
		return s_memoryManager.AllocatePooled(unSize);						\
	}																		\
																			\
	void P_CLASS::operator delete(void *pObject, size_t unSize)				\
	{																		\
		::operator delete(pObject);											\
	}																		\
																			\
	Meson::Common::Memory::MemoryManager *P_CLASS::GetMemoryManager(void)	\
	{																		\
		return &s_memoryManager;											\
	}
