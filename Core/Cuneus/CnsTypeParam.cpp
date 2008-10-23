#include "CnsTypeParam.h"

#include "CnsClassLoader.h"
#include "CnsDataTypeMgr.h"

#include "MesonException.h"

Meson_Cuneus_RTS_Reflection_BEGIN

TypeParam::TypeParam( void ) : m_pDataType(  NULL ), m_dwTypeCode( 0 )
{
}

TypeParam::~TypeParam( void )
{
}


dword TypeParam::GetTypeCode( void )
{
	return m_dwTypeCode;
}

void TypeParam::SetTypeCode( dword p_dwTypeCode )
{
	m_dwTypeCode = p_dwTypeCode;
}


Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > TypeParam::GetValue( void )
{
	return m_pDataType;
}

void TypeParam::SetValue( Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > p_pDataType )
{
	if( p_pDataType->GetTypeCode() != m_dwTypeCode )
		throw new Meson::Common::MesonException( "Actual Parameter does not match formal parameter.", __FILE__, __LINE__ );

	m_pDataType = p_pDataType;
}


bool TypeParam::operator==( const TypeParam& p_typeParam )
{
	
	return m_dwTypeCode == p_typeParam.m_dwTypeCode;
}

void TypeParam::Load( Meson::Common::IO::IInputStream& p_stream )
{
	//Load Name
	//SetName( ClassLoader::ReadString( p_stream ) );

	//Load Type
	//String strTypeName = ClassLoader::ReadString( p_stream );

	//m_dataType = *DataTypeMgr::GetInstance()->Get( strTypeName );
}

void TypeParam::Store( Meson::Common::IO::IOutputStream& p_stream )
{
	p_stream.Write( m_strName.ToArray(), m_strName.Length() );
	//p_stream.Write( m_dataType.GetName().ToArray(), m_dataType.GetName().Length() );
}

Meson_Cuneus_RTS_Reflection_END