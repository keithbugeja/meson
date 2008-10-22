// Note: adapted from: Copyright (c) 1989 Mark R. Nelson


#include "CompressedOutputStream.h"

#include "MesonException.h"
#include "DebugSupport.h"

#include <memory.h>

#ifndef NULL
	#define NULL 0
#endif

using namespace Meson::Common::Text;

Meson_Common_IO_BEGIN

	const ushort LZW_MIN_CODE_SIZE = 9;
	const ushort LZW_MAX_CODE_SIZE = 15;
	const ushort LZW_MAX_TABLE_SIZE = 1 << LZW_MAX_CODE_SIZE;
	const ushort LZW_INCREASE_CODE_SIZE = 256;
	const ushort LZW_RESET_DICTIONARY = 257;
	const ushort LZW_END_CODE = 258;
	const ushort LZW_FIRST_CODE = 259;

	void CompressedOutputStream::WriteCode(ushort p_usCode)
	{
		MESON_ASSERT(p_usCode < LZW_MAX_TABLE_SIZE, "Write Code must be less than MAX")

		if (p_usCode >= 1 << m_usCodeSize)
		{
			WriteCode(LZW_INCREASE_CODE_SIZE);
			++m_usCodeSize;
		}

		m_ulCodeStream |= (ulong) p_usCode << (32 - m_usCodeSize - m_nCodeBitCount);
		m_nCodeBitCount += m_usCodeSize;
		while (m_nCodeBitCount >= 16)
		{
			ushort usBits = (ushort) (m_ulCodeStream >> 16);
			m_outputStream.Write(&usBits, sizeof(usBits));
			m_ulCodeStream <<= 16;
			m_nCodeBitCount -= 16;
		}
	}

	CompressedOutputStream::CompressedOutputStream(IOutputStream& p_outputStream)
		: m_outputStream(p_outputStream)
		, m_usCodeSize(LZW_MIN_CODE_SIZE)
		, m_strWorkString((size_t) LZW_MAX_TABLE_SIZE)
		, m_mapCodes(LZW_MAX_TABLE_SIZE)
		, m_usNextCode(LZW_FIRST_CODE)
		, m_ulCodeStream(0L)
		, m_nCodeBitCount(0)
	{
		for (int nCode = 0; nCode < 256; nCode++)
		{
			String strKey = "";
			strKey.Append((char) nCode);
			m_mapCodes.Insert(strKey, nCode);
		}
	}

	CompressedOutputStream::~CompressedOutputStream(void)
	{
		if (m_outputStream.IsOpen())
			m_outputStream.Close();
	}

	inline bool CompressedOutputStream::IsOpen(void) const
	{
		return m_outputStream.IsOpen();
	}

	inline bool CompressedOutputStream::IsEndOfStream(void) const
	{
		return m_outputStream.IsEndOfStream();
	}

	void CompressedOutputStream::Reopen(void)
	{
		m_outputStream.Reopen();

		m_usCodeSize = LZW_MIN_CODE_SIZE;

		m_strWorkString = "";
		m_usNextCode = LZW_FIRST_CODE;

		m_ulCodeStream = 0L;
		m_nCodeBitCount = 0;

		m_mapCodes.Clear();
		for (int nCode = 0; nCode < 256; nCode++)
		{
			String strKey = "";
			strKey.Append((char) nCode);
			m_mapCodes.Insert(strKey, nCode);
		}
	}

	void CompressedOutputStream::Close(void)
	{
		// write code for last work string
		WriteCode(m_mapCodes.ValueAt(m_strWorkString));

		// write end code (ensure at least one stored)
		WriteCode(LZW_END_CODE);
		WriteCode(LZW_END_CODE);

		m_outputStream.Close();
	}

	size_t CompressedOutputStream::Write(void* p_pBuffer, size_t p_unLength)
	{
		if (p_unLength == 0) return 0;

		size_t unCount = p_unLength;
		char* pBuffer = (char*) p_pBuffer;

		String strNewString;
		strNewString.EnsureBufferSize(256);

		while (unCount-- > 0)
		{
			// get next character from input
			char chNextChar = *pBuffer++;

			// compute concatenation for work string with char
			strNewString = m_strWorkString + chNextChar;

			// concatenated string already in code table?
			if (m_mapCodes.ContainsKey(strNewString))
				// if yes, work string becomes concatenated string
				m_strWorkString += chNextChar;	
			else
			{
				// otherwise, write out code for work string
				WriteCode(m_mapCodes.ValueAt(m_strWorkString));

				// add concatenated string to code table
				if (m_usNextCode < LZW_MAX_TABLE_SIZE)
					m_mapCodes.Insert(strNewString, m_usNextCode++);
				else
				{
					// send reset code
					WriteCode(LZW_RESET_DICTIONARY);

					// request dictionary reset if full
					m_usCodeSize = LZW_MIN_CODE_SIZE;

					m_strWorkString = "";
					m_usNextCode = LZW_FIRST_CODE;

					//m_ulCodeStream = 0L;
					//m_nCodeBitCount = 0;

					m_mapCodes.Clear();
					for (int nCode = 0; nCode < 256; nCode++)
					{
						String strKey = "";
						strKey.Append((char) nCode);
						m_mapCodes.Insert(strKey, nCode);
					}
				}

				// work string becomes current char
				m_strWorkString = chNextChar;
			}
		}

		return p_unLength;
	}

	void CompressedOutputStream::Flush(void)
	{
		m_outputStream.Flush();
	}

	inline IOutputStream& CompressedOutputStream::operator<<(char p_cValue)
	{
		Write(&p_cValue, sizeof(p_cValue));
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(bool p_bValue)
	{
		char p_cValue = (char) p_bValue;
		Write(&p_cValue, sizeof(p_cValue));
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(short p_sValue)
	{
		Write(&p_sValue, sizeof(p_sValue));
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(ushort p_usValue)
	{
		Write(&p_usValue, sizeof(p_usValue));
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(int p_nValue)
	{
		Write(&p_nValue, sizeof(p_nValue));
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(uint p_unValue)
	{
		Write(&p_unValue, sizeof(p_unValue));
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(long p_lValue)
	{
		Write(&p_lValue, sizeof(p_lValue));
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(ulong p_ulValue)
	{
		Write(&p_ulValue, sizeof(p_ulValue));
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(long long p_llValue)
	{
		Write(&p_llValue, sizeof(p_llValue));
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(float p_fValue)
	{
		Write(&p_fValue, sizeof(p_fValue));
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(double p_dValue)
	{
		Write(&p_dValue, sizeof(p_dValue));
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(const char *p_pszValue)
	{
		size_t unLength = 1;
		const char *pChar = p_pszValue;
		while (*pChar++ != 0) ++unLength;
		Write((void *)p_pszValue, unLength);
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(const String &p_strValue)
	{
		*this << (uint) p_strValue.Length();
		const char *pBuffer = ((String &) p_strValue).GetBuffer();
		Write((void *) pBuffer, p_strValue.Length());
		return *this;
	}

	inline IOutputStream& CompressedOutputStream::operator<<(
		IInputStream &p_inputStream)
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

Meson_Common_IO_END
