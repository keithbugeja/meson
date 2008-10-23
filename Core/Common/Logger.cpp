//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#include "Logger.h"

#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Common_Logging_BEGIN

Logger::Logger( const String& p_strName )
{
	m_strName = p_strName;
	
	m_pmapChannel = new TStringMap< Channel* >();

	Init();
}


void Logger::Init( void )
{
	m_pmapChannel->Insert( "Out", &Out );
	m_pmapChannel->Insert( "Err", &Err );
}


Logger::Logger( const Logger& p_logger )
{
	m_strName = p_logger.m_strName;
	m_pmapChannel = p_logger.m_pmapChannel;

	(*m_pmapChannel)[ "Out" ] = &Out;
	(*m_pmapChannel)[ "Err" ] = &Err;
}


Logger::~Logger(void)
{
	m_pmapChannel->RemoveKey( "Out" );
	m_pmapChannel->RemoveKey( "Err" );
	FreePointers( *m_pmapChannel );

	delete m_pmapChannel;
}

bool Logger::Contains( const String& p_strChannelName )
{
	return m_pmapChannel->ContainsKey( p_strChannelName );
}

void Logger::CreateChannel( const String& p_strChannelName )
{
	AddChannel( p_strChannelName, new Channel() );
}

void Logger::AddChannel( const String& p_strChannelName, Channel* p_channel )
{
	m_pmapChannel->Insert( p_strChannelName, p_channel );
}



Logger& Logger::operator=( const Logger& p_logger )
{
	m_strName = p_logger.m_strName;
	m_pmapChannel = p_logger.m_pmapChannel;

	(*m_pmapChannel)[ "Out" ] = &Out;
	(*m_pmapChannel)[ "Err" ] = &Err;

	return *this;
}


Channel& Logger::operator[]( const String& p_strChannelName )
{
	Channel* pChannel = NULL;
		
	if( m_pmapChannel->ContainsKey( p_strChannelName, pChannel ) )
		return* pChannel;
	else
		throw new MesonException( "Requested Channel does not exist.", __FILE__, __LINE__ );
}

Meson_Common_Logging_END
