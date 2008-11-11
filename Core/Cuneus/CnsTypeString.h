#pragma once

#include "CnsDataType.h"

Meson_Cuneus_RTS_Typing_BEGIN


class CUNEUS_API TypeString : public DataType
{
private:
	Meson::Common::Text::String m_strValue;

public:
	TypeString( void );
	TypeString( Meson::Common::Text::String& p_str );

	~TypeString( void );

	Meson::Common::TPointer< DataType > Clone( void );

	dword GetSize( void ) { return (dword) m_strValue.Size(); }

	String& GetValue( void );
	void SetValue( Meson::Common::Text::String p_strValue );

	void GetData( byte* p_pbyData );
	void SetData( byte* p_pbyData );

	TypeString& operator=( Meson::Common::Text::String p_strValue );
	operator Meson::Common::Text::String( void );

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
