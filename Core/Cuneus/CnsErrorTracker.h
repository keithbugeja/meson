//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::ErrorTracker
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	07/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#pragma once

#include "CnsNamespaces.h"
#include "CnsSourceInfo.h"

#include <String8.h>
#include <BufferList.h>


Meson_Cuneus_Compiler_BEGIN

class ErrorEntry
{
private:
	PositionInfo m_position;
	Meson::Common::Text::String	m_strMessage;

public:
	ErrorEntry( void );
	ErrorEntry( PositionInfo p_position );
	ErrorEntry( Meson::Common::Text::String p_strMessage );
	ErrorEntry( PositionInfo p_position, Meson::Common::Text::String p_strMessage );
	~ErrorEntry( void );

	PositionInfo GetPosition( void );
	void SetPosition( PositionInfo p_position );

	Meson::Common::Text::String& GetMessage( void );
	void SetMessage( Meson::Common::Text::String& p_strMessage );

	Meson::Common::Text::String ToString( void );
};



//---------------------------------------------------------------------------------------------------------



class ErrorTracker
{
private:
	SourceInfo* m_pSourceInfo;
	Meson::Common::Collections::TBufferList< ErrorEntry* > m_lstErrorEntry;

public:
	ErrorTracker( SourceInfo* p_pSourceInfo );
	~ErrorTracker( void );

	SourceInfo* GetSourceInfo( void );

	void ReportError( PositionInfo p_position, Meson::Common::Text::String& p_strMessage );

	void OutputErrors( void );
};

Meson_Cuneus_Compiler_END
