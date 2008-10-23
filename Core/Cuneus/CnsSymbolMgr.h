#pragma once

#include "CnsNamespaces.h"
#include <StringMap.h>

Meson_Cuneus_BEGIN

class SymbolMgr
{
private:
	Meson::Common::Collections::TStringMap< dword > m_mapSymbol;

public:
	SymbolMgr( void );
	~SymbolMgr( void );

	bool Contains( Meson::Common::Text::String& p_strSymbol );
	bool Contains( Meson::Common::Text::String& p_strSymbol, dword& p_dwIndex );

	dword GetSymbolIndex( Meson::Common::Text::String& p_strSymbol );
	void SetSymbolIndex( Meson::Common::Text::String& p_strSymbol, dword p_dwIndex );
};

Meson_Cuneus_END
