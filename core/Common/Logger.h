
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
#include "String8.h"
#include "StringMap.h"
#include "Channel.h"
#include "Pointer.h"


Meson_Common_Logging_BEGIN

class Logger
{
protected:
	Meson::Common::Text::String m_strName;
	Meson::Common::Collections::TStringMap< Channel* >* m_pmapChannel;

public:
	Channel Out,
			Err;

protected:
	void Init( void );

public:
	Logger( const Meson::Common::Text::String& p_strName );
	Logger( const Logger &p_logger );
	virtual ~Logger( void );

	bool Contains( const Meson::Common::Text::String& p_strChannelName );

	void CreateChannel( const Meson::Common::Text::String& p_strChannelName );
	void AddChannel( const Meson::Common::Text::String& p_strChannelName, Channel* p_channel );

	Logger&  operator=( const Logger& p_logger );
	Channel& operator[]( const Meson::Common::Text::String& p_strChannelName );
};

Meson_Common_Logging_END

