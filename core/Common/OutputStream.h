#pragma once

#include "Stream.h"
#include "String8.h"
#include "InputStream.h"

Meson_Common_IO_BEGIN

class IOutputStream
	: public IStream
{
public:
	IOutputStream(void) {};
	virtual size_t Write(void* p_pBuffer, size_t p_unLength) = 0;
	virtual void Flush(void) = 0;
	virtual IOutputStream& operator<<(char p_cValue) = 0;
	virtual IOutputStream& operator<<(bool p_bValue) = 0;
	virtual IOutputStream& operator<<(short p_sValue) = 0;
	virtual IOutputStream& operator<<(ushort p_usValue) = 0;
	virtual IOutputStream& operator<<(int p_nValue) = 0;
	virtual IOutputStream& operator<<(uint p_unValue) = 0;
	virtual IOutputStream& operator<<(long p_lValue) = 0;
	virtual IOutputStream& operator<<(ulong p_ulValue) = 0;
	virtual IOutputStream& operator<<(long long p_llValue) = 0;
	virtual IOutputStream& operator<<(float p_fValue) = 0;
	virtual IOutputStream& operator<<(double p_dValue) = 0;
	virtual IOutputStream& operator<<(const char* p_pszValue) = 0;
	virtual IOutputStream& operator<<(const Meson::Common::Text::String& p_strValue) = 0;
	virtual IOutputStream& operator<<(IInputStream& p_inputStream) = 0;
};

typedef Meson::Common::TPointer<IOutputStream> OutputStreamPtr;

Meson_Common_IO_END
