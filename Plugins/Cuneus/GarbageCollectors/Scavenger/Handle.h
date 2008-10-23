
#pragma once
#include <CnsNamespaces.h>
#include <CnsCuneusEngine.h>
#include <CnsMemoryHandle.h>

Meson_Cuneus_BEGIN

struct HandlePointer
{

	// |-Marked-|-Size-|-Offset-|
	// |    1   |  31  |   32   |
	// |--------|------|--------|
	// |	 qwValue   64       |
	// |------------------------|
	union
	{
		struct
		{
			qword Offset : 32;
			qword Size   : 31;
			qword Marked :  1;
		};
		qword qwValue;
	};

	// Reference Counting
	word RefCount;


	// Default Constructor
	HandlePointer( void )
	{
		RefCount = 1;
	}

	// Copy Constructor
	HandlePointer( const HandlePointer &p_handleptr )
	{
		qwValue = p_handleptr.qwValue;
		Marked = 0;
		RefCount = 1;
	}

	// Destructor
	~HandlePointer( void )
	{
	}

	// Increment the Reference Counter
	inline void RefCountInc( void )
	{
		RefCount++;
	}

	// Decrement the Reference Counter if > 0
	inline void RefCountDec( void )
	{
		if( RefCount > 0 )
			RefCount--;
	}
	
	// Assignment Operator
	void operator=( const HandlePointer &p_handleptr )
	{
		qwValue = p_handleptr.qwValue;
		Marked = 0;
		RefCount = 1;
	}

	// Equality Operator
	bool operator==( const HandlePointer &p_handleptr )
	{
		return ( qwValue == p_handleptr.qwValue );
	}
};


struct Handle
{

	// |-Native-|-Valid-|-Region-|-HandlePtr-|
	// |    1   |    1  |    3   |     59    |
	// |--------|-------|--------|-----------|
	// |	       qwValue   64              |
	// |-------------------------------------|
	union
	{
		struct
		{
			qword HandlePtr : 59;
			qword Region : 3;
			qword Valid  : 1;
			qword Native : 1;
		};
		MemoryHandle hMemory;
		qword qwValue;
	};


	// Default Constructor
	Handle( void )
	{
		qwValue = 0;
	}

	// Copy Constructor
	Handle( const Handle &p_handle )
	{
		qwValue = p_handle.qwValue;
		p_handle.GetHandlePointer().RefCountInc();
	}

	Handle( const MemoryHandle &p_hMemory )
	{
		qwValue = p_hMemory.qwValue;
		GetHandlePointer().RefCountInc();
	}

	// Destructor
	~Handle( void )
	{
		if( Valid && !Native)
		{
			CuneusEngine::GetInstance()->GetGarbageCollector()->Free( *this );
		}
	}


	// Returns a reference to the HandlePointer structure it points to
	inline HandlePointer &GetHandlePointer( void ) const
	{
		return *((HandlePointer*)HandlePtr);
	}

	// Assignment Operator
	void operator=( const Handle &p_handle )
	{
		qwValue = p_handle.qwValue;
		p_handle.GetHandlePointer().RefCountInc();
	}

	// Equality Operator
	bool operator==( const Handle &p_handle )
	{
		return ( qwValue == p_handle.qwValue );
	}

	operator MemoryHandle()
	{
		return hMemory;
	}

	operator MemoryHandle&()
	{
		return hMemory;
	}
};


Meson_Cuneus_END
