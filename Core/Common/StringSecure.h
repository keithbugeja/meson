#pragma once

#include "Platform.h"

#if !((defined WIN32) || (defined PLATFORM_WIN32))

#include "Types.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

bool __meson_uitoa( unsigned long long p_ullValue, char *p_szBuffer, int p_nRadix)
{
	// Early exit if buffer is NULL
	if (!p_szBuffer) return false;

	int nRadix = MIN(MAX(0x02,p_nRadix),0x10),
		nIdx;
	
	char cDigit;

	for (nIdx = 0; p_ullValue > 0; nIdx++)
	{
		cDigit = (char)(p_ullValue % nRadix);
		p_ullValue /= nRadix;

		if (cDigit < 0x0A) cDigit += '0';
		else cDigit += '7';

		p_szBuffer[nIdx] = cDigit;
	}

	// Null terminate string
	p_szBuffer[nIdx] = '\0';
	_strrev( p_szBuffer );

	return true;
}

bool __meson_itoa( long long p_llValue, char* p_szBuffer, int p_nRadix)
{
	// Early exit if buffer is NULL
	if (!p_szBuffer) return false;

	long long llValue = p_llValue > 0 ? p_llValue : -p_llValue;
	
	int nRadix = MIN(MAX(0x02,p_nRadix),0x10),
		nIdx;
	
	char cDigit;

	for (nIdx = 0; llValue > 0; nIdx++)
	{
		cDigit = (char)(llValue % nRadix);
		llValue /= nRadix;

		if (cDigit < 0x0A) cDigit += '0';
		else cDigit += '7';

		p_szBuffer[nIdx] = cDigit;
	}

	// Add sign
	if (p_llValue < 0) 
		p_szBuffer[nIdx++] = '-';

	// Null terminate string
	p_szBuffer[nIdx] = '\0';
	_strrev( p_szBuffer );

	return true;
}

int _ultoa_s(unsigned long value, char *buffer, size_t sizeInCharacters, int radix )
{
	return __meson_uitoa(value, buffer, radix);
};

int _ltoa_s(long value, char *buffer, size_t sizeInCharacters, int radix )
{
	return __meson_itoa(value, buffer, radix);
};

int _ui64toa_s( unsigned long long value, char *buffer, size_t sizeInCharacters, int radix )
{
	return __meson_uitoa(value, buffer, radix);
};

int _i64toa_s( long long value, char *buffer, size_t sizeInCharacters, int radix )
{
	return __meson_itoa(value, buffer, radix);
};

int _itoa_s( int value, char *buffer, size_t sizeInCharacters, int radix )
{
	return __meson_itoa(value, buffer, radix);
};

int sprintf_s(char *_DstBuf, size_t _DstSize, const char *_Format, ...)
{
	va_list argptr;
	va_start(argptr, _Format);

	char *args = va_arg(argptr, char*);
	return sprintf(_DstBuf, _Format, args);
}

void memcpy_s(void *_Dst, size_t _DstSize, const void*_Src, size_t _MaxCount)
{
	memcpy(_Dst, _Src, _MaxCount);
}

#endif


unsigned long long atoull( const char *p_szNumber )
{
	unsigned long long ullResult = 0;

	size_t unLen = strlen( p_szNumber );
	for( size_t unPos = 0; unPos < unLen; unPos++)
	{
		ullResult *= 10;
		ullResult += p_szNumber[unPos] - '0';
	}

	return ullResult;
}


long long atoll( const char *p_szNumber )
{
	if( p_szNumber[0] == '+' )
		return atoull( p_szNumber+1 );

	if( p_szNumber[0] == '-' )
		return -((long long)atoull( p_szNumber+1 ));

	return -1;
}
