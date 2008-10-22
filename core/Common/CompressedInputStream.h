#pragma once

#include "InputStream.h"
#include "String8.h"
#include "ArrayList.h"

Meson_Common_IO_BEGIN

	class CompressedInputStream :
		public IInputStream
	{
	private: // private variables
		IInputStream& m_inputStream;
		ushort m_usCodeSize;
		Meson::Common::Collections::TArrayList<Meson::Common::Text::String> m_listCodes;
		bool m_bFirstCode;
		ushort m_usCode;
		Meson::Common::Text::String m_strBuffer;
		ulong m_ulCodeStream;
		int m_nCodeBitCount;

	private: // private methods
		ushort ReadCode(void);

	public: // public methods
		CompressedInputStream(IInputStream& p_inputStream);
		~CompressedInputStream(void);
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
		IInputStream& operator>>(long& p_nValue);
		IInputStream& operator>>(ulong& p_unValue);
		IInputStream& operator>>(long long& p_llValue);
		IInputStream& operator>>(float& p_fValue);
		IInputStream& operator>>(double& p_dValue);
		IInputStream& operator>>(Meson::Common::Text::String& p_strValue);
	};

Meson_Common_IO_END
