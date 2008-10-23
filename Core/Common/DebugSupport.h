#pragma once

#include "Namespaces.h"
#include "Platform.h"

#ifndef PLATFORM_WIN32

#include <assert.h>

#endif

Meson_Common_System_BEGIN

#ifdef NDEBUG

#define MESON_ASSERT(P_TEST ,P_MESSAGE) ;

#else

#define MESON_DEBUG

#ifdef PLATFORM_WIN32

#ifdef __cplusplus_cli
	#define MESON_ASSERT(P_TEST ,P_MESSAGE) ;
#else
	bool HandleDebug(const char *p_pszTest, const char *p_pszMessage,
		const char *p_pszFilename , int p_nLine, bool &p_pbReassert);

	#define MESON_ASSERT(P_TEST, P_MESSAGE)					\
	{														\
		if (!(P_TEST))										\
		{													\
			static bool s_bReassert = true;					\
			if (s_bReassert)								\
			{												\
				if (Meson::Common::System::HandleDebug(		\
					#P_TEST, P_MESSAGE,						\
					__FILE__ ,__LINE__, s_bReassert))		\
				{											\
					_asm { int 3 }							\
				}											\
			}												\
		}													\
	}
#endif

#else

#define MESON_ASSERT(P_TEST ,P_MESSAGE) assert(P_TEST);

#endif

#endif

Meson_Common_System_END
