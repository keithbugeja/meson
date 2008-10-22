#pragma once
#include "renode.h"

Meson_Common_RE_BEGIN

class RENodeClosure :
	public RENode
{
protected:
	char m_cType;

public:
	RENodeClosure( char p_cClosureType );
	virtual ~RENodeClosure(void);

	virtual bool Parse( String &p_strExpr, String &p_strTermChars );
	virtual String ToString( void );
};

Meson_Common_RE_END
