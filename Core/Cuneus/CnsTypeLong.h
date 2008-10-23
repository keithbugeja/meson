#pragma once

#include "CnsDataType.h"

Meson_Cuneus_RTS_Typing_BEGIN


class TypeLong : public DataType
{
private:
	long long m_llValue;

public:
	TypeLong( void );
	TypeLong( long long p_llValue );

	Meson::Common::TPointer< DataType > Clone( void );

	dword GetSize( void ) { return sizeof( m_llValue ); }

	long long GetValue( void );
	void SetValue( long long p_llValue );

	void GetData( byte* p_pbyData );
	void SetData( byte* p_pbyData );

	TypeLong& operator=( long long p_llValue );
	operator long long( void );

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
