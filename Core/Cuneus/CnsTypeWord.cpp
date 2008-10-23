#include "CnsTypeWord.h"

#include "CnsTypeCodes.h"
#include "CnsAllDataTypes.h"

#include <MesonException.h>

using namespace Meson::Common;

Meson_Cuneus_RTS_Typing_BEGIN

TypeWord::TypeWord( void )
	: DataType( "Word", CNS_TC_WORD )
{
	m_wValue = 0;
}

TypeWord::TypeWord( word p_wValue )
	: DataType( "Word", CNS_TC_WORD )
{
	m_wValue = p_wValue;
}

TPointer< DataType > TypeWord::Clone( void )
{
	TPointer< DataType > ptr;
	ptr = new TypeWord( GetValue() );
	return ptr;
}


word TypeWord::GetValue( void )
{
	return m_wValue;
}

void TypeWord::SetValue( word p_wValue )
{
	m_wValue = p_wValue;
}

void TypeWord::GetData( byte* p_pbyData )
{
	*((word*)p_pbyData) = m_wValue;
}

void TypeWord::SetData( byte* p_pbyData )
{
	m_wValue = *((word*)p_pbyData);
}

TypeWord& TypeWord::operator=( word p_wValue )
{
	m_wValue = p_wValue;
	return *this;
}

TypeWord::operator word( void )
{
	return m_wValue;
}

String TypeWord::ToString( void )
{
	String str;
	str += ( m_wValue );
	return str;
}


Meson::Common::TPointer< DataType > TypeWord::operator+( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeWord( GetValue() + (word)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeWord( GetValue() + (word)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeWord( GetValue() + (word)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() + (word)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeWord( GetValue() + (word)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( (dword)GetValue() + ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( (int)GetValue() + ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( (qword)GetValue() + ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( (long long)GetValue() + ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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


Meson::Common::TPointer< DataType > TypeWord::operator-( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeWord( GetValue() - (word)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeWord( GetValue() - (word)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeWord( GetValue() - (word)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() - (word)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeWord( GetValue() - (word)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( (dword)GetValue() - ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( (int)GetValue() - ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( (qword)GetValue() - ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( (long long)GetValue() - ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeWord::operator*( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeWord( GetValue() * (word)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeWord( GetValue() * (word)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeWord( GetValue() * (word)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() * (word)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeWord( GetValue() * (word)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( (dword)GetValue() * ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( (int)GetValue() * ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( (qword)GetValue() * ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( (long long)GetValue() * ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeWord::operator/( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeWord( GetValue() / (word)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeWord( GetValue() / (word)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeWord( GetValue() / (word)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() / (word)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeWord( GetValue() / (word)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( (dword)GetValue() / ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( (int)GetValue() / ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( (qword)GetValue() / ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeLong( (long long)GetValue() / ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeWord::operator%( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeWord( GetValue() % (word)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeWord( GetValue() % (word)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeWord( GetValue() % (word)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() % (word)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeWord( GetValue() % (word)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( (dword)GetValue() % ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( (int)GetValue() % ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( (qword)GetValue() % ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeQWord( (long long)GetValue() % ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}



Meson::Common::TPointer< DataType > TypeWord::operator&( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeWord( GetValue() & (word)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeWord( GetValue() & (word)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeWord( GetValue() & (word)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() & (word)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeWord( GetValue() & (word)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( (dword)GetValue() & ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( (int) GetValue() & ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( (qword)GetValue() & ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeQWord( (long long)GetValue() & ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}
	return pDT;
}

Meson::Common::TPointer< DataType > TypeWord::operator|( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeWord( GetValue() | (word)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeWord( GetValue() | (word)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeWord( GetValue() | (word)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() | (word)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeWord( GetValue() | (word)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( (dword)GetValue() | ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( (int) GetValue() | ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( (qword)GetValue() | ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeQWord( (long long)GetValue() | ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}

	return pDT;
}

Meson::Common::TPointer< DataType > TypeWord::operator^( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeWord( GetValue() ^ (word)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeWord( GetValue() ^ (word)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeWord( GetValue() ^ (word)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() ^ (word)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeWord( GetValue() ^ (word)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( (dword)GetValue() ^ ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( (int) GetValue() ^ ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( (qword)GetValue() ^ ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeQWord( (long long)GetValue() ^ ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}

	return pDT;
}

Meson::Common::TPointer< DataType > TypeWord::operator>>( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeWord( GetValue() >> (word)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeWord( GetValue() >> (word)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeWord( GetValue() >> (word)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() & (word)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeWord( GetValue() >> (word)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( (dword)GetValue() >> ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( (int) GetValue() >> ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( (qword)GetValue() >> ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeQWord( (long long)GetValue() >> ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}

	return pDT;
}

Meson::Common::TPointer< DataType > TypeWord::operator<<( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeWord( GetValue() << (word)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeWord( GetValue() << (word)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeWord( GetValue() << (word)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeWord( GetValue() << (word)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeWord( GetValue() << (word)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeDWord( (dword)GetValue() << ( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeInt( (int) GetValue() << ( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeQWord( (qword)GetValue() << ( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeQWord( (long long)GetValue() << ( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
			throw except;
	}

	return pDT;
}



Meson::Common::TPointer< DataType > TypeWord::operator-( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( -GetValue() );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeWord::operator~( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( ~GetValue() );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeWord::operator++( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( GetValue() + 1 );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeWord::operator--( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( GetValue() - 1 );
	return pDT;
}



int TypeWord::Compare( Meson::Common::TPointer< DataType >& p_pDataType )
{
	qword qwTemp;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			qwTemp = ( (qword)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_BYTE:
			qwTemp = ( (qword)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_CHAR:
			qwTemp = ( (qword)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_WORD:
			qwTemp = ( (qword)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_SHORT:
			qwTemp = ( (qword)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_DWORD:
			qwTemp = ( (qword)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_INTEGER:
			qwTemp = ( (qword)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_QWORD:
			qwTemp = ( (qword)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_LONG:
			qwTemp = ( (qword)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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
	
	if( GetValue() < qwTemp  )
		return -1;
	else if( GetValue() > qwTemp )
		return 1;
	else
		return 0;
}


void TypeWord::Store( Meson::Common::IO::IOutputStream& p_outStream )
{
	p_outStream << GetValue();
}

void TypeWord::Load(  Meson::Common::IO::IInputStream& p_inStream )
{
	p_inStream >> m_wValue;
}

Meson_Cuneus_RTS_Typing_END
