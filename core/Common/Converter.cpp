
#include "Converter.h"
#include "Maths.h"
#include "string.h"

using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

Meson_Common_BEGIN

static const char* g_szHex = "0123456789abcdef";
static String g_strDigit("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_!");

void Converter::FromHex( char c1, char c2, String &p_strBuffer )
{
	p_strBuffer.Append( (char)( ( g_strDigit.IndexOf( c1 ) << 4 ) | g_strDigit.IndexOf( c2 ) ) );
}



qword Converter::HexToQWord( String &p_string )
{
	qword qwValue = 0;

	char cChar = 0;
	
	size_t unSize = p_string.Size();

	if( unSize > 16)
		throw new MesonException( "Hex value too big!" );

	for( size_t un = 0; un < unSize; un++ )
	{
		cChar = (p_string[ un ] | 32);
		qwValue <<= 4;
		qwValue |= ( strchr( g_szHex, cChar ) - g_szHex ) & 0x0F;
	}

	return qwValue;
}

qword Converter::OctToQWord( String &p_string )
{
	qword qwValue = 0;

	char cChar = 0;
	
	size_t unSize = p_string.Size();

	if( unSize > 21)
		throw new MesonException( "Oct value too big!" );

	for( size_t un = 0; un < unSize; un++ )
	{
		cChar = p_string[ un ];
		qwValue <<= 3;
		qwValue |= ( strchr( g_szHex, cChar ) - g_szHex ) & 0x0F;
	}

	return qwValue;
}

qword Converter::BinToQWord( String &p_string )
{
	qword qwValue = 0;

	char cChar = 0;
	
	size_t unSize = p_string.Size();
	
	if( unSize > 64 )
		throw new MesonException( "Bin value too big!" );

	for( size_t un = 0; un < unSize; un++ )
	{
		cChar = p_string[ un ];
		qwValue <<= 1;
		qwValue |= ( cChar - '0' );
	}

	return qwValue;
}







bool Converter::ToHex( qword p_qwValue, String &p_strBuffer )
{
	char *pDigit = (char*)&p_qwValue;
	
	for( int n=7; n>=0; n-- )
	{
		p_strBuffer += g_strDigit( ( pDigit[n] & 0xF0 ) >> 4 );
		p_strBuffer += g_strDigit( ( pDigit[n] & 0x0F ) );
	}

	return true;
}

bool Converter::ToHex( dword p_dwValue, String &p_strBuffer )
{
	char *pDigit = (char*)&p_dwValue;
	
	for( int n=3; n>=0; n-- )
	{
		p_strBuffer += g_strDigit( ( pDigit[n] & 0xF0 ) >> 4 );
		p_strBuffer += g_strDigit( ( pDigit[n] & 0x0F ) );
	}

	return true;
}

bool Converter::ToHex( unsigned int p_unValue, String &p_strBuffer )
{
	char *pDigit = (char*)&p_unValue;
	
	for( int n=3; n>=0; n-- )
	{
		p_strBuffer += g_strDigit( ( pDigit[n] & 0xF0 ) >> 4 );
		p_strBuffer += g_strDigit( ( pDigit[n] & 0x0F ) );
	}

	return true;
}


bool Converter::ToHex( word p_wValue, String &p_strBuffer )
{
	char *pDigit = (char*)&p_wValue;
	
	p_strBuffer += g_strDigit( ( pDigit[1] & 0xF0 ) >> 4 );
	p_strBuffer += g_strDigit( ( pDigit[1] & 0x0F ) );
	p_strBuffer += g_strDigit( ( *pDigit & 0xF0 ) >> 4 );
	p_strBuffer += g_strDigit( ( *pDigit & 0x0F ) );

	return true;
}

bool Converter::ToHex( byte p_byValue, String &p_strBuffer )
{
	p_strBuffer += g_strDigit( ( p_byValue & 0xF0 ) >> 4 );
	p_strBuffer += g_strDigit( ( p_byValue & 0x0F ) );

	return true;
}


bool Converter::ToString( qword p_qwValue, String &p_strBuffer, int p_nRadix )
{

	if( p_nRadix > 64 )
		return false;

	if( p_qwValue == 0 )
	{
		p_strBuffer += '0';
		return true;
	}

	char cDigit;
	size_t unPos = p_strBuffer.Size();

	while( p_qwValue > 0 )
	{
		cDigit = (char)( p_qwValue % p_nRadix);

		cDigit = g_strDigit.GetCharAt( cDigit );

		p_strBuffer.Insert( unPos, cDigit );
		p_qwValue /= p_nRadix;
	}

	return true;
}

bool Converter::ToString( long double p_ldfValue, String &p_strBuffer, int p_nRadix, bool p_bExpForm, int p_nPrecision )
{
	if( p_nRadix > 64 )
		return false;

	if( p_ldfValue < 0 )
	{
		p_strBuffer += '-';
		p_ldfValue = -p_ldfValue;
	}

	long lExp = 0;

	if( p_bExpForm )
	{
		long double ldfReciprocal = 1 / p_nRadix;
		
		if( p_ldfValue < 0 )
		{
			while( p_ldfValue < 0 )
			{
				p_ldfValue *= p_nRadix;
				lExp--;
			}
		}
		else
		{
			while( p_ldfValue > p_nRadix )
			{
				p_ldfValue *= ldfReciprocal;
				lExp++;
			}
		}
	}

	long lValue = (long) p_ldfValue;
	long double ldValue = p_ldfValue - lValue;

	Converter::ToString( (qword)lValue, p_strBuffer, p_nRadix );

	char cDigit;
	
	if( ldValue > 0 )
	{
		p_strBuffer += '.';

		do
		{
			ldValue *= p_nRadix;
			cDigit = (char) ldValue;
			p_strBuffer.Append( g_strDigit.GetCharAt( cDigit ) );
			
			ldValue = (ldValue - cDigit);
			p_nPrecision--;
		}
		while( ldValue > 0 && p_nPrecision > 0);
	}

	if( p_bExpForm )
	{
		p_strBuffer += 'E';
		if( lExp >= 0 )
			p_strBuffer += '+';
		Converter::ToString( (long long)lExp, p_strBuffer, p_nRadix );
	}

	return true;
}

bool Converter::ToString( double p_dfValue, String &p_strBuffer, int p_nRadix, bool p_bExpForm, int p_nPrecision )
{
	return Converter::ToString( (long double) p_dfValue, p_strBuffer, p_nRadix, p_bExpForm, p_nPrecision );
}

bool Converter::ToString( float  p_fValue, String &p_strBuffer, int p_nRadix, bool p_bExpForm, int p_nPrecision )
{
	return Converter::ToString( (long double) p_fValue, p_strBuffer, p_nRadix, p_bExpForm, p_nPrecision );
}


bool Converter::ToString( byte p_byValue, String &p_strBuffer, int p_nRadix )
{
	return Converter::ToString( (qword)p_byValue, p_strBuffer, p_nRadix );
}

bool Converter::ToString( char p_cValue,  String &p_strBuffer, int p_nRadix )
{
	return Converter::ToString( (long long)p_cValue, p_strBuffer, p_nRadix );
}

bool Converter::ToString( word p_wValue, String &p_strBuffer, int p_nRadix )
{
	return Converter::ToString( (qword)p_wValue, p_strBuffer, p_nRadix );
}

bool Converter::ToString( short p_sValue,  String &p_strBuffer, int p_nRadix )
{
	return Converter::ToString( (long long)p_sValue, p_strBuffer, p_nRadix );
}

bool Converter::ToString( dword p_dwValue, String &p_strBuffer, int p_nRadix )
{
	return Converter::ToString( (qword)p_dwValue, p_strBuffer, p_nRadix );
}

bool Converter::ToString( long  p_lValue,  String &p_strBuffer, int p_nRadix )
{
	return Converter::ToString( (long long)p_lValue, p_strBuffer, p_nRadix );
}

bool Converter::ToString( long long p_llValue,  String &p_strBuffer, int p_nRadix )
{
	if( p_llValue < 0 )
	{
		p_llValue = -p_llValue;
		p_strBuffer += '-';
	}

	return Converter::ToString( (qword)p_llValue, p_strBuffer, p_nRadix );
}

bool Converter::ToString( bool p_bValue,  String &p_strBuffer )
{
	if( p_bValue )
		p_strBuffer.Append( "True" );
	else
		p_strBuffer.Append( "False" );
		
	return true;		
}

byte Converter::ToByte( String &p_strBuffer )
{
	return (byte) ToQWord( p_strBuffer );
}

char Converter::ToChar( String &p_strBuffer )
{
	return (char) ToLongLong( p_strBuffer );
}

word Converter::ToWord( String &p_strBuffer )
{
	return (word) ToQWord( p_strBuffer );
}

short Converter::ToShort( String &p_strBuffer )
{
	return (short) ToLongLong( p_strBuffer );
}

long Converter::ToLong( String &p_strBuffer )
{
	return (long) ToLongLong( p_strBuffer );
}

dword Converter::ToDWord( String &p_strBuffer )
{
	return (dword) ToQWord( p_strBuffer );
}

long long Converter::ToLongLong( String &p_strBuffer )
{
	int nLen = (int)p_strBuffer.Size();
	if( nLen == 0 )
		return 0;

	int nSign = 1;
	int nStart = 0;

	if( p_strBuffer(0) == '-' )
	{
		nSign = -1;
		nStart = 1;
	} 
	else if( p_strBuffer(0) == '+' )
		nStart = 1;
		

	long long llValue = ToQWord( p_strBuffer.Substring( nStart, nLen ) ) * nSign;
	return llValue;
}

qword Converter::ToQWord( String &p_strBuffer )
{
	int nLen = (int)p_strBuffer.Size();
	if( nLen == 0 )
		return 0;

	qword qwValue = 0;
	char  ch;
	for( int n=0; n<nLen; n++ )
	{
		ch =  p_strBuffer( n ) - '0';
		if( ch >=0 && ch < 10 )
		{
			qwValue *= 10;
			qwValue += ch;
		}
		else
			return qwValue;
	}

	return qwValue;
}

float Converter::ToFloat( String &p_strBuffer )
{
	return (float) ToLongDouble( p_strBuffer );
}

double Converter::ToDouble( String &p_strBuffer )
{
	return (double) ToLongDouble( p_strBuffer );
}

double Converter::ToLongDouble( String &p_strBuffer )
{
	int nLen = (int)p_strBuffer.Size();
	if( nLen == 0 )
		return 0;

	int nPos = 0;
	int nSign = 1;
	int nExp  = 0;
	long double dfIntegral = 0;
	long double dfFraction = 0;
	long double dfDivisor  = 1;
	
	if (p_strBuffer[0] == '-')
	{
		nSign = -1;
		nPos++;
	}

	char ch;

	for( ; nPos < nLen; nPos++ )
	{
		ch = p_strBuffer[ nPos ];
		if( ch >='0' && ch <= '9' )
		{
			dfIntegral *= 10;
			dfIntegral += (ch - '0');
		}
		else
			break;
	}

	if( ch == '.' )
	{
		nPos++;
		for( ; nPos < nLen; nPos++ )
		{
			ch =  p_strBuffer( nPos );
			if( ch >='0' && ch <= '9' )
			{
				dfFraction *= 10;
				dfFraction += (ch - '0');
				dfDivisor  *= 10;
			}
			else
				break;
		}
	}

	if( ( ch | 32 ) =='e' )
	{
		nPos++;
		nExp = ToLong( p_strBuffer.SubstringIndex( nPos, nLen - 1 ) );
		return (dfIntegral + ( dfFraction / dfDivisor) ) * (long double)nSign * TMaths<long double>::Pow(10.0, nExp);
	}

	return (dfIntegral + ( dfFraction / dfDivisor) ) * (long double)nSign;
}

bool Converter::ToBool( String &p_strBuffer )
{
	return ( p_strBuffer.CompareIgnoreCase( "true" ) == 0  || p_strBuffer.Compare("1") == 0 ) ? true : false;
}

Meson_Common_END
