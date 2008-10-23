#pragma once

#include "CnsDataType.h"

Meson_Cuneus_RTS_Typing_BEGIN

class TypeByte : public DataType
{

private:
	byte m_byValue;

public:
	TypeByte( void );
	TypeByte( byte p_byValue );

	Meson::Common::TPointer< DataType > Clone( void );

	byte GetValue( void );
	void SetValue( byte p_byValue );

	void GetData( byte* p_pbyData );
	void SetData( byte* p_pbyData );

	TypeByte& operator=( byte p_byValue );
	operator byte( void );

	Meson::Common::Text::String ToString( void );

	Meson::Common::TPointer< DataType > operator+( Meson::Common::TPointer< DataType >& p_pDataType );

	Meson::Common::TPointer< DataType > operator-( Meson::Common::TPointer< DataType >& p_pDataType );
	Meson::Common::TPointer< DataType > operator*( Meson::Common::TPointer< DataType >& p_pDataType );
	Meson::Common::TPointer< DataType > operator/( Meson::Common::TPointer< DataType >& p_pDataType );
	Meson::Common::TPointer< DataType > operator%( Meson::Common::TPointer< DataType >& p_pDataType );


	Meson::Common::TPointer< DataType > operator&( Meson::Common::TPointer< DataType >& p_pDataType );
	Meson::Common::TPointer< DataType > operator|( Meson::Common::TPointer< DataType >& p_pDataType );
	Meson::Common::TPointer< DataType > operator^( Meson::Common::TPointer< DataType >& p_pDataType );
	Meson::Common::TPointer< DataType > operator>>( Meson::Common::TPointer< DataType >& p_pDataType );
	Meson::Common::TPointer< DataType > operator<<( Meson::Common::TPointer< DataType >& p_pDataType );

	Meson::Common::TPointer< DataType > operator-( void );
	Meson::Common::TPointer< DataType > operator~( void );

	Meson::Common::TPointer< DataType > operator++( void );
	Meson::Common::TPointer< DataType > operator--( void );

	int Compare( Meson::Common::TPointer< DataType >& p_pDataType );

	virtual void Store( Meson::Common::IO::IOutputStream& p_outStream );
	virtual void Load(  Meson::Common::IO::IInputStream& p_inStream );
};

Meson_Cuneus_RTS_Typing_END
