#include "Timer.h"

#include "DebugSupport.h"

#include "String8.h"

#ifdef PLATFORM_WIN32
	#include <Windows.h>
#endif

Meson_Common_System_BEGIN

#ifdef PLATFORM_WIN32 // Windows-specific implementation

using namespace Meson::Common;
using namespace Meson::Common::Text;

bool HandleDebug(const char *p_pszTest, const char *p_pszMessage,
	const char *p_pszFilename , int p_nLine, bool &p_pbReassert)
{
	// assert message and return flag regarding aborting
	// p_pbReassert cleared if repeating forbidden

	// static string allocation to avoid memory problems
	static String s_strText = "Assertion: ";
	s_strText += p_pszMessage;
	s_strText += "\nTest: ";
	s_strText += p_pszTest;
	s_strText += "\nFile: ";
	s_strText += p_pszFilename;
	s_strText += "\nLine: ";
	s_strText += p_nLine;
	s_strText += "\nAbort execution, allow assert Retry, or Ignore in future?";

	switch (::MessageBox(NULL, s_strText,
		"Assertion Failure",MB_ABORTRETRYIGNORE))
	{
	case IDIGNORE: // prevent calling again - turn off flag
		p_pbReassert = false; // never call again
		return false;
		break;
	case IDABORT:  // return flag and break
		return true;    // abort/break
	default: // retry
		return false;
	}
}

#else // generic implementation


#endif

Meson_Common_System_END
