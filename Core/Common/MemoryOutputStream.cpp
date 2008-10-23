#include "MemoryOutputStream.h"

#include "MesonException.h"
#include <memory.h>

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::IO;

Meson_Common_IO_BEGIN

struct MemoryOutputStreamState
{
	char *m_pBuffer;
	size_t m_unLength;
	char *m_pPosition;
	bool m_bOpen;
};

Meson_Common_IO_END

MemoryOutputStream::MemoryOutputStream(void)
	: m_pMemoryOutputStreamState(NULL)
{
	const size_t INITIAL_SIZE = 4096;
	m_pMemoryOutputStreamState = new MemoryOutputStreamState();
	m_pMemoryOutputStreamState->m_pBuffer = new char[INITIAL_SIZE];
	m_pMemoryOutputStreamState->m_pPosition = m_pMemoryOutputStreamState->m_pBuffer;
	m_pMemoryOutputStreamState->m_unLength = INITIAL_SIZE;
	m_pMemoryOutputStreamState->m_bOpen = true;
}

MemoryOutputStream::~MemoryOutputStream(void)
{
	delete m_pMemoryOutputStreamState->m_pBuffer;
	delete m_pMemoryOutputStreamState;
}

inline bool MemoryOutputStream::IsOpen(void) const
{
	return m_pMemoryOutputStreamState->m_bOpen;
}

inline bool MemoryOutputStream::IsEndOfStream(void) const
{
	return false;
}

void MemoryOutputStream::Reopen(void)
{
	m_pMemoryOutputStreamState->m_pPosition = m_pMemoryOutputStreamState->m_pBuffer;
	m_pMemoryOutputStreamState->m_bOpen = true;
}

void MemoryOutputStream::Close(void)
{
	if (!m_pMemoryOutputStreamState->m_bOpen)
		throw new MesonException(
			"The file output stream is closed.", __FILE__, __LINE__);

	m_pMemoryOutputStreamState->m_bOpen = false;
}

size_t MemoryOutputStream::Write(void* p_pBuffer, size_t p_unLength)
{
	if (!m_pMemoryOutputStreamState->m_bOpen)
		throw new MesonException(
			"The file output stream is closed.", __FILE__, __LINE__);

	size_t unOffset = m_pMemoryOutputStreamState->m_pPosition
		- m_pMemoryOutputStreamState->m_pBuffer;
	size_t unRemaining = m_pMemoryOutputStreamState->m_unLength - unOffset;

	if (unRemaining < p_unLength)
	{
		size_t unNewLength = m_pMemoryOutputStreamState->m_unLength;
		while (unNewLength - unOffset < p_unLength)
			unNewLength *= 2;

		char *pNewBuffer = new char[unNewLength];
		if (pNewBuffer == NULL)
			throw new MesonException(
				"Unable to extend memory output stream buffer",
				__FILE__, __LINE__);

		memcpy(pNewBuffer, m_pMemoryOutputStreamState->m_pBuffer, unOffset);
		delete m_pMemoryOutputStreamState->m_pBuffer;
		m_pMemoryOutputStreamState->m_pBuffer = pNewBuffer;
		m_pMemoryOutputStreamState->m_unLength = unNewLength;
		m_pMemoryOutputStreamState->m_pPosition = pNewBuffer + unOffset;
	}

	memcpy(m_pMemoryOutputStreamState->m_pPosition, p_pBuffer, p_unLength);

	m_pMemoryOutputStreamState->m_pPosition += p_unLength;

	return p_unLength;
}

void MemoryOutputStream::Flush(void)
{
	if (!m_pMemoryOutputStreamState->m_bOpen)
		throw new MesonException(
			"The file output stream is closed.", __FILE__, __LINE__);
}

inline IOutputStream& MemoryOutputStream::operator<<(char p_cValue)
{
	Write(&p_cValue, sizeof(p_cValue));
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(bool p_bValue)
{
	char p_cValue = (char) p_bValue;
	Write(&p_cValue, sizeof(p_cValue));
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(short p_sValue)
{
	Write(&p_sValue, sizeof(p_sValue));
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(ushort p_usValue)
{
	Write(&p_usValue, sizeof(p_usValue));
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(int p_nValue)
{
	Write(&p_nValue, sizeof(p_nValue));
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(uint p_unValue)
{
	Write(&p_unValue, sizeof(p_unValue));
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(long p_lValue)
{
	Write(&p_lValue, sizeof(p_lValue));
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(ulong p_ulValue)
{
	Write(&p_ulValue, sizeof(p_ulValue));
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(long long p_llValue)
{
	Write(&p_llValue, sizeof(p_llValue));
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(float p_fValue)
{
	Write(&p_fValue, sizeof(p_fValue));
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(double p_dValue)
{
	Write(&p_dValue, sizeof(p_dValue));
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(const char* p_pszValue)
{
	size_t unLength = 1;
	const char *pChar = p_pszValue;
	while (*pChar++ != 0) ++unLength;
	Write((void *)p_pszValue, unLength);
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(const String& p_strValue)
{
	*this << (uint) p_strValue.Length();
	const char *pBuffer = ((String &) p_strValue).GetBuffer();
	Write((void *) pBuffer, p_strValue.Length());
	return *this;
}

inline IOutputStream& MemoryOutputStream::operator<<(
	IInputStream &p_inputStream)
{
	const size_t BUFFER_LENGTH = 4096;
	char *pBuffer[BUFFER_LENGTH];
	while (!p_inputStream.IsEndOfStream())
	{
		size_t unBytesRead = p_inputStream.Read(pBuffer, BUFFER_LENGTH);
		size_t unBytesWritten = Write(pBuffer, unBytesRead);
		if (unBytesWritten == 0) break;
	}
	return *this;
}

const void* MemoryOutputStream::GetBuffer(void) const
{
	return m_pMemoryOutputStreamState->m_pBuffer;
}

size_t MemoryOutputStream::GetBufferLength(void) const
{
	return m_pMemoryOutputStreamState->m_pPosition - m_pMemoryOutputStreamState->m_pBuffer;
}
