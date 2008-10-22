#pragma once

//----------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	08/07/2007	1.0.0		Initial version.
//	Gordon Mangion 	05/03/2008	2.0.0		Renamed and Implemented Listener pattern.
//----------------------------------------------------------------------------------------------


//TODO: Optimisation


#include "Namespaces.h"
#include "Property.h"
#include "StringMap.h"


Meson_Common_Properties_BEGIN


//----------------------------------------------------------------------------------------------
// Property Set
//----------------------------------------------------------------------------------------------
class PropertySet
{
protected:
	Meson::Common::Collections::TStringMap< IProperty* > m_mapProperty;

public:
	PropertySet( void );
	virtual ~PropertySet( void );

	bool ContainsProperty( const Meson::Common::Text::String& p_strName );
	bool ContainsProperty( const Meson::Common::Text::String& p_strName, IProperty*& p_pProperty );

	IProperty* CreateProperty( const Meson::Common::Text::String& p_strName );
	
	template< typename TType > IProperty* CreateReferencedProperty( const Meson::Common::Text::String& p_strName, TType* p_pRef );
	
	IProperty* CreateDelegatedProperty( const Meson::Common::Text::String& p_strName, DelegatedProperty* p_pDelegatedProperty );

	Meson::Common::Collections::TEnumerator<IProperty*>& GetPropertyEnumerator( void );

	IProperty& operator[]( const Meson::Common::Text::String& p_strName );


protected:
	PropertySet& CreatePropertyPath( const Meson::Common::Text::String& p_strName );
};


template< typename TType > IProperty* PropertySet::CreateReferencedProperty( const Meson::Common::Text::String& p_strName, TType* p_pRef )
{
	PropertySet& propertySet = CreatePropertyPath( p_strName );

	IProperty* pProperty = new TReferencedProperty<TType>( p_strName, p_pRef );
	m_mapProperty.Insert( p_strName, pProperty );
	return pProperty;
}


Meson_Common_Properties_END