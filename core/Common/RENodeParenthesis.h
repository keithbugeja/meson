#pragma once
#include "RENode.h"

Meson_Common_RE_BEGIN

class RENodeParenthesis :
	public RENode
{

public:
	RENodeParenthesis(void);
	virtual ~RENodeParenthesis(void);

	virtual String ToString( void );
};

Meson_Common_RE_END
