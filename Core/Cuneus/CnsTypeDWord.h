#pragma once

#include "CnsDataType.h"

Meson_Cuneus_RTS_Typing_BEGIN


class TypeDWord : public DataType
{
private:
	dword m_dwValue;

public:
	TypeDWord( void );
	TypeDWord( dword p_dwValue );

	Meson::Common::TPointer< DataType > Clone( void );

	dword GetValue( void );
	void SetValue( dword p_dwValue );

	void GetData( byte* p_pbyData );
	void SetData( byte* p_pbyData );

	TypeDWord& operator=( dword p_dwValue );
	operator dword( void );

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
