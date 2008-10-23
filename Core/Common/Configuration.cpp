
#include "Configuration.h"

#include "Converter.h"
#include "LinkedList.h"
#include "String8.h"
#include "ArrayList.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Serialisation;

#include <sstream>


Meson_Common_Config_BEGIN


Property::Property( void )
{
	m_eType = PropertyType::T_UNKNOWN;
	m_pStrValue = NULL;
	m_bUnlock = true;
	m_bChanged = false;
}

Property::Property( const Property &p_property )
{
	Copy( p_property );
}

Property::~Property( void )
{
	Clear();
}


void Property::Lock( void )
{
	m_bUnlock = false;
}

void Property::Unlock( void )
{
	m_bUnlock = true;
}

bool Property::IsLocked( void )
{
	return !m_bUnlock;
}

bool Property::IsConfiguration( void )
{
	return m_eType == PropertyType::T_CONFIG;
}

bool Property::HasChanged( void )
{
	//if( m_eType == PropertyType::T_CONFIG )
	//	return m_pMapProperty->HasChanged();

	return m_bChanged;
}

PropertyType::PropertyType Property::GetType( void )
{
	return m_eType;
}

void Property::Clear( void )
{
	if( m_eType == PropertyType::T_STRING && m_pStrValue != NULL )
		delete m_pStrValue;

	if( m_eType == PropertyType::T_CONFIG && m_pMapProperty != NULL )
	{
		//m_pMapProperty->ReleasePtrValues();
		delete m_pMapProperty;
	}

	m_pStrValue = NULL;

	m_eType = PropertyType::T_UNKNOWN;
	m_bUnlock = true;
	m_bChanged = true;
}

void Property::Copy( const Property &p_property )
{
	m_eType = p_property.m_eType;
	m_bUnlock = p_property.m_bUnlock;
	
	if( p_property.m_eType == PropertyType::T_STRING )
	{
		if( p_property.m_pStrValue == NULL )
			m_pStrValue = new String( *p_property.m_pStrValue );
		else
		{
			m_pStrValue->Clear();
			m_pStrValue->Append( *p_property.m_pStrValue );
		}
	}
	else
	{
		m_qwValue = p_property.m_qwValue;
	}
	m_bChanged = true;
}

const char *Property::GetName( void )
{
	return m_szName;
}

void Property::SetName( const char *p_szName )
{
	m_szName = p_szName;
}

inline void Property::Set( PropertyType::PropertyType p_eType, qword p_qwValue )
{
	if( !m_bUnlock )
		return;

	if( m_eType != PropertyType::T_STRING )
	{
		Clear();
		m_eType = p_eType;
		m_bChanged = true;
		m_qwValue = p_qwValue;
		return;
	}

	m_eType = p_eType;
	m_bChanged = true;
	if( p_qwValue == 0 )
	{
		m_pStrValue = new String( *((String*)p_qwValue) );
	}
	else
	{
		m_pStrValue->Clear();
		m_pStrValue->Append( *((String*)p_qwValue) );
	}

}

bool Property::GetBool( void )
{
	if( m_eType == PropertyType::T_BOOL )
		return m_byValue!=0;

	if( m_eType == PropertyType::T_STRING )
	{
		m_eType = PropertyType::T_BOOL;
		m_byValue = Converter::ToBool( *m_pStrValue );

		return m_byValue!=0;
	}

	return false;
}

char Property::GetChar( void )
{
	if( m_eType == PropertyType::T_CHAR )
		return m_cValue;

	if( m_eType == PropertyType::T_STRING )
	{
		m_eType = PropertyType::T_CHAR;
		m_cValue = Converter::ToChar( *m_pStrValue );

		return m_cValue;
	}

	return -1;
}

byte Property::GetByte( void )
{
	if( m_eType == PropertyType::T_BYTE )
		return m_byValue;

	if( m_eType == PropertyType::T_STRING )
	{
		m_eType = PropertyType::T_BYTE;
		m_byValue = Converter::ToByte( *m_pStrValue );
		return m_byValue;
	}

	return -1;
}

word Property::GetWord( void )
{
	if( m_eType == PropertyType::T_WORD )
		return m_wValue;

	if( m_eType == PropertyType::T_STRING )
	{
		m_eType = PropertyType::T_WORD;
		m_wValue = Converter::ToWord( *m_pStrValue );
		return m_wValue;
	}

	return -1;
}

short Property::GetShort( void )
{
	if( m_eType == PropertyType::T_SHORT )
		return m_sValue;

	if( m_eType == PropertyType::T_STRING )
	{
		m_eType = PropertyType::T_SHORT;
		m_sValue = Converter::ToShort( *m_pStrValue );
		return m_sValue;
	}

	return -1;
}

dword Property::GetDWord( void )
{
	if( m_eType == PropertyType::T_DWORD )
		return m_dwValue;

	if( m_eType == PropertyType::T_STRING )
	{
		m_eType = PropertyType::T_DWORD;
		m_dwValue = Converter::ToDWord( *m_pStrValue );
		return m_dwValue;
	}

	return -1;
}

long Property::GetLong( void )
{
	if( m_eType == PropertyType::T_LONG )
		return m_lValue;

	if( m_eType == PropertyType::T_STRING )
	{
		m_eType = PropertyType::T_LONG;
		m_lValue = Converter::ToLong( *m_pStrValue );
		return m_lValue;
	}

	return -1;
}



qword Property::GetQWord( void )
{
	if( m_eType == PropertyType::T_QWORD )
		return m_qwValue;

	if( m_eType == PropertyType::T_STRING )
	{
		m_eType = PropertyType::T_QWORD;
		m_qwValue = Converter::ToQWord( *m_pStrValue );
		return m_qwValue;
	}

	return -1;
}

long long Property::GetLongLong( void )
{
	if( m_eType == PropertyType::T_LONGLONG )
		return m_llValue;

	if( m_eType == PropertyType::T_STRING )
	{
		m_eType = PropertyType::T_LONGLONG;
		m_llValue = Converter::ToLongLong( *m_pStrValue );
		return m_llValue;
	}

	return -1;
}

float Property::GetFloat( void )
{
	if( m_eType == PropertyType::T_FLOAT )
		return m_fValue;

	if( m_eType == PropertyType::T_STRING )
	{
		m_eType = PropertyType::T_FLOAT;
		m_fValue = Converter::ToFloat( *m_pStrValue );
		return m_fValue;
	}

	return -1;
}

double Property::GetDouble( void )
{
	if( m_eType == PropertyType::T_DOUBLE )
		return m_dfValue;

	if( m_eType == PropertyType::T_FLOAT)
	{
		m_eType = PropertyType::T_DOUBLE;
		m_dfValue = (double) m_fValue;
		return m_dfValue;
	}

	if( m_eType == PropertyType::T_STRING )
	{
		m_eType = PropertyType::T_DOUBLE;
		m_dfValue = Converter::ToDouble( *m_pStrValue );
		return m_dfValue;
	}

	return -1;
}

String *Property::GetString( void )
{
	if( m_eType == PropertyType::T_STRING )
		return m_pStrValue;

	String *pStr = new String();

	switch( m_eType )
	{
	case PropertyType::T_ERROR: 
		*pStr = "Error"; 
		break;

	case PropertyType::T_UNKNOWN: 
		*pStr = "Unknown"; 
		break;

	case PropertyType::T_BOOL: 
		*pStr = (m_byValue) ? "true" : "false";
		break;

	case PropertyType::T_CHAR: 
		Converter::ToString( m_cValue, *pStr );
		break;

	case PropertyType::T_BYTE: 
		Converter::ToString( m_byValue, *pStr );
		break;

	case PropertyType::T_WORD: 
		Converter::ToString( m_wValue, *pStr );
		break;

	case PropertyType::T_SHORT: 
		Converter::ToString( m_sValue, *pStr );
		break;

	case PropertyType::T_DWORD: 
		Converter::ToString( m_dwValue, *pStr );
		break;

	case PropertyType::T_LONG: 
		Converter::ToString( m_lValue, *pStr );
		break;

	case PropertyType::T_QWORD: 
		Converter::ToString( m_qwValue, *pStr );
		break;

	case PropertyType::T_LONGLONG: 
		Converter::ToString( m_llValue, *pStr );
		break;

	case PropertyType::T_FLOAT: 
		Converter::ToString( m_fValue, *pStr );
		break;

	case PropertyType::T_DOUBLE: 
		Converter::ToString( m_dfValue, *pStr );
		break;

	case PropertyType::T_CONFIG:
		*pStr = "Config"; 
		break;

	}

	m_eType = PropertyType::T_STRING;
	m_pStrValue = pStr;

	return m_pStrValue;
}


Property *Property::GetPropertyNode( const char *p_szName )
{
	Property *pProp = NULL;

	if( !m_pMapProperty )
	{
		m_eType = PropertyType::T_CONFIG;
		m_pMapProperty = new TStringMap<Property*>();
	}

	if (!m_pMapProperty->ContainsKey(p_szName, pProp))
	{
		pProp = new Meson::Common::Config::Property();
		pProp->SetName( p_szName );
		m_pMapProperty->Insert(p_szName, pProp);
	}

	return pProp;
}


Property &Property::GetProperty( const char *p_szName )
{
	Property *pProp = this;

	String strName(p_szName);

	//TODO: leak
	TArrayList< String >* pListName = new TArrayList<String>(8);
	strName.Split( '.', *pListName );

	size_t unLen = pListName->Size();
	for( size_t unPos = 0; unPos < unLen; unPos++ )
	{
		pProp = pProp->GetPropertyNode( (*pListName)[unPos] );
	}

	pProp->SetName( p_szName );
	
	return *pProp;
}


void Property::SetProperty( const char *p_szName, String *p_strValue )
{
	Property &pProp = GetProperty( p_szName );
	pProp.SetString( p_strValue );
}

void Property::SetBool( bool p_bValue )
{
	Set( PropertyType::T_BOOL, p_bValue?true:false );
}


void Property::SetChar( char p_cValue )
{
	Set( PropertyType::T_CHAR, p_cValue );
}

void Property::SetByte( byte p_byValue )
{
	Set( PropertyType::T_BYTE, p_byValue );
}

void Property::SetWord( word p_wValue )
{
	Set( PropertyType::T_WORD, p_wValue );
}

void Property::SetShort( short p_sValue )
{
	Set( PropertyType::T_SHORT, p_sValue );
}

void Property::SetDWord( dword p_dwValue )
{
	Set( PropertyType::T_DWORD, p_dwValue );
}

void Property::SetLong( long p_lValue )
{
	Set( PropertyType::T_LONG, p_lValue );
}

void Property::SetQWord( qword p_qwValue )
{
	Set( PropertyType::T_QWORD, p_qwValue );
}

void Property::SetLongLong( long long p_llValue )
{
	Set( PropertyType::T_LONGLONG, p_llValue );
}

void Property::SetFloat( float p_fValue )
{
	if( m_bUnlock )
	{
		Clear();
		m_eType = PropertyType::T_FLOAT;
		m_fValue = p_fValue;
		m_bChanged = true;
	}
}

void Property::SetDouble( double p_dfValue )
{
	if( m_bUnlock )
	{
		Clear();
		m_eType = PropertyType::T_DOUBLE;
		m_dfValue = p_dfValue;
		m_bChanged = true;
	}
}

void Property::SetString( String *p_strValue )
{
	if( m_bUnlock )
	{
		Clear();
		m_eType = PropertyType::T_STRING;
		m_pStrValue = p_strValue;
		m_bChanged = true;
	}
}


bool Property::Load( ISerialiser& p_serialiser, Meson::Common::IO::IInputStream& p_inputStream )
{
	return true;
}

bool Property::Store( ISerialiser& p_serialiser, Meson::Common::IO::IOutputStream& p_outputStream )
{

	p_serialiser.WriteString( p_outputStream, String(m_szName) );
	p_serialiser.WriteChar( p_outputStream, '=' ); 

	if( m_eType == PropertyType::T_CONFIG )
	{
		p_serialiser.WriteChar( p_outputStream, '{' );

		//TEnumerator< Property* > &enumerator = m_pMapProperty->GetValueList()->GetEnumerator();
		//while( enumerator.HasMoreElements() )
		//{
		//	enumerator.NextElement()->Store( p_serialiser, p_outputStream );
		//}
		
		p_serialiser.WriteChar( p_outputStream, '}' );

	}
	else
		p_serialiser.WriteString( p_outputStream, *GetString() ); 
	return true;
}


void Property::operator=( const Property &p_property )
{
	Copy( p_property );
}

//*************************************************************************************************



Configuration::Configuration(void)
{
	m_root.SetName( "Root" );
}

Configuration::~Configuration(void)
{
}



//Property *Configuration::GetProperty( String &p_strName )
//{
//	return &m_root.GetProperty( p_strName );
//}
//
//const char *Configuration::GetProperty( const char *p_szName )
//{
//	return *m_root.GetProperty( String( p_szName ) ).GetString();
//}

Property *Configuration::Get( String &p_strName )
{
	return &m_root.GetProperty( p_strName );
}

void Configuration::Set( const char *p_szName, const char *p_szValue )
{
	m_root.SetProperty( String( p_szName ), new String(p_szValue) );
}

void Configuration::Set( String &p_strName, String *p_strValue )
{
	m_root.SetProperty( p_strName, p_strValue );
}

Property *Configuration::Get( const char *p_szName )
{
	return &m_root.GetProperty( String( p_szName ) );
}

bool Configuration::Contains( String &p_strName )
{
	Property &prop = m_root.GetProperty( p_strName );
	return prop.GetType() != PropertyType::T_UNKNOWN;
}

bool Configuration::Contains( const char *p_szName )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	return prop.GetType() != PropertyType::T_UNKNOWN;
}


bool Configuration::GetBool( const char *p_szName )
{
	return m_root.GetProperty( String( p_szName ) ).GetBool();
}

char Configuration::GetChar( const char *p_szName )
{
	return m_root.GetProperty( String( p_szName ) ).GetChar();
}

byte Configuration::GetByte( const char *p_szName )
{
	return m_root.GetProperty( String( p_szName ) ).GetByte();
}

word Configuration::GetWord( const char *p_szName )
{
	return m_root.GetProperty( String( p_szName ) ).GetWord();
}

short Configuration::GetShort( const char *p_szName )
{
	return m_root.GetProperty( String( p_szName ) ).GetShort();
}

dword Configuration::GetDWord( const char *p_szName )
{
	return m_root.GetProperty( String( p_szName ) ).GetDWord();
}

long Configuration::GetLong( const char *p_szName )
{
	return m_root.GetProperty( String( p_szName ) ).GetLong();
}

qword Configuration::GetQWord( const char *p_szName )
{
	return m_root.GetProperty( String( p_szName ) ).GetQWord();
}

long long Configuration::GetLongLong( const char *p_szName )
{
	return m_root.GetProperty( String( p_szName ) ).GetLongLong();
}

float Configuration::GetFloat( const char *p_szName )
{
	return m_root.GetProperty( String( p_szName ) ).GetFloat();
}

double Configuration::GetDouble( const char *p_szName )
{
	return m_root.GetProperty( String( p_szName ) ).GetDouble();
}

String Configuration::GetString( const char* p_szName )
{
	return *m_root.GetProperty( String( p_szName ) ).GetString();
}

void Configuration::SetBool( const char *p_szName, bool p_bValue )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	prop.SetBool( p_bValue );
}

void Configuration::SetChar( const char *p_szName, char p_cValue )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	prop.SetChar( p_cValue );
}

void Configuration::SetByte( const char *p_szName, byte p_byValue )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	prop.SetByte( p_byValue );
}

void Configuration::SetWord( const char *p_szName, word p_wValue )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	prop.SetWord( p_wValue );
}

void Configuration::SetShort( const char *p_szName, short p_sValue )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	prop.SetShort( p_sValue );
}

void Configuration::SetDWord( const char *p_szName, dword p_dwValue )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	prop.SetDWord( p_dwValue );
}

void Configuration::SetLong( const char *p_szName, long p_lValue )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	prop.SetLong( p_lValue );
}

void Configuration::SetQWord( const char *p_szName, qword p_qwValue )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	prop.SetQWord( p_qwValue );
}

void Configuration::SetLongLong( const char *p_szName, long long p_llValue )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	prop.SetLongLong( p_llValue );
}

void Configuration::SetFloat( const char *p_szName, float p_fValue )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	prop.SetFloat( p_fValue );
}

void Configuration::SetDouble( const char *p_szName, double p_dfValue )
{
	Property &prop = m_root.GetProperty( String( p_szName ) );
	prop.SetDouble( p_dfValue );
}


bool Configuration::Load( ISerialiser& p_serialiser, Meson::Common::IO::IInputStream& p_inputStream )
{
	return m_root.Load( p_serialiser, p_inputStream );
}

bool Configuration::Store( ISerialiser& p_serialiser, Meson::Common::IO::IOutputStream& p_outputStream )
{
	return m_root.Store( p_serialiser, p_outputStream );
}

Meson_Common_Config_END
