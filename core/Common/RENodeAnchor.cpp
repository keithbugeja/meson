#include "RENodeAnchor.h"

using namespace Meson::Common::RE;

RENodeAnchor::RENodeAnchor( char p_cType )
{
	m_cType = p_cType;
}

RENodeAnchor::~RENodeAnchor( void )
{
}

String RENodeAnchor::ToString( void )
{
	return String( m_cType );
}
