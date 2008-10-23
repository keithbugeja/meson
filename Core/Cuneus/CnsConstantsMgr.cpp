#include "CnsConstantsMgr.h"

Meson_Cuneus_BEGIN

ConstantsMgr::ConstantsMgr(void)
{
}

ConstantsMgr::~ConstantsMgr(void)
{
}

dword ConstantsMgr::AddConstant( Meson::Cuneus::RTS::Typing::DataType* p_pDataType )
{
	dword dwSize = (dword) m_lstDataType.Size();
	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > pDataType( p_pDataType );
	m_lstDataType.Add( pDataType );

	return dwSize;
}

Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > ConstantsMgr::GetConstant( dword p_dwIndex )
{
	//Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > pDataType( m_lstDataType[ p_dwIndex ] );
	//return pDataType;
	return ( m_lstDataType[ p_dwIndex ] );
}

void ConstantsMgr::SetConstant( dword p_dwIndex, Meson::Cuneus::RTS::Typing::DataType* p_pDataType )
{
	m_lstDataType[ p_dwIndex ] = p_pDataType;
}

Meson_Cuneus_END