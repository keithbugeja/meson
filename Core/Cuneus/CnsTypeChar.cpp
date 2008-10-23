#include "CnsTypeChar.h"

#include "CnsTypeCodes.h"
#include "CnsAllDataTypes.h"

#include <MesonException.h>

using namespace Meson::Common;

Meson_Cuneus_RTS_Typing_BEGIN

TypeChar::TypeChar( void )
	: DataType( "Char", CNS_TC_CHAR )
{
	m_cValue = 0;
}

TypeChar::TypeChar( char p_cValue )
	: DataType( "Char", CNS_TC_CHAR )
{
	m_cValue = p_cValue;
}

TPointer< DataType > TypeChar::Clone( void )
{
	TPointer< DataType > ptr;
	ptr = new TypeChar( GetValue() );
	return ptr;
}

char TypeChar::GetValue( void )
{
	return m_cValue;
}

void TypeChar::SetValue( char p_cValue )
{
	p_cValue = p_cValue;
}

void TypeChar::GetData( byte* p_pbyData )
{
	*((char*)p_pbyData) = m_cValue;
}

void TypeChar::SetData( byte* p_pbyData )
{
	m_cValue = *((char*)p_pbyData);
}

TypeChar& TypeChar::operator=( char p_cValue )
{
	m_cValue = p_cValue;
	return *this;
}

TypeChar::operator char( void )
{
	return m_cValue;
}

String TypeChar::ToString( void )
{
	String str;
	str += ( m_cValue );
	return str;
}



Meson::Common::TPointer< DataType > TypeChar::operator+( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeChar( GetValue() + (char)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeChar( GetValue() + ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() + (char)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
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


Meson::Common::TPointer< DataType > TypeChar::operator-( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeChar( GetValue() - (char)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeChar( GetValue() - ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() - (char)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeChar::operator*( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeChar( GetValue() * (char)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeChar( GetValue() * ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() * (char)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeChar::operator/( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeChar( GetValue() / (char)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeChar( GetValue() / ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() / (char)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeChar::operator%( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeChar( GetValue() % (char)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeChar( GetValue() % ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() % (char)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
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



Meson::Common::TPointer< DataType > TypeChar::operator&( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeChar( GetValue() & (char)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeChar( GetValue() & ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() & (char)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() & ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeShort( GetValue() & ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( GetValue() & ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( GetValue() & ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( GetValue() & ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() & ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;


		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeChar::operator|( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeChar( GetValue() | (char)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeChar( GetValue() | ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() | (char)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() | ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeShort( GetValue() | ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( GetValue() | ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( GetValue() | ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( GetValue() | ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() | ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeChar::operator^( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeChar( GetValue() ^ (char)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeChar( GetValue() ^ ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() ^ (char)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() ^ ( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeShort( GetValue() ^ ( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( GetValue() ^ ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( GetValue() ^ ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( GetValue() ^ ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() ^ ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeChar::operator>>( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeChar( GetValue() >> (char)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeChar( GetValue() >> ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() >> (char)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeChar::operator<<( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeChar( GetValue() << (char)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeChar( GetValue() << ( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeChar( GetValue() << (char)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
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



Meson::Common::TPointer< DataType > TypeChar::operator-( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( -GetValue() );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeChar::operator~( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( ~GetValue() );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeChar::operator++( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( GetValue() + 1 );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeChar::operator--( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( GetValue() - 1 );
	return pDT;
}



int TypeChar::Compare( Meson::Common::TPointer< DataType >& p_pDataType )
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


void TypeChar::Store( Meson::Common::IO::IOutputStream& p_outStream )
{
	p_outStream << GetValue();
}

void TypeChar::Load(  Meson::Common::IO::IInputStream& p_inStream )
{
	p_inStream >> m_cValue;
}

Meson_Cuneus_RTS_Typing_END
