#include "CnsErrorTracker.h"

#include <iostream>

using namespace Meson::Common::Text;

Meson_Cuneus_Compiler_BEGIN

ErrorEntry::ErrorEntry( void ) : m_position(), m_strMessage()
{
}

ErrorEntry::ErrorEntry( PositionInfo p_position ) : m_strMessage()
{
	m_position = p_position;
}

ErrorEntry::ErrorEntry( Meson::Common::Text::String p_strMessage ) : m_position(), m_strMessage( p_strMessage )
{
}

ErrorEntry::ErrorEntry( PositionInfo p_position, Meson::Common::Text::String p_strMessage ) : m_strMessage( p_strMessage )
{
	m_position = p_position;
}

ErrorEntry::~ErrorEntry( void )
{
}

PositionInfo ErrorEntry::GetPosition( void )
{
	return m_position;
}

void ErrorEntry::SetPosition( PositionInfo p_position )
{
	m_position = p_position;
}


String& ErrorEntry::GetMessage( void )
{
	return m_strMessage;
}

void ErrorEntry::SetMessage( String& p_strMessage )
{
	m_strMessage = p_strMessage;
}

String ErrorEntry::ToString( void )
{
	String str;

	str += "Error at (";
	str += m_position.LineNumber;
	str += ", ";
	str += m_position.Coloumn;
	str += "): ";
	str += m_strMessage;

	return str;
}



//---------------------------------------------------------------------------------------------------------



ErrorTracker::ErrorTracker( SourceInfo* p_pSourceInfo ) : m_lstErrorEntry( 32 )
{
	m_pSourceInfo = p_pSourceInfo;
}

ErrorTracker::~ErrorTracker( void )
{
}

SourceInfo* ErrorTracker::GetSourceInfo( void ) 
{ 
	return m_pSourceInfo; 
}

void ErrorTracker::ReportError( PositionInfo p_position, Meson::Common::Text::String& p_strMessage )
{
	m_lstErrorEntry.Add( new ErrorEntry( p_position, p_strMessage ) );
}

void ErrorTracker::OutputErrors( void )
{
	size_t unSize = m_lstErrorEntry.Size();
	for( size_t un = 0; un < unSize; un++ )
	{
		std::cout << m_lstErrorEntry[ un ]->ToString() << std::endl;
	}
}

Meson_Cuneus_Compiler_END
