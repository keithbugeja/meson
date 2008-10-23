#pragma once

#include "CnsDataType.h"

#include <InputStream.h>
#include <OutputStream.h>

Meson_Cuneus_RTS_Reflection_BEGIN

class TypeField : public Meson::Cuneus::RTS::Typing::Entity
{
private:
	//Meson::Cuneus::RTS::Typing::DataType m_dataType;
	dword m_dwTypeCode;

public:
	TypeField( void );
	~TypeField( void );

	dword GetTypeCode( void );
	void SetTypeCode( dword p_dwTypeCode );

	bool operator==( const TypeField& p_typeField );

	void Load( Meson::Common::IO::IInputStream& p_stream );
	void Store( Meson::Common::IO::IOutputStream& p_stream );
};

Meson_Cuneus_RTS_Reflection_END
