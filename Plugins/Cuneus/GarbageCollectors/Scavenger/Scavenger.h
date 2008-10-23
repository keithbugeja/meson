#pragma once

#include <Configuration.h>

#include <CnsNamespaces.h>
#include <CnsGarbageCollector.h>

#include "Handle.h"
#include "MemoryMgr.h"

#define MEMMGR_NUM 8

Meson_Cuneus_BEGIN

class Scavenger : public IGarbageCollector
{
private:
	MemoryMgr *m_pMemoryMgr[ MEMMGR_NUM ];

public:
	Scavenger( void );
	~Scavenger( void );


	virtual void Startup( Meson::Common::Config::Configuration &p_config );
	virtual void Shutdown( void );


	//----------------------------------------------------------------------------------------------
	// Allocates a block of memory
	//----------------------------------------------------------------------------------------------
	MemoryHandle Alloc( dword p_dwSize );

	//----------------------------------------------------------------------------------------------
	// Frees an allocated block of memory
	//----------------------------------------------------------------------------------------------
	void Free( MemoryHandle &p_handle );
	
	//----------------------------------------------------------------------------------------------
	// Re-allocates a block of memory
	//----------------------------------------------------------------------------------------------
	void Realloc( MemoryHandle &p_handle, dword p_dwSize );

	//----------------------------------------------------------------------------------------------
	// Clears a block of memory
	//----------------------------------------------------------------------------------------------
	void Clear( MemoryHandle &p_handle );

	//----------------------------------------------------------------------------------------------
	// Copies a block of memory
	//----------------------------------------------------------------------------------------------
	MemoryHandle Copy( MemoryHandle &p_handle );

	//----------------------------------------------------------------------------------------------
	// Compares two blocks of memory
	//----------------------------------------------------------------------------------------------
	int Compare( MemoryHandle &p_handle1, MemoryHandle &p_handle2 ); 

	//----------------------------------------------------------------------------------------------
	// Fills a block of memory
	//----------------------------------------------------------------------------------------------
	void Fill( MemoryHandle &p_handle, byte p_byValue );

	//----------------------------------------------------------------------------------------------
	// Perform Garbage Collection
	//----------------------------------------------------------------------------------------------
	void Collect( void );
	void Collect( dword p_dwTimespan );
};

Meson_Cuneus_END