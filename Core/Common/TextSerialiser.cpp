//----------------------------------------------------------------------------------------------
//	Meson::Common::Serialisation::TextSerialiser
//	ISerialiser implementation for plain-text files
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	25/08/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "Namespaces.h"
#include "TextSerialiser.h"
#include "Maths.h"

using namespace Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

#include <sstream>

Meson_Common_Serialisation_BEGIN

//----------------------------------------------------------------------------------------------
/// Tests whether the given character is either a number (0-9) or a letter (a-z,A-Z).
/// \param p_cSymbol Character to test
/// \returns True if alphanumeric, false otherwise
//----------------------------------------------------------------------------------------------
bool TextSerialiser::IsAlphaNumeric(char p_cSymbol)
{
	return IsLetter(p_cSymbol) || IsNumber(p_cSymbol);
}

//----------------------------------------------------------------------------------------------
/// Tests whether the given character is a letter (a-z,A-Z).
/// \param p_cSymbol Character to test
/// \returns True if a letter, false otherwise
//----------------------------------------------------------------------------------------------
bool TextSerialiser::IsLetter(char p_cSymbol)
{
	return (p_cSymbol >= 'a' && p_cSymbol <= 'z') || (p_cSymbol >= 'A' && p_cSymbol <= 'Z');
}

//----------------------------------------------------------------------------------------------
/// Tests whether the given character is a number (a-z,A-Z).
/// \param p_cSymbol Character to test
/// \returns True if a number, false otherwise
//----------------------------------------------------------------------------------------------
bool TextSerialiser::IsNumber(char p_cSymbol)
{
	return (p_cSymbol >= '0' && p_cSymbol <= '9');
}

//----------------------------------------------------------------------------------------------
/// Tests whether the given character is null (0x00).
/// \param p_cSymbol Character to test
/// \returns True if null, false otherwise
//----------------------------------------------------------------------------------------------
bool TextSerialiser::IsNull(char p_cSymbol)
{
	return (p_cSymbol == '\0');
}

//----------------------------------------------------------------------------------------------
/// Tests whether the given character is horizontal tab.
/// \param p_cSymbol Character to test
/// \returns True if a tab, false otherwise
//----------------------------------------------------------------------------------------------
bool TextSerialiser::IsTab(char p_cSymbol)
{
	return (p_cSymbol == '\t');
}

//----------------------------------------------------------------------------------------------
/// Tests whether the given character is a newline character.
/// \param p_cSymbol Character to test
/// \returns True if a newline, false otherwise
//----------------------------------------------------------------------------------------------
bool TextSerialiser::IsNewLine(char p_cSymbol)
{
	return (p_cSymbol == '\n'); 
}

//----------------------------------------------------------------------------------------------
/// Tests whether the given character is a carriage return character.
/// \param p_cSymbol Character to test
/// \returns True if a CR, false otherwise
//----------------------------------------------------------------------------------------------
bool TextSerialiser::IsCarriageReturn(char p_cSymbol)
{
	return (p_cSymbol == '\r'); 
}

//----------------------------------------------------------------------------------------------
/// Tests whether the given character is a whitespace character.
/// \param p_cSymbol Character to test
/// \returns True if a whitespace, false otherwise
//----------------------------------------------------------------------------------------------
bool TextSerialiser::IsWhiteSpace(char p_cSymbol)
{
	return (p_cSymbol == 0x20);
}

//----------------------------------------------------------------------------------------------
/// Tests whether the given character is a an end-of-line character (either \r or \n).
/// \param p_cSymbol Character to test
/// \returns True if end-of-line, false otherwise
//----------------------------------------------------------------------------------------------
bool TextSerialiser::IsEndOfLine(char p_cSymbol)
{
	return IsCarriageReturn(p_cSymbol) || IsNewLine(p_cSymbol);
}

//----------------------------------------------------------------------------------------------
/// Repositions the stream to a non-whitespace character. The stream is effectively
/// moved to the second-next non-whitespace character, while the first is returned
/// as an output from the function.
/// \param p_inputStream Input stream
/// \returns The first non-whitespace character encountered.
//----------------------------------------------------------------------------------------------
char TextSerialiser::SkipWhiteSpace(IInputStream &p_inputStream)
{
	char cSymbol = 0x20;

	while( p_inputStream.IsDataAvailable() && IsWhiteSpace(cSymbol) ) p_inputStream>>cSymbol;
	
	return cSymbol;
}

//----------------------------------------------------------------------------------------------
/// Repositions the stream to a non-control character. The stream is effectively
/// moved to the second-next non-control character, while the first is returned
/// as an output from the function.
/// \param p_inputStream Input stream
/// \returns The first non-control character encountered.
//----------------------------------------------------------------------------------------------
char TextSerialiser::SkipControlCharacters(IInputStream &p_inputStream)
{
	char cSymbol = 0x20;

	while( p_inputStream.IsDataAvailable() && ( IsWhiteSpace(cSymbol) || IsEndOfLine(cSymbol) || IsNull(cSymbol)  || IsTab(cSymbol)) ) p_inputStream>>cSymbol;
	
	return cSymbol;
}

//----------------------------------------------------------------------------------------------
/// Reads a string of text, stopping at whitespace or newline characters.
/// \param p_inputStream Input stream
/// \param p_strBlock String to which characters read are appended
//----------------------------------------------------------------------------------------------
void TextSerialiser::ReadTextBlock( IInputStream& p_inputStream, String& p_strBlock )
{
	char cNextCharacter = SkipControlCharacters(p_inputStream); 

	while( !IsWhiteSpace(cNextCharacter) && !IsEndOfLine(cNextCharacter) )
	{
		p_strBlock.Append( cNextCharacter );

		if ( p_inputStream.IsDataAvailable() ) 
			p_inputStream>>cNextCharacter;
		else
			break;
	}
}

//----------------------------------------------------------------------------------------------
/// Extracts the numeric value of a real number represented by the given string.
/// \param p_strTextBlock String containing textual representation of number
/// \returns Converted number
//----------------------------------------------------------------------------------------------
long double TextSerialiser::ParseReal( String p_strTextBlock )
{
	if ( p_strTextBlock.Length() == 0 )
		return 0;

	long double ldfValue = 0, ldfFractional = 0,
				ldfSign = (p_strTextBlock.GetCharAt(0) == '-' ? -1 : 1);

	char cCharacter;

	size_t nIdx = (ldfSign == 1) ? 0 : 1;

	// Whole part
	for (;nIdx < p_strTextBlock.Size(); nIdx++)
	{
		cCharacter = p_strTextBlock.GetCharAt(nIdx);

		if ( IsNumber(cCharacter) )
			ldfValue = ldfValue * 10 + (cCharacter - '0');
		else
			break;
	}

	// Fractional part
	if ( cCharacter == '.' )
	{
		long double ldfDivisor = 1;

		for (nIdx++; nIdx < p_strTextBlock.Size(); nIdx++)
		{
			cCharacter = p_strTextBlock.GetCharAt(nIdx);

			if ( IsNumber(cCharacter) )
			{
				ldfDivisor *= 10;
				ldfFractional += (long double)(cCharacter - '0') / ldfDivisor;
			}
			else
				break;
		}

		// Exponent
		if ( cCharacter == 'e' || cCharacter == 'E' )
		{
			String strExponent = p_strTextBlock.SubstringIndex( ++nIdx, p_strTextBlock.Size() );
			double dfExponent = (double) ParseSignedInteger( strExponent );
			return (ldfValue + ldfFractional) * ldfSign * TMaths<double>::Pow(10.0, dfExponent);
		}
	}

	return (ldfValue + ldfFractional) * ldfSign;
}

//----------------------------------------------------------------------------------------------
/// Extracts the numeric value of a signed integer string representation.
/// \param p_strTextBlock String containing textual representation of number
/// \returns Converted number
//----------------------------------------------------------------------------------------------
long long TextSerialiser::ParseSignedInteger( String p_strTextBlock )
{
	if ( p_strTextBlock.Length() == 0 )
		return 0;

	long long llValue = 0, 
			  llSign = (p_strTextBlock.GetCharAt(0) == '-' ? -1 : 1);

	char cCharacter;

	for (size_t nIdx = (llSign == 1) ? 0 : 1; nIdx < p_strTextBlock.Size(); nIdx++)
	{
		cCharacter = p_strTextBlock.GetCharAt(nIdx);

		if ( IsNumber(cCharacter) )
			llValue = llValue * 10 + (cCharacter - '0');
		else if ( cCharacter = '+' )
			continue;
		else
			break;
	}

	return llValue * llSign;
}

//----------------------------------------------------------------------------------------------
/// Extracts the numeric value of an unsigned integer string representation.
/// \param p_strTextBlock String containing textual representation of number
/// \returns Converted number
//----------------------------------------------------------------------------------------------
qword TextSerialiser::ParseUnsignedInteger( String p_strTextBlock )
{
	if ( p_strTextBlock.Length() == 0 )
		return 0;

	qword qwValue = 0;
	char cCharacter;

	for (size_t nIdx = 0; nIdx < p_strTextBlock.Size(); nIdx++)
	{
		cCharacter = p_strTextBlock.GetCharAt(nIdx);

		if ( IsNumber(cCharacter) )
			qwValue = qwValue * 10 + (cCharacter - '0');
		else
			break;
	}

	return qwValue;
}

//----------------------------------------------------------------------------------------------
/// Converts an unsigned integer value to a string representation.
/// \param p_qwValue Number to convert
/// \returns String containing texture representation of number
//----------------------------------------------------------------------------------------------
String TextSerialiser::UnsignedIntegerToString( qword p_qwValue )
{
	std::ostringstream streamOut;
	streamOut<<p_qwValue<<' ';
	return String(streamOut.str().c_str());
}

//----------------------------------------------------------------------------------------------
/// Converts a signed integer value to a string representation.
/// \param p_llValue Number to convert
/// \returns String containing texture representation of number
//----------------------------------------------------------------------------------------------
String TextSerialiser::SignedIntegerToString( long long p_llValue )
{
	std::ostringstream streamOut;
	streamOut<<p_llValue<<' ';
	return String(streamOut.str().c_str());
}

//----------------------------------------------------------------------------------------------
/// Converts a real number to a string representation.
/// \param p_ldfValue Number to convert
/// \returns String containing texture representation of number
//----------------------------------------------------------------------------------------------
String TextSerialiser::RealToString( long double p_ldfValue )
{
	std::ostringstream streamOut;
	streamOut<<p_ldfValue<<' ';
	return String(streamOut.str().c_str());
}

//----------------------------------------------------------------------------------------------
/// Reads an 8-bit signed integer from the stream.
/// \param p_inputStream Input stream
/// \returns Value read
//----------------------------------------------------------------------------------------------
char TextSerialiser::ReadChar( IInputStream& p_inputStream )
{ 
	if (p_inputStream.IsDataAvailable())
	{
		String strBlock;
		ReadTextBlock( p_inputStream, strBlock );
		return (char)ParseSignedInteger(strBlock);
	}
	
	return 0;
}

//----------------------------------------------------------------------------------------------
/// Reads a 16-bit signed integer from the stream.
/// \param p_inputStream Input stream
/// \returns Value read
//----------------------------------------------------------------------------------------------
short TextSerialiser::ReadShort( IInputStream& p_inputStream )
{ 
	if (p_inputStream.IsDataAvailable())
	{
		String strBlock;
		ReadTextBlock( p_inputStream, strBlock );
		return (short)ParseSignedInteger(strBlock);
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
/// Reads a 32-bit signed integer from the stream.
/// \param p_inputStream Input stream
/// \returns Value read
//----------------------------------------------------------------------------------------------
long TextSerialiser::ReadLong( IInputStream& p_inputStream )
{ 
	if (p_inputStream.IsDataAvailable())
	{
		String strBlock;
		ReadTextBlock( p_inputStream, strBlock );
		return (long)ParseSignedInteger(strBlock);
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
/// Reads a 64-bit signed integer from the stream.
/// \param p_inputStream Input stream
/// \returns Value read
//----------------------------------------------------------------------------------------------
long long TextSerialiser::ReadLongLong( IInputStream& p_inputStream )
{ 
	if (p_inputStream.IsDataAvailable())
	{
		String strBlock;
		ReadTextBlock( p_inputStream, strBlock );
		return ParseSignedInteger(strBlock);
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
/// Reads a 32-bit floating point number from the stream.
/// \param p_inputStream Input stream
/// \returns Value read
//----------------------------------------------------------------------------------------------
float TextSerialiser::ReadFloat( IInputStream& p_inputStream )
{ 	
	if (p_inputStream.IsDataAvailable())
	{
		String strBlock;
		ReadTextBlock( p_inputStream, strBlock );
		return (float)ParseReal(strBlock);
	}
	
	return 0;
}

//----------------------------------------------------------------------------------------------
/// Reads a 64-bit floating point number from the stream.
/// \param p_inputStream Input stream
/// \returns Value read
//----------------------------------------------------------------------------------------------
double TextSerialiser::ReadDouble( IInputStream& p_inputStream )
{ 	
	if (p_inputStream.IsDataAvailable())
	{
		String strBlock;
		ReadTextBlock( p_inputStream, strBlock );
		return (double)ParseReal(strBlock);
	}
	
	return 0;
}

//----------------------------------------------------------------------------------------------
/// Reads a long double (word size is compiler dependent) floating point number from the stream.
/// \param p_inputStream Input stream
/// \returns Value read
//----------------------------------------------------------------------------------------------
long double TextSerialiser::ReadLongDouble( IInputStream& p_inputStream )
{ 	
	if (p_inputStream.IsDataAvailable())
	{
		String strBlock;
		ReadTextBlock( p_inputStream, strBlock );
		return ParseReal(strBlock);
	}
	
	return 0;
}

//----------------------------------------------------------------------------------------------
/// Reads an 8-bit unsigned integer from the stream.
/// \param p_inputStream Input stream
/// \returns Value read
//----------------------------------------------------------------------------------------------
byte TextSerialiser::ReadByte( IInputStream& p_inputStream )
{ 
	if (p_inputStream.IsDataAvailable())
	{
		String strBlock;
		ReadTextBlock( p_inputStream, strBlock );
		return (byte)ParseUnsignedInteger(strBlock);
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
/// Reads a 16-bit unsigned integer from the stream.
/// \param p_inputStream Input stream
/// \returns Value read
//----------------------------------------------------------------------------------------------
word TextSerialiser::ReadWord( IInputStream& p_inputStream )
{ 
	if (p_inputStream.IsDataAvailable())
	{
		String strBlock;
		ReadTextBlock( p_inputStream, strBlock );
		return (word)ParseUnsignedInteger(strBlock);
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
/// Reads a 32-bit unsigned integer from the stream.
/// \param p_inputStream Input stream
/// \returns Value read
//----------------------------------------------------------------------------------------------
dword TextSerialiser::ReadDoubleWord( IInputStream& p_inputStream )
{ 
	if (p_inputStream.IsDataAvailable())
	{
		String strBlock;
		ReadTextBlock( p_inputStream, strBlock );
		return (dword)ParseUnsignedInteger(strBlock);
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
/// Reads a 64-bit unsigned integer from the stream.
/// \param p_inputStream Input stream
/// \returns Value read
//----------------------------------------------------------------------------------------------
qword TextSerialiser::ReadQuadWord( IInputStream& p_inputStream )
{ 
	if (p_inputStream.IsDataAvailable())
	{
		String strBlock;
		ReadTextBlock( p_inputStream, strBlock );
		return ParseUnsignedInteger(strBlock);
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
/// Reads a list of 8-bit signed integers from the stream.
//----------------------------------------------------------------------------------------------
int TextSerialiser::ReadCharArray( IInputStream& p_inputStream, char* p_pcBuffer, int p_nCount )
{ 
	int nCount = 0;
	
	for (char *pcBuffer = p_pcBuffer; nCount < p_nCount; nCount++, pcBuffer++)
		*pcBuffer = ReadChar(p_inputStream);

	return nCount; 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
int TextSerialiser::ReadShortArray( IInputStream& p_inputStream, short* p_psBuffer, int p_nCount )
{ 
	int nCount = 0;
	
	for (short *psBuffer = p_psBuffer; nCount < p_nCount; nCount++, psBuffer++)
		*psBuffer = ReadShort(p_inputStream);

	return nCount; 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
int TextSerialiser::ReadLongArray( IInputStream& p_inputStream, long* p_plBuffer, int p_nCount )
{ 
	int nCount = 0;
	
	for (long *plBuffer = p_plBuffer; nCount < p_nCount; nCount++, plBuffer++)
		*plBuffer = ReadLong(p_inputStream);

	return nCount; 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
int TextSerialiser::ReadLongLongArray( IInputStream& p_inputStream, long long* p_pllBuffer, int p_nCount )
{ 
	int nCount = 0;
	
	for (long long *pllBuffer = p_pllBuffer; nCount < p_nCount; nCount++, pllBuffer++)
		*pllBuffer = ReadLongLong(p_inputStream);

	return nCount; 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
int TextSerialiser::ReadFloatArray( IInputStream& p_inputStream, float* p_pfBuffer, int p_nCount )
{ 
	int nCount = 0;
	
	for (float *pfBuffer = p_pfBuffer; nCount < p_nCount; nCount++, pfBuffer++)
		*pfBuffer = ReadFloat(p_inputStream);

	return nCount; 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
int TextSerialiser::ReadDoubleArray( IInputStream& p_inputStream, double* p_pdfBuffer, int p_nCount )
{ 
	int nCount = 0;
	
	for (double *pdfBuffer = p_pdfBuffer; nCount < p_nCount; nCount++, pdfBuffer++)
		*pdfBuffer = ReadDouble(p_inputStream);

	return nCount; 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
int TextSerialiser::ReadLongDoubleArray( IInputStream& p_inputStream, long double* p_pldfBuffer, int p_nCount )
{ 
	int nCount = 0;
	
	for (long double *pldfBuffer = p_pldfBuffer; nCount < p_nCount; nCount++, pldfBuffer++)
		*pldfBuffer = ReadLongDouble(p_inputStream);

	return nCount; 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
int TextSerialiser::ReadByteArray( IInputStream& p_inputStream, byte* p_pbyBuffer, int p_nCount )
{ 
	int nCount = 0;
	
	for (byte *pbyBuffer = p_pbyBuffer; nCount < p_nCount; nCount++, pbyBuffer++)
		*pbyBuffer = ReadByte(p_inputStream);

	return nCount; 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
int TextSerialiser::ReadWordArray( IInputStream& p_inputStream, word* p_pwBuffer, int p_nCount )
{ 
	int nCount = 0;
	
	for (word *pwBuffer = p_pwBuffer; nCount < p_nCount; nCount++, pwBuffer++)
		*pwBuffer = ReadWord(p_inputStream);

	return nCount; 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
int TextSerialiser::ReadDoubleWordArray( IInputStream& p_inputStream, dword* p_pdwBuffer, int p_nCount )
{ 
	int nCount = 0;
	
	for (dword *pdwBuffer = p_pdwBuffer; nCount < p_nCount; nCount++, pdwBuffer++)
		*pdwBuffer = ReadDoubleWord(p_inputStream);

	return nCount; 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
int TextSerialiser::ReadQuadWordArray( IInputStream& p_inputStream, qword* p_pqwBuffer, int p_nCount )
{ 
	int nCount = 0;
	
	for (qword *pqwBuffer = p_pqwBuffer; nCount < p_nCount; nCount++, pqwBuffer++)
		*pqwBuffer = ReadQuadWord(p_inputStream);

	return nCount; 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::ReadString( IInputStream& p_inputStream, String& p_strInput )
{
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
char TextSerialiser::ReadString( IInputStream& p_inputStream )
{
	char cValue;
	p_inputStream >> cValue;
	return cValue;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteChar( IOutputStream& p_outputStream, char p_cValue )
{ 
	String strOut = SignedIntegerToString(p_cValue);
	p_outputStream.Write( strOut.GetBuffer(), strOut.Size() );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteShort( IOutputStream& p_outputStream, short p_sValue )
{ 
	String strOut = SignedIntegerToString(p_sValue);
	p_outputStream.Write( strOut.GetBuffer(), strOut.Size() );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteLong( IOutputStream& p_outputStream, long p_lValue )
{ 
	String strOut = SignedIntegerToString(p_lValue);
	p_outputStream.Write( strOut.GetBuffer(), strOut.Size() );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteLongLong( IOutputStream& p_outputStream, long long p_llValue )
{ 
	String strOut = SignedIntegerToString(p_llValue);
	p_outputStream.Write( strOut.GetBuffer(), strOut.Size() );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteFloat( IOutputStream& p_outputStream, float p_fValue )
{ 
	String strOut = RealToString(p_fValue);
	p_outputStream.Write( strOut.GetBuffer(), strOut.Size() );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteDouble( IOutputStream& p_outputStream, double p_dfValue )
{ 
	String strOut = RealToString(p_dfValue);
	p_outputStream.Write( strOut.GetBuffer(), strOut.Size() );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteLongDouble( IOutputStream& p_outputStream, long double p_ldfValue )
{ 
	String strOut = RealToString(p_ldfValue);
	p_outputStream.Write( strOut.GetBuffer(), strOut.Size() );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteByte( IOutputStream& p_outputStream, byte p_byValue )
{ 
	String strOut = UnsignedIntegerToString(p_byValue);
	p_outputStream.Write( strOut.GetBuffer(), strOut.Size() );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteWord( IOutputStream& p_outputStream, word p_wValue )
{ 
	String strOut = UnsignedIntegerToString(p_wValue);
	p_outputStream.Write( strOut.GetBuffer(), strOut.Size() );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteDoubleWord( IOutputStream& p_outputStream, dword p_dwValue )
{ 
	String strOut = UnsignedIntegerToString(p_dwValue);
	p_outputStream.Write( strOut.GetBuffer(), strOut.Size() );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteQuadWord( IOutputStream& p_outputStream, qword p_qwValue )
{ 
	String strOut = UnsignedIntegerToString(p_qwValue);
	p_outputStream.Write( strOut.GetBuffer(), strOut.Size() );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteCharArray( IOutputStream& p_outputStream, char* p_pcBuffer, int p_nCount )
{ 
	for (int nIdx = 0; nIdx < p_nCount; nIdx++)
		WriteChar( p_outputStream, p_pcBuffer[nIdx] );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteShortArray( IOutputStream& p_outputStream, short* p_psBuffer, int p_nCount )
{ 
	for (int nIdx = 0; nIdx < p_nCount; nIdx++)
		WriteShort( p_outputStream, p_psBuffer[nIdx] );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteLongArray( IOutputStream& p_outputStream, long* p_plBuffer, int p_nCount )
{ 
	for (int nIdx = 0; nIdx < p_nCount; nIdx++)
		WriteLong( p_outputStream, p_plBuffer[nIdx] );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteLongLongArray( IOutputStream& p_outputStream, long long* p_pllBuffer, int p_nCount )
{ 
	for (int nIdx = 0; nIdx < p_nCount; nIdx++)
		WriteLongLong( p_outputStream, p_pllBuffer[nIdx] );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteFloatArray( IOutputStream& p_outputStream, float* p_pfBuffer, int p_nCount )
{ 
	for (int nIdx = 0; nIdx < p_nCount; nIdx++)
		WriteFloat( p_outputStream, p_pfBuffer[nIdx] );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteDoubleArray( IOutputStream& p_outputStream, double* p_pdfBuffer, int p_nCount )
{ 
	for (int nIdx = 0; nIdx < p_nCount; nIdx++)
		WriteDouble( p_outputStream, p_pdfBuffer[nIdx] );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteLongDoubleArray( IOutputStream& p_outputStream, long double* p_pldfBuffer, int p_nCount )
{ 
	for (int nIdx = 0; nIdx < p_nCount; nIdx++)
		WriteLongDouble( p_outputStream, p_pldfBuffer[nIdx] );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteByteArray( IOutputStream& p_outputStream, byte* p_pbyBuffer, int p_nCount )
{ 
	for (int nIdx = 0; nIdx < p_nCount; nIdx++)
		WriteByte( p_outputStream, p_pbyBuffer[nIdx] );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteWordArray( IOutputStream& p_outputStream, word* p_pwBuffer, int p_nCount )
{ 
	for (int nIdx = 0; nIdx < p_nCount; nIdx++)
		WriteWord( p_outputStream, p_pwBuffer[nIdx] );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteDoubleWordArray( IOutputStream& p_outputStream, dword* p_pdwBuffer, int p_nCount )
{ 
	for (int nIdx = 0; nIdx < p_nCount; nIdx++)
		WriteDoubleWord( p_outputStream, p_pdwBuffer[nIdx] );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteQuadWordArray( IOutputStream& p_outputStream, qword* p_pqwBuffer, int p_nCount )
{ 
	for (int nIdx = 0; nIdx < p_nCount; nIdx++)
		WriteQuadWord( p_outputStream, p_pqwBuffer[nIdx] );
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteString( IOutputStream& p_outputStream, String& p_strOutput ){ }

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void TextSerialiser::WriteString( IOutputStream& p_outputStream, char p_strCharacter )
{
	p_outputStream<<p_strCharacter;
}

Meson_Common_Serialisation_END
