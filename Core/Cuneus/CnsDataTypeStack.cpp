#include "CnsDataTypeStack.h"
#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Cuneus::RTS::Typing;


Meson_Cuneus_Util_BEGIN

//DataTypeStack::DataTypeStack( void )
//	: m_lstDataType()
//	, m_nPos( 0 )
//{
//}


DataTypeStack::DataTypeStack( int p_nSize )
	: m_lstDataType( p_nSize )
	, m_nPos( 0 )
{
	while( p_nSize )
	{
		m_lstDataType.Add( TPointer< DataType >( NULL ) );
		p_nSize--;
	}
}

DataTypeStack::~DataTypeStack( void )
{
}

void DataTypeStack::Clear( void )
{
	//m_lstDataType.Clear();
	m_nPos = 0;
}

void DataTypeStack::Push( TPointer< DataType > p_pDataType )
{
	m_lstDataType[m_nPos] = p_pDataType;
	m_nPos++;
}

TPointer< DataType > DataTypeStack::Pop( void )
{
	m_nPos--;
	if( m_nPos < 0 )
	{
		throw new Meson::Common::MesonException( "DataTypeStack underflow!", __FILE__, __LINE__ );
	}

	return m_lstDataType[ m_nPos ];
}

TPointer< DataType > DataTypeStack::Peek( void )
{
	if( m_nPos <= 0 )
	{
		throw new Meson::Common::MesonException( "DataTypeStack underflow!", __FILE__, __LINE__ );
	}

	return m_lstDataType[ m_nPos - 1 ];
}

Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > DataTypeStack::Peek( dword p_dwIndex )
{
	/*if( p_dwIndex >= (dword)m_nPos )
	{
		throw new Meson::Common::MesonException( "DataTypeStack overflow!", __FILE__, __LINE__ );
	}*/

	return m_lstDataType[ p_dwIndex ];
}

void DataTypeStack::PopN( dword p_dwIndex )
{
	if( (m_nPos - (int)p_dwIndex) <= 0 )
	{
		Meson::Common::MesonException* except = new Meson::Common::MesonException( "DataTypeStack Problem underflow!", __FILE__, __LINE__ );
		throw except;
	}

	m_nPos -= p_dwIndex;
}

void DataTypeStack::Poke( dword p_dwIndex, Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > p_pDataType )
{
	m_lstDataType[ p_dwIndex ] = p_pDataType;
}

void DataTypeStack::LocalVars( dword p_dwNum )
{
	/*while( p_dwNum )
	{
		Push( TPointer< DataType >( NULL ) );
		p_dwNum--;
	}*/
	m_nPos += p_dwNum;
}

dword DataTypeStack::GetTopPos( void )
{
	return (dword)m_nPos;
}

Meson_Cuneus_Util_END
