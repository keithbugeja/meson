#pragma once

#include "CnsNamespaces.h"
#include <StringMap.h>
#include <ArrayList.h>

#include "CnsDataType.h"

Meson_Cuneus_BEGIN

class CUNEUS_API ConstantsMgr
{
private:
	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > > m_lstDataType;
public:
	ConstantsMgr(void);
	~ConstantsMgr(void);


	dword AddConstant( Meson::Cuneus::RTS::Typing::DataType* p_pDataType );
	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > GetConstant( dword p_dwIndex );
	
	void SetConstant( dword p_dwIndex, Meson::Cuneus::RTS::Typing::DataType* p_pDataType );

};

Meson_Cuneus_END
