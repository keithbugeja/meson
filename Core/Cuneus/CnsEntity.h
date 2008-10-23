#pragma once

#include <String8.h>
#include "CnsNamespaces.h"
#include "CnsTypes.h"

using namespace Meson::Common::Text;

Meson_Cuneus_RTS_Typing_BEGIN

class Entity : public Meson::Common::TReferenceCounter< Entity >
{
protected:
	Meson::Common::Text::String m_strName;
	dword  m_dwTypeCode;

public:
	Entity( void );
	Entity( const Entity& p_entity );

	Entity( Meson::Common::Text::String& p_strName, dword p_dwTypeCode );

	virtual ~Entity( void );

public:
	void SetName( Meson::Common::Text::String& p_strName );
	Meson::Common::Text::String GetName( void );

	virtual dword GetTypeCode( void );
	virtual void SetTypeCode( dword p_dwTypeCode );

	virtual dword GetSize( void );
	
	virtual Entity* CreateInstance( void );

	virtual Entity* Clone( bool p_bShallow = true );

	virtual String ToString( void );
};

Meson_Cuneus_RTS_Typing_END
