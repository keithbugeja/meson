#pragma once

#include "InputStream.h"

Meson_Common_IO_BEGIN

struct LookaheadInputStreamState;

class LookaheadInputStream
	: public IInputStream
{
private:
	LookaheadInputStreamState* m_pLookaheadInputStreamState;

public:
	LookaheadInputStream(InputStreamPtr p_pInputStream);
	~LookaheadInputStream(void);

	// IStream methods
	bool IsOpen(void) const;
	bool IsEndOfStream(void) const;
	void Reopen(void);
	void Close(void);

	// IInputStream methods
	bool IsDataAvailable(void) const;
	size_t Read(void* p_pBuffer, size_t p_unLength);
	IInputStream& operator>>(char& p_cValue);
	IInputStream& operator>>(bool& p_bValue);
	IInputStream& operator>>(short& p_sValue);
	IInputStream& operator>>(ushort& p_usValue);
	IInputStream& operator>>(int& p_nValue);
	IInputStream& operator>>(uint& p_unValue);
	IInputStream& operator>>(long& p_lValue);
	IInputStream& operator>>(ulong& p_ulValue);
	IInputStream& operator>>(long long& p_llValue);
	IInputStream& operator>>(float& p_fValue);
	IInputStream& operator>>(double& p_dValue);
	IInputStream& operator>>(Meson::Common::Text::String& p_strValue);

	// custom methods
	size_t Lookahead(void* p_pBuffer, size_t p_unLength);
	void Pushback(const void* p_pBuffer, size_t p_unLength);
};

typedef TPointer<LookaheadInputStream> LookaheadInputStreamPtr;

Meson_Common_IO_END
