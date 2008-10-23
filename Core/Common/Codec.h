//----------------------------------------------------------------------------------------------
//	Meson::Common::Codec
//	Helper class that performs de/encoding of data.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	02/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Namespaces.h"

Meson_Common_BEGIN

class Codec
{
public:
	Codec(void);
	~Codec(void);

	static dword CRC32( byte *p_pData, dword p_dwSize );
};

Meson_Common_END
