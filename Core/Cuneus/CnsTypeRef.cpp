#include "CnsTypeRef.h"

Meson_Cuneus_RTS_Typing_BEGIN

TypeRef::TypeRef( void )
	: DataType( "Ref", 0 )
{
	m_pValue = NULL;
}

TypeRef::TypeRef( void* p_pValue )
	: DataType( "Ref", 0 )
{
	m_pValue = p_pValue;
}

void* TypeRef::GetValue( void )
{
	return m_pValue;
}

void TypeRef::SetValue( void* p_pValue )
{
	m_pValue = p_pValue;
}

void TypeRef::GetData( byte* p_pbyData )
{
	*((void**)p_pbyData) = m_pValue;
}

void TypeRef::SetData( byte* p_pbyData )
{
	m_pValue = *((void**)p_pbyData);
}

TypeRef& TypeRef::operator=( void* p_pValue )
{
	m_pValue = p_pValue;
	return *this;
}

TypeRef::operator void*( void )
{
	return m_pValue;
}

String TypeRef::ToString( void )
{
	String str;
	str += ( (long long)m_pValue );
	return str;
}


Meson_Cuneus_RTS_Typing_END
