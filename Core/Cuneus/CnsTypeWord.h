#pragma once

#include "CnsDataType.h"

Meson_Cuneus_RTS_Typing_BEGIN


class CUNEUS_API TypeWord : public DataType
{
private:
	word m_wValue;

public:
	TypeWord( void );
	TypeWord( word p_wValue );

	Meson::Common::TPointer< DataType > Clone( void );

	word GetValue( void );
	void SetValue( word p_wValue );

	void GetData( byte* p_pbyData );
	void SetData( byte* p_pbyData );

	TypeWord& operator=( word p_wValue );
	operator word( void );

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
