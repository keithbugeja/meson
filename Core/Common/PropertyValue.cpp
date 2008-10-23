
#include "PropertyValue.h"
#include <string.h>

using namespace Meson::Common;
using namespace Meson::Common::Text;

Meson_Common_Properties_BEGIN


//---------------------------------------------------------------------------------------------------------

PropertyType::PropertyType TypeOf( bool p_pValue )
{ 
	return PropertyType::Bool; 
}

PropertyType::PropertyType TypeOf( char p_pValue )
{ 
	return PropertyType::Char;
}

PropertyType::PropertyType TypeOf( byte p_pValue )
{ 
	return PropertyType::Byte;
}

PropertyType::PropertyType TypeOf( word p_pValue )
{ 
	return PropertyType::Word;
}

PropertyType::PropertyType TypeOf( short p_pValue )
{ 
	return PropertyType::Short;

}

PropertyType::PropertyType TypeOf( dword p_pValue )
{ 
	return PropertyType::DWord;
}

PropertyType::PropertyType TypeOf( int p_pValue )
{ 
	return PropertyType::Int;
}

PropertyType::PropertyType TypeOf( long p_pValue )
{ 
	return PropertyType::Long;
}

PropertyType::PropertyType TypeOf( qword p_pValue )
{ 
	return PropertyType::QWord;
}

PropertyType::PropertyType TypeOf( long long p_pValue )
{ 
	return PropertyType::LongLong;

}

PropertyType::PropertyType TypeOf( float p_pValue )
{ 
	return PropertyType::Float;
}

PropertyType::PropertyType TypeOf( double p_pValue )
{ 
	return PropertyType::Double;
}

PropertyType::PropertyType TypeOf( long double p_pValue )
{ 
	return PropertyType::LongDouble;

}

PropertyType::PropertyType TypeOf( const String& p_pValue )
{ 
	return PropertyType::String;
}

//---------------------------------------------------------------------------------------------------------




PropertyValue::PropertyValue( void ) 
{
	m_eType = PropertyType::Unknown;
	m_eSubType = PropertyType::Unknown;
	m_pValue = NULL;
}

PropertyValue::PropertyValue( PropertyType::PropertyType p_eType ) 
{
	m_eType = p_eType;
	m_eSubType = PropertyType::Unknown;
	m_pValue = NULL;
}

PropertyValue::PropertyValue( bool p_bValue )
{ 
	m_eType = PropertyType::Bool;
	m_eSubType = PropertyType::Unknown;
	m_bValue = p_bValue; 
}

PropertyValue::PropertyValue( char p_cValue )
{ 
	m_eType = PropertyType::Char;
	m_eSubType = PropertyType::Unknown;
	m_cValue = p_cValue; 
}

PropertyValue::PropertyValue( byte p_byValue )
{ 
	m_eType = PropertyType::Byte;
	m_eSubType = PropertyType::Unknown;
	m_byValue = p_byValue;
}

PropertyValue::PropertyValue( word p_wValue )
{ 
	m_eType = PropertyType::Word;
	m_eSubType = PropertyType::Unknown;
	m_wValue = p_wValue; 
}

PropertyValue::PropertyValue( short p_sValue )
{
	m_eType = PropertyType::Short;
	m_eSubType = PropertyType::Unknown;
	m_sValue = p_sValue; 
}

PropertyValue::PropertyValue( dword p_dwValue )
{ 
	m_eType = PropertyType::DWord;
	m_eSubType = PropertyType::Unknown;
	m_dwValue = p_dwValue; 
}

PropertyValue::PropertyValue( int p_nValue )
{ 
	m_eType = PropertyType::Int;
	m_eSubType = PropertyType::Unknown;
	m_nValue = p_nValue; 
}

PropertyValue::PropertyValue( long p_lValue )
{
	m_eType = PropertyType::Long;
	m_eSubType = PropertyType::Unknown;
	m_lValue = p_lValue; 
}

PropertyValue::PropertyValue( qword p_qwValue )
{ 
	m_eType = PropertyType::QWord;
	m_eSubType = PropertyType::Unknown;
	m_qwValue = p_qwValue; 
}

PropertyValue::PropertyValue( long long p_llValue )
{
	m_eType = PropertyType::LongLong;
	m_eSubType = PropertyType::Unknown;
	m_llValue = p_llValue; 
}

PropertyValue::PropertyValue( float p_fValue )
{ 
	m_eType = PropertyType::Float;
	m_eSubType = PropertyType::Unknown;
	m_fValue = p_fValue; 
}

PropertyValue::PropertyValue( double p_dfValue )
{ 
	m_eType = PropertyType::Double;
	m_eSubType = PropertyType::Unknown;
	m_dfValue = p_dfValue; 
}

PropertyValue::PropertyValue( long double p_ldfValue )
{ 
	m_eType = PropertyType::LongDouble;
	m_eSubType = PropertyType::Unknown;
	m_ldfValue = p_ldfValue; 
}

PropertyValue::PropertyValue( const Meson::Common::Text::String& p_Value )
{ 
	m_eType = PropertyType::String;
	m_eSubType = PropertyType::Unknown;
	m_pValue = (void*) new Meson::Common::Text::String( p_Value ); 
}

PropertyValue::PropertyValue( void* p_pValue )
{ 
	m_eType = PropertyType::Custom;
	m_eSubType = PropertyType::Unknown;
	m_pValue = p_pValue; 
}
	
PropertyValue::PropertyValue( void* p_pValue, PropertyType::PropertyType p_eRefType )
{ 
	m_eType = PropertyType::Reference; 
	m_eSubType = p_eRefType;
	m_pValue = p_pValue; 
}

PropertyValue::PropertyValue( const PropertyValue& p_value )
{
	memcpy( this, &p_value, sizeof( PropertyValue ) );
}

PropertyValue::~PropertyValue(void)
{
	Clear();
}


void PropertyValue::Clear( void )
{
	switch( m_eType )
	{
	case PropertyType::List:
	case PropertyType::String:
		delete m_pValue;
		break;

	}
	
	m_eType = PropertyType::Unknown;
	m_eSubType = PropertyType::Unknown;
}




bool PropertyValue::operator==(  const PropertyValue& p_value )
{
	return memcmp( this, &p_value, sizeof( PropertyValue ) ) == 0;
}

PropertyValue& PropertyValue::operator=( const PropertyValue& p_Value )
{
	CopyPropertyValue( p_Value );
	return *this;
}



PropertyValue::operator bool( void ) const
{ 
	if( IsReference() )
		return *((bool*)m_pValue);
	else
		return m_bValue;
}

PropertyValue& PropertyValue::operator=( const bool& p_bValue ) 
{ 
	CopyValue( p_bValue );
	return *this; 
}


PropertyValue::operator byte( void ) const
{ 
	if( IsReference() )
		return *((byte*)m_pValue);
	else
		return m_byValue; 
}

PropertyValue& PropertyValue::operator=( const byte& p_byValue ) 
{ 
	CopyValue( p_byValue );
	return *this; 
}

PropertyValue::operator char( void ) const 
{ 
	if( IsReference() )
		return *((char*)m_pValue);
	else
		return m_cValue; 
}

PropertyValue& PropertyValue::operator=( const char& p_cValue ) 
{ 
	CopyValue( p_cValue );
	return *this; 
}

PropertyValue::operator word( void ) const 
{ 
	if( IsReference() )
		return *((word*)m_pValue);
	else
		return m_wValue;
}

PropertyValue& PropertyValue::operator=( const word& p_wValue ) 
{ 
	CopyValue( p_wValue );
	return *this; 
}

PropertyValue::operator short( void ) const
{ 
	if( IsReference() )
		return *((short*)m_pValue);
	else
		return m_sValue; 
}

PropertyValue& PropertyValue::operator=( const short& p_sValue ) 
{ 
	CopyValue( p_sValue );
	return *this; 
}

PropertyValue::operator int( void ) const
{
	if( IsReference() )
		return *((int*)m_pValue);
	else
		return m_nValue; 
}

PropertyValue& PropertyValue::operator=( const int& p_nValue ) 
{ 
	CopyValue( p_nValue );
	return *this;
}

PropertyValue::operator long( void ) const
{ 
	if( IsReference() )
		return *((long*)m_pValue);
	else
		return m_lValue;
}

PropertyValue& PropertyValue::operator=( const long& p_lValue )
{ 
	CopyValue( p_lValue );
	return *this; 
}

PropertyValue::operator dword( void ) const
{ 
	if( IsReference() )
		return *((dword*)m_pValue);
	else
		return m_dwValue; 
}

PropertyValue& PropertyValue::operator=( const dword& p_dwValue )
{ 
	CopyValue( p_dwValue );
	return *this; 
}

PropertyValue::operator long long( void ) const
{
	if( IsReference() )
		return *((long long*)m_pValue);
	else
		return m_llValue; 
}

PropertyValue& PropertyValue::operator=( const long long& p_llValue )
{ 
	CopyValue( p_llValue );
	return *this; 
}

PropertyValue::operator qword( void ) const
{ 
	if( IsReference() )
		return *((qword*)m_pValue);
	else
		return m_qwValue; 
}

PropertyValue& PropertyValue::operator=( const qword& p_qwValue )
{
	CopyValue( p_qwValue );
	return *this; 
}

PropertyValue::operator float( void ) const
{ 
	if( IsReference() )
		return *((float*)m_pValue);
	else
		return m_fValue; 
}

PropertyValue& PropertyValue::operator=( const float& p_fValue ) 
{ 
	CopyValue( p_fValue );
	return *this;
}

PropertyValue::operator double( void ) const
{ 
	if( IsReference() )
		return *((double*)m_pValue);
	else
		return m_dfValue; 
}

PropertyValue& PropertyValue::operator=( const double& p_dfValue ) 
{ 
	CopyValue( p_dfValue );
	return *this; 
}

PropertyValue::operator long double( void ) const
{ 
	if( IsReference() )
		return *((long double*)m_pValue);
	else
		return m_ldfValue; 
}

PropertyValue& PropertyValue::operator=( const long double& p_ldfValue )
{ 
	CopyValue( p_ldfValue );
	return *this; 
}

PropertyValue::operator void*( void ) const
{ 
	return m_pValue; 
}

PropertyValue& PropertyValue::operator=( void* p_pValue ) 
{ 
	CopyValue( p_pValue );
	return *this; 
}


Meson_Common_Properties_END
