#pragma once

#include "Types.h"
#include "Stream.h"
#include "String8.h"

Meson_Common_IO_BEGIN

class IInputStream
	: public IStream
{
public:
	IInputStream(void) {};
	virtual bool IsDataAvailable(void) const = 0;
	virtual size_t Read(void* p_pBuffer, size_t p_unLength) = 0;
	virtual IInputStream& operator>>(char& p_cValue) = 0;
	virtual IInputStream& operator>>(bool& p_bValue) = 0;
	virtual IInputStream& operator>>(short& p_sValue) = 0;
	virtual IInputStream& operator>>(ushort& p_usValue) = 0;
	virtual IInputStream& operator>>(int& p_nValue) = 0;
	virtual IInputStream& operator>>(uint& p_unValue) = 0;
	virtual IInputStream& operator>>(long& p_lValue) = 0;
	virtual IInputStream& operator>>(ulong& p_ulValue) = 0;
	virtual IInputStream& operator>>(long long& p_llValue) = 0;
	virtual IInputStream& operator>>(float& p_fValue) = 0;
	virtual IInputStream& operator>>(double& p_dValue) = 0;
	virtual IInputStream& operator>>(Meson::Common::Text::String& p_strValue) = 0;
};

typedef Meson::Common::TPointer<IInputStream> InputStreamPtr;

Meson_Common_IO_END
