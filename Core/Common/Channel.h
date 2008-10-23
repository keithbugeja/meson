#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "Namespaces.h"
#include "Types.h"
#include "OutputDevice.h"
#include "String8.h"



Meson_Common_Logging_BEGIN

class Channel
{
private:
	Meson::Common::Text::String8 m_strBuffer;
	IOutputDevice *m_pOutputDevice;

public:
	Channel(void);
	virtual ~Channel(void);

	const char *GetBuffer( void );

	IOutputDevice *GetOutputDevice( void );
	void SetOutputDevice( IOutputDevice *p_pOutputDevice );

	inline void Write( char p_ch );

	Channel &operator<<( const unsigned long long p_ullValue );
	Channel &operator<<( const signed   long long p_llValue );
	Channel &operator<<( const unsigned long p_ulValue );
	Channel &operator<<( const signed   long p_lValue );
	Channel &operator<<( const unsigned int  p_unValue );
	Channel &operator<<( const signed   int  p_nValue );

	Channel &operator<<( const word  p_wValue );
	Channel &operator<<( const signed  short  p_sValue );
	Channel &operator<<( const byte p_byValue );
	Channel &operator<<( const char p_cValue );

	Channel &operator<<( const float  p_fValue );
	Channel &operator<<( const double p_dfValue );
	Channel &operator<<( const long double p_ldfValue );

	Channel &operator<<( const char *p_pszValue );

	void Flush( void );

};

Meson_Common_Logging_END

