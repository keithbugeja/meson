#pragma once


#include "Namespaces.h"
#include "Types.h"
#include "Pointer.h"
#include "String8.h"
#include "Enumerator.h"
#include "ArrayList.h"
#include "BufferList.h"
#include "LinkedList.h"


Meson_Common_Properties_BEGIN


//---------------------------------------------------------------------------------------------------------
//	PropertyType enumeration of types that can be held in the Properties
//---------------------------------------------------------------------------------------------------------

namespace PropertyType
{ 
	enum PropertyType 
	{ 
		Unknown,
		Bool,
		Char,	Byte,	Word,		Short, 
		DWord,	Int,	Long,		QWord,	LongLong, 
		Float,	Double, LongDouble,
		String,	Reference,
		List,
		Custom
	}; 
}

//---------------------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------
//	TypeOf Helper functions
//	Return PropertyType enumertion member for the dummy value passed as parameter
//---------------------------------------------------------------------------------------------------------

PropertyType::PropertyType TypeOf( bool p_pValue );
PropertyType::PropertyType TypeOf( char p_pValue );
PropertyType::PropertyType TypeOf( byte p_pValue );
PropertyType::PropertyType TypeOf( word p_pValue );
PropertyType::PropertyType TypeOf( short p_pValue );
PropertyType::PropertyType TypeOf( dword p_pValue );
PropertyType::PropertyType TypeOf( int p_pValue );
PropertyType::PropertyType TypeOf( long p_pValue );
PropertyType::PropertyType TypeOf( qword p_pValue );
PropertyType::PropertyType TypeOf( long long p_pValue );
PropertyType::PropertyType TypeOf( float p_pValue );
PropertyType::PropertyType TypeOf( double p_pValue );
PropertyType::PropertyType TypeOf( long double p_pValue );
PropertyType::PropertyType TypeOf( const Meson::Common::Text::String& p_pValue );

template< typename TType > inline PropertyType::PropertyType TypeOf( TType p_pValue )
{ 
	return PropertyType::Custom; 
}

//--- Property Assign and Conversion Definition Macros ----------------------------------------------------

#define CTOR_DEFN( CONTAINER ) template< typename TType > PropertyValue( const Meson::Common::Collections::CONTAINER< TType >& p_lst );

#define ASSIGN_DEFN(CONTAINER) template< typename Type > PropertyValue& operator=( const Meson::Common::Collections::CONTAINER< Type >& p_lstValue );
#define CONV_DEFN(CONTAINER) template< typename Type > operator Meson::Common::Collections::CONTAINER< Type >( void ) const;

//---------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------
//	PropertyValue class
//---------------------------------------------------------------------------------------------------------

class PropertyValue
{

protected:
	PropertyType::PropertyType	m_eType,
								m_eSubType;

	// Union of suppored values
	union
	{
		bool m_bValue;
		byte m_byValue;
		char m_cValue;
		word m_wValue;
		short m_sValue;
		int m_nValue;
		long m_lValue;
		dword m_dwValue;
		long long m_llValue;
		qword m_qwValue;

		float m_fValue;
		double m_dfValue;
		long double m_ldfValue;

		void* m_pValue;
	};


public:

	//--- Construction ----------------------------------------------------------------------------------------
	PropertyValue( void );
	PropertyValue( PropertyType::PropertyType p_eType );

	PropertyValue( bool p_bValue );
	PropertyValue( char p_cValue );
	PropertyValue( byte p_byValue );
	PropertyValue( word p_wValue );
	PropertyValue( short p_sValue );
	PropertyValue( dword p_dwValue );
	PropertyValue( int p_nValue );
	PropertyValue( long p_lValue );
	PropertyValue( qword p_qwValue );
	PropertyValue( long long p_llValue );
	PropertyValue( float p_fValue );
	PropertyValue( double p_dfValue );
	PropertyValue( long double p_ldfValue );
	PropertyValue( const Meson::Common::Text::String& p_Value );
	PropertyValue( void* p_pValue );
	PropertyValue( void* p_pValue, PropertyType::PropertyType p_eRefType );

	// Constructors for List Types
	CTOR_DEFN(TImmutableList)
	CTOR_DEFN(TList)
	CTOR_DEFN(TArrayList)
	CTOR_DEFN(TBufferList)
	CTOR_DEFN(TLinkedList)
	
	PropertyValue( const PropertyValue& p_value );
	
	virtual ~PropertyValue( void );


public:
	//--- Value Copying methods ------------------------------------------------------------------------------

	inline void Copy( const PropertyValue& p_value )
	{
		memcpy( &m_pValue, &(p_value.m_pValue), p_value.Size() );
	}


	inline void CopyReference( const PropertyValue& p_value )
	{
		memcpy( m_pValue, &p_value.m_pValue, p_value.Size() );
	}

	inline void CopyPropertyValue( const PropertyValue& p_value )
	{
		if( IsReference() )
		{
			memcpy( m_pValue, &(p_value.m_pValue), p_value.Size() );
			SetSubType( p_value.GetType() );
		}
		else
			memcpy( this, &p_value, sizeof( PropertyValue ) );
	}

	template< typename TType > inline void CopyValue( TType p_value )
	{
		if( IsReference() )
		{
			memcpy( m_pValue, &p_value, sizeof( TType ) );
			SetSubType( TypeOf( p_value ) );
		}
		else
		{
			memcpy( &m_pValue, &p_value, sizeof( TType ) );
			SetType( TypeOf( p_value ) );
		}
	}

public:	
	void Clear( void );

	//---------------------------------------------------------------------------------------------------------

	inline size_t Size( void ) const
	{
		switch( m_eType )
		{
			case PropertyType::Bool:
				return sizeof( bool );
							
			case PropertyType::Char:
				return sizeof( char );
			
			case PropertyType::Byte:
				return sizeof( byte );
			
			case PropertyType::Word:
				return sizeof( word );
			
			case PropertyType::Short:
				return sizeof( short );
			
			case PropertyType::DWord:
				return sizeof( dword );
			
			case PropertyType::Int:
				return sizeof( int );

			case PropertyType::Long:
				return sizeof( long );

			case PropertyType::QWord:
				return sizeof( qword );
			
			case PropertyType::LongLong:
				return sizeof( long long );
			
			case PropertyType::Float:
				return sizeof( float );
			
			case PropertyType::Double:
				return sizeof( double );
			
			case PropertyType::LongDouble:
				return sizeof( long double );
			
			default:
				return sizeof( void* );
		}
	}


	inline PropertyType::PropertyType GetType( void )  const
	{
		return m_eType;
	}

	inline void SetType( PropertyType::PropertyType p_eType, PropertyType::PropertyType p_eSubType = PropertyType::Unknown ) 
	{
		m_eType = p_eType;
		m_eSubType = p_eSubType;
	}

	inline PropertyType::PropertyType GetSubType( void )  const
	{
		return m_eSubType;
	}

	inline void SetSubType( PropertyType::PropertyType p_eSubType ) 
	{
		m_eSubType = p_eSubType;
	}

	inline bool IsReference( void ) const
	{
		return ( m_eType == PropertyType::Reference );
	}

protected:
	template< typename TType > void StoreList( const Meson::Common::Collections::TImmutableList< TType >& p_lst );

public:
	bool operator==(  const PropertyValue& p_value );

	PropertyValue& operator=( const PropertyValue& p_Value );


	operator bool( void ) const;
	PropertyValue& operator=( const bool& p_bValue );

	operator byte( void ) const;
	PropertyValue& operator=( const byte& p_byValue );

	operator char( void ) const;
	PropertyValue& operator=( const char& p_cValue );

	operator word( void ) const;
	PropertyValue& operator=( const word& p_wValue );

	operator short( void ) const;
	PropertyValue& operator=( const short& p_sValue );

	operator int( void ) const;
	PropertyValue& operator=( const int& p_nValue );

	operator long( void ) const;
	PropertyValue& operator=( const long& p_lValue );

	operator dword( void ) const;
	PropertyValue& operator=( const dword& p_dwValue );

	operator long long( void ) const;
	PropertyValue& operator=( const long long& p_llValue );

	operator qword( void ) const;
	PropertyValue& operator=( const qword& p_qwValue );

	operator float( void ) const;
	PropertyValue& operator=( const float& p_fValue );

	operator double( void ) const;
	PropertyValue& operator=( const double& p_dfValue );

	operator long double( void ) const;
	PropertyValue& operator=( const long double& p_ldfValue );

	operator void*( void ) const;
	PropertyValue& operator=( void* p_pValue );

	ASSIGN_DEFN(TImmutableList)
	ASSIGN_DEFN(TList)
	ASSIGN_DEFN(TArrayList)
	ASSIGN_DEFN(TBufferList)
	ASSIGN_DEFN(TLinkedList)

	CONV_DEFN(TImmutableList)
	CONV_DEFN(TList)
	template< typename TType > inline operator Meson::Common::Collections::TArrayList< TType >&( void );
	CONV_DEFN(TBufferList)
	CONV_DEFN(TLinkedList)



	inline PropertyValue operator[]( int p_nIndex )
	{
		switch( m_eSubType )
		{
			case PropertyType::Bool:
				return PropertyValue( (void*)&(*(Meson::Common::Collections::TArrayList<  bool >*)m_pValue)[ p_nIndex ], m_eSubType );
			
			case PropertyType::Char:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList<  char >*)m_pValue))[ p_nIndex ], m_eSubType );
			
			case PropertyType::Byte:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList<  byte >*)m_pValue))[ p_nIndex ], m_eSubType );
			
			case PropertyType::Word:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList<  word >*)m_pValue))[ p_nIndex ], m_eSubType );
			
			case PropertyType::Short:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList<  short >*)m_pValue))[ p_nIndex ], m_eSubType );
			
			case PropertyType::DWord:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList<  dword >*)m_pValue))[ p_nIndex ], m_eSubType );
			
			case PropertyType::Int:
				return PropertyValue( (void*)&(*(Meson::Common::Collections::TArrayList< int >*)m_pValue)[ p_nIndex ], m_eSubType );
			
			case PropertyType::Long:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList<  long >*)m_pValue))[ p_nIndex ], m_eSubType );
			
			case PropertyType::QWord:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList<  qword >*)m_pValue))[ p_nIndex ], m_eSubType );
			
			case PropertyType::LongLong:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList<  long long >*)m_pValue))[ p_nIndex ], m_eSubType );
			
			case PropertyType::Float:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList<  float >*)m_pValue))[ p_nIndex ], m_eSubType );
			
			case PropertyType::Double:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList<  double >*)m_pValue))[ p_nIndex ], m_eSubType );
			
			case PropertyType::LongDouble:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList<  long double >*)m_pValue))[ p_nIndex ], m_eSubType );
			
			case PropertyType::String:
				return PropertyValue( (void*)&(*((Meson::Common::Collections::TArrayList< Meson::Common::Text::String >*)m_pValue))[ p_nIndex ], m_eSubType );
		}

		return PropertyValue(NULL);
	}

};


#undef CTOR_DEFN
#undef ASSIGN_DEFN
#undef CONV_DEFN

//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------


template< typename TType > void PropertyValue::StoreList( const Meson::Common::Collections::TImmutableList< TType >& p_lst )
{
	m_eType = PropertyType::List;
	Meson::Common::Collections::TArrayList< TType >* pList = new Meson::Common::Collections::TArrayList< TType >( p_lst );
	m_pValue = pList;
	TType dummy = (TType)0;
	m_eSubType = TypeOf( dummy );
}


#define CTOR_IMPL( CONTAINER ) template< typename TType > PropertyValue::PropertyValue( const Meson::Common::Collections::CONTAINER< TType >& p_lst ) \
	{ StoreList( p_lst.AsImmutableList() ); }

CTOR_IMPL(TImmutableList)
CTOR_IMPL(TList)
CTOR_IMPL(TArrayList)
CTOR_IMPL(TBufferList)
CTOR_IMPL(TLinkedList)

#undef CTOR_IMPL


#define PROPERTYVALUE_ASSIGN(CONTAINER) template< typename Type > inline PropertyValue& PropertyValue::operator=( const Meson::Common::Collections::CONTAINER< Type >& p_lstValue ) \
{ \
	Clear(); \
	StoreList( p_lstValue.AsImmutableList() ); \
	return *this; \
}

#define PROPERTYVALUE_CONV(CONTAINER) template< typename Type > inline PropertyValue::operator Meson::Common::Collections::CONTAINER< Type >( void ) const \
{ \
	return Meson::Common::Collections::CONTAINER< Type >( *(Meson::Common::Collections::TImmutableList< Type >*)m_pValue ); \
}


PROPERTYVALUE_ASSIGN(TImmutableList)
PROPERTYVALUE_CONV(TImmutableList)
PROPERTYVALUE_ASSIGN(TList)
PROPERTYVALUE_CONV(TList)
PROPERTYVALUE_ASSIGN(TArrayList)

PROPERTYVALUE_ASSIGN(TBufferList)
PROPERTYVALUE_CONV(TBufferList)
PROPERTYVALUE_ASSIGN(TLinkedList)
PROPERTYVALUE_CONV(TLinkedList)

#undef ASSIGN_DEFN
#undef PROPERTYVALUE_ASSIGN


template< typename TType > inline PropertyValue::operator Meson::Common::Collections::TArrayList< TType >&( void )
{
	return *((Meson::Common::Collections::TArrayList< TType >*)m_pValue );
}

Meson_Common_Properties_END



