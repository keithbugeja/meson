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

#include "CnsNamespaces.h"

Meson_Cuneus_Util_BEGIN

class Stack
{
private:
	byte	*m_pbyStack,
			*m_pbyTop,
			*m_pbyEnd;
	

public:
	//----------------------------------------------------------------------------------------------
	// Construction
	//----------------------------------------------------------------------------------------------
	Stack( int p_nSize );
	~Stack( void );

	//----------------------------------------------------------------------------------------------
	// Returns the position of the top of stack
	//----------------------------------------------------------------------------------------------
	int GetTop( void );

	//----------------------------------------------------------------------------------------------
	// Sets the new position of the top of stack
	//----------------------------------------------------------------------------------------------
	void SetTop( int p_nPosition );

	//----------------------------------------------------------------------------------------------
	// Moves the top of stack by a number of bytes
	//----------------------------------------------------------------------------------------------
	void MoveTop( int p_nDelta );

	void Clear( void );

	//----------------------------------------------------------------------------------------------
	// The stack operations Push, Pop, Peek and Ref
	//----------------------------------------------------------------------------------------------
	void Push( byte p_byValue );
	byte Pop( void );

	void PushWord( word p_wValue );
	word PopWord( void );

	void PushDWord( dword p_dwValue );
	dword PopDWord( void );

	void PushQWord( qword p_qwValue );
	qword PopQWord( void );

	void Push( void *p_pData, dword p_qwSize );
	void Pop ( void *p_pData, dword p_qwSize );

	byte Peek( void );
	word PeekWord( void );
	dword PeekDWord( void );
	qword PeekQWord( void );

	byte Peek( int p_nPosition );
	word PeekWord( int p_nPosition );
	dword PeekDWord( int p_nPosition );
	qword PeekQWord( int p_nPosition );
	
	byte &Ref( int p_nPosition );
	word &RefWord( int p_nPosition );
	dword &RefDWord( int p_nPosition );
	qword &RefQWord( int p_nPosition );

	void Dup( void );
	void DupDWord( void );
	void DupQWord(void );

	dword &operator[]( int p_nPosition );
	byte &operator()( int p_nPosition );
};

Meson_Cuneus_Util_END
