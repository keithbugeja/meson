#pragma once

// byte type use 'by' for hungarian notation
typedef unsigned char byte;

// word type use 'w' for hungarian notation
typedef unsigned short word;

// double word type use 'dw' for hungarian notation
typedef unsigned long dword;

// quad word type use 'qw' for hungarian notation
typedef unsigned long long qword;

// uchar type use 'uc' for hungarian notation
typedef unsigned char uchar;

// ushort type use 'us' for hungarian notation
typedef unsigned short ushort;

// uint type use 'un' for hungarian notation
typedef unsigned int uint;

// ulong type use 'ul' for hungarian notation
typedef unsigned long ulong;

// ulonglong type use 'ull' for hungarian notation
typedef unsigned long long ulonglong;

typedef union variant
{
	byte byValue;
	char cValue;
	word wValue;
	short sValue;
	int nValue;
	long lValue;
	dword dwValue;
	long long llValue;
	qword qwValue;

	float fValue;
	double dfValue;
	long double ldfValue;

	void *pValue;

	operator const byte( void ) { return byValue; }
	variant &operator=( byte p_byValue ) { byValue = p_byValue; return *this; }

	operator const char( void ) { return cValue; }
	variant &operator=( char p_cValue ) { cValue = p_cValue; return *this; }

	operator const word( void ) { return wValue; }
	variant &operator=( word p_wValue ) { wValue = p_wValue; return *this; }

	operator const short( void ) { return sValue; }
	variant &operator=( short p_sValue ) { sValue = p_sValue; return *this; }

	operator const int( void ) { return nValue; }
	variant &operator=( int p_nValue ) { nValue = p_nValue; return *this; }

	operator const long( void ) { return lValue; }
	variant &operator=( long p_lValue ) { lValue = p_lValue; return *this; }

	operator const dword( void ) { return dwValue; }
	variant &operator=( dword p_dwValue ) { dwValue = p_dwValue; return *this; }

	operator const long long( void ) { return llValue; }
	variant &operator=( long long p_llValue ) { llValue = p_llValue; return *this; }

	operator const qword( void ) { return qwValue; }
	variant &operator=( qword p_qwValue ) { qwValue = p_qwValue; return *this; }

	operator const float( void ) { return fValue; }
	variant &operator=( float p_fValue ) { fValue = p_fValue; return *this; }

	operator const double( void ) { return dfValue; }
	variant &operator=( double p_dfValue ) { dfValue = p_dfValue; return *this; }

	operator const long double( void ) { return ldfValue; }
	variant &operator=( long double p_ldfValue ) { ldfValue = p_ldfValue; return *this; }

	operator const void*( void ) { return pValue; }
	variant &operator=( void *p_pValue ) { pValue = p_pValue; return *this; }


} variant;
