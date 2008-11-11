#pragma once

#include "CnsDataType.h"

#include <Pointer.h>
#include <InputStream.h>
#include <OutputStream.h>

Meson_Cuneus_RTS_Reflection_BEGIN

class CUNEUS_API TypeParam : public Meson::Cuneus::RTS::Typing::Entity
{
private:
	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > m_pDataType;
	dword m_dwTypeCode;

public:
	TypeParam( void );
	~TypeParam( void );

	dword GetTypeCode( void );
	void SetTypeCode( dword p_dwTypeCode );

	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > GetValue( void );
	void  SetValue( Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > p_pDataType );

	bool operator==( const TypeParam& TypeParam );

	void Load( Meson::Common::IO::IInputStream& p_stream );
	void Store( Meson::Common::IO::IOutputStream& p_stream );
};

Meson_Cuneus_RTS_Reflection_END
