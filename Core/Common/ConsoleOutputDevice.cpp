
#include "ConsoleOutputDevice.h"
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>



Meson_Common_System_LoggingDevice_BEGIN

word ConsoleOutputDevice::m_wCreated = 0;

ConsoleOutputDevice::ConsoleOutputDevice(void)
{
	if( m_wCreated == 0 )
	{
		FreeConsole();
		if ( AllocConsole() )
		{
			int hCrt = _open_osfhandle((intptr_t)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
			*stdout = *(::_fdopen(hCrt, "w"));
			::setvbuf(stdout, NULL, _IONBF, 0);
			*stderr = *(::_fdopen(hCrt, "w"));
			::setvbuf(stderr, NULL, _IONBF, 0);
			m_wCreated++;
		}
	}
}

ConsoleOutputDevice::~ConsoleOutputDevice(void)
{
	m_wCreated--;
	if( m_wCreated == 0 )
		FreeConsole();
}

void ConsoleOutputDevice::Output( char p_cValue )
{
	std::cout<<p_cValue;
	if( p_cValue == '\n' )
		std::cout.flush();
}

void ConsoleOutputDevice::Output( const char *p_cValue, size_t p_unSize )
{
	std::cout.write( p_cValue, (std::streamsize)p_unSize );
	std::cout.flush();
}

Meson_Common_System_LoggingDevice_END
