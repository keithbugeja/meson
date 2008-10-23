//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Core::Stack
//	This class is the implementation of a stack for the Cuneus virtual machine.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	25/10/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "CnsStack.h"

#include <string.h>

#include "MesonException.h"

using namespace Meson::Common;

Meson_Cuneus_Util_BEGIN

//----------------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------------
Stack::Stack( int p_nSize )
{
	m_pbyStack = new byte[ p_nSize ];
	m_pbyTop = m_pbyStack;
	m_pbyEnd = m_pbyStack + p_nSize;
}

//----------------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------------
Stack::~Stack(void)
{
	if( m_pbyStack )
	{
		delete m_pbyStack;
	}
}

//----------------------------------------------------------------------------------------------
// Returns the position of the top of stack
//----------------------------------------------------------------------------------------------
int Stack::GetTop( void )
{
	return (int)( m_pbyTop - m_pbyStack );
}

//----------------------------------------------------------------------------------------------
// Sets the new position of the top of stack
//----------------------------------------------------------------------------------------------
void Stack::SetTop( int p_nPosition )
{
	m_pbyTop = ( m_pbyStack + p_nPosition );
}


//----------------------------------------------------------------------------------------------
// Moves the top of stack by a number of bytes
//----------------------------------------------------------------------------------------------
void Stack::MoveTop( int p_nDelta )
{
	m_pbyTop += p_nDelta;
}

//----------------------------------------------------------------------------------------------
// Clears the stack
//----------------------------------------------------------------------------------------------
void Stack::Clear( void )
{
	m_pbyTop = m_pbyStack;
}

//----------------------------------------------------------------------------------------------
// The stack operations Push, Pop, Peek and Ref
//----------------------------------------------------------------------------------------------
void Stack::Push( byte p_byValue )
{
	if( m_pbyTop >= m_pbyEnd )
		throw new MesonException( "Stack: tried to push into full stack.", __FILE__, __LINE__ );

	*m_pbyTop = p_byValue;
	m_pbyTop++;
}

byte Stack::Pop( void )
{
	if( m_pbyTop <= m_pbyStack )
		throw new MesonException( "Stack: tried to pop from empty stack.", __FILE__, __LINE__ );

	byte byValue = *m_pbyTop;
	m_pbyTop--;
	return byValue;
}

void Stack::PushWord( word p_wValue )
{
	if( (m_pbyTop+2) >= m_pbyEnd )
		throw new MesonException( "Stack: tried to push into full stack.", __FILE__, __LINE__ );

	*((word*)m_pbyTop) = p_wValue;
	m_pbyTop+=2;
}

word Stack::PopWord( void )
{
	if( (m_pbyTop-2) <= m_pbyStack )
		throw new MesonException( "Stack: tried to pop from empty stack.", __FILE__, __LINE__ );

	word wValue = *((word*)m_pbyTop);
	m_pbyTop-=2;
	return wValue;
}

void Stack::PushDWord( dword p_dwValue )
{
	if( (m_pbyTop+4) >= m_pbyEnd )
		throw new MesonException( "Stack: tried to push into full stack.", __FILE__, __LINE__ );

	*((dword*)m_pbyTop) = p_dwValue;
	m_pbyTop+=4;
}

dword Stack::PopDWord( void )
{
	if( (m_pbyTop-4) <= m_pbyStack )
		throw new MesonException( "Stack: tried to pop from empty stack.", __FILE__, __LINE__ );

	dword dwValue = *((dword*)m_pbyTop);
	m_pbyTop-=4;
	return dwValue;
}

void Stack::PushQWord( qword p_qwValue )
{
	if( (m_pbyTop+8) >= m_pbyEnd )
		throw new MesonException( "Stack: tried to push into full stack.", __FILE__, __LINE__ );

	*((qword*)m_pbyTop) = p_qwValue;
	m_pbyTop+=8;
}

qword Stack::PopQWord( void )
{
	if( (m_pbyTop-8) <= m_pbyStack )
		throw new MesonException( "Stack: tried to pop from empty stack.", __FILE__, __LINE__ );

	qword dwValue = *((qword*)m_pbyTop);
	m_pbyTop-=8;
	return dwValue;
}

void Stack::Push( void *p_pData, dword p_qwSize )
{
	if( (m_pbyTop+p_qwSize) >= m_pbyEnd )
		throw new MesonException( "Stack: tried to push into full stack.", __FILE__, __LINE__ );

	memcpy( m_pbyTop, p_pData, p_qwSize );
	m_pbyTop+=p_qwSize;
}

void Stack::Pop ( void *p_pData, dword p_qwSize )
{
	if( (m_pbyTop-p_qwSize) <= m_pbyStack )
		throw new MesonException( "Stack: tried to pop from empty stack.", __FILE__, __LINE__ );

	m_pbyTop-=p_qwSize;
	memcpy( p_pData, m_pbyTop, p_qwSize );
}



byte Stack::Peek( void )
{
	byte byValue = *(m_pbyTop-1);
	return byValue;
}

word Stack::PeekWord( void )
{
	word wValue = *((word*)(m_pbyTop-2));
	return wValue;
}

dword Stack::PeekDWord( void )
{
	dword dwValue = *((dword*)(m_pbyTop-4));
	return dwValue;
}

qword Stack::PeekQWord( void )
{
	qword dwValue = *((qword*)(m_pbyTop-8));
	return dwValue;
}



byte Stack::Peek( int p_nPosition )
{
	byte byValue = *(m_pbyStack+p_nPosition);
	return byValue;
}

word Stack::PeekWord( int p_nPosition )
{
	word wValue = *((word*)(m_pbyStack+p_nPosition));
	return wValue;
}

dword Stack::PeekDWord( int p_nPosition )
{
	dword dwValue = *((dword*)(m_pbyStack+p_nPosition));
	return dwValue;
}

qword Stack::PeekQWord( int p_nPosition )
{
	qword dwValue = *((qword*)(m_pbyStack+p_nPosition));
	return dwValue;
}

byte &Stack::Ref( int p_nPosition )
{
	return *(m_pbyStack+p_nPosition);
}

word &Stack::RefWord( int p_nPosition )
{
	return *((word*)(m_pbyStack+p_nPosition));
}

dword &Stack::RefDWord( int p_nPosition )
{
	return *((dword*)(m_pbyStack+p_nPosition));
}

qword &Stack::RefQWord( int p_nPosition )
{
	return *((qword*)(m_pbyStack+p_nPosition));
}

void Stack::Dup( void )
{
	Push( Peek( (int) (m_pbyTop - m_pbyStack) - 1 ) );
}

void Stack::DupDWord( void )
{
	PushDWord( PeekDWord( (int) (m_pbyTop - m_pbyStack) - 4 ) );
}

void Stack::DupQWord(void )
{
	PushQWord( PeekQWord( (int) (m_pbyTop - m_pbyStack) - 8 ) );
}


dword &Stack::operator[]( int p_nPosition )
{
	return *((dword*)(m_pbyStack+p_nPosition));
}

byte &Stack::operator()( int p_nPosition )
{
	return *(m_pbyStack+p_nPosition);
}

Meson_Cuneus_Util_END
