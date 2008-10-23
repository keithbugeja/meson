#include "CnsEntity.h"

using namespace Meson::Common::Text;

Meson_Cuneus_RTS_Typing_BEGIN

Entity::Entity( void ) 
	: m_strName()
	, m_dwTypeCode( 0 )
{

}

Entity::Entity( const Entity& p_entity )
{
	m_strName = p_entity.m_strName;
	m_dwTypeCode = p_entity.m_dwTypeCode;
}

Entity::Entity( Meson::Common::Text::String& p_strName, dword p_dwTypeCode )
{
	m_strName = p_strName;
	m_dwTypeCode = p_dwTypeCode;
}

Entity::~Entity(void)
{
}

String Entity::GetName( void )
{
	return m_strName;
}

void Entity::SetName( String& p_strName )
{
	m_strName = p_strName;
}

dword Entity::GetTypeCode( void )
{
	return m_dwTypeCode;
}

void Entity::SetTypeCode( dword p_dwTypeCode )
{
	m_dwTypeCode = p_dwTypeCode;
}

dword Entity::GetSize( void )
{
	return 0;
}

Entity* Entity::CreateInstance( void )
{
	return new Entity( *this );
}

Entity* Entity::Clone( bool p_bShallow )
{
	return new Entity( *this );
}


String Entity::ToString( void )
{
	return m_strName;
}

Meson_Cuneus_RTS_Typing_END
