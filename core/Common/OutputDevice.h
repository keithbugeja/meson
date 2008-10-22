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

Meson_Common_Logging_BEGIN

class IOutputDevice
{
public:
	virtual void Output( char p_cValue ) = 0;
	virtual void Output( const char *p_cValue, size_t p_unSize ) = 0;
};

Meson_Common_Logging_END
