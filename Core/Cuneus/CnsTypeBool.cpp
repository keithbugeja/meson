#include "CnsTypeBool.h"

#include "CnsTypeCodes.h"
#include "CnsAllDataTypes.h"

#include <MesonException.h>

using namespace Meson::Common;

Meson_Cuneus_RTS_Typing_BEGIN

TypeBool::TypeBool( void ) 
	: DataType( "Boolean", CNS_TC_BOOLEAN )
{
	m_bValue = false;
}

TypeBool::TypeBool( bool p_bValue )
	: DataType( "Boolean", CNS_TC_BOOLEAN )
{
	m_bValue = p_bValue;
}

TypeBool::TypeBool( const TypeBool& p_bValue )
{
	m_bValue = p_bValue.m_bValue;
}

TPointer< DataType > TypeBool::Clone( void )
{
	TPointer< DataType > ptr;
	ptr = new TypeBool( GetValue() );
	return ptr;
}

DataType* TypeBool::CreateInstance( void )
{
	return new TypeBool( m_bValue );
}

bool TypeBool::GetValue( void )
{
	return m_bValue;
}

void TypeBool::SetValue( bool p_bValue )
{
	m_bValue = p_bValue;
}

void TypeBool::GetData( byte* p_pbyData )
{
	*((bool*)p_pbyData) = m_bValue;
}

void TypeBool::SetData( byte* p_pbyData )
{
	m_bValue = *((bool*)p_pbyData);
}

TypeBool& TypeBool::operator=( const TypeBool& p_bValue )
{
	m_bValue = p_bValue.m_bValue;
	return *this;
}

TypeBool& TypeBool::operator=( bool p_bValue )
{
	m_bValue = p_bValue;
	return *this;
}

TypeBool::operator bool( void )
{
	return m_bValue;
}

String TypeBool::ToString( void )
{
	String str;
	str += ( m_bValue );
	return str;
}

#pragma warning( disable : 4800 4804 )


Meson::Common::TPointer< DataType > TypeBool::operator+( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeBool( GetValue() + (bool)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeByte( GetValue() + ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() + ( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() + ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeShort( GetValue() + ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( GetValue() + ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( GetValue() + ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( GetValue() + ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() + ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_FLOAT:
			pDT = new TypeFloat( (float) GetValue() + ( (TypeFloat*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DOUBLE:
			pDT = new TypeDouble( (double) GetValue() + ( (TypeDouble*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_STRING:
			pDT = new TypeString( ToString() + p_pDataType->ToString() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}

	return pDT;
}


Meson::Common::TPointer< DataType > TypeBool::operator-( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeBool( GetValue() - (bool)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeByte( GetValue() - ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() - ( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() - ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeShort( GetValue() - ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( GetValue() - ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( GetValue() - ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( GetValue() - ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() - ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_FLOAT:
			pDT = new TypeFloat( (float) GetValue() - ( (TypeFloat*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DOUBLE:
			pDT = new TypeDouble( (double) GetValue() - ( (TypeDouble*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}


	return pDT;
}

Meson::Common::TPointer< DataType > TypeBool::operator*( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeBool( GetValue() * (bool)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeByte( GetValue() * ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() * ( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() * ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeShort( GetValue() * ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( GetValue() * ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( GetValue() * ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( GetValue() * ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() * ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_FLOAT:
			pDT = new TypeFloat( (float) GetValue() * ( (TypeFloat*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DOUBLE:
			pDT = new TypeDouble( (double) GetValue() * ( (TypeDouble*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}

	return pDT;
}

Meson::Common::TPointer< DataType > TypeBool::operator/( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeBool( GetValue() / (bool)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeByte( GetValue() / ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() / ( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() / ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeShort( GetValue() / ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( GetValue() / ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( GetValue() / ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( GetValue() / ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() / ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_FLOAT:
			pDT = new TypeFloat( (float) GetValue() / ( (TypeFloat*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DOUBLE:
			pDT = new TypeDouble( (double) GetValue() / ( (TypeDouble*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}

	return pDT;
}

Meson::Common::TPointer< DataType > TypeBool::operator%( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeBool( GetValue() % (bool)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeByte( GetValue() % ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() % ( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() % ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeShort( GetValue() % ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( GetValue() % ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( GetValue() % ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( GetValue() % ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() % ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}

	return pDT;
}



Meson::Common::TPointer< DataType > TypeBool::operator&( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeBool( GetValue() && (bool)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeBool( GetValue() && ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeBool( GetValue() && ( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeBool( GetValue() && ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeBool( GetValue() && ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeBool( GetValue() && ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeBool( GetValue() && ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeBool( GetValue() && ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeBool( GetValue() && ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;


		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeBool::operator|( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeBool( GetValue() || (bool)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeBool( GetValue() || ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeBool( GetValue() || ( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeBool( GetValue() || ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeBool( GetValue() || ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeBool( GetValue() || ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeBool( GetValue() || ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeBool( GetValue() || ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeBool( GetValue() || ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeBool::operator^( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeBool( GetValue() ^ (bool)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeBool( GetValue() ^ (bool)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeBool( GetValue() ^ (bool)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeBool( GetValue() ^ (bool)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeBool( GetValue() ^ (bool)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeBool( GetValue() ^ (bool)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeBool( GetValue() ^ (bool)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeBool( GetValue() ^ (bool)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeBool( GetValue() ^ (bool)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeBool::operator>>( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeBool( GetValue() >> (bool)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeByte( GetValue() >> ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() >> ( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() >> ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeShort( GetValue() >> ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( GetValue() >> ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( GetValue() >> ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( GetValue() >> ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() >> ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeBool::operator<<( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeBool( GetValue() << (bool)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeByte( GetValue() << ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() << ( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() << ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeShort( GetValue() << ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( GetValue() << ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( GetValue() << ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( GetValue() << ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() << ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}

	return pDT;
}



Meson::Common::TPointer< DataType > TypeBool::operator-( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( -GetValue() );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeBool::operator~( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( ~GetValue() );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeBool::operator++( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( GetValue() + 1 );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeBool::operator--( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( GetValue() - 1 );
	return pDT;
}



int TypeBool::Compare( Meson::Common::TPointer< DataType >& p_pDataType )
{
	long long llTemp;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			llTemp = ( (long long)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_BYTE:
			llTemp = ( (long long)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_CHAR:
			llTemp = ( (long long)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_WORD:
			llTemp = ( (long long)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_SHORT:
			llTemp = ( (long long)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_DWORD:
			llTemp = ( (long long)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_INTEGER:
			llTemp = ( (long long)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_QWORD:
			llTemp = ( (long long)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_LONG:
			llTemp = ( (long long)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_FLOAT:
			{
				float fTemp = ( ( (TypeFloat*) (DataType*) p_pDataType )->GetValue() );
				if( (float)GetValue() < fTemp )
					return -1;
				else if( (float)GetValue() > fTemp )
					return 1;
				else
					return 0;
			}

		case CNS_TC_DOUBLE:
			{
				double dfTemp = ( ( (TypeDouble*) (DataType*) p_pDataType )->GetValue() );
				if( (double)GetValue() < dfTemp )
					return -1;
				else if( (double)GetValue() > dfTemp )
					return 1;
				else
					return 0;
			}

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation Compare not supported between Long and DataType", __FILE__, __LINE__ );
			throw except; 
	}
	
	if( GetValue() < llTemp  )
		return -1;
	else if( GetValue() > llTemp )
		return 1;
	else
		return 0;
}

void TypeBool::Store( Meson::Common::IO::IOutputStream& p_outStream )
{
	p_outStream << GetValue();
}

void TypeBool::Load(  Meson::Common::IO::IInputStream& p_inStream )
{
	p_inStream >> m_bValue;
}

#pragma warning( default : 4800 4804 )

Meson_Cuneus_RTS_Typing_END
