#include "CnsTypeField.h"

#include "CnsClassLoader.h"
#include "CnsDataTypeMgr.h"

using namespace Meson::Cuneus::RTS::Typing;

Meson_Cuneus_RTS_Reflection_BEGIN

TypeField::TypeField( void ) : Entity(), m_dwTypeCode( 0 )
{
}

TypeField::~TypeField( void )
{
}

dword TypeField::GetTypeCode( void )
{
	return m_dwTypeCode;
}

void TypeField::SetTypeCode( dword p_dwTypeCode )
{
	m_dwTypeCode = p_dwTypeCode;
}

bool TypeField::operator==( const TypeField& p_typeField )
{
	//return this->m_dataType.Compare( ((TypeField)p_typeField).m_dataType ) == 0;
	return m_dwTypeCode == p_typeField.m_dwTypeCode;
}

void TypeField::Load( Meson::Common::IO::IInputStream& p_stream )
{
	//Load Name
	//SetName( ClassLoader::ReadString( p_stream ) );

	//Load Type
	//String strTypeName = ClassLoader::ReadString( p_stream );

	//m_dataType = *DataTypeMgr::GetInstance()->Get( strTypeName );
}

void TypeField::Store( Meson::Common::IO::IOutputStream& p_stream )
{
	p_stream.Write( m_strName.ToArray(), m_strName.Length() );
	//p_stream.Write( m_dataType.GetName().ToArray(), m_dataType.GetName().Length() );
}


Meson_Cuneus_RTS_Reflection_END
