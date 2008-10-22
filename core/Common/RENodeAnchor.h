#pragma once
#include "RENode.h"

Meson_Common_RE_BEGIN

class RENodeAnchor :
	public RENode
{
protected:
	char m_cType;

public:
	RENodeAnchor( char p_cType );
	virtual ~RENodeAnchor(void);

	virtual String ToString( void );
};

Meson_Common_RE_END
