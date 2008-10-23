//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::MMU::ByteCodeMgr
//	The ByteCode Memory Management Unit class manages the memory containing bytecode 
//  of the virtual machine.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	24/10/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once


#include <Types.h>
#include <Configuration.h>

#include "CnsNamespaces.h"

Meson_Cuneus_BEGIN

class IByteCodeMgr
{
public:
	virtual void Startup( Meson::Common::Config::Configuration &p_config ) = 0;
	virtual void Shutdown( void ) = 0;

	//----------------------------------------------------------------------------------------------
	// Returns the used size of the buffer.
	//----------------------------------------------------------------------------------------------
	virtual size_t Size( void ) = 0;

	//----------------------------------------------------------------------------------------------
	// Returns the current offset of the buffer.
	//----------------------------------------------------------------------------------------------
	virtual dword GetOffset( void ) = 0;

	//----------------------------------------------------------------------------------------------
	// Appends new bytecode to the bytecode buffer
	//----------------------------------------------------------------------------------------------
	virtual void AppendByteCode( const byte *p_pbyCode, size_t p_unSize ) = 0;

	//----------------------------------------------------------------------------------------------
	// Removes bytecode from the bytecode buffer
	//----------------------------------------------------------------------------------------------
	virtual void RemoveByteCode( size_t p_unOffset, size_t p_unSize ) = 0;

	//----------------------------------------------------------------------------------------------
	// Copies bytecode into a buffer passed as parameter
	//----------------------------------------------------------------------------------------------
	virtual void GetByteCode( byte *p_pbyCode, size_t p_unOffset, size_t p_unSize ) = 0;

	//----------------------------------------------------------------------------------------------
	// Returns a pointer to bytecode offset in the bytecode buffer
	//----------------------------------------------------------------------------------------------
	virtual byte *GetByteCodePointer( size_t p_unOffset ) = 0;

};

Meson_Cuneus_END
