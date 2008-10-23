//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#include "Channel.h"
#include <stdio.h>
#include <string.h>
#include "Converter.h"

#define BUFFERSIZE 1024

#pragma warning( disable : 4996 )

using namespace Meson::Common::Text;
using namespace Meson::Common::Logging;

Channel::Channel(void) : m_strBuffer( BUFFERSIZE )
{
	//m_wPos = 0;
	m_pOutputDevice = NULL;
}

Channel::~Channel(void)
{
}


const char *Channel::GetBuffer( void )
{
	return m_strBuffer;
}


IOutputDevice *Channel::GetOutputDevice( void )
{
	return m_pOutputDevice;
}

void Channel::SetOutputDevice( IOutputDevice *p_pOutputDevice )
{
	m_pOutputDevice = p_pOutputDevice;
}


inline void Channel::Write( char p_ch )
{
	m_strBuffer += p_ch;
	if( p_ch == '\n' )
		Flush();
}


Channel &Channel::operator<<( unsigned long long p_ullValue )
{
	Converter::ToString( p_ullValue, m_strBuffer );
	return *this;
}

Channel &Channel::operator<<( signed long long p_llValue )
{
	Converter::ToString( p_llValue, m_strBuffer );

	return *this;
}

Channel &Channel::operator<<( unsigned long  p_ulValue )
{
	Converter::ToString( p_ulValue, m_strBuffer );
	return *this;
}

Channel &Channel::operator<<( signed long p_lValue )
{
	Converter::ToString( p_lValue, m_strBuffer );
	return *this;
}

Channel &Channel::operator<<( unsigned int p_unValue )
{
	Converter::ToString( (unsigned long)p_unValue, m_strBuffer );
	return *this;
}

Channel &Channel::operator<<( signed int p_nValue )
{
	Converter::ToString( (long)p_nValue, m_strBuffer );
	return *this;
}


Channel &Channel::operator<<( const word p_wValue )
{
	Converter::ToString( p_wValue, m_strBuffer );
	return *this;
}

Channel &Channel::operator<<( const signed short p_sValue )
{
	Converter::ToString( p_sValue, m_strBuffer );
	return *this;
}

Channel &Channel::operator<<( const byte p_byValue )
{
	Converter::ToString( p_byValue, m_strBuffer );
	return *this;
}

Channel &Channel::operator<<( const char p_cValue )
{
	Write( p_cValue );
	return *this;
}

Channel &Channel::operator<<( const float p_fValue )
{
	Converter::ToString( p_fValue, m_strBuffer );
	return *this;
}

Channel &Channel::operator<<( const double p_dfValue )
{
	Converter::ToString( p_dfValue, m_strBuffer );
	return *this;
}

Channel &Channel::operator<<( const long double p_ldfValue )
{
	Converter::ToString( p_ldfValue, m_strBuffer );
	return *this;
}

Channel &Channel::operator<<( const char *p_pszValue )
{
	m_strBuffer += p_pszValue;
	Flush();

	return *this;
}

void Channel::Flush( void )
{
	m_pOutputDevice->Output( m_strBuffer, m_strBuffer.Size() );
	m_strBuffer.Clear();
}
