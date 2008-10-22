#include "StringBuffer.h"

#include <stdlib.h>
#pragma warning( disable : 4996 )
#include <string.h>

using namespace Meson::Common::Text;

StringBuffer::StringBuffer(void)
{
	//m_eType = BufferType::NONE;
	m_pcBufferStart = NULL;
	m_pcDataStart = NULL;
	m_unRefCount = 1;
	m_unBufferSize = 0;
	m_unDataSize = 0;
}

StringBuffer::StringBuffer( const StringBuffer &p_stringBuffer )
{
	Alloc( p_stringBuffer.GetDataSize() );
	Copy( m_pcDataStart, p_stringBuffer.m_pcDataStart, p_stringBuffer.m_unDataSize, true );
	m_unDataSize = p_stringBuffer.m_unDataSize;
}

StringBuffer::StringBuffer( int p_nSize )
{
	Alloc( p_nSize );
}

StringBuffer::~StringBuffer(void)
{
}

void StringBuffer::Alloc( size_t p_unSize )
{
	if( p_unSize < 8 )
		p_unSize = 8;

	//m_eType = BufferType::DYNAMIC;
	m_unRefCount = 1;
	m_unBufferSize = p_unSize << 1;
	m_unDataSize = 0;
	//m_pcBufferStart = new char[ m_unBufferSize ];
	m_pcBufferStart = (char*) malloc( m_unBufferSize );
	m_pcDataStart = m_pcBufferStart + ( p_unSize >> 1 );
}

void StringBuffer::Realloc( size_t p_unSize )
{
	size_t	unNewBufferSize = p_unSize << 1,
			unOffset = p_unSize >> 1;

	//char *pcNewBufferStart = new char[ unNewBufferSize ];
	char *pcNewBufferStart = (char*) malloc( unNewBufferSize );
	Copy( pcNewBufferStart + unOffset, m_pcDataStart, m_unDataSize, true );
	//delete[] m_pcBufferStart;
	free( m_pcBufferStart );

	m_pcBufferStart = pcNewBufferStart;
	m_unBufferSize = unNewBufferSize;
	m_pcDataStart = m_pcBufferStart + unOffset;
}

void StringBuffer::Realloc( size_t p_unOffset, size_t p_unSize )
{
	size_t	unNewBufferSize = p_unOffset + p_unSize;

	//char *pcNewBufferStart = new char[ unNewBufferSize ];
	char *pcNewBufferStart = (char*) malloc( unNewBufferSize );
	Copy( pcNewBufferStart + p_unOffset, m_pcDataStart, m_unDataSize, true );
	//delete[] m_pcBufferStart;
	free( m_pcBufferStart );

	m_pcBufferStart = pcNewBufferStart;
	m_unBufferSize = unNewBufferSize;
	m_pcDataStart = m_pcBufferStart + p_unOffset;
}

void StringBuffer::Copy( char *p_pcDst, const char *p_pcSrc, size_t p_unSize, bool p_bFwd )
{
	if( p_bFwd )
		strncpy( p_pcDst, p_pcSrc, p_unSize );
	else
	{
		while( p_unSize > 0 )
		{
			*p_pcDst = *p_pcSrc;
			p_pcDst--;
			p_pcSrc--;
			p_unSize--;
		}
	}
}


//void StringBuffer::SetStatic( const char *p_psz )
//{
//	m_eType = BufferType::STATIC;
//	m_pcBufferStart = (char*) p_psz;
//	m_pcDataStart = m_pcBufferStart;
//	m_unRefCount = 1;
//	m_unBufferSize = strlen( p_psz );
//	m_unDataSize = m_unBufferSize;
//}


char *StringBuffer::GetBufferStart( void )
{
	return m_pcBufferStart;
}

char *StringBuffer::GetData( void )
{
	return m_pcDataStart;
}

size_t StringBuffer::GetBufferSize( void ) const
{
	return m_unBufferSize;
}

size_t StringBuffer::GetDataSize( void ) const
{
	return m_unDataSize;
}

void StringBuffer::SetDataStart( char *p_pcData )
{
	m_pcDataStart = p_pcData;
}

void StringBuffer::SetDataSize( size_t p_unSize )
{
	m_unDataSize = p_unSize;
}


void StringBuffer::IncRef( void )
{
	m_unRefCount++;
}

void StringBuffer::DecRef( void )
{
	m_unRefCount--;
	if( m_pcBufferStart && (m_unRefCount==0) )//&& m_eType == BufferType::DYNAMIC )
	{
		//delete[] m_pcBufferStart;
		free( m_pcBufferStart );
		m_pcBufferStart = NULL;
	}
	
}

size_t StringBuffer::GetRef( void )
{

	return m_unRefCount;
}


//BufferType::BufferType StringBuffer::GetType( void ) const
//{
//	return m_eType;
//}

//void StringBuffer::SetType( BufferType::BufferType p_eType )
//{
//	m_eType = p_eType;
//}

size_t StringBuffer::Size( void )
{
	return m_unDataSize;
}


StringBuffer *StringBuffer::Clone( void )
{
	return new StringBuffer( *this );
}

void StringBuffer::Clear( void )
{
	//m_eType = BufferType::DYNAMIC;
	m_pcDataStart = m_pcBufferStart + (m_unBufferSize >> 2);
	m_unRefCount = 1;
	m_unDataSize = 0;
}

void StringBuffer::Append( char p_cData )
{
	if( m_pcDataStart + m_unDataSize >= m_pcBufferStart + m_unBufferSize )
		Realloc( m_unBufferSize );

	m_pcDataStart[ m_unDataSize++ ] = p_cData;
}

void StringBuffer::Append( const char *p_szData )
{
	size_t unSize = strlen( p_szData );
	
	if( ( m_pcDataStart + m_unDataSize + unSize ) >= m_pcBufferStart + m_unBufferSize )
		Realloc( m_unDataSize + ( unSize << 1 )  );

	Copy( m_pcDataStart + m_unDataSize, p_szData, unSize, true );
	m_unDataSize += unSize;
}

void StringBuffer::Append( const char *p_szData, size_t p_unSize )
{	
	if( ( m_pcDataStart + m_unDataSize + p_unSize ) >= m_pcBufferStart + m_unBufferSize )
		Realloc( m_unDataSize + ( p_unSize << 1 )  );

	Copy( m_pcDataStart + m_unDataSize, p_szData, p_unSize, true );
	m_unDataSize += p_unSize;
}


void StringBuffer::Insert( char p_cData, int p_nPos )
{
	if( ( ( m_pcDataStart + m_unDataSize ) >= ( m_pcBufferStart + m_unBufferSize ) ) || ( m_pcDataStart <= m_pcBufferStart ) )
		Realloc( m_unBufferSize );

	if( p_nPos <= 0 )
	{
		m_pcDataStart--;
		*m_pcDataStart = p_cData;
		m_unDataSize++;
	}
	else if( p_nPos >= (int)m_unDataSize )
	{
		Append( p_cData );
	}
	else
	{
		Copy( m_pcDataStart + m_unDataSize + 1, m_pcDataStart + m_unDataSize, m_unDataSize - p_nPos + 1, false );
		m_pcDataStart[ p_nPos ] = p_cData;
		m_unDataSize++;
	}
}


void StringBuffer::Insert( const char *p_szData, int p_nPos )
{
	size_t unSize = strlen( p_szData );

	if( p_nPos <= 0 )
	{
		if ( (m_pcDataStart - unSize) < m_pcBufferStart)
			Realloc( unSize, m_unBufferSize + unSize );

		m_pcDataStart-=unSize;
		Copy( m_pcDataStart, p_szData, unSize, true );
		m_unDataSize += unSize;
	}
	else if( p_nPos >= (int)m_unDataSize )
	{
		Append( p_szData );
	}
	else
	{
		if( ( m_pcDataStart + m_unDataSize + unSize ) >= ( m_pcBufferStart + m_unBufferSize ) )
			Realloc( m_unBufferSize + unSize );

		Copy( m_pcDataStart + m_unDataSize + unSize, m_pcDataStart + m_unDataSize, m_unDataSize - p_nPos + unSize, false );
		Copy( m_pcDataStart+p_nPos, p_szData, unSize, true );
		m_unDataSize += unSize;
	}
}


void StringBuffer::Delete( size_t p_unPos )
{
	if( p_unPos == 0 )
	{
		m_pcDataStart++;
		m_unDataSize--;
	}
	else if( p_unPos >= ( m_unDataSize - 1 ) )
	{
		m_unDataSize--;
	}
	else
	{
		Copy( m_pcDataStart + p_unPos, m_pcDataStart + p_unPos - 1, p_unPos, false );
		m_unDataSize--;
		m_pcDataStart++;
	}
}

void StringBuffer::Delete( size_t p_unStart, size_t p_unLength )
{
	if( p_unStart == 0 )
	{
		m_pcDataStart += p_unLength;
		m_unDataSize -= p_unLength;
	}
	else if( p_unStart >= ( m_unDataSize - p_unLength ) )
	{
		m_unDataSize -= p_unLength;
	}
	else
	{
		Copy( m_pcDataStart + p_unStart, m_pcDataStart + p_unStart + p_unLength, m_unDataSize - p_unLength, true );
		m_unDataSize -= p_unLength;
	}
}

void StringBuffer::DeleteToEnd( size_t p_unStart )
{
	m_unDataSize = p_unStart;
}


char StringBuffer::Pop( size_t p_unPos )
{
	if( p_unPos == 0 )
	{
		m_pcDataStart++;
		m_unDataSize--;
		return *( m_pcDataStart-1 );
	}
	else if( p_unPos >= ( m_unDataSize - 1 ) )
	{
		m_unDataSize--;
		return m_pcDataStart[m_unDataSize];
	}
	else
	{
		char cResult = m_pcDataStart[p_unPos];
		Copy( m_pcDataStart + p_unPos, m_pcDataStart + p_unPos - 1, p_unPos, false );
		m_unDataSize--;
		m_pcDataStart++;
		return cResult;
	}
}

char &StringBuffer::operator[]( int p_nPos )
{
	if( p_nPos < 0 )
		return *m_pcDataStart;
	if( p_nPos >= (int)m_unDataSize )
		return *(m_pcDataStart + m_unDataSize - 1);

	return m_pcDataStart[ p_nPos ];
}

char &StringBuffer::operator()( int p_nPos )
{
	return m_pcDataStart[ p_nPos ];
}

void StringBuffer::Trim( bool p_bLeft, bool p_bRight )
{
	char *m_pDataBegin = m_pcDataStart;
	char *m_pDataEnd   = m_pcDataStart + m_unDataSize;

	for( bool bFinished = false; ( m_pcDataStart <= m_pcDataStart + m_unDataSize ) && ( !bFinished ); )
	{
		bFinished = true;

		if( p_bLeft && ( *m_pcDataStart < 33  ) )
		{
			m_pcDataStart++;
			bFinished = false;
		}

		if( p_bRight && ( m_pcDataStart[ m_unDataSize ] < 33  ) )
		{
			m_unDataSize--;
			bFinished = false;
		}
	}
}

const char *StringBuffer::ToSZ( void )
{
	m_pcDataStart[ m_unDataSize ] = '\0';
	return m_pcDataStart;
}
