#include "CnsSymbolMgr.h"

using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Cuneus_BEGIN

SymbolMgr::SymbolMgr( void ) : m_mapSymbol()
{
}

SymbolMgr::~SymbolMgr( void )
{
}

bool SymbolMgr::Contains( Meson::Common::Text::String& p_strSymbol )
{
	return m_mapSymbol.ContainsKey( p_strSymbol );
}

bool SymbolMgr::Contains( Meson::Common::Text::String& p_strSymbol, dword& p_nIndex )
{
	return m_mapSymbol.ContainsKey( p_strSymbol, p_nIndex );
}

dword SymbolMgr::GetSymbolIndex( Meson::Common::Text::String& p_strSymbol )
{
	return m_mapSymbol[ p_strSymbol ];
}

void SymbolMgr::SetSymbolIndex( Meson::Common::Text::String& p_strSymbol, dword p_nIndex )
{
	m_mapSymbol.Insert( p_strSymbol, p_nIndex );
}

Meson_Cuneus_END
