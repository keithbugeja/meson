
#include "FileOutputDevice.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::IO;

Meson_Common_System_LoggingDevice_BEGIN

FileOutputDevice::FileOutputDevice( const char *p_pszFileName )
{
	m_pFileStream = new FileOutputStream( String(p_pszFileName) );
}

FileOutputDevice::~FileOutputDevice( void )
{
	if( m_pFileStream )
	{
		m_pFileStream->Flush();
		m_pFileStream->Close();
		delete m_pFileStream;
	}
}

void FileOutputDevice::Output( char p_cValue )
{
	(*m_pFileStream) << p_cValue;
	if( p_cValue == '\n' )
		m_pFileStream->Flush();
}

void FileOutputDevice::Output( const char *p_cValue, size_t p_unSize )
{
	m_pFileStream->Write( (void*)p_cValue, p_unSize );
	m_pFileStream->Flush();
}

Meson_Common_System_LoggingDevice_END
