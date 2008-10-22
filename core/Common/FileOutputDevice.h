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


class FileOutputDevice :
	public Meson::Common::Logging::IOutputDevice
{
private:
	Meson::Common::IO::FileOutputStream *m_pFileStream;

public:
	FileOutputDevice( const char *p_pszFileName );
	virtual ~FileOutputDevice( void );

	void Output( char p_cValue );
	void Output( const char *p_cValue, size_t p_unSize );
};

Meson_Common_System_LoggingDevice_END
