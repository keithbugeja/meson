#include "Scavenger.h"

Meson_Cuneus_BEGIN

Scavenger::Scavenger(void)
{
	for( int n=0; n<MEMMGR_NUM; n++ )
	{
		m_pMemoryMgr[ n ] = NULL;
	}
}

Scavenger::~Scavenger(void)
{
	Shutdown();
}


void Scavenger::Startup( Meson::Common::Config::Configuration &p_config )
{
	dword dwMin = p_config.GetDWord( CuneusVirtualMachine::m_szVM_Heap_Min );
	dword dwMax = p_config.GetDWord( CuneusVirtualMachine::m_szVM_Heap_Min );

	if( dwMin == 0 ) dwMin = 65536;
	if( dwMax == 0 ) dwMax = 65536;


	for( int n=0; n<MEMMGR_NUM; n++ )
	{
		m_pMemoryMgr[ n ] = new MemoryMgr();
		m_pMemoryMgr[ n ]->Startup( dwMin, dwMax );
	}
}

void Scavenger::Shutdown( void )
{
	for( int n=0; n<MEMMGR_NUM; n++ )
	{
		if( m_pMemoryMgr[ n ] )
			delete m_pMemoryMgr[ n ];
	}
}

//----------------------------------------------------------------------------------------------
// Allocates a block of memory
//----------------------------------------------------------------------------------------------
MemoryHandle Scavenger::Alloc( dword p_dwSize )
{
	Handle handle;

	if( p_dwSize <= 32 )
	{
		handle.Region = 0;
		handle = m_pMemoryMgr[ handle.Region ]->Alloc( p_dwSize );
		return handle.hMemory;
	}
	else if( p_dwSize <= 64 )
	{
		handle.Region = 1;
		handle = m_pMemoryMgr[ handle.Region ]->Alloc( p_dwSize );
		return handle.hMemory;
	}
	else if( p_dwSize <= 256 )
	{
		handle.Region = 2;
		handle = m_pMemoryMgr[ handle.Region ]->Alloc( p_dwSize );
		return handle.hMemory;
	}
	else if( p_dwSize <= 512 )
	{
		handle.Region = 3;
		handle = m_pMemoryMgr[ handle.Region ]->Alloc( p_dwSize );
		return handle.hMemory;
	}
	else if( p_dwSize <= 1024 )
	{
		handle.Region = 4;
		handle = m_pMemoryMgr[ handle.Region ]->Alloc( p_dwSize );
		return handle.hMemory;
	}
	else if( p_dwSize <= 2048 )
	{
		handle.Region = 5;
		handle = m_pMemoryMgr[ handle.Region ]->Alloc( p_dwSize );
		return handle.hMemory;
	}
	else if( p_dwSize <= 4096 )
	{
		handle.Region = 6;
		handle = m_pMemoryMgr[ handle.Region ]->Alloc( p_dwSize );
		return handle.hMemory;
	}
	
	handle.Region = 7;
	handle = m_pMemoryMgr[ handle.Region ]->Alloc( p_dwSize );
	return handle.hMemory;
}

//----------------------------------------------------------------------------------------------
// Frees an allocated block of memory
//----------------------------------------------------------------------------------------------
void Scavenger::Free( MemoryHandle &p_handle )
{
	Handle handle( p_handle );
	if( handle.Valid && !handle.Native )
		m_pMemoryMgr[ handle.Region ]->Free( handle );
}

//----------------------------------------------------------------------------------------------
// Re-allocates a block of memory
//----------------------------------------------------------------------------------------------
void Scavenger::Realloc( MemoryHandle &p_handle, dword p_dwSize )
{
	Handle handle( p_handle );
	if( handle.Valid && !handle.Native )
		m_pMemoryMgr[ handle.Region ]->Realloc( handle, p_dwSize );
}

//----------------------------------------------------------------------------------------------
// Clears a block of memory
//----------------------------------------------------------------------------------------------
void Scavenger::Clear( MemoryHandle &p_handle )
{
	Handle handle( p_handle );
	if( handle.Valid && !handle.Native )
	{
		m_pMemoryMgr[ handle.Region ]->FillActive(	
			handle.GetHandlePointer().Offset, 
			0,
			handle.GetHandlePointer().Size );
	}
}

//----------------------------------------------------------------------------------------------
// Copies a block of memory
//----------------------------------------------------------------------------------------------
MemoryHandle Scavenger::Copy( MemoryHandle &p_handle )
{ 
	Handle handle( p_handle );
	Handle hResult = m_pMemoryMgr[ handle.Region ]->Alloc( handle.GetHandlePointer().Size );
	m_pMemoryMgr[ handle.Region ]->CopyActive( hResult.GetHandlePointer().Offset, handle.GetHandlePointer().Offset, handle.GetHandlePointer().Size );
	return hResult.hMemory;
}

//----------------------------------------------------------------------------------------------
// Compares two blocks of memory
//----------------------------------------------------------------------------------------------
int Scavenger::Compare( MemoryHandle &p_handle1, MemoryHandle &p_handle2 )
{
	Handle handle1( p_handle1 );
	Handle handle2( p_handle2 );

	dword unSize1 = handle1.GetHandlePointer().Size;
	dword unSize2 = handle2.GetHandlePointer().Size;
	dword unSize = unSize1 < unSize2 ? unSize1 : unSize2;

	int nResult = memcmp(	m_pMemoryMgr[ handle1.Region ]->ResolveActivePtr( handle1.GetHandlePointer().Offset ),
							m_pMemoryMgr[ handle2.Region ]->ResolveActivePtr( handle2.GetHandlePointer().Offset ),
							unSize );
	if( nResult!= 0 ) 
		return nResult;
	
	if (unSize1 < unSize2) 
		return -1;

	if (unSize2 < unSize1) 
		return 1;

	return 0;
} 

//----------------------------------------------------------------------------------------------
// Fills a block of memory
//----------------------------------------------------------------------------------------------
void Scavenger::Fill( MemoryHandle &p_handle, byte p_byValue )
{
	Handle handle( p_handle );

	if( handle.Valid && !handle.Native )
	{
		m_pMemoryMgr[ handle.Region ]->FillActive(	
			handle.GetHandlePointer().Offset, 
			p_byValue,
			handle.GetHandlePointer().Size );
	}
}

//----------------------------------------------------------------------------------------------
// Perform Garbage Collection
//----------------------------------------------------------------------------------------------
void Scavenger::Collect( void )
{
	for( int n=0; n<MEMMGR_NUM; n++ )
	{
		m_pMemoryMgr[ n ]->Migrate();
	}
}

void Scavenger::Collect( dword p_dwTimespan )
{
	for( int n=0; n<MEMMGR_NUM; n++ )
	{
		m_pMemoryMgr[ n ]->Migrate();
	}
}


Meson_Cuneus_END
