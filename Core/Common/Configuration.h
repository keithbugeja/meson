#pragma once

#include "Namespaces.h"
#include "String.h"
#include "StringMap.h"
#include "Serialisable.h"



Meson_Common_Config_BEGIN

namespace PropertyType
{ 
	enum PropertyType 
	{ 
		T_ERROR,	T_UNKNOWN,	T_BOOL,
		T_CHAR,		T_BYTE,		T_WORD,		T_SHORT, 
		T_DWORD,	T_LONG,		T_QWORD,	T_LONGLONG, 
		T_FLOAT,	T_DOUBLE, 
		T_STRING,	T_CONFIG 
	}; 
}



class Property : public Meson::Common::Serialisation::ISerialisable
{

protected:
	const char *m_szName;
	PropertyType::PropertyType m_eType;
	
	union
	{
		char		m_cValue;
		byte		m_byValue;
		word		m_wValue;
		short		m_sValue;
		dword		m_dwValue;
		long		m_lValue;
		qword		m_qwValue;
		long long	m_llValue;
		float		m_fValue;
		double		m_dfValue;

		Meson::Common::Text::String *m_pStrValue;
		Meson::Common::Collections::TStringMap<Property*> *m_pMapProperty;
	};
	
	struct
	{
		byte m_bUnlock : 1;
		byte m_bChanged : 1;
	};

public:
	Property( void );
	Property( const Property &p_property );
	~Property( void );

	void Lock( void );
	void Unlock( void );
	bool IsLocked( void );

	bool IsConfiguration( void );

	bool HasChanged( void );

	void Clear( void );
	void Copy( const Property &p_property );

	const char *GetName( void );
	void SetName(const char *);

	PropertyType::PropertyType GetType( void );

	bool GetBool( void );
	char GetChar( void );
	byte GetByte( void );
	word GetWord( void );
	short GetShort( void );
	dword GetDWord( void );
	long GetLong( void );
	qword GetQWord( void );
	long long GetLongLong( void );
	float GetFloat( void );
	double GetDouble( void );
	Meson::Common::Text::String *GetString( void );
	Property *GetPropertyNode( const char *p_szName );
	Property &GetProperty( const char *p_szName );
	void SetProperty( const char *p_szName, Meson::Common::Text::String *p_strValue );

	void SetBool( bool p_bValue );
	void SetChar( char p_cValue );
	void SetByte( byte p_byValue );
	void SetWord( word p_wValue );
	void SetShort( short p_sValue );
	void SetDWord( dword p_dwValue );
	void SetLong( long p_lValue );
	void SetQWord( qword p_qwValue );
	void SetLongLong( long long p_llValue );
	void SetFloat( float p_fValue );
	void SetDouble( double p_dfValue );
	void SetString( Meson::Common::Text::String *p_strValue );

	bool Load( Meson::Common::Serialisation::ISerialiser& p_serialiser, Meson::Common::IO::IInputStream& p_inputStream  );
	bool Store( Meson::Common::Serialisation::ISerialiser& p_serialiser, Meson::Common::IO::IOutputStream& p_outputStream );

	void operator=( const Property &p_property );

private:
	inline void Set( PropertyType::PropertyType  p_eType, qword p_llValue );
};




class Configuration : public Meson::Common::Serialisation::ISerialisable
{
private:
	Property m_root;

public:
	Configuration(void);
	virtual ~Configuration(void);

	void Set( const char* p_szName, const char* p_szValue );
	void Set( Meson::Common::Text::String& p_strName, Meson::Common::Text::String* p_strValue );
	Property *Get( Meson::Common::Text::String& p_strName );
	Property *Get( const char* p_szName );

	bool Contains( Meson::Common::Text::String &p_strName );
	bool Contains( const char *p_szName );

	bool GetBool( const char* p_szName );
	char GetChar( const char* p_szName );
	byte GetByte( const char* p_szName );
	word GetWord( const char* p_szName );
	short GetShort( const char* p_szName );
	dword GetDWord( const char* p_szName );
	long GetLong( const char* p_szName );
	qword GetQWord( const char* p_szName );
	long long GetLongLong( const char* p_szName );
	float GetFloat( const char* p_szName );
	double GetDouble( const char* p_szName );
	Meson::Common::Text::String GetString( const char* p_szName );

	void SetBool( const char* p_szName, bool p_bValue );
	void SetChar( const char* p_szName, char p_cValue );
	void SetByte( const char* p_szName, byte p_byValue );
	void SetWord( const char* p_szName, word p_wValue );
	void SetShort( const char* p_szName, short p_sValue );
	void SetDWord( const char* p_szName, dword p_dwValue );
	void SetLong( const char* p_szName, long p_lValue );
	void SetQWord( const char* p_szName, qword p_qwValue );
	void SetLongLong( const char* p_szName, long long p_llValue );
	void SetFloat( const char* p_szName, float p_fValue );
	void SetDouble( const char* p_szName, double p_dfValue );

	bool Load( Meson::Common::Serialisation::ISerialiser& p_serialiser, Meson::Common::IO::IInputStream& p_inputStream );
	bool Store( Meson::Common::Serialisation::ISerialiser& p_serialiser, Meson::Common::IO::IOutputStream& p_outputStream );
};

Meson_Common_Config_END

