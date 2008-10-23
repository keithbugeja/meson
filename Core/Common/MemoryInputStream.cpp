#include "MemoryInputStream.h"

#include "MesonException.h"

#include <memory.h>

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::IO;

Meson_Common_IO_BEGIN

struct MemoryInputStreamState
{
	char *m_pBuffer;
	char *m_pPosition;
	size_t m_unLength;
	bool m_bOpen;
};

Meson_Common_IO_END

MemoryInputStream::MemoryInputStream(
	const void *p_pBuffer, size_t p_unLength)
	: m_pMemoryInputStreamState(NULL)
{
	m_pMemoryInputStreamState = new MemoryInputStreamState();
	m_pMemoryInputStreamState->m_pBuffer = (char *)p_pBuffer;
	m_pMemoryInputStreamState->m_pPosition = (char *)p_pBuffer;
	m_pMemoryInputStreamState->m_unLength = p_unLength;
	m_pMemoryInputStreamState->m_bOpen = true;
}

MemoryInputStream::~MemoryInputStream(void)
{
	delete m_pMemoryInputStreamState;
}

bool MemoryInputStream::IsOpen(void) const
{
	return m_pMemoryInputStreamState->m_bOpen;
}

bool MemoryInputStream::IsDataAvailable(void) const
{
	if (!m_pMemoryInputStreamState->m_bOpen)
		throw new MesonException(
			"The memory input stream is closed.", __FILE__, __LINE__);

	size_t unOffset = m_pMemoryInputStreamState->m_pPosition
		- m_pMemoryInputStreamState->m_pBuffer;

	return unOffset < m_pMemoryInputStreamState->m_unLength;
}

bool MemoryInputStream::IsEndOfStream(void) const
{
	return !IsDataAvailable();
}

void MemoryInputStream::Reopen(void)
{
	m_pMemoryInputStreamState->m_pPosition = m_pMemoryInputStreamState->m_pBuffer;
	m_pMemoryInputStreamState->m_bOpen = true;
}

void MemoryInputStream::Close(void)
{
	if (!m_pMemoryInputStreamState->m_bOpen)
		throw new MesonException(
			"The file input stream is closed.", __FILE__, __LINE__);

	m_pMemoryInputStreamState->m_pPosition = m_pMemoryInputStreamState->m_pBuffer;
	m_pMemoryInputStreamState->m_bOpen = false;
}

size_t MemoryInputStream::Read(void* pBuffer, size_t unLength)
{
	if (!m_pMemoryInputStreamState->m_bOpen)
		throw new MesonException(
			"The file input stream is closed.", __FILE__, __LINE__);

	size_t unOffset = m_pMemoryInputStreamState->m_pPosition
		- m_pMemoryInputStreamState->m_pBuffer;
	size_t unRemaining = m_pMemoryInputStreamState->m_unLength - unOffset;
	if (unLength > unRemaining)
		unLength = unRemaining;

	memcpy(pBuffer, m_pMemoryInputStreamState->m_pPosition, unLength);
	m_pMemoryInputStreamState->m_pPosition += unLength;

	return unLength;
}

inline IInputStream& MemoryInputStream::operator>>(char &p_cValue)
{
	size_t unBytesRead = Read(&p_cValue, sizeof(p_cValue));
	if (unBytesRead < sizeof(p_cValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& MemoryInputStream::operator>>(bool &p_bValue)
{
	size_t unBytesRead = Read(&p_bValue, sizeof(p_bValue));
	if (unBytesRead < sizeof(p_bValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& MemoryInputStream::operator>>(short& p_sValue)
{
	size_t unBytesRead = Read(&p_sValue, sizeof(p_sValue));
	if (unBytesRead < sizeof(p_sValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& MemoryInputStream::operator>>(ushort& p_usValue)
{
	size_t unBytesRead = Read(&p_usValue, sizeof(p_usValue));
	if (unBytesRead < sizeof(p_usValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& MemoryInputStream::operator>>(int& p_nValue)
{
	size_t unBytesRead = Read(&p_nValue, sizeof(p_nValue));
	if (unBytesRead < sizeof(p_nValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& MemoryInputStream::operator>>(uint& p_unValue)
{
	size_t unBytesRead = Read(&p_unValue, sizeof(p_unValue));
	if (unBytesRead < sizeof(p_unValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& MemoryInputStream::operator>>(long& p_lValue)
{
	size_t unBytesRead = Read(&p_lValue, sizeof(p_lValue));
	if (unBytesRead < sizeof(p_lValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& MemoryInputStream::operator>>(ulong& p_ulValue)
{
	size_t unBytesRead = Read(&p_ulValue, sizeof(p_ulValue));
	if (unBytesRead < sizeof(p_ulValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& MemoryInputStream::operator>>(long long& p_llValue)
{
	size_t unBytesRead = Read(&p_llValue, sizeof(p_llValue));
	if (unBytesRead < sizeof(p_llValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& MemoryInputStream::operator>>(float& p_fValue)
{
	size_t unBytesRead = Read(&p_fValue, sizeof(p_fValue));
	if (unBytesRead < sizeof(p_fValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& MemoryInputStream::operator>>(double& p_dValue)
{
	size_t unBytesRead = Read(&p_dValue, sizeof(p_dValue));
	if (unBytesRead < sizeof(p_dValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& MemoryInputStream::operator>>(String& p_strValue)
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
