//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::MemoryMgr
//	The Memory Manager class manages a heap block. The class uses copying garbage collection
//  to free the garbage blocks.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	18/10/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------


#include <stdlib.h>


#include "MemoryMgr.h"
#include "Handle.h"

//----------------------------------------------------------------------------------------------
//	Namespaces
//----------------------------------------------------------------------------------------------
using namespace Meson::Common::Collections;

Meson_Cuneus_BEGIN

//----------------------------------------------------------------------------------------------
// Construction
//----------------------------------------------------------------------------------------------
MemoryMgr::MemoryMgr(void)
{
	m_byActive = 0;
	m_byPassive = 1;

	m_dwHeapSize = 0;
	m_dwOffset = 0;

	m_pbyHeap[0] = NULL;
	m_pbyHeap[1] = NULL;

	m_plstHandle[0] = NULL;
	m_plstHandle[1] = NULL;
}

MemoryMgr::~MemoryMgr(void)
{
	Shutdown();
}

//----------------------------------------------------------------------------------------------
// Initialisation of the Heap Memory
//----------------------------------------------------------------------------------------------
void MemoryMgr::Startup( dword p_dwMin, dword p_dwMax )
{
	m_plstHandle[0] = new TArrayList<HandlePointer*>();
	m_plstHandle[1] = new TArrayList<HandlePointer*>();

	m_dwHeapSize = p_dwMin;
	m_dwMax = p_dwMax;

	m_byActive = 0;
	m_byPassive = 1;

	m_pbyHeap[0] = new byte[ m_dwHeapSize ];
	m_pbyHeap[1] = new byte[ m_dwHeapSize ];
}

//----------------------------------------------------------------------------------------------
// Releases the Heap Memory
//----------------------------------------------------------------------------------------------
void MemoryMgr::Shutdown( void )
{
	if( m_plstHandle[0] )
	{
		FreePointers(*m_plstHandle[0]);
		FreePointers(*m_plstHandle[1]);

		delete m_plstHandle[0];
		delete m_plstHandle[1];

		delete[] m_pbyHeap[0];
		delete[] m_pbyHeap[1];
	}
}

//----------------------------------------------------------------------------------------------
// Resizes the heap to the passed size given it is less than the Maximum
//----------------------------------------------------------------------------------------------
dword MemoryMgr::ExpandHeap( dword p_dwHint )
{
	dword dwTemp = ( m_dwHeapSize << 1 );

	if( p_dwHint < dwTemp )
		p_dwHint = dwTemp;

	if( p_dwHint > m_dwMax )
		p_dwHint = m_dwMax;


	delete m_pbyHeap[ m_byPassive ];

	m_pbyHeap[ m_byPassive ] = new byte[ p_dwHint ];
	memcpy( m_pbyHeap[ m_byPassive ], m_pbyHeap[ m_byActive ], m_dwOffset );
	delete m_pbyHeap[ m_byActive ];
	m_pbyHeap[ m_byActive ] = m_pbyHeap[ m_byPassive ];
	m_pbyHeap[ m_byPassive ] = new byte[ p_dwHint ];

	m_dwHeapSize = p_dwHint;

	return p_dwHint;
}

//----------------------------------------------------------------------------------------------
// Recycles or produces new HandlePointer structures
//----------------------------------------------------------------------------------------------
HandlePointer *MemoryMgr::CreateHandlePointer( void )
{
	if( m_plstHandle[ m_byPassive ]->Size() > 0 )
		return m_plstHandle[ m_byPassive ]->Pop( m_plstHandle[ m_byPassive ]->Size() -1 );


	return new HandlePointer();
}

//----------------------------------------------------------------------------------------------
// Allocates a block of memory from the heap
//----------------------------------------------------------------------------------------------
Handle MemoryMgr::Alloc( dword p_dwSize )
{
	Handle handle;
	HandlePointer *pHandlePtr = CreateHandlePointer();

	if( (m_dwOffset + p_dwSize) >= m_dwHeapSize )
		ExpandHeap( m_dwHeapSize + (p_dwSize << 2) );

	pHandlePtr->Marked = 0;
	pHandlePtr->Size = p_dwSize;
	pHandlePtr->Offset = m_dwOffset;
	m_dwOffset += p_dwSize;

	m_plstHandle[ m_byActive ]->Add( pHandlePtr );
	handle.qwValue = (qword) pHandlePtr;
	handle.Valid = 1;
	handle.Native = 0;

	return handle;
}


//----------------------------------------------------------------------------------------------
// Frees a block of memory pointed to by the handle
//----------------------------------------------------------------------------------------------
void MemoryMgr::Free( Handle &p_handle )
{
	HandlePointer &hndlPtr = p_handle.GetHandlePointer();

	hndlPtr.RefCountDec();

	if( hndlPtr.RefCount == 0 )
	{
		hndlPtr.Marked = 1;
	}

	p_handle.Valid = 0;
	p_handle.HandlePtr = 0;
}

//----------------------------------------------------------------------------------------------
// Re-Allocates a block of memory
//----------------------------------------------------------------------------------------------
void MemoryMgr::Realloc( Handle &p_handle, dword p_dwSize )
{
	HandlePointer &handlePtr = p_handle.GetHandlePointer();

	if( (m_dwOffset + p_dwSize) >= m_dwHeapSize )
		ExpandHeap( m_dwHeapSize + (p_dwSize << 2) );

	handlePtr.Marked = 0;
	handlePtr.Size = p_dwSize;
	handlePtr.Offset = m_dwOffset;
	m_dwOffset += p_dwSize;
}

//----------------------------------------------------------------------------------------------
// Performs the 'scavenging' operation by transferring the live blocks to the passive and then
// switching the heaps
//----------------------------------------------------------------------------------------------
void MemoryMgr::Migrate( void )
{
	size_t unLen = m_plstHandle[ m_byActive ]->Size();
	TArrayList< HandlePointer* > *m_plstTemp = new TArrayList< HandlePointer* >(unLen);

	HandlePointer *pHandlePtr = NULL;
	m_dwOffset = 0;

	for( size_t unPos=0; unPos < unLen; unPos++)
	{
		pHandlePtr = (*m_plstHandle[ m_byActive ])( unPos );
		if( pHandlePtr->Marked )
		{
			m_plstHandle[ m_byPassive ]->Add( pHandlePtr );
			pHandlePtr->RefCount = 1;
		}
		else
		{
			MoveActiveToPassive( m_dwOffset, pHandlePtr->Offset, pHandlePtr->Size );
			pHandlePtr->Offset = m_dwOffset;
			m_dwOffset += (dword) pHandlePtr->Size;
			m_plstTemp->Add( pHandlePtr );
		}
	}

	delete m_plstHandle[ m_byActive ];

	Switch();

	m_plstHandle[ m_byPassive ] = m_plstHandle[ m_byActive ];
	m_plstHandle[ m_byActive ] = m_plstTemp;
}

Meson_Cuneus_END
