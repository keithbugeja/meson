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

class IInputDevice
{

public:
	virtual char   Input( void ) = 0;
	virtual size_t InputLine( char *p_cValue ) = 0;

};

Meson_Common_Logging_END
