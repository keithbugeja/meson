#include "CnsTypeFloat.h"

#include "CnsTypeCodes.h"
#include "CnsAllDataTypes.h"

#include <MesonException.h>

using namespace Meson::Common;

Meson_Cuneus_RTS_Typing_BEGIN

TypeFloat::TypeFloat(void)
	: DataType( "Float", CNS_TC_FLOAT )
{
	m_fValue = 0;
}

TypeFloat::TypeFloat( float p_fValue )
	: DataType( "Float", CNS_TC_FLOAT )
{
	m_fValue = p_fValue;
}

TPointer< DataType > TypeFloat::Clone( void )
{
	TPointer< DataType > ptr;
	ptr = new TypeFloat( GetValue() );
	return ptr;
}

float TypeFloat::GetValue( void )
{
	return m_fValue;
}

void TypeFloat::SetValue( float p_fValue )
{
	m_fValue = p_fValue;
}

void TypeFloat::GetData( byte* p_pbyData )
{
	*((float*)p_pbyData) = m_fValue;
}

void TypeFloat::SetData( byte* p_pbyData )
{
	m_fValue = *((float*)p_pbyData);
}

TypeFloat& TypeFloat::operator=( float p_fValue )
{
	m_fValue = p_fValue;
	return *this;
}

TypeFloat::operator float( void )
{
	return m_fValue;
}

String TypeFloat::ToString( void )
{
	String str;
	str += ( m_fValue );
	return str;
}



Meson::Common::TPointer< DataType > TypeFloat::operator+( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeFloat( GetValue() + (float)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeFloat( GetValue() + (float)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeFloat( GetValue() + (float)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeFloat( GetValue() + (float)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeFloat( GetValue() + (float)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeFloat( GetValue() + (float)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeFloat( GetValue() + (float)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeFloat( GetValue() + (float)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeFloat( GetValue() + (float)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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


Meson::Common::TPointer< DataType > TypeFloat::operator-( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;

	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeFloat( GetValue() - (float)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeFloat( GetValue() - (float)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeFloat( GetValue() - (float)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeFloat( GetValue() - (float)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeFloat( GetValue() - (float)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeFloat( GetValue() - (float)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeFloat( GetValue() - (float)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeFloat( GetValue() - (float)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeFloat( GetValue() - (float)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeFloat::operator*( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeFloat( GetValue() * (float)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeFloat( GetValue() * (float)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeFloat( GetValue() * (float)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeFloat( GetValue() * (float)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeFloat( GetValue() * (float)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeFloat( GetValue() * (float)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeFloat( GetValue() * (float)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeFloat( GetValue() * (float)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeFloat( GetValue() * (float)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeFloat::operator/( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			 pDT = new TypeFloat( GetValue() / (float)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			 return pDT;

		case CNS_TC_BYTE:
			pDT = new TypeFloat( GetValue() / (float)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_CHAR:
			pDT = new TypeFloat( GetValue() / (float)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_WORD:
			pDT = new TypeFloat( GetValue() / (float)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_SHORT:
			pDT = new TypeFloat( GetValue() / (float)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_DWORD:
			pDT = new TypeFloat( GetValue() / (float)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_INTEGER:
			pDT = new TypeFloat( GetValue() / (float)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_QWORD:
			pDT = new TypeFloat( GetValue() / (float)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			return pDT;

		case CNS_TC_LONG:
			pDT = new TypeFloat( GetValue() / (float)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
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

Meson::Common::TPointer< DataType > TypeFloat::operator%( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
	throw except;
}



Meson::Common::TPointer< DataType > TypeFloat::operator&( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeFloat::operator|( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeFloat::operator^( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeFloat::operator>>( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeFloat::operator<<( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
	throw except;
}



Meson::Common::TPointer< DataType > TypeFloat::operator-( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( -GetValue() );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeFloat::operator~( void )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported between Long and DataType", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeFloat::operator++( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( GetValue() + 1 );
	return pDT;
}

Meson::Common::TPointer< DataType > TypeFloat::operator--( void )
{
	Meson::Common::TPointer< DataType > pDT( this );
	SetValue( GetValue() - 1 );
	return pDT;
}



int TypeFloat::Compare( Meson::Common::TPointer< DataType >& p_pDataType )
{
	double dfTemp;
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_BOOLEAN:
			dfTemp = ( (float)( (TypeBool*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_BYTE:
			dfTemp = ( (float)( (TypeByte*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_CHAR:
			dfTemp = ( (float)( (TypeChar*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_WORD:
			dfTemp = ( (float)( (TypeWord*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_SHORT:
			dfTemp = ( (float)( (TypeShort*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_DWORD:
			dfTemp = ( (float)( (TypeDWord*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_INTEGER:
			dfTemp = ( (float)( (TypeInt*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_QWORD:
			dfTemp = ( (float)( (TypeQWord*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_LONG:
			dfTemp = ( (float)( (TypeLong*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_FLOAT:
			dfTemp = ( ( (TypeFloat*) (DataType*) p_pDataType )->GetValue() );
			break;

		case CNS_TC_DOUBLE:
			dfTemp = ( ( (TypeDouble*) (DataType*) p_pDataType )->GetValue() );
			break;

		default:
			Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation Compare not supported between Long and DataType", __FILE__, __LINE__ );
			throw except; 
	}
	
	if( GetValue() < dfTemp  )
		return -1;
	else if( GetValue() > dfTemp )
		return 1;
	else
		return 0;
}



void TypeFloat::Store( Meson::Common::IO::IOutputStream& p_outStream )
{
	p_outStream << GetValue();
}

void TypeFloat::Load(  Meson::Common::IO::IInputStream& p_inStream )
{
	p_inStream >> m_fValue;
}

Meson_Cuneus_RTS_Typing_END
