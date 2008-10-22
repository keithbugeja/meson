#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	17/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "Namespaces.h"

#include "OutputDevice.h"
#include "ArrayList.h"

Meson_Common_System_LoggingDevice_BEGIN

class OutputDeviceGroup :
	public Meson::Common::Logging::IOutputDevice
{
private:
	Meson::Common::Collections::TArrayList<IOutputDevice*> m_lstDevice;

public:
	OutputDeviceGroup(void);
	~OutputDeviceGroup(void);

	void Output( char p_cValue );
	void Output( const char *p_cValue, size_t p_unSize );

	void AddDevice( IOutputDevice *p_pOutputDevice );
	void RemoveDevice( IOutputDevice *p_pOutputDevice );
};

Meson_Common_System_LoggingDevice_END
