
#include "BasicByteCodeManager.h"
#include <string.h>

Meson_Cuneus_BEGIN

BasicByteCodeMgr::BasicByteCodeMgr( void )
{
	m_pByteCode = new byte[ 10240 ];
	m_pOffset = m_pByteCode;
	m_unSize = 10240;
}

BasicByteCodeMgr::~BasicByteCodeMgr( void )
{
	if( m_pByteCode )
	{
		delete m_pByteCode;
		m_pByteCode = NULL;
	}
}


void BasicByteCodeMgr::Startup( Meson::Common::Config::Configuration &p_config ) {}
void BasicByteCodeMgr::Shutdown( void ) {}

size_t BasicByteCodeMgr::Size( void )
{
	return ( m_pOffset - m_pByteCode );
}

dword BasicByteCodeMgr::GetOffset( void )
{
	return (dword)( m_pOffset - m_pByteCode );
}

void BasicByteCodeMgr::AppendByteCode( const byte *p_pbyCode, size_t p_unSize )
{
	if( ( m_pOffset + p_unSize ) >= ( m_pByteCode + m_unSize ) )
		ExpandBuffer();

	memcpy( m_pOffset, p_pbyCode, p_unSize );

	m_pOffset += p_unSize;
}

void BasicByteCodeMgr::RemoveByteCode( size_t p_unOffset, size_t p_unSize )
{
	memcpy( m_pByteCode + p_unOffset, 0, p_unSize );
}

void BasicByteCodeMgr::GetByteCode( byte *p_pbyCode, size_t p_unOffset, size_t p_unSize )
{
	memcpy( p_pbyCode, m_pByteCode + p_unOffset, p_unSize );
}


byte *BasicByteCodeMgr::GetByteCodePointer( size_t p_unOffset )
{
	return ( m_pByteCode + p_unOffset );
}


//----------------------------------------------------------------------------------------------
// Expands the bytecode buffer
//----------------------------------------------------------------------------------------------
void BasicByteCodeMgr::ExpandBuffer( void )
{
	m_unSize <<= 1;
	byte *pByte = new byte[ m_unSize ];
	size_t dwSize = ( m_pOffset - m_pByteCode );
	memcpy( pByte, m_pByteCode, dwSize );
	delete[] m_pByteCode;
	
	m_pByteCode = pByte;
	m_pOffset = m_pByteCode + dwSize;
}

Meson_Cuneus_END
