#pragma once


//----------------------------------------------------------------------------------------------
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	05/03/2008	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------


//TODO: Optimisation


#include "Namespaces.h"
#include "Types.h"
#include "String8.h"
#include "Pointer.h"
#include "StringMap.h"
#include "PropertyValue.h"

#pragma region Property

Meson_Common_Properties_BEGIN


// Forward Delaration
class PropertySet;


class IProperty
{

protected:
	Meson::Common::Text::String m_strName;
	PropertySet* m_pPropertySet;

public:
	IProperty( Meson::Common::Text::String p_strName );
	virtual ~IProperty( void );

public:
	PropertySet* GetPropertySet( void );

	virtual PropertyValue& Get( void ) const = 0;
	virtual void Set( PropertyValue p_value ) = 0;

	virtual PropertyValue GetIndexed( int p_nIndex ) = 0;
	virtual void SetIndexed( int p_nIndex, const PropertyValue&  p_value ) = 0;

	IProperty& operator[]( Meson::Common::Text::String p_strName );
	
	inline PropertyValue operator[]( int p_nIndex )
	{
		return Get()[ p_nIndex ];
	}

	inline IProperty& operator=( const PropertyValue p_value )
	{
		Set( p_value );
		return *this;
	}

	template< typename TType > operator TType( void );
	template< typename TType > operator Meson::Common::Collections::TArrayList< TType >&( void );
};


template< typename TType > IProperty::operator TType( void )
{
	return Get();
}

template< typename TType > IProperty::operator Meson::Common::Collections::TArrayList< TType >&( void )
{
	PropertyValue& value = Get();
	return (Meson::Common::Collections::TArrayList< TType >&)value;
}


class StoredProperty
	: public IProperty
{
protected:
	PropertyValue m_propertyValue;

public:
	StoredProperty( Meson::Common::Text::String p_strName );
	~StoredProperty( void );

public:
	PropertyValue& Get( void ) const;
	void Set( PropertyValue  p_value );

	PropertyValue GetIndexed( int p_nIndex );
	void SetIndexed( int p_nIndex, const PropertyValue&  p_value );
};



template< typename TType > 
class TBoundProperty
	: public IProperty
{
protected:
	TType* m_pReference;

public:
	TBoundProperty( Meson::Common::Text::String p_strName, TType* p_pReference )
		: IProperty( p_strName )
		, m_pReference( p_pReference )
	{

	}

	~TBoundProperty( void )
	{
	}

public:
	PropertyValue& Get( void ) const
	{
		return *m_pReference;
	}

	void Set( PropertyValue p_value )
	{
		*m_pReference = p_value;
	}

	PropertyValue GetIndexed( int p_nIndex ) 
	{
		return (*m_pReference)[ p_nIndex ];
	}

	void SetIndexed( int p_nIndex, const PropertyValue&  p_value )
	{
		(*m_pReference)[ p_nIndex ] = p_value;
	}
};



#pragma warning( disable: 4172 )

class DelegatedProperty
	: public IProperty
{

public:
	DelegatedProperty( Meson::Common::Text::String p_strName );
	~DelegatedProperty( void ) {}

public:
	PropertyValue& Get( void ) const { return PropertyValue(NULL); }
	void Set( PropertyValue  p_value ) {}

	PropertyValue GetIndexed( int p_nIndex ) { return PropertyValue(NULL); }
	void SetIndexed( int p_nIndex, const PropertyValue&  p_value ) {}
};

#pragma warning( default: 4172 )

Meson_Common_Properties_END





#pragma endregion