#include "LookaheadInputStream.h"

#include <string.h>

#include "ArrayList.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;

Meson_Common_IO_BEGIN

struct LookaheadInputStreamState
{
	InputStreamPtr InputStream;
	TArrayList<byte> Buffer;

	LookaheadInputStreamState(InputStreamPtr p_pInputStream)
		: InputStream(p_pInputStream)
		, Buffer()
	{
	}

	void FillBuffer(size_t p_unMinSize)
	{
		byte by;
		while (Buffer.Size() < p_unMinSize && !InputStream->IsEndOfStream())
		{
			InputStream->Read(&by, 1);
			Buffer.Add(by);
		}
	}
};

Meson_Common_IO_END

LookaheadInputStream::LookaheadInputStream(
	InputStreamPtr p_pInputStream)
	: m_pLookaheadInputStreamState(NULL)
{
	m_pLookaheadInputStreamState
		= new LookaheadInputStreamState(p_pInputStream);
}

LookaheadInputStream::~LookaheadInputStream(void)
{
	delete m_pLookaheadInputStreamState;
}

bool LookaheadInputStream::IsOpen(void) const
{
	return m_pLookaheadInputStreamState->InputStream->IsOpen();
}

bool LookaheadInputStream::IsEndOfStream(void) const
{
	return m_pLookaheadInputStreamState->Buffer.Size() == 0
		&& m_pLookaheadInputStreamState->InputStream->IsEndOfStream();
}

void LookaheadInputStream::Reopen(void)
{
	m_pLookaheadInputStreamState->InputStream->Reopen();
	m_pLookaheadInputStreamState->Buffer.Clear();
}

void LookaheadInputStream::Close(void)
{
	m_pLookaheadInputStreamState->InputStream->Close();
	m_pLookaheadInputStreamState->Buffer.Clear();
}

bool LookaheadInputStream::IsDataAvailable(void) const
{
	return m_pLookaheadInputStreamState->Buffer.Size() > 0
		|| m_pLookaheadInputStreamState->InputStream->IsDataAvailable();
}

size_t LookaheadInputStream::Read(void* p_pBuffer, size_t p_unLength)
{
	size_t unLength = Lookahead(p_pBuffer, p_unLength);
	m_pLookaheadInputStreamState->Buffer.RemoveRange(0, unLength);
	return unLength;
}

inline IInputStream& LookaheadInputStream::operator>>(char& p_cValue)
{
	size_t unBytesRead = Read(&p_cValue, sizeof(p_cValue));
	if (unBytesRead < sizeof(p_cValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& LookaheadInputStream::operator>>(bool& p_bValue)
{
	size_t unBytesRead = Read(&p_bValue, sizeof(p_bValue));
	if (unBytesRead < sizeof(p_bValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& LookaheadInputStream::operator>>(short& p_sValue)
{
	size_t unBytesRead = Read(&p_sValue, sizeof(p_sValue));
	if (unBytesRead < sizeof(p_sValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& LookaheadInputStream::operator>>(ushort& p_usValue)
{
	size_t unBytesRead = Read(&p_usValue, sizeof(p_usValue));
	if (unBytesRead < sizeof(p_usValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& LookaheadInputStream::operator>>(int& p_nValue)
{
	size_t unBytesRead = Read(&p_nValue, sizeof(p_nValue));
	if (unBytesRead < sizeof(p_nValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& LookaheadInputStream::operator>>(uint& p_unValue)
{
	size_t unBytesRead = Read(&p_unValue, sizeof(p_unValue));
	if (unBytesRead < sizeof(p_unValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& LookaheadInputStream::operator>>(long& p_lValue)
{
	size_t unBytesRead = Read(&p_lValue, sizeof(p_lValue));
	if (unBytesRead < sizeof(p_lValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& LookaheadInputStream::operator>>(ulong& p_ulValue)
{
	size_t unBytesRead = Read(&p_ulValue, sizeof(p_ulValue));
	if (unBytesRead < sizeof(p_ulValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& LookaheadInputStream::operator>>(long long& p_llValue)
{
	size_t unBytesRead = Read(&p_llValue, sizeof(p_llValue));
	if (unBytesRead < sizeof(p_llValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& LookaheadInputStream::operator>>(float& p_fValue)
{
	size_t unBytesRead = Read(&p_fValue, sizeof(p_fValue));
	if (unBytesRead < sizeof(p_fValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& LookaheadInputStream::operator>>(double& p_dValue)
{
	size_t unBytesRead = Read(&p_dValue, sizeof(p_dValue));
	if (unBytesRead < sizeof(p_dValue))
		throw new MesonException("Unexpected end of stream.",
			__FILE__, __LINE__);
	return *this;
}

inline IInputStream& LookaheadInputStream::operator>>(String& p_strValue)
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

size_t LookaheadInputStream::Lookahead(void* p_pBuffer, size_t p_unLength)
{
	m_pLookaheadInputStreamState->FillBuffer(p_unLength);
	size_t unBufferSize = m_pLookaheadInputStreamState->Buffer.Size();
	size_t unLength = p_unLength < unBufferSize ? p_unLength : unBufferSize;
	memcpy(p_pBuffer, m_pLookaheadInputStreamState->Buffer.GetBuffer(), unLength);
	return unLength;
}

void LookaheadInputStream::Pushback(const void* p_pBuffer, size_t p_unLength)
{
	byte byFiller = 0;
	for (size_t unIndex = 0; unIndex < p_unLength; unIndex++)
		m_pLookaheadInputStreamState->Buffer.Insert(0, byFiller);
	memcpy(m_pLookaheadInputStreamState->Buffer.GetBuffer(), p_pBuffer, p_unLength);
}
