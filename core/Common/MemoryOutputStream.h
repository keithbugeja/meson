#pragma once

#include "OutputStream.h"

Meson_Common_IO_BEGIN

struct MemoryOutputStreamState;

class MemoryOutputStream :
	public IOutputStream
{
private:
	MemoryOutputStreamState* m_pMemoryOutputStreamState;

public:
	MemoryOutputStream(void);
	~MemoryOutputStream(void);
	bool IsOpen(void) const;
	bool IsEndOfStream(void) const;
	void Reopen(void);
	void Close(void);
	size_t Write(void* p_pBuffer, size_t p_unLength);
	void Flush(void);
	IOutputStream& operator<<(char p_cValue);
	IOutputStream& operator<<(bool p_bValue);
	IOutputStream& operator<<(short p_sValue);
	IOutputStream& operator<<(ushort p_usValue);
	IOutputStream& operator<<(int p_nValue);
	IOutputStream& operator<<(uint p_unValue);
	IOutputStream& operator<<(long p_lValue);
	IOutputStream& operator<<(ulong p_ulValue);
	IOutputStream& operator<<(long long p_llValue);
	IOutputStream& operator<<(float p_fValue);
	IOutputStream& operator<<(double p_dValue);
	IOutputStream& operator<<(const char* p_pszValue);
	IOutputStream& operator<<(const Meson::Common::Text::String& p_strValue);
	IOutputStream& operator<<(IInputStream& p_inputStream);
	const void* GetBuffer(void) const;
	size_t GetBufferLength(void) const;
};

Meson_Common_IO_END
