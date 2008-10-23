//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::SourceInfo
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	07/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#pragma once

#include "CnsNamespaces.h"
#include "CnsTypes.h"

Meson_Cuneus_Compiler_BEGIN

typedef struct PositionInfo
{
	qword Coloumn : 8;
	qword LineNumber : 24;

	inline PositionInfo( void )
	{
		Coloumn = 0;
		LineNumber = 1;
	}

	inline void Reset( void )
	{
		Coloumn = 0;
		LineNumber = 1;
	}

	inline bool operator==( const PositionInfo& p_position )
	{
		return ( Coloumn == p_position.Coloumn ) && ( LineNumber == p_position.LineNumber );
	}

} PositionInfo;




class SourceInfo
{
private:
	PositionInfo m_positionInfo;


public:
	inline size_t GetLineNumber( void )
	{
		return m_positionInfo.LineNumber;
	}
	
	inline byte GetColoumn( void )
	{
		return m_positionInfo.Coloumn;
	}

	inline PositionInfo GetPosition( void )
	{
		return m_positionInfo;
	}


	inline void Reset( void )
	{
		m_positionInfo.Reset();
	}

	inline void NewLine( void )
	{
		m_positionInfo.LineNumber++;
		m_positionInfo.Coloumn = 0;
	}

	inline void NextColoumn( void )
	{
		m_positionInfo.Coloumn++;
	}

	inline void PreviousColoumn( void )
	{
		m_positionInfo.Coloumn--;
		
		if( m_positionInfo.Coloumn == 0 )
			m_positionInfo.LineNumber--;
	}
};

Meson_Cuneus_Compiler_END

