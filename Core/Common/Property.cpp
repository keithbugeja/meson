
#include "Property.h"

#include "PropertySet.h"

using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;


Meson_Common_Properties_BEGIN

IProperty::IProperty( Meson::Common::Text::String p_strName )
	: m_strName( p_strName )
	, m_pPropertySet( NULL )
{

}

IProperty::~IProperty( void )
{
	if( m_pPropertySet )
		delete m_pPropertySet;
}

PropertySet* IProperty::GetPropertySet( void )
{
	if( !m_pPropertySet )
		m_pPropertySet = new PropertySet();

	return m_pPropertySet;
}


IProperty& IProperty::operator[]( Meson::Common::Text::String p_strName )
{
	IProperty* pProperty = NULL;

	if( !m_pPropertySet )
		m_pPropertySet = new PropertySet();

	return (*m_pPropertySet)[ p_strName ];
}

//---------------------------------------------------------------------------------------------------------

StoredProperty::StoredProperty( Meson::Common::Text::String p_strName )
	: IProperty( p_strName )
	, m_propertyValue()
{

}

StoredProperty::~StoredProperty( void )
{

}

PropertyValue& StoredProperty::Get( void ) const
{
	return (PropertyValue&) m_propertyValue;
}

void StoredProperty::Set( PropertyValue p_value )
{
	m_propertyValue = p_value;
}

PropertyValue StoredProperty::GetIndexed( int p_nIndex )
{
	return m_propertyValue[ p_nIndex ];
}

void StoredProperty::SetIndexed( int p_nIndex, const PropertyValue& p_value )
{
	m_propertyValue[ p_nIndex ] = p_value;
}


//---------------------------------------------------------------------------------------------------------


DelegatedProperty::DelegatedProperty( Meson::Common::Text::String p_strName )
: IProperty( p_strName )
{
}



Meson_Common_Properties_END
