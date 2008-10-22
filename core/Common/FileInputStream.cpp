#include "FileInputStream.h"

#include "MesonException.h"

#pragma warning( disable : 4996 )

#include <stdio.h>

using namespace Meson::Common::Text;
using namespace Meson::Common::IO;

Meson_Common_IO_BEGIN

struct FileInputStreamState
{
	String m_strFilename;
	FILE *m_pFile;
	bool m_bOpen;
};

Meson_Common_IO_END

FileInputStream::FileInputStream(const String& p_strFilename)
	: m_pFileInputStreamState(NULL)
{
	FILE* pFile = fopen((String&)p_strFilename, "rb");
	if (pFile == NULL)
		throw new MesonException(
			"Unable to open file '" + p_strFilename + "' for input.",
			__FILE__, __LINE__);

	m_pFileInputStreamState = new FileInputStreamState();
	m_pFileInputStreamState->m_strFilename = p_strFilename;
	m_pFileInputStreamState->m_pFile = pFile;
	m_pFileInputStreamState->m_bOpen = true;
}

FileInputStream::~FileInputStream(void)
{
	if (m_pFileInputStreamState->m_bOpen)
		fclose(m_pFileInputStreamState->m_pFile);

	delete m_pFileInputStreamState;
}

bool FileInputStream::IsOpen(void) const
{
	return m_pFileInputStreamState->m_bOpen;
}

bool FileInputStream::IsDataAvailable(void) const
{
	if (!m_pFileInputStreamState->m_bOpen)
		throw new MesonException(
			"The file input stream is closed.", __FILE__, __LINE__);

	int nResult = fgetc(m_pFileInputStreamState->m_pFile);
	if (nResult == EOF) return false;
	ungetc(nResult, m_pFileInputStreamState->m_pFile);
	return true;
}

bool FileInputStream::IsEndOfStream(void) const
{
	if (!m_pFileInputStreamState->m_bOpen)
		throw new MesonException(
			"The file input stream is closed.", __FILE__, __LINE__);

	return feof(m_pFileInputStreamState->m_pFile) != 0;
}

void FileInputStream::Reopen(void)
{
	if (m_pFileInputStreamState->m_bOpen)
		Close();

	FILE *pFile = freopen(m_pFileInputStreamState->m_strFilename,
		"rb", m_pFileInputStreamState->m_pFile);
	if (pFile == NULL)
		throw new MesonException(
			"Unable to open file '" + m_pFileInputStreamState->m_strFilename + "' for input.",
			__FILE__, __LINE__);	
	m_pFileInputStreamState->m_pFile = pFile;
}

void FileInputStream::Close(void)
{
	if (!m_pFileInputStreamState->m_bOpen)
		throw new MesonException(
			"The file input stream is closed.", __FILE__, __LINE__);

	fclose(m_pFileInputStreamState->m_pFile);
	m_pFileInputStreamState->m_pFile = NULL;
	m_pFileInputStreamState->m_bOpen = false;
}

size_t FileInputStream::Read(void* pBuffer, size_t unLength)
{
	if (!m_pFileInputStreamState->m_bOpen)
		throw new MesonException(
			"The file input stream is closed.", __FILE__, __LINE__);

	return fread(pBuffer, 1, unLength, m_pFileInputStreamState->m_pFile);
}

inline IInputStream& FileInputStream::operator>>(char& p_cValue)
{
	size_t unBytesRead = Read(&p_cValue, sizeof(p_cValue));
	if (unBytesRead < sizeof(p_cValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& FileInputStream::operator>>(bool& p_bValue)
{
	size_t unBytesRead = Read(&p_bValue, sizeof(p_bValue));
	if (unBytesRead < sizeof(p_bValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& FileInputStream::operator>>(short& p_sValue)
{
	size_t unBytesRead = Read(&p_sValue, sizeof(p_sValue));
	if (unBytesRead < sizeof(p_sValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& FileInputStream::operator>>(ushort& p_usValue)
{
	size_t unBytesRead = Read(&p_usValue, sizeof(p_usValue));
	if (unBytesRead < sizeof(p_usValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& FileInputStream::operator>>(int& p_nValue)
{
	size_t unBytesRead = Read(&p_nValue, sizeof(p_nValue));
	if (unBytesRead < sizeof(p_nValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& FileInputStream::operator>>(uint& p_unValue)
{
	size_t unBytesRead = Read(&p_unValue, sizeof(p_unValue));
	if (unBytesRead < sizeof(p_unValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& FileInputStream::operator>>(long& p_lValue)
{
	size_t unBytesRead = Read(&p_lValue, sizeof(p_lValue));
	if (unBytesRead < sizeof(p_lValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& FileInputStream::operator>>(ulong& p_ulValue)
{
	size_t unBytesRead = Read(&p_ulValue, sizeof(p_ulValue));
	if (unBytesRead < sizeof(p_ulValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& FileInputStream::operator>>(long long& p_llValue)
{
	size_t unBytesRead = Read(&p_llValue, sizeof(p_llValue));
	if (unBytesRead < sizeof(p_llValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& FileInputStream::operator>>(float& p_fValue)
{
	size_t unBytesRead = Read(&p_fValue, sizeof(p_fValue));
	if (unBytesRead < sizeof(p_fValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& FileInputStream::operator>>(double& p_dValue)
{
	size_t unBytesRead = Read(&p_dValue, sizeof(p_dValue));
	if (unBytesRead < sizeof(p_dValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& FileInputStream::operator>>(String& p_strValue)
{
	p_strValue.Clear();
	size_t unLength;
	*this >> unLength;

	char c;
	while (unLength-- > 0)
	{
		if (Read(&c, 1) == 0)
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		p_strValue += c;
	}
	return *this;
}
