#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "RENode.h"

Meson_Common_RegularExpressions_BEGIN

class REParser
{
private:
	RENode *m_pRegExp;

public:
	REParser( void );
	~REParser( void );

	bool Parse( const char *p_pszRegExp );
	bool Parse( Meson::Common::Text::String &p_strRegExp );

	RENode *GetRegularExpression( void );

	Meson::Common::Text::String GetExpression( void );

private:
	int ParseNumber( Meson::Common::Text::String &p_strRegExp );
	int ParseHex( Meson::Common::Text::String &p_strRegExp );
	RENode *ParseEscape( Meson::Common::Text::String &p_strRegExp );
	bool ParseParenthesis( RENode *p_pNode, Meson::Common::Text::String &p_strRegExp );
	RENode *ParseBraces( Meson::Common::Text::String &p_strRegExp );
	RENode *ParseBrackets( Meson::Common::Text::String &p_strRegExp );
	bool ParseExpression( RENode *p_pNode, Meson::Common::Text::String &p_strRegExp );
	RENode *ParseExpressions( Meson::Common::Text::String &p_strRegExp );
};

Meson_Common_RegularExpressions_END
