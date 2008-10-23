#pragma once

#include "CnsDataType.h"
#include "CnsTypeRef.h"


#include <ArrayList.h>

Meson_Cuneus_RTS_Reflection_BEGIN

class TypeClass;

Meson_Cuneus_RTS_Reflection_END


Meson_Cuneus_RTS_Typing_BEGIN


class TypeObject : public DataType
{
private:
	Meson::Cuneus::RTS::Reflection::TypeClass* m_pTypeClass;

	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< DataType > >* m_plstData;
	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< TypeRef  > >* m_plstRef;


public:
	TypeObject( Meson::Cuneus::RTS::Reflection::TypeClass* p_pTypeClass );
	~TypeObject(void);

	dword GetTypeCode( void );

	void SetTypeCode( dword p_dwTypeCode );

	dword GetSize( void );

	Meson::Cuneus::RTS::Reflection::TypeClass* GetClass( void );

	TypeObject* Clone( bool p_bShallow = true );

	Meson::Common::Text::String ToString( void );

	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< DataType > >* GetDataList( void );
	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< TypeRef  > >* GetRefList( void );

	Meson::Common::TPointer< DataType > GetDataItem( int p_nIndex );
	Meson::Common::TPointer< TypeRef  > GetRefItem( int p_nIndex );

	int  AddDataItem( Meson::Common::TPointer< DataType > p_pTypeData );
	int  AddRefItem(  Meson::Common::TPointer< TypeRef  > p_pTypeRef );

	void SetDataItem( int p_nIndex, Meson::Common::TPointer< DataType > p_pTypeData );
	void SetRefItem(  int p_nIndex,  Meson::Common::TPointer< TypeRef  > p_pTypeRef );
};

Meson_Cuneus_RTS_Typing_END