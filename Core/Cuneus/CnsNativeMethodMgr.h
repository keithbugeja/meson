#pragma once

#include "CnsNamespaces.h"
#include <StringMap.h>
#include <ArrayList.h>

#include "CnsTypeMethod.h"

Meson_Cuneus_BEGIN

class NativeMethodMgr
{
private:
	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > > m_lstMethods;
	Meson::Common::Collections::TStringMap< Meson::Common::TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > > m_mapMethods;

public:
	NativeMethodMgr( void );
	~NativeMethodMgr( void );

	dword AddMethod( Meson::Common::TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > p_pMethod );
	
	dword AddMethod( Meson::Common::Text::String& p_strMethodName, Meson::Cuneus::RTS::Reflection::TypeMethod* p_pMethod );

	Meson::Common::TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > GetMethod( dword p_dwIndex );

	Meson::Common::TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > GetMethod( Meson::Common::Text::String& p_strMethodName );

	dword RegisterMethod( Meson::Common::Text::String& p_strMethodName, void* p_pMethod, dword p_dwReturnTypeCode );

};

Meson_Cuneus_END
