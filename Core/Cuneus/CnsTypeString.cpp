#include "CnsTypeString.h"

#include "CnsAllDataTypes.h"
#include "CnsTypeCodes.h"

#include <MesonException.h>

using namespace Meson::Common;

Meson_Cuneus_RTS_Typing_BEGIN


TypeString::TypeString( void )
	: DataType( "String", CNS_TC_STRING )
{
}

TypeString::TypeString( String& p_str )
	: DataType( "String", CNS_TC_STRING )
{
	m_strValue = p_str;
}

TypeString::~TypeString(void)
{
}

TPointer< DataType > TypeString::Clone( void )
{
	TPointer< DataType > ptr;
	ptr = new TypeString( GetValue() );
	return ptr;
}

String& TypeString::GetValue( void )
{
	return m_strValue;
}

void TypeString::SetValue( Meson::Common::Text::String p_strValue )
{
	m_strValue = p_strValue;
}

void TypeString::GetData( byte* p_pbyData )
{
	dword dwSize = (dword)m_strValue.Size();
	*((dword*)p_pbyData) = dwSize;

	p_pbyData += sizeof( dword );
	for( dword dw = 0; dw < dwSize; dw++ )
	{
		*p_pbyData = m_strValue.GetCharAt( dwSize );
		p_pbyData++;
	}

}

void TypeString::SetData( byte* p_pbyData )
{
	dword dwSize = *((dword*)p_pbyData);
	p_pbyData += sizeof( dword );
	
	m_strValue.Clear();
	m_strValue.Append( (char*)p_pbyData, dwSize );
}


TypeString& TypeString::operator=( Meson::Common::Text::String p_strValue )
{
	m_strValue = p_strValue;
	return *this;
}

TypeString::operator Meson::Common::Text::String( void )
{
	return m_strValue;
}


Meson::Common::Text::String TypeString::ToString( void )
{
	return m_strValue;
}

//*********************************************************************************************************

Meson::Common::TPointer< DataType > TypeString::operator+( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::TPointer< DataType > pDT;
	pDT = this;

	m_strValue.Append( p_pDataType->ToString() );

	return pDT;
}


Meson::Common::TPointer< DataType > TypeString::operator-( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator*( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator/( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator%( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator&( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator|( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator^( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator>>( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator<<( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator-( void )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator~( void )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator++( void )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}

Meson::Common::TPointer< DataType > TypeString::operator--( void )
{
	Meson::Common::MesonException* except = new Meson::Common::MesonException( " Operation not supported for TypeString", __FILE__, __LINE__ );
	throw except;
}



int TypeString::Compare( Meson::Common::TPointer< DataType >& p_pDataType )
{
	return m_strValue.Compare( p_pDataType->ToString() );
}

//*********************************************************************************************************



void TypeString::Store( Meson::Common::IO::IOutputStream& p_outStream )
{
	p_outStream << GetValue();
}

void TypeString::Load(  Meson::Common::IO::IInputStream& p_inStream )
{
	p_inStream >> m_strValue;
}

Meson_Cuneus_RTS_Typing_END
