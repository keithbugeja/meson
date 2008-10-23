//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::MemoryMgr
//	The Memory Manager class manages a heap block. The class uses copying garbage collection
//  to free the garbage blocks.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	18/10/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <string.h>

#include <types.h>
#include <ArrayList.h>

#include <CnsNamespaces.h>

Meson_Cuneus_BEGIN

// Forward Referencing
struct Handle;
struct HandlePointer;



class MemoryMgr
{
private:
	byte *m_pbyHeap[2];

	dword	m_dwHeapSize,
			m_dwOffset,
			m_dwMax;

	//----------------------------------------------------------------------------------------------
	// Used to preserve the condition m_byActive = !m_byPassive
	//----------------------------------------------------------------------------------------------
	union
	{
		struct
		{
			byte m_byActive;
			byte m_byPassive;
		};

		word m_wState;
	};

	//----------------------------------------------------------------------------------------------
	// Collections of HandlePointer* for Active, Passive and Temporary
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TArrayList< HandlePointer* > *m_plstHandle[3];


public:
	//----------------------------------------------------------------------------------------------
	// Construction
	//----------------------------------------------------------------------------------------------
	MemoryMgr( void );
	~MemoryMgr( void );

	//----------------------------------------------------------------------------------------------
	// Initialisation of the Heap Memory
	//----------------------------------------------------------------------------------------------
	void Startup( dword p_dwMin, dword p_dwMax );

	//----------------------------------------------------------------------------------------------
	// Releases the Heap Memory
	//----------------------------------------------------------------------------------------------
	void Shutdown( void );

	//----------------------------------------------------------------------------------------------
	// Resizes the heap to the passed size given it is less than the Maximum
	//----------------------------------------------------------------------------------------------
	dword ExpandHeap( dword p_dwHint );

	//----------------------------------------------------------------------------------------------
	// Recycles or produces new HandlePointer structures
	//----------------------------------------------------------------------------------------------
	HandlePointer *CreateHandlePointer( void );

	//----------------------------------------------------------------------------------------------
	// Allocates a block of memory from the heap
	//----------------------------------------------------------------------------------------------
	Handle Alloc( dword p_dwSize );

	//----------------------------------------------------------------------------------------------
	// Frees a block of memory pointed to by the handle
	//----------------------------------------------------------------------------------------------
	void Free( Handle &p_handle );

	//----------------------------------------------------------------------------------------------
	// Re-Allocates a block of memory
	//----------------------------------------------------------------------------------------------
	void Realloc( Handle &p_handle, dword p_dwSize );

	//----------------------------------------------------------------------------------------------
	// Performs the 'scavenging' operation by transferring the live blocks to the passive and then
	// switching the heaps
	//----------------------------------------------------------------------------------------------
	void Migrate( void );

	//----------------------------------------------------------------------------------------------
	// Returns the Active Space
	//----------------------------------------------------------------------------------------------
	inline byte *GetActive( void )
	{
		return m_pbyHeap[ m_byActive ];
	}

	//----------------------------------------------------------------------------------------------
	// Returns the Passive Space
	//----------------------------------------------------------------------------------------------
	inline byte *GetPassive( void )
	{
		return m_pbyHeap[ m_byPassive ];
	}

	//----------------------------------------------------------------------------------------------
	// Switch Spaces
	//----------------------------------------------------------------------------------------------
	inline byte *Switch( void )
	{
		m_wState = ~m_wState & 0x0101;
		return m_pbyHeap[ m_byActive ];
	}

	//----------------------------------------------------------------------------------------------
	// Returns the Active space number
	//----------------------------------------------------------------------------------------------
	inline byte GetActiveNumber( void )
	{
		return m_byActive;
	}

	//----------------------------------------------------------------------------------------------
	// Returns an absolute pointer to the Active Heap Address
	//----------------------------------------------------------------------------------------------
	inline byte *ResolveActivePtr( dword p_dwAddress )
	{
		return (m_pbyHeap[ m_byActive ] + p_dwAddress);
	}

	//----------------------------------------------------------------------------------------------
	// Returns an absolute pointer to the Passive Heap Address
	//----------------------------------------------------------------------------------------------
	inline byte *ResolvePassivePtr( dword p_dwAddress )
	{
		return (m_pbyHeap[ m_byPassive ] + p_dwAddress);
	}

	//----------------------------------------------------------------------------------------------
	// Returns an absolute pointer to the Active or Passive Heap Address
	//----------------------------------------------------------------------------------------------
	inline byte *ResolvePtr( byte p_byPage, dword p_dwAddress )
	{
		return (m_pbyHeap[ p_byPage ] + p_dwAddress);
	}

	//----------------------------------------------------------------------------------------------
	// Sets the byte value of the Active Address
	//----------------------------------------------------------------------------------------------
	inline void Set( dword p_dwAddress, byte p_byValue )
	{
		if( p_dwAddress >= m_dwHeapSize )
			throw Meson::Common::MesonException( "Heap index out of range.", __FILE__, __LINE__ );
		m_pbyHeap[ m_byActive ][ p_dwAddress ] = p_byValue;
	}

	//----------------------------------------------------------------------------------------------
	// Sets the byte value of the Passive Address
	//----------------------------------------------------------------------------------------------
	inline void SetPassive( dword p_dwAddress, byte p_byValue )
	{
		if( p_dwAddress >= m_dwHeapSize )
			throw Meson::Common::MesonException( "Heap index out of range.", __FILE__, __LINE__ );
		m_pbyHeap[ m_byPassive ][ p_dwAddress ] = p_byValue;
	}

	//----------------------------------------------------------------------------------------------
	// Returns the byte value of the Active Address
	//----------------------------------------------------------------------------------------------
	inline byte Get( dword p_dwAddress )
	{
		if( p_dwAddress >= m_dwHeapSize )
			throw Meson::Common::MesonException( "Heap index out of range.", __FILE__, __LINE__ );

		return m_pbyHeap[ m_byActive ][ p_dwAddress ];
	}

	//----------------------------------------------------------------------------------------------
	// Returns the byte value of the Passive Address
	//----------------------------------------------------------------------------------------------
	inline byte GetPassive( dword p_dwAddress )
	{
		if( p_dwAddress >= m_dwHeapSize )
			throw Meson::Common::MesonException( "Heap index out of range.", __FILE__, __LINE__ );

		return m_pbyHeap[ m_byPassive ][ p_dwAddress ];
	}

	//----------------------------------------------------------------------------------------------
	// Returns reference to byte at Active Heap address including range check
	//----------------------------------------------------------------------------------------------
	inline byte &operator[]( dword p_dwAddress )
	{
		if( p_dwAddress >= m_dwHeapSize )
			throw Meson::Common::MesonException( "Heap index out of range.", __FILE__, __LINE__ );

		return m_pbyHeap[ m_byActive ][ p_dwAddress ];
	}

	//----------------------------------------------------------------------------------------------
	// Returns reference to byte at Active Heap address
	//----------------------------------------------------------------------------------------------
	inline byte &operator()( dword p_dwAddress )
	{
		return m_pbyHeap[ m_byActive ][ p_dwAddress ];
	}

	//----------------------------------------------------------------------------------------------
	// Copies a block of memory in the Active Heap
	//----------------------------------------------------------------------------------------------
	inline void CopyActive( dword p_dwDstAddress, dword p_dwSrcAddress, dword p_dwSize )
	{
		byte *pBase = GetActive();
		memcpy( pBase + p_dwDstAddress, pBase + p_dwSrcAddress, p_dwSize );
	}

	//----------------------------------------------------------------------------------------------
	// Copies a block of memory
	//----------------------------------------------------------------------------------------------
	inline void Copy( void *p_pDst, void *p_pSrc, dword p_dwSize )
	{
		memcpy( p_pDst, p_pSrc, p_dwSize );
	}

	//----------------------------------------------------------------------------------------------
	// Moves a block of memory from the Active to the Passive Heap
	//----------------------------------------------------------------------------------------------
	inline void MoveActiveToPassive( dword p_dwDstAddress, dword p_dwSrcAddress, dword p_dwSize )
	{
		byte *pActiveBase = GetActive();
		byte *pPassiveBase = GetPassive();
		memcpy( pPassiveBase + p_dwDstAddress, pActiveBase + p_dwSrcAddress, p_dwSize );
	}

	//----------------------------------------------------------------------------------------------
	// Moves a block of memory from the Passive to the Active Heap
	//----------------------------------------------------------------------------------------------
	inline void MovePassiveToActive( dword p_dwDstAddress, dword p_dwSrcAddress, dword p_dwSize )
	{
		byte *pActiveBase = GetActive();
		byte *pPassiveBase = GetPassive();
		memcpy( pActiveBase + p_dwDstAddress, pPassiveBase + p_dwSrcAddress, p_dwSize );
	}

	//----------------------------------------------------------------------------------------------
	// Fills a block of memory in the Active Heap
	//----------------------------------------------------------------------------------------------
	inline void FillActive( dword p_dwDstAddress, dword p_dwValue, dword p_dwSize )
	{
		byte *pBase = GetActive();
		memset( pBase + p_dwDstAddress, p_dwValue, p_dwSize );
	}

	//----------------------------------------------------------------------------------------------
	// Fills a block of memory
	//----------------------------------------------------------------------------------------------
	inline void Fill( void *p_pDst, dword p_dwValue, dword p_dwSize )
	{
		memset( p_pDst, p_dwValue, p_dwSize );
	}


};

Meson_Cuneus_END
