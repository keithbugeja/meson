#pragma once

#include "Namespaces.h"
#include "StringMap.h"

Meson_Common_PlugIn_BEGIN

template< typename TPlugInType > class TPlugInTypeManager
{
private:
	Meson::Common::Collections::TStringMap< TPlugInType* > m_mapPlugIn;

public:
	TPlugInTypeManager( void )
	{
	}

	virtual ~TPlugInTypeManager( void )
	{

	}

	void Add( const char *p_szName, TPlugInType *p_pPlugIn )
	{
		m_mapPlugIn[ p_szName ] = p_pPlugIn;
	}
	
	void Remove( const char *p_szName )
	{
		m_mapPlugIn.RemoveKey( p_szName );
	}

	TPlugInType *Get( const char *p_szName )
	{
		return m_mapPlugIn[ p_szName ];
	}


	TPlugInType *operator[]( const char *p_szName )
	{
		return m_mapPlugIn[ p_szName ];
	}
};

Meson_Common_PlugIn_END
