#pragma once
#include "Namespaces.h"
#include "Types.h"
#include "OutputDevice.h"

Meson_Common_System_LoggingDevice_BEGIN

class ConsoleOutputDevice : 
	public Meson::Common::Logging::IOutputDevice
{
private:
	static word m_wCreated;

public:
	ConsoleOutputDevice(void);
	~ConsoleOutputDevice(void);

	void Output( char p_cValue );
	void Output( const char *p_cValue, size_t p_unSize );
};

Meson_Common_System_LoggingDevice_END
