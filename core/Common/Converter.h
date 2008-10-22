#pragma once

//----------------------------------------------------------------------------------------------
//	Meson::Common::Converter
//	Helper class that performs conversion between types.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	02/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------

#include "Namespaces.h"
#include "String8.h"

Meson_Common_BEGIN

class Converter
{
public:
	static void FromHex( char c1, char c2, Meson::Common::Text::String &p_strBuffer );

	static qword HexToQWord( Meson::Common::Text::String &p_string );
	static qword OctToQWord( Meson::Common::Text::String &p_string );
	static qword BinToQWord( Meson::Common::Text::String &p_string );

	static bool ToHex( qword p_qwValue, Meson::Common::Text::String &p_strBuffer );
	static bool ToHex( dword p_dwValue, Meson::Common::Text::String &p_strBuffer );
	static bool ToHex( unsigned int p_unValue, Meson::Common::Text::String &p_strBuffer );
	static bool ToHex( word p_wValue, Meson::Common::Text::String &p_strBuffer );
	static bool ToHex( byte p_byValue, Meson::Common::Text::String &p_strBuffer );

	static bool ToString( qword p_qwValue, Meson::Common::Text::String &p_strBuffer, int p_nRadix = 10 );
	static bool ToString( long double p_ldfValue, Meson::Common::Text::String &p_strBuffer, int p_nRadix = 10, bool p_bExpForm = false, int p_nPrecision = 3 );

	static bool ToString( double p_dfValue, Meson::Common::Text::String &p_strBuffer, int p_nRadix = 10, bool p_bExpForm = false, int p_nPrecision = 3 );
	static bool ToString( float  p_fValue,  Meson::Common::Text::String &p_strBuffer, int p_nRadix = 10, bool p_bExpForm = false, int p_nPrecision = 3 );

	static bool ToString( byte p_byValue, Meson::Common::Text::String &p_strBuffer, int p_nRadix = 10 );
	static bool ToString( char p_cValue,  Meson::Common::Text::String &p_strBuffer, int p_nRadix = 10 );
	static bool ToString( word p_wValue, Meson::Common::Text::String &p_strBuffer, int p_nRadix = 10 );
	static bool ToString( short p_sValue,  Meson::Common::Text::String &p_strBuffer, int p_nRadix = 10 );
	static bool ToString( dword p_dwValue, Meson::Common::Text::String &p_strBuffer, int p_nRadix = 10 );
	static bool ToString( long  p_lValue,  Meson::Common::Text::String &p_strBuffer, int p_nRadix = 10 );
	static bool ToString( long long p_llValue,  Meson::Common::Text::String &p_strBuffer, int p_nRadix = 10 );
	static bool ToString( bool p_bValue,  Meson::Common::Text::String &p_strBuffer );

	static byte ToByte( Meson::Common::Text::String &p_strBuffer );
	static char ToChar( Meson::Common::Text::String &p_strBuffer );
	static word ToWord( Meson::Common::Text::String &p_strBuffer );
	static short ToShort( Meson::Common::Text::String &p_strBuffer );
	static long ToLong( Meson::Common::Text::String &p_strBuffer );
	static dword ToDWord( Meson::Common::Text::String &p_strBuffer );
	static long long ToLongLong( Meson::Common::Text::String &p_strBuffer );
	static qword ToQWord( Meson::Common::Text::String &p_strBuffer );
	static float ToFloat( Meson::Common::Text::String &p_strBuffer );
	static double ToDouble( Meson::Common::Text::String &p_strBuffer );
	static double ToLongDouble( Meson::Common::Text::String &p_strBuffer );
	static bool ToBool( Meson::Common::Text::String &p_strBuffer );

	
};

Meson_Common_END
