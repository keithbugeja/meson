#pragma once

#include <Singleton.h>
#include <String8.h>
#include <StringMap.h>
#include <ArrayList.h>
#include <Pointer.h>

#include "CnsNamespaces.h"
#include "CnsTypes.h"

#include "CnsDataType.h"

Meson_Cuneus_RTS_Typing_BEGIN

class DataTypeMgr : public Meson::Common::TSingleton< DataTypeMgr >
{
	friend class Meson::Common::TSingleton< DataTypeMgr >;

protected:
	Meson::Common::Collections::TArrayList< Meson::Common::TPointer<DataType> > m_lstDataType;
	Meson::Common::Collections::TStringMap< Meson::Common::TPointer<DataType> > m_mapDataType;

protected:
	DataTypeMgr( void );
	~DataTypeMgr( void );

	
public:
	void Startup( void );
	void Shutdown(void );

	void RegisterType( Meson::Common::Text::String p_strName, dword p_dwSize = 0 );
	void RegisterType( DataType& p_dataType );
	void RegisterType( DataType* p_pDataType );

	Meson::Common::TPointer<DataType> Get( dword p_dwTypeCode );
	Meson::Common::TPointer<DataType> Get( Meson::Common::Text::String p_strName );

	dword GetTypeCode( Meson::Common::Text::String p_strName );

	//Meson::Common::TPointer< DataType > CreateDataType( dword p_dwTypeCode );
	DataType* CreateDataType( dword p_dwTypeCode );
	size_t GetDataTypeSize( dword p_dwTypeCode );
};

Meson_Cuneus_RTS_Typing_END
