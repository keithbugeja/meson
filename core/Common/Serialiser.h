//----------------------------------------------------------------------------------------------
//	Meson::Common::Serialisation::ISerialiser
//	Interface class for serialisers
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	25/08/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Definitions.h"
#include "Types.h"

#include "DebugSupport.h"
#include "MesonException.h"

#include "InputStream.h"
#include "OutputStream.h"

Meson_Common_Serialisation_BEGIN

class ISerialiser
{
public:
	virtual char ReadChar( Meson::Common::IO::IInputStream& p_inputStream ) = 0;
	virtual short ReadShort( Meson::Common::IO::IInputStream& p_inputStream ) = 0;
	virtual long ReadLong( Meson::Common::IO::IInputStream& p_inputStream ) = 0;
	virtual long long ReadLongLong( Meson::Common::IO::IInputStream& p_inputStream ) = 0;
	virtual float ReadFloat( Meson::Common::IO::IInputStream& p_inputStream ) = 0;
	virtual double ReadDouble( Meson::Common::IO::IInputStream& p_inputStream ) = 0;
	virtual long double ReadLongDouble( Meson::Common::IO::IInputStream& p_inputStream ) = 0;

	virtual byte ReadByte( Meson::Common::IO::IInputStream& p_inputStream ) = 0;
	virtual word ReadWord( Meson::Common::IO::IInputStream& p_inputStream ) = 0;
	virtual dword ReadDoubleWord( Meson::Common::IO::IInputStream& p_inputStream ) = 0;
	virtual qword ReadQuadWord( Meson::Common::IO::IInputStream& p_inputStream ) = 0;

	virtual int ReadCharArray( Meson::Common::IO::IInputStream& p_inputStream, char* p_pcBuffer, int p_nCount ) = 0;
	virtual int ReadShortArray( Meson::Common::IO::IInputStream& p_inputStream, short* p_psBuffer, int p_nCount ) = 0;
	virtual int ReadLongArray( Meson::Common::IO::IInputStream& p_inputStream, long* p_plBuffer, int p_nCount ) = 0;
	virtual int ReadLongLongArray( Meson::Common::IO::IInputStream& p_inputStream, long long* p_pllBuffer, int p_nCount ) = 0;
	virtual int ReadFloatArray( Meson::Common::IO::IInputStream& p_inputStream, float* p_pfBuffer, int p_nCount ) = 0;
	virtual int ReadDoubleArray( Meson::Common::IO::IInputStream& p_inputStream, double* p_pdfBuffer, int p_nCount ) = 0;
	virtual int ReadLongDoubleArray( Meson::Common::IO::IInputStream& p_inputStream, long double* p_pldfBuffer, int p_nCount ) = 0;

	virtual int ReadByteArray( Meson::Common::IO::IInputStream& p_inputStream, byte* p_pbyBuffer, int p_nCount ) = 0;
	virtual int ReadWordArray( Meson::Common::IO::IInputStream& p_inputStream, word* p_pwBuffer, int p_nCount ) = 0;
	virtual int ReadDoubleWordArray( Meson::Common::IO::IInputStream& p_inputStream, dword* p_pdwBuffer, int p_nCount ) = 0;
	virtual int ReadQuadWordArray( Meson::Common::IO::IInputStream& p_inputStream, qword* p_pqwBuffer, int p_nCount ) = 0;

	virtual void ReadString( Meson::Common::IO::IInputStream& p_inputStream, Meson::Common::Text::String& p_strInput ) = 0;
	virtual char ReadString( Meson::Common::IO::IInputStream& p_inputStream ) = 0;

	virtual void WriteChar( Meson::Common::IO::IOutputStream& p_outputStream, char p_cValue ) = 0;
	virtual void WriteShort( Meson::Common::IO::IOutputStream& p_outputStream, short p_sValue ) = 0;
	virtual void WriteLong( Meson::Common::IO::IOutputStream& p_outputStream, long p_lValue ) = 0;
	virtual void WriteLongLong( Meson::Common::IO::IOutputStream& p_outputStream, long long p_llValue ) = 0;
	virtual void WriteFloat( Meson::Common::IO::IOutputStream& p_outputStream, float p_fValue ) = 0;
	virtual void WriteDouble( Meson::Common::IO::IOutputStream& p_outputStream, double p_dfValue ) = 0;
	virtual void WriteLongDouble( Meson::Common::IO::IOutputStream& p_outputStream, long double p_ldfValue ) = 0;

	virtual void WriteByte( Meson::Common::IO::IOutputStream& p_outputStream, byte p_byValue ) = 0;
	virtual void WriteWord( Meson::Common::IO::IOutputStream& p_outputStream, word p_wValue ) = 0;
	virtual void WriteDoubleWord( Meson::Common::IO::IOutputStream& p_outputStream, dword p_dwValue ) = 0;
	virtual void WriteQuadWord( Meson::Common::IO::IOutputStream& p_outputStream, qword p_qwValue ) = 0;

	virtual void WriteCharArray( Meson::Common::IO::IOutputStream& p_outputStream, char* p_pcBuffer, int p_nCount ) = 0;
	virtual void WriteShortArray( Meson::Common::IO::IOutputStream& p_outputStream, short* p_psBuffer, int p_nCount ) = 0;
	virtual void WriteLongArray( Meson::Common::IO::IOutputStream& p_outputStream, long* p_plBuffer, int p_nCount ) = 0;
	virtual void WriteLongLongArray( Meson::Common::IO::IOutputStream& p_outputStream, long long* p_pllBuffer, int p_nCount ) = 0;
	virtual void WriteFloatArray( Meson::Common::IO::IOutputStream& p_outputStream, float* p_pfBuffer, int p_nCount ) = 0;
	virtual void WriteDoubleArray( Meson::Common::IO::IOutputStream& p_outputStream, double* p_pdfBuffer, int p_nCount ) = 0;
	virtual void WriteLongDoubleArray( Meson::Common::IO::IOutputStream& p_outputStream, long double* p_pldfBuffer, int p_nCount ) = 0;

	virtual void WriteByteArray( Meson::Common::IO::IOutputStream& p_outputStream, byte* p_pbyBuffer, int p_nCount ) = 0;
	virtual void WriteWordArray( Meson::Common::IO::IOutputStream& p_outputStream, word* p_pwBuffer, int p_nCount ) = 0;
	virtual void WriteDoubleWordArray( Meson::Common::IO::IOutputStream& p_outputStream, dword* p_pdwBuffer, int p_nCount ) = 0;
	virtual void WriteQuadWordArray( Meson::Common::IO::IOutputStream& p_outputStream, qword* p_pqwBuffer, int p_nCount ) = 0;

	virtual void WriteString( Meson::Common::IO::IOutputStream& p_outputStream, Meson::Common::Text::String& p_strOutput ) = 0;
	virtual void WriteString( Meson::Common::IO::IOutputStream& p_outputStream, char p_strCharacter ) = 0;
};

Meson_Common_Serialisation_END

