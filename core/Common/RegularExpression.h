#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "REParser.h"

Meson_Common_RegularExpressions_BEGIN

class RegularExpression
{
private:
	REParser m_parser;
	RENode  *m_pRENode;

public:
	RegularExpression( void );
	RegularExpression( Meson::Common::Text::String &p_str );

	~RegularExpression( void );

	void Reset( void );
	RENode *Get( void );
	bool Set( Meson::Common::Text::String &p_str );
	bool Matches( Meson::Common::Text::String &p_str );

	int Fetch( Meson::Common::Text::String &p_str );

	Meson::Common::Text::String ToString( void );

};

Meson_Common_RegularExpressions_END
