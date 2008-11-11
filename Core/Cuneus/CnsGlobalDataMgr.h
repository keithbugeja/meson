#pragma once

#include <ArrayList.h>

#include "CnsNamespaces.h"
#include "CnsDataType.h"


Meson_Cuneus_BEGIN

class CUNEUS_API GlobalDataMgr
{
protected:
	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > >* m_plstData;

public:
	GlobalDataMgr( void );
	~GlobalDataMgr( void );

	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > >* GetDataList( void );
	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > GetDataItem( int p_nIndex );
	int  AddDataItem( Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > p_pDataType );
	void SetDataItem( int p_nIndex, Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > p_pDataType );
};

Meson_Cuneus_END
