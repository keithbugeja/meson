#include "Platform.h"

#include "Types.h"

Meson_Common_System_BEGIN

	Platform::Platform(void)
	{
	}

	Platform::~Platform(void)
	{
	}

#ifdef PLATFORM_WIN32

	PlatformType::PlatformType Platform::GetType(void)
	{
		return PlatformType::Win32;
	}

	bool Platform::IsLittleEndian(void)
	{
		return true;
	}

#elif defined PLATFORM_OSX

	PlatformType::PlatformType Platform::GetType(void)
	{
		return PlatformType::OSX;
	}

	// may be either, e.g. Motorola / Intel CPU
	bool Platform::IsLittleEndian(void)
	{
		struct Endian
		{
			union
			{
				uchar m_pucValues[2];
				short m_sValue;
			};
		};

		static Endian s_endian = {{1, 0}};

		return s_endian.m_sValue == 1;
	}

#elif defined PLATFORM_LINUX

	PlatformType::PlatformType Platform::GetType(void)
	{
		return PlatformType::Linux;
	}

	bool Platform::IsLittleEndian(void)
	{
		return true;
	}

#else

	PlatformType::PlatformType Platform::GetType(void)
	{
		return PlatformType::Generic;
	}

	bool Platform::IsLittleEndian(void)
	{
		struct Endian
		{
			union
			{
				uchar m_pucValues[2];
				short m_sValue;
			};
		};

		static Endian s_endian = {{1, 0}};

		return s_endian.m_sValue == 1;
	}

#endif

	short Platform::NormaliseShort(short sValue)
	{
		if (Platform::IsLittleEndian())
			return sValue;

		uchar ucLo = (sValue >> 8) & 0xFF;
		uchar ucHi = sValue & 0xFF;
		short sNormalisedValue =  (ucHi << 8) | ucLo;

		return sNormalisedValue;
	}

	short (*Platform::DenormaliseShort)(short sValue) = Platform::NormaliseShort;

	long Platform::NormaliseLong(long lValue)
	{
		if (Platform::IsLittleEndian())
			return lValue;

		uchar ucByte0 = (uchar) (lValue & 0xFF);
		uchar ucByte1 = (uchar) ((lValue >> 8) & 0xFF);
		uchar ucByte2 = (uchar) ((lValue >> 16) & 0xFF);
		uchar ucByte3 = (uchar) ((lValue >> 24) & 0xFF);
		long lNormalisedValue
			= ((long)ucByte0 << 24) | ((long)ucByte1 << 16)
				| ((long)ucByte2 << 8) | (long)ucByte3;

		return lNormalisedValue;
	}

	long (*Platform::DenormaliseLong)(long lValue) = Platform::NormaliseLong;

	float Platform::NormaliseFloat(float fValue)
	{
		if (Platform::IsLittleEndian())
			return fValue;

		uchar *pucBytes = (uchar *) &fValue;

		uchar ucBytes[4];
		ucBytes[0] = pucBytes[3];
		ucBytes[1] = pucBytes[2];
		ucBytes[2] = pucBytes[1];
		ucBytes[3] = pucBytes[0];

		return *(float *) ucBytes;
	}

	float (*Platform::DenormaliseFloat)(float fValue) = Platform::NormaliseFloat;

Meson_Common_System_END
