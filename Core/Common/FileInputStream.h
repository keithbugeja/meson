#pragma once

#include "InputStream.h"
#include "String8.h"

Meson_Common_IO_BEGIN

struct FileInputStreamState;

class FileInputStream :
	public IInputStream
{
private:
	FileInputStreamState *m_pFileInputStreamState;

public:
	FileInputStream(const Meson::Common::Text::String& p_strFilename);
	~FileInputStream(void);
	bool IsOpen(void) const;
	bool IsDataAvailable(void) const;
	bool IsEndOfStream(void) const;
	void Reopen(void);
	void Close(void);
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
};

Meson_Common_IO_END
