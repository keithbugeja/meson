#include "FileOutputStream.h"

#include "MesonException.h"

#pragma warning( disable : 4996 )

#include <stdio.h>

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::IO;

Meson_Common_IO_BEGIN

struct FileOutputStreamState
{
	String m_strFilename;
	FILE *m_pFile;
	bool m_bOpen;
};

Meson_Common_IO_END

FileOutputStream::FileOutputStream(const String& p_strFilename)
	: m_pFileOutputStreamState(NULL)
{
	FILE* pFile = fopen((String &)p_strFilename, "wb");
	if (pFile == NULL)
		throw new MesonException(
			"Unable to open file '" + p_strFilename + "' for output.",
			__FILE__, __LINE__);

	m_pFileOutputStreamState = new FileOutputStreamState();
	m_pFileOutputStreamState->m_strFilename = p_strFilename;
	m_pFileOutputStreamState->m_pFile = pFile;
	m_pFileOutputStreamState->m_bOpen = true;
}

FileOutputStream::~FileOutputStream(void)
{
	if (m_pFileOutputStreamState->m_bOpen)
		fclose(m_pFileOutputStreamState->m_pFile);

	delete m_pFileOutputStreamState;
}

bool FileOutputStream::IsOpen(void) const
{
	return m_pFileOutputStreamState->m_bOpen;
}

bool FileOutputStream::IsEndOfStream(void) const
{
	return false;
}

void FileOutputStream::Reopen(void)
{
	if (m_pFileOutputStreamState->m_bOpen)
		Close();

	FILE* pFile = freopen(m_pFileOutputStreamState->m_strFilename,
		"wb", m_pFileOutputStreamState->m_pFile);
	if (pFile == NULL)
		throw new MesonException(
			"Unable to open file '" + m_pFileOutputStreamState->m_strFilename + "' for output.",
			__FILE__, __LINE__);

	m_pFileOutputStreamState->m_pFile = pFile;
}

void FileOutputStream::Close(void)
{
	if (!m_pFileOutputStreamState->m_bOpen)
		throw new MesonException(
			"The file output stream is closed.", __FILE__, __LINE__);

	fclose(m_pFileOutputStreamState->m_pFile);
	m_pFileOutputStreamState->m_pFile = NULL;
	m_pFileOutputStreamState->m_bOpen = false;
}

size_t FileOutputStream::Write(void* p_pBuffer, size_t p_unLength)
{
	if (!m_pFileOutputStreamState->m_bOpen)
		throw new MesonException(
			"The file output stream is closed.", __FILE__, __LINE__);

	return fwrite(p_pBuffer, 1, p_unLength, m_pFileOutputStreamState->m_pFile);
}

void FileOutputStream::Flush(void)
{
	if (!m_pFileOutputStreamState->m_bOpen)
		throw new MesonException(
			"The file output stream is closed.", __FILE__, __LINE__);

	if (fflush(m_pFileOutputStreamState->m_pFile) != 0)
		throw new MesonException(
			"Error while flushing file output stream.",
			__FILE__, __LINE__);
}

IOutputStream& FileOutputStream::operator<<(char p_cValue)
{
	Write(&p_cValue, sizeof(p_cValue));
	return *this;
}

IOutputStream& FileOutputStream::operator<<(bool p_bValue)
{
	char p_cValue = (char) p_bValue;
	Write(&p_cValue, sizeof(p_cValue));
	return *this;
}

IOutputStream& FileOutputStream::operator<<(short p_sValue)
{
	Write(&p_sValue, sizeof(p_sValue));
	return *this;
}

IOutputStream& FileOutputStream::operator<<(ushort p_usValue)
{
	Write(&p_usValue, sizeof(p_usValue));
	return *this;
}

inline IOutputStream& FileOutputStream::operator<<(int p_nValue)
{
	Write(&p_nValue, sizeof(p_nValue));
	return *this;
}

inline IOutputStream& FileOutputStream::operator<<(uint p_unValue)
{
	Write(&p_unValue, sizeof(p_unValue));
	return *this;
}

inline IOutputStream& FileOutputStream::operator<<(long p_lValue)
{
	Write(&p_lValue, sizeof(p_lValue));
	return *this;
}

inline IOutputStream& FileOutputStream::operator<<(ulong p_ulValue)
{
	Write(&p_ulValue, sizeof(p_ulValue));
	return *this;
}

inline IOutputStream& FileOutputStream::operator<<(long long p_llValue)
{
	Write(&p_llValue, sizeof(p_llValue));
	return *this;
}

inline IOutputStream& FileOutputStream::operator<<(float p_fValue)
{
	Write(&p_fValue, sizeof(p_fValue));
	return *this;
}

inline IOutputStream& FileOutputStream::operator<<(double p_dValue)
{
	Write(&p_dValue, sizeof(p_dValue));
	return *this;
}

inline IOutputStream& FileOutputStream::operator<<(const char* p_pszValue)
{
	size_t unLength = 1;
	const char *pChar = p_pszValue;
	while (*pChar++ != 0) ++unLength;
	Write((void *)p_pszValue, unLength);
	return *this;
}

inline IOutputStream& FileOutputStream::operator<<(const String& p_strValue)
{
	*this << (uint) p_strValue.Length();

	if( p_strValue.Length() == 0 )
		return *this;

	const char *pBuffer = ((String &) p_strValue).GetBuffer();
	Write((void *) pBuffer, p_strValue.Length());
	return *this;
}

inline IOutputStream& FileOutputStream::operator<<(
	IInputStream& p_inputStream)
{
	const size_t BUFFER_LENGTH = 4096;
	char* pBuffer[BUFFER_LENGTH];

	while (!p_inputStream.IsEndOfStream())
	{
		size_t unBytesRead = p_inputStream.Read(pBuffer, BUFFER_LENGTH);
		size_t unBytesWritten = Write(pBuffer, unBytesRead);
		Flush();
		if (unBytesWritten == 0) break;
	}
	return *this;
}
