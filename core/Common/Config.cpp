
#include "Config.h"

#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;

Meson_Common_Config_BEGIN

IProperty::IProperty( void )
{
}

IProperty::~IProperty( void )
{
}

String& IProperty::GetName( void )
{
	return m_strName;
}

void IProperty::SetName( String& p_strName )
{
	m_strName = p_strName;
}

String& IProperty::GetType( void )
{
	return m_strType;
}

void IProperty::SetType( String& p_strType  )
{
	if( m_strType.IsEmpty() )
		m_strType = p_strType;
	else
		throw new MesonException( "" );
}


//-------------------------------------------------------------------------------------------------

Config::Config(void)
{
}

Config::~Config(void)
{
}

Meson_Common_Config_END
