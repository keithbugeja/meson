#pragma once

#include "CnsDataType.h"

Meson_Cuneus_RTS_Typing_BEGIN


class TypeFloat : public DataType
{
private:
	float m_fValue;

public:
	TypeFloat( void );
	TypeFloat( float p_fValue );

	Meson::Common::TPointer< DataType > Clone( void );

	float GetValue( void );
	void SetValue( float p_fValue );

	void GetData( byte* p_pbyData );
	void SetData( byte* p_pbyData );

	TypeFloat& operator=( float p_fValue );
	operator float( void );

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
