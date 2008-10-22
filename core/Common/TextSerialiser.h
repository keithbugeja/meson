//----------------------------------------------------------------------------------------------
//	Meson::Common::Serialisation::TextSerialiser
//	ISerialiser implementation for plain-text files
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	25/08/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "DebugSupport.h"
#include "MesonException.h"
#include "Types.h"

#include "Serialiser.h"

Meson_Common_Serialisation_BEGIN

class TextSerialiser : public ISerialiser
{
private:
	void ReadTextBlock( Meson::Common::IO::IInputStream& p_inputStream, Meson::Common::Text::String& p_strBlock );
	
	char SkipWhiteSpace( Meson::Common::IO::IInputStream& p_inputStream );
	char SkipControlCharacters( Meson::Common::IO::IInputStream& p_inputStream );
	
	bool IsAlphaNumeric( char p_cSymbol );
	bool IsNumber( char p_cSymbol );
	bool IsLetter( char p_cSymbol );
	bool IsNull(char p_cSymbol);
	bool IsTab(char p_cSymbol);
	bool IsNewLine(char p_cSymbol);
	bool IsCarriageReturn(char p_cSymbol);
	bool IsWhiteSpace(char p_cSymbol);
	bool IsEndOfLine(char p_cSymbol);

	long double ParseReal( Meson::Common::Text::String p_strBlock );
	long long ParseSignedInteger( Meson::Common::Text::String p_strBlock );
	qword ParseUnsignedInteger( Meson::Common::Text::String p_strBlock );

	Meson::Common::Text::String UnsignedIntegerToString( qword p_qwValue );
	Meson::Common::Text::String SignedIntegerToString( long long p_llValue );
	Meson::Common::Text::String RealToString( long double p_ldfValue );

public:
	char ReadChar( Meson::Common::IO::IInputStream& p_inputStream );
	short ReadShort( Meson::Common::IO::IInputStream& p_inputStream );
	long ReadLong( Meson::Common::IO::IInputStream& p_inputStream );
	long long ReadLongLong( Meson::Common::IO::IInputStream& p_inputStream );
	float ReadFloat( Meson::Common::IO::IInputStream& p_inputStream );
	double ReadDouble( Meson::Common::IO::IInputStream& p_inputStream );
	long double ReadLongDouble( Meson::Common::IO::IInputStream& p_inputStream );

	byte ReadByte( Meson::Common::IO::IInputStream& p_inputStream );
	word ReadWord( Meson::Common::IO::IInputStream& p_inputStream );
	dword ReadDoubleWord( Meson::Common::IO::IInputStream& p_inputStream );
	qword ReadQuadWord( Meson::Common::IO::IInputStream& p_inputStream );

	int ReadCharArray( Meson::Common::IO::IInputStream& p_inputStream, char* p_pcBuffer, int p_nCount );
	int ReadShortArray( Meson::Common::IO::IInputStream& p_inputStream, short* p_psBuffer, int p_nCount );
	int ReadLongArray( Meson::Common::IO::IInputStream& p_inputStream, long* p_plBuffer, int p_nCount );
	int ReadLongLongArray( Meson::Common::IO::IInputStream& p_inputStream, long long* p_pllBuffer, int p_nCount );
	int ReadFloatArray( Meson::Common::IO::IInputStream& p_inputStream, float* p_pfBuffer, int p_nCount );
	int ReadDoubleArray( Meson::Common::IO::IInputStream& p_inputStream, double* p_pdfBuffer, int p_nCount );
	int ReadLongDoubleArray( Meson::Common::IO::IInputStream& p_inputStream, long double* p_pldfBuffer, int p_nCount );

	int ReadByteArray( Meson::Common::IO::IInputStream& p_inputStream, byte* p_pbyBuffer, int p_nCount );
	int ReadWordArray( Meson::Common::IO::IInputStream& p_inputStream, word* p_pwBuffer, int p_nCount );
	int ReadDoubleWordArray( Meson::Common::IO::IInputStream& p_inputStream, dword* p_pdwBuffer, int p_nCount );
	int ReadQuadWordArray( Meson::Common::IO::IInputStream& p_inputStream, qword* p_pqwBuffer, int p_nCount );

	void ReadString( Meson::Common::IO::IInputStream& p_inputStream, Meson::Common::Text::String& p_strInput );
	char ReadString( Meson::Common::IO::IInputStream& p_inputStream );

	void WriteChar( Meson::Common::IO::IOutputStream& p_outputStream, char p_cValue );
	void WriteShort( Meson::Common::IO::IOutputStream& p_outputStream, short p_sValue );
	void WriteLong( Meson::Common::IO::IOutputStream& p_outputStream, long p_lValue );
	void WriteLongLong( Meson::Common::IO::IOutputStream& p_outputStream, long long p_llValue );
	void WriteFloat( Meson::Common::IO::IOutputStream& p_outputStream, float p_fValue );
	void WriteDouble( Meson::Common::IO::IOutputStream& p_outputStream, double p_dfValue );
	void WriteLongDouble( Meson::Common::IO::IOutputStream& p_outputStream, long double p_ldfValue );

	void WriteByte( Meson::Common::IO::IOutputStream& p_outputStream, byte p_byValue );
	void WriteWord( Meson::Common::IO::IOutputStream& p_outputStream, word p_wValue );
	void WriteDoubleWord( Meson::Common::IO::IOutputStream& p_outputStream, dword p_dwValue );
	void WriteQuadWord( Meson::Common::IO::IOutputStream& p_outputStream, qword p_qwValue );

	void WriteCharArray( Meson::Common::IO::IOutputStream& p_outputStream, char* p_pcBuffer, int p_nCount );
	void WriteShortArray( Meson::Common::IO::IOutputStream& p_outputStream, short* p_psBuffer, int p_nCount );
	void WriteLongArray( Meson::Common::IO::IOutputStream& p_outputStream, long* p_plBuffer, int p_nCount );
	void WriteLongLongArray( Meson::Common::IO::IOutputStream& p_outputStream, long long* p_pllBuffer, int p_nCount );
	void WriteFloatArray( Meson::Common::IO::IOutputStream& p_outputStream, float* p_pfBuffer, int p_nCount );
	void WriteDoubleArray( Meson::Common::IO::IOutputStream& p_outputStream, double* p_pdfBuffer, int p_nCount );
	void WriteLongDoubleArray( Meson::Common::IO::IOutputStream& p_outputStream, long double* p_pldfBuffer, int p_nCount );

	void WriteByteArray( Meson::Common::IO::IOutputStream& p_outputStream, byte* p_pbyBuffer, int p_nCount );
	void WriteWordArray( Meson::Common::IO::IOutputStream& p_outputStream, word* p_pwBuffer, int p_nCount );
	void WriteDoubleWordArray( Meson::Common::IO::IOutputStream& p_outputStream, dword* p_pdwBuffer, int p_nCount );
	void WriteQuadWordArray( Meson::Common::IO::IOutputStream& p_outputStream, qword* p_pqwBuffer, int p_nCount );

	void WriteString( Meson::Common::IO::IOutputStream& p_outputStream, Meson::Common::Text::String& p_strOutput );
	void WriteString( Meson::Common::IO::IOutputStream& p_outputStream, char p_strCharacter );
};

Meson_Common_Serialisation_END
