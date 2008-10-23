#pragma once

#include "CnsDataType.h"
#include "CnsTypeField.h"
#include "CnsTypeMethod.h"
#include "CnsTypeObject.h"

#include <ArrayList.h>
#include <InputStream.h>
#include <OutputStream.h>




Meson_Cuneus_RTS_Reflection_BEGIN

class TypeClass : public Meson::Cuneus::RTS::Typing::DataType
{

protected:
	Meson::Common::Collections::TArrayList< TypeField >*  m_plstClassData;
	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< TypeMethod > > m_lstMethod;

public:
	TypeClass(void);
	~TypeClass(void);

	dword GetSize( void );

	Entity* Clone( bool p_bShallow = true );

	String ToString( void );

	Meson::Common::Collections::TArrayList< TypeField >* GetDataList( void );
	TypeField GetDataItem( int p_nIndex );
	void SetDataItem( int p_nIndex, TypeField p_dataType );

	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > CreateObject( void );

	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< TypeMethod > >& GetMethodList( void );
	int AddMethod( TypeMethod* p_pMethod );
	TypeMethod* GetMethod( int p_nIndex );
	void SetMethod( int p_nIndex, TypeMethod* p_pMethod );

	void Load( Meson::Common::IO::IInputStream& p_stream );
	void Store( Meson::Common::IO::IOutputStream& p_stream );
};

Meson_Cuneus_RTS_Reflection_END
