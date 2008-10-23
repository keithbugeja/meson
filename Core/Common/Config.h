#pragma once


#include "Namespaces.h"
#include "Types.h"
#include "Definitions.h"
#include "String.h"
#include "StringMap.h"
#include "Serialisable.h"



Meson_Common_Config_BEGIN



class IProperty
{
protected:
	Meson::Common::Text::String	m_strName,
								m_strType;

public:
	IProperty( void );
	IProperty( const char* p_szName, const char* p_szType );
	~IProperty( void );

	Meson::Common::Text::String& GetName( void );
	void SetName( Meson::Common::Text::String& p_strName );

	Meson::Common::Text::String& GetType( void );

	void SetType( Meson::Common::Text::String& p_strType );
};






//-------------------------------------------------------------------------------------------------


template< typename TType > 
class TProperty 
	: public IProperty
{
protected:
	TType m_value;

public:
	/*static*/ const char* ms_TPropertyType;

public:

	TProperty( const char* p_szName, const char* p_szType ) {}

	inline TType GetValue( void )
	{
		return m_value;
	}

	inline void SetValue( const TType& p_value )
	{
		m_value = p_value;
	}
	
	inline operator TType()
	{
		return GetValue();
	}

	//template< typename T >
	//inline virtual TProperty& operator=( const T& p_value )
	//{
	//	SetValue( p_value );
	//	return *this;
	//}

	//inline TProperty& operator=( const TType& p_value )
	//{
	//	SetValue( p_value );
	//	return *this;
	//}

	//inline TProperty& operator=( const TProperty< TType >& p_value )
	//{
	//	SetValue( p_value.GetValue() );
	//	return *this;
	//}
};



//
//template< typename TType, typename T > 
//TProperty< TType >& TProperty< TType >::operator=( const T& p_value );





//template< typename TType > 
//TProperty< TType >& TProperty< TType >::operator=( const TType& p_value )
//{
//	SetValue( p_value );
//	return *this;
//}



//template< typename TType > const char *TProperty<TType>::ms_TPropertyType;
//
//#define PropertyType( NAME, TYPE, STRTYPE ) typedef TProperty< TYPE > NAME; NAME::ms_TPropertyType = STRTYPE

//-------------------------------------------------------------------------------------------------




class Config
{
public:
	Config(void);
	~Config(void);
};



Meson_Common_Config_END

//template< typename TElement >
//TElement& operator=( TElement& p_Element, const Meson::Common::Config::TProperty< TElement >& p_Prop )
//{
//	p_Element = p_Prop.GetValue();
//	return p_Element;
//}

//template< typename TElement >
//operator TElement( const Meson::Common::Config::TProperty< TElement >& p_Prop )
//{
//	p_Element = p_Prop.GetValue();
//	return p_Element;
//}