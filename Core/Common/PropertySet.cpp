
#include "PropertySet.h"
#include "ArrayList.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::Text;

Meson_Common_Properties_BEGIN


PropertySet::PropertySet( void )
	: m_mapProperty()
{
}

PropertySet::~PropertySet( void )
{
	FreePointers( m_mapProperty );
}


PropertySet& PropertySet::CreatePropertyPath( const Meson::Common::Text::String& p_strName )
{
	IProperty* pProperty = NULL;
	int nIndex = p_strName.LastIndexOf( '.' );

	if( nIndex == -1 )
		return *this;

	TArrayList< String > lstSubKeys;

	// Remove last Key and split string
	p_strName.Substring( 0, nIndex - 1 ).Split( '.', lstSubKeys );

	PropertySet* pSet = this;

	TEnumerator< String >& eKey = lstSubKeys.GetEnumerator();
	while( eKey.HasMoreElements() )
	{
		pProperty = pSet->CreateDelegatedProperty( eKey.NextElement(), NULL );
		pSet = pProperty->GetPropertySet();
	}

	return *pSet;
}


bool PropertySet::ContainsProperty( const Meson::Common::Text::String& p_strName )
{
	return m_mapProperty.ContainsKey( p_strName );
}

bool PropertySet::ContainsProperty( const Meson::Common::Text::String& p_strName, IProperty*& p_pProperty )
{
	return m_mapProperty.ContainsKey( p_strName, p_pProperty );
}

IProperty* PropertySet::CreateProperty( const Meson::Common::Text::String& p_strName )
{
	PropertySet& propertySet = CreatePropertyPath( p_strName );

	IProperty* pProperty = new StoredProperty( p_strName );
	m_mapProperty.Insert( p_strName, pProperty );
	return pProperty;
}

IProperty* PropertySet::CreateDelegatedProperty( const Meson::Common::Text::String& p_strName, DelegatedProperty* p_pDelegatedProperty )
{
	PropertySet& propertySet = CreatePropertyPath( p_strName );

	if( !p_pDelegatedProperty )
		p_pDelegatedProperty = new DelegatedProperty( p_strName );

	m_mapProperty.Insert( p_strName, p_pDelegatedProperty );
	return p_pDelegatedProperty;
}


TEnumerator<IProperty*>& PropertySet::GetPropertyEnumerator( void )
{
	return m_mapProperty.GetValueEnumerator();
}

IProperty& PropertySet::operator[]( const String& p_strName )
{
	IProperty* pProperty = NULL;
	int nIndex = p_strName.IndexOf( '.' );
	if( nIndex == -1 )
	{
		if( ContainsProperty( p_strName, pProperty ) )
			return *pProperty;

		return *CreateProperty( p_strName );
	}
	
	if( ContainsProperty( p_strName.SubstringIndex( 0, nIndex - 1 ), pProperty ) )
	{
		pProperty = &(*pProperty)[ p_strName.SubstringIndex( nIndex + 1, p_strName.Size() - 1 ) ];
	}
	else
	{
		TArrayList< String > lstSubKeys;
		p_strName.Split( '.', lstSubKeys );
		
		PropertySet* pSet = this;

		TEnumerator< String >& eKey = lstSubKeys.GetEnumerator();
		while( eKey.HasMoreElements() )
		{
			pProperty = pSet->CreateProperty( eKey.NextElement() );
			pSet = pProperty->GetPropertySet();
		}
	}

	return *pProperty;
}


Meson_Common_Properties_END
