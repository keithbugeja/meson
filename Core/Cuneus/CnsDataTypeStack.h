//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Core::Stack
//	This class is the implementation of a stack for the Cuneus virtual machine.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	25/10/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <Types.h>
#include <ArrayList.h>
//#include <Pointer.h>

#include "CnsNamespaces.h"
#include "CnsDataType.h"


Meson_Cuneus_Util_BEGIN

class CUNEUS_API DataTypeStack
{
protected:
	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > > m_lstDataType;
	int m_nPos;

public:
	//DataTypeStack( void );
	DataTypeStack( int p_nSize );
	~DataTypeStack( void );

	void Clear( void );
	
	void Push( Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > p_pDataType );
	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > Pop( void );

	void PopN( dword p_dwIndex );

	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > Peek( void );
	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > Peek( dword p_dwIndex );

	void Poke( dword p_dwIndex, Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > p_pDataType );

	void LocalVars( dword p_dwNum );

	dword GetTopPos( void );
};

Meson_Cuneus_Util_END
