// 14/12/2007
#pragma once

typedef union RegAccess
{
	struct
	{
		byte Index  : 5;
		byte Type_R : 1;
		byte Type_L : 1;
		byte Type_S : 1;
	} ;
	byte m_byValue;
	
	RegAccess( byte p_byValue ) { m_byValue = p_byValue; }
	byte Get( void ) { return m_byValue; }
	void Set( byte p_byValue )  { m_byValue = p_byValue; }

} RegAccess;

