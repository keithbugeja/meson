#pragma once

#include "CnsEntity.h"
#include <Pointer.h>
#include <InputStream.h>
#include <OutputStream.h>

Meson_Cuneus_RTS_Typing_BEGIN

class DataType : public Entity
{
public:
	DataType( void );
	DataType( Meson::Common::Text::String p_strName, dword p_dwTypeCode );

	virtual bool operator==( const DataType& p_dataType );

	virtual DataType* CreateInstance( void );

	virtual Meson::Common::TPointer< DataType > Clone( void );

	virtual void GetData( byte* p_pbyData );
	virtual void SetData( byte* p_pbyData );

	static DataType* Cast( Meson::Common::TPointer< DataType > p_pDataType );

	virtual Meson::Common::TPointer< DataType > operator+( Meson::Common::TPointer< DataType >& p_pDataType );

	virtual Meson::Common::TPointer< DataType > operator-( Meson::Common::TPointer< DataType >& p_pDataType );
	virtual Meson::Common::TPointer< DataType > operator*( Meson::Common::TPointer< DataType >& p_pDataType );
	virtual Meson::Common::TPointer< DataType > operator/( Meson::Common::TPointer< DataType >& p_pDataType );
	virtual Meson::Common::TPointer< DataType > operator%( Meson::Common::TPointer< DataType >& p_pDataType );


	virtual Meson::Common::TPointer< DataType > operator&( Meson::Common::TPointer< DataType >& p_pDataType );
	virtual Meson::Common::TPointer< DataType > operator|( Meson::Common::TPointer< DataType >& p_pDataType );
	virtual Meson::Common::TPointer< DataType > operator^( Meson::Common::TPointer< DataType >& p_pDataType );
	virtual Meson::Common::TPointer< DataType > operator>>( Meson::Common::TPointer< DataType >& p_pDataType );
	virtual Meson::Common::TPointer< DataType > operator<<( Meson::Common::TPointer< DataType >& p_pDataType );

	virtual Meson::Common::TPointer< DataType > operator-( void );
	virtual Meson::Common::TPointer< DataType > operator~( void );

	virtual Meson::Common::TPointer< DataType > operator++( void );
	virtual Meson::Common::TPointer< DataType > operator--( void );

	virtual int Compare( Meson::Common::TPointer< DataType >& p_pDataType );


	virtual void Store( Meson::Common::IO::IOutputStream& p_outStream );
	virtual void Load(  Meson::Common::IO::IInputStream& p_inStream );

};


Meson_Cuneus_RTS_Typing_END
