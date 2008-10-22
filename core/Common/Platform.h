#pragma once

#include "Namespaces.h"

#ifdef _WIN32
	#define PLATFORM_WIN32
	#define LITTLE_ENDIAN
#elif (defined _OSX) || (defined __APPLE__)
	#define PLATFORM_OSX
	#ifdef TARGET_CPU_PPC
		#define BIG_ENDIAN
	#else
		#define LITTLE_ENDIAN
	#endif
#elif defined _LINUX
	#define PLATFORM_LINUX
	#define BIG_ENDIAN
#else
	#define PLATFORM_GENERIC
#endif

#ifdef LITTLE_ENDIAN
	#define NORMALISE_SHORT(P_VALUE) ( P_VALUE )
	#define DENORMALISE_SHORT(P_VALUE) ( P_VALUE )
	#define NORMALISE_LONG(P_VALUE) ( P_VALUE )
	#define DENORMALISE_LONG(P_VALUE) ( P_VALUE )
#elif defined BIG_ENDIAN
	#define NORMALISE_SHORT(P_VALUE) ( (((P_VALUE) & 0x00FF) << 8) | ((P_VALUE) >> 8) & 0x00FF)
	#define DENORMALISE_SHORT(P_VALUE) NORMALISE_SHORT(PVALUE)
	#define NORMALISE_LONG(P_VALUE) ( (((P_VALUE) & 0x000000FF) << 24) | (((P_VALUE) & 0x0000FF00) << 8) | (((P_VALUE) & 0x00FF0000) >> 8) | ((P_VALUE) >> 24) & 0x000000FF )
	#define DENORMALISE_LONG(P_VALUE) NORMALISE_LONG(PVALUE)
#else
	#define NORMALISE_SHORT(P_VALUE) Platform::NormaliseShort((PVALUE))
	#define DENORMALISE_SHORT(P_VALUE) Platform::DenormaliseShort((PVALUE))
	#define NORMALISE_LONG(P_VALUE) Platform::NormaliseLong((PVALUE))
	#define DENORMALISE_LONG(P_VALUE) Platform::DenormaliseLong((PVALUE))
#endif

#define NORMALISE_FLOAT(P_VALUE) Platform::NormaliseFloat((PVALUE))
#define DENORMALISE_FLOAT(P_VALUE) Platform::DenormaliseFloat((PVALUE))

Meson_Common_System_BEGIN

	namespace PlatformType { enum PlatformType { Win32, OSX, Linux, Generic }; }

	class Platform
	{
	private:
		Platform();
		~Platform();

	public:
		static PlatformType::PlatformType GetType(void);
		static bool IsLittleEndian(void);
		static short NormaliseShort(short sValue);
		static short (*DenormaliseShort)(short sValue);
		static long NormaliseLong(long sValue);
		static long (*DenormaliseLong)(long sValue);
		static float NormaliseFloat(float fValue);
		static float (*DenormaliseFloat)(float fValue);
	};

Meson_Common_System_END
