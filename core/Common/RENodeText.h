#pragma once
#include "renode.h"
#include "String8.h"

Meson_Common_RE_BEGIN

class RENodeText :
	public RENode
{
protected:
	String m_strValue;

public:
	RENodeText( void );
	RENodeText( char p_cValue );
	virtual ~RENodeText( void );

	virtual bool Parse( String &p_strExpr, String &p_strTermChars );

	virtual String ToString( void );
};

Meson_Common_RE_END
