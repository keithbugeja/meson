#include "CompressedInputStream.h"

#include "MesonException.h"
#include "DebugSupport.h"

#include <memory.h>

#ifndef NULL
	#define NULL 0
#endif

using namespace Meson::Common;
using namespace Meson::Common::Text;

Meson_Common_IO_BEGIN

	const ushort LZW_MIN_CODE_SIZE = 9;
	const ushort LZW_MAX_CODE_SIZE = 15;
	const ushort LZW_MAX_TABLE_SIZE = 1 << LZW_MAX_CODE_SIZE;
	const ushort LZW_INCREASE_CODE_SIZE = 256;
	const ushort LZW_RESET_DICTIONARY = 257;
	const ushort LZW_END_CODE = 258;

	ushort CompressedInputStream::ReadCode(void)
	{
		while (m_nCodeBitCount <= 16)
		{
			ushort usBits = 0;
			m_inputStream.Read(&usBits, sizeof(usBits));
			m_ulCodeStream |= 
				(ulong) usBits << (16 - m_nCodeBitCount);
			m_nCodeBitCount += 16;
		}
		ushort usCode
			= (ushort) (m_ulCodeStream >> (32 - m_usCodeSize));
		m_ulCodeStream <<= m_usCodeSize;
		m_nCodeBitCount -= m_usCodeSize;

		MESON_ASSERT(usCode <= LZW_MAX_TABLE_SIZE, "Read Code must be less than MAX")

		if (usCode == LZW_INCREASE_CODE_SIZE)
		{
			++m_usCodeSize;
			return ReadCode();
		}
		else
			return usCode;
	}

	CompressedInputStream::CompressedInputStream(
		IInputStream& p_inputStream)
		: m_inputStream(p_inputStream)
		, m_usCodeSize(LZW_MIN_CODE_SIZE)
		, m_listCodes(LZW_MAX_TABLE_SIZE)
		, m_bFirstCode(true)
		, m_usCode(0)
		, m_strBuffer((size_t) LZW_MAX_TABLE_SIZE)
		, m_ulCodeStream(0L)
		, m_nCodeBitCount(0)
	{
		for (int nCode = 0; nCode < 256; nCode++)
		{
			String strValue = "";
			strValue.Append((char) nCode);
			m_listCodes.Add(strValue);
		}

		// dummy entries
		m_listCodes.Add("_INCREASE_CODE_SIZE_");
		m_listCodes.Add("_RESET_DICTIONARY_");
		m_listCodes.Add("_END_CODE_");
	}

	CompressedInputStream::~CompressedInputStream(void)
	{
		if (m_inputStream.IsOpen())
			m_inputStream.Close();
	}

	bool CompressedInputStream::IsOpen(void) const
	{
		return m_inputStream.IsOpen();
	}

	bool CompressedInputStream::IsDataAvailable(void) const
	{
		return m_inputStream.IsDataAvailable();
	}

	bool CompressedInputStream::IsEndOfStream(void) const
	{
		return !IsDataAvailable();
	}

	void CompressedInputStream::Reopen(void)
	{
		m_inputStream.Reopen();

		m_usCodeSize = LZW_MIN_CODE_SIZE;

		m_listCodes.Clear();
		for (int nCode = 0; nCode < 256; nCode++)
		{
			String strValue = "";
			strValue.Append((char) nCode);
			m_listCodes.Add(strValue);
		}

		// dummy entries
		m_listCodes.Add("_INCREASE_CODE_SIZE_");
		m_listCodes.Add("_RESET_DICTIONARY_");
		m_listCodes.Add("_END_CODE_");

		m_bFirstCode = true;
		m_usCode = 0;
		m_strBuffer = "";
		m_ulCodeStream = 0L;
		m_nCodeBitCount = 0;
	}

	void CompressedInputStream::Close(void)
	{
		m_inputStream.Close();
	}

	size_t CompressedInputStream::Read(void* p_pBuffer, size_t p_unLength)
	{
		if (p_unLength == 0) return 0;

		if (m_bFirstCode)
		{
			m_usCode = ReadCode();

			if (m_usCode == LZW_END_CODE) return 0;

			m_strBuffer = m_listCodes[m_usCode];

			m_bFirstCode = false;
		}		

		String strPrefix = "";
		strPrefix.EnsureBufferSize(LZW_MAX_TABLE_SIZE);

		while ((m_strBuffer.Length() < p_unLength))
		{
			ushort usPrevCode = m_usCode;

			m_usCode = ReadCode();

			if (m_usCode == LZW_END_CODE) break;

			if (m_usCode == LZW_RESET_DICTIONARY)
			{
				// dictionary reset code
				m_usCodeSize = LZW_MIN_CODE_SIZE;

				m_listCodes.Clear();
				for (int nCode = 0; nCode < 256; nCode++)
				{
					String strValue = "";
					strValue.Append((char) nCode);
					m_listCodes.Add(strValue);
				}

				// dummy entries
				m_listCodes.Add("_INCREASE_CODE_SIZE_");
				m_listCodes.Add("_RESET_DICTIONARY_");
				m_listCodes.Add("_END_CODE_");

				// get next code
				m_usCode = ReadCode();
				m_strBuffer += m_listCodes[m_usCode];
				continue;
			}

			strPrefix.Clear();
			if (m_usCode < m_listCodes.Size())
			{
				m_strBuffer += m_listCodes[m_usCode];

				if (m_listCodes.Size() < LZW_MAX_TABLE_SIZE)
				{
					strPrefix = m_listCodes[usPrevCode];
					char chFirst = m_listCodes[m_usCode][0];
					m_listCodes.Add(strPrefix + chFirst);
				}
			}
			else
			{
				strPrefix =  m_listCodes[usPrevCode];
				char chFirst = strPrefix[0];
				String strNew = strPrefix + chFirst;
				m_strBuffer += strNew;

				if (m_listCodes.Size() < LZW_MAX_TABLE_SIZE)
					m_listCodes.Add(strNew);
			}
		}

		size_t unCount = p_unLength;
		if (unCount > m_strBuffer.Length())
			unCount = m_strBuffer.Length();
		memcpy(p_pBuffer, m_strBuffer.GetBuffer(), unCount);
		m_strBuffer = m_strBuffer.Substring(unCount, m_strBuffer.Length() - unCount);

		return unCount;
	}

	inline IInputStream& CompressedInputStream::operator>>(char& p_cValue)
	{
		size_t unBytesRead = Read(&p_cValue, sizeof(p_cValue));
		if (unBytesRead < sizeof(p_cValue))
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		return *this;
	}

	inline IInputStream& CompressedInputStream::operator>>(bool& p_bValue)
	{
		size_t unBytesRead = Read(&p_bValue, sizeof(p_bValue));
		if (unBytesRead < sizeof(p_bValue))
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		return *this;
	}

	inline IInputStream& CompressedInputStream::operator>>(short& p_sValue)
	{
		size_t unBytesRead = Read(&p_sValue, sizeof(p_sValue));
		if (unBytesRead < sizeof(p_sValue))
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		return *this;
	}

	inline IInputStream& CompressedInputStream::operator>>(ushort& p_usValue)
	{
		size_t unBytesRead = Read(&p_usValue, sizeof(p_usValue));
		if (unBytesRead < sizeof(p_usValue))
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		return *this;
	}

	inline IInputStream& CompressedInputStream::operator>>(int& p_nValue)
	{
		size_t unBytesRead = Read(&p_nValue, sizeof(p_nValue));
		if (unBytesRead < sizeof(p_nValue))
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		return *this;
	}

	inline IInputStream& CompressedInputStream::operator>>(uint& p_unValue)
	{
		size_t unBytesRead = Read(&p_unValue, sizeof(p_unValue));
		if (unBytesRead < sizeof(p_unValue))
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		return *this;
	}

	inline IInputStream& CompressedInputStream::operator>>(long& p_lValue)
	{
		size_t unBytesRead = Read(&p_lValue, sizeof(p_lValue));
		if (unBytesRead < sizeof(p_lValue))
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		return *this;
	}

	inline IInputStream& CompressedInputStream::operator>>(ulong& p_ulValue)
	{
		size_t unBytesRead = Read(&p_ulValue, sizeof(p_ulValue));
		if (unBytesRead < sizeof(p_ulValue))
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		return *this;
	}

	inline IInputStream& CompressedInputStream::operator>>(long long& p_llValue)
	{
		size_t unBytesRead = Read(&p_llValue, sizeof(p_llValue));
		if (unBytesRead < sizeof(p_llValue))
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		return *this;
	}

	inline IInputStream& CompressedInputStream::operator>>(float& p_fValue)
	{
		size_t unBytesRead = Read(&p_fValue, sizeof(p_fValue));
		if (unBytesRead < sizeof(p_fValue))
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		return *this;
	}

	inline IInputStream& CompressedInputStream::operator>>(double& p_dValue)
	{
		size_t unBytesRead = Read(&p_dValue, sizeof(p_dValue));
		if (unBytesRead < sizeof(p_dValue))
			throw new MesonException("Unexpected end of stream.",
				__FILE__, __LINE__);
		return *this;
	}

	inline IInputStream& CompressedInputStream::operator>>(String& p_strValue)
	{
		p_strValue.Clear();
		size_t unLength = 0;
		Read(&unLength, sizeof(unLength));

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

Meson_Common_IO_END
