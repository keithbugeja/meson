#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "Namespaces.h"

#include "OutputDevice.h"
#include "FileOutputStream.h"

Meson_Common_System_LoggingDevice_BEGIN

class NullOutputDevice :
	public Meson::Common::Logging::IOutputDevice
{
public:
	NullOutputDevice(void);
	virtual ~NullOutputDevice(void);

	void Output( char p_cValue );
	void Output( const char *p_cValue, size_t p_unSize );
};

Meson_Common_System_LoggingDevice_END
