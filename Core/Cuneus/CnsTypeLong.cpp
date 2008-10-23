#include "CnsTypeLong.h"

#include "CnsAllDataTypes.h"
#include "CnsTypeCodes.h"

#include <MesonException.h>

using namespace Meson::Common;

Meson_Cuneus_RTS_Typing_BEGIN

TypeLong::TypeLong( void )
	: DataType( "Long", CNS_TC_LONG )
{
	m_llValue = 0;
}

TypeLong::TypeLong( long long p_llValue )
	: DataType( "Long", CNS_TC_LONG )
{
	m_llValue = p_llValue;
}

TPointer< DataType > TypeLong::Clone( void )
{
	TPointer< DataType > ptr;
	ptr = new TypeLong( GetValue() );
	return ptr;
}

long long TypeLong::GetValue( void )
{
	return m_llValue;
}

void TypeLong::SetValue( long long p_llValue )
{
	m_llValue = p_llValue;
}


void TypeLong::GetData( byte* p_pbyData )
{
	*((long long*)p_pbyData) = m_llValue;
}

void TypeLong::SetData( byte* p_pbyData )
{
	m_llValue = *((long long*)p_pbyData);
}


TypeLong& TypeLong::operator=( long long p_llValue )
{
	m_llValue = p_llValue;
	return *this;
}

TypeLong::operator long long( void )
{
	return m_llValue;
}

String TypeLong::ToString( void )
{
	String str;
	str += ( m_llValue );
	return str;
}




Meson::Common::TPointer< DataType > TypeLong::operator+( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeLong( GetValue() + (long long)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeLong( GetValue() + (long long)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeLong( GetValue() + (long long)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeLong( GetValue() + (long long)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeLong( GetValue() + (long long)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeLong( GetValue() + (long long)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeLong( GetValue() + (long long)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeLong( GetValue() + (long long)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() + (long long)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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


Meson::Common::TPointer< DataType > TypeLong::operator-( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeLong( GetValue() - (long long)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeLong( GetValue() - (long long)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeLong( GetValue() - (long long)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeLong( GetValue() - (long long)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeLong( GetValue() - (long long)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeLong( GetValue() - (long long)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeLong( GetValue() - (long long)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeLong( GetValue() - (long long)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() - (long long)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeLong::operator*( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeLong( GetValue() * (long long)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeLong( GetValue() * (long long)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeLong( GetValue() * (long long)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeLong( GetValue() * (long long)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeLong( GetValue() * (long long)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeLong( GetValue() * (long long)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeLong( GetValue() * (long long)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeLong( GetValue() * (long long)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() * (long long)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeLong::operator/( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeLong( GetValue() / (long long)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeLong( GetValue() / (long long)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeLong( GetValue() / (long long)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeLong( GetValue() / (long long)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeLong( GetValue() / (long long)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeLong( GetValue() / (long long)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeLong( GetValue() / (long long)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeLong( GetValue() / (long long)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() / (long long)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeLong::operator%( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeLong( GetValue() % (long long)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeLong( GetValue() % (long long)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeLong( GetValue() % (long long)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeLong( GetValue() % (long long)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeLong( GetValue() % (long long)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeLong( GetValue() % (long long)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeLong( GetValue() % (long long)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeLong( GetValue() % (long long)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() % (long long)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}



Meson::Common::TPointer< DataType > TypeLong::operator&( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeLong( GetValue() & (long long)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeLong( GetValue() & (long long)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeLong( GetValue() & (long long)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeLong( GetValue() & (long long)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeLong( GetValue() & (long long)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeLong( GetValue() & (long long)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeLong( GetValue() & (long long)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeLong( GetValue() & (long long)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() & (long long)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeLong::operator|( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeLong( GetValue() | (long long)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeLong( GetValue() | (long long)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeLong( GetValue() | (long long)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeLong( GetValue() | (long long)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeLong( GetValue() | (long long)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeLong( GetValue() | (long long)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeLong( GetValue() | (long long)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeLong( GetValue() | (long long)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() | (long long)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeLong::operator^( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeLong( GetValue() ^ (long long)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeLong( GetValue() ^ (long long)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeLong( GetValue() ^ (long long)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeLong( GetValue() ^ (long long)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeLong( GetValue() ^ (long long)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeLong( GetValue() ^ (long long)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeLong( GetValue() ^ (long long)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeLong( GetValue() ^ (long long)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() ^ (long long)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeLong::operator>>( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeLong( GetValue() >> (long long)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeLong( GetValue() >> (long long)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeLong( GetValue() >> (long long)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeLong( GetValue() >> (long long)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeLong( GetValue() >> (long long)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeLong( GetValue() >> (long long)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeLong( GetValue() >> (long long)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeLong( GetValue() >> (long long)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() >> (long long)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeLong::operator<<( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeLong( GetValue() << (long long)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeLong( GetValue() << (long long)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeLong( GetValue() << (long long)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeLong( GetValue() << (long long)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeLong( GetValue() << (long long)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeLong( GetValue() << (long long)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeLong( GetValue() << (long long)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeLong( GetValue() << (long long)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( GetValue() << (long long)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}



Meson::Common::TPointer< DataType > TypeLong::operator-( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( -GetValue() );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeLong::operator~( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( ~GetValue() );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeLong::operator++( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( GetValue() + 1 );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeLong::operator--( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( GetValue() - 1 );
	return pDT;
}



int TypeLong::Compare( Meson::Common::TPointer< DataType >& p_pDataType )
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



void TypeLong::Store( Meson::Common::IO::IOutputStream& p_outStream )
{
	p_outStream << GetValue();
}

void TypeLong::Load(  Meson::Common::IO::IInputStream& p_inStream )
{
	p_inStream >> m_llValue;
}

Meson_Cuneus_RTS_Typing_END
