#include "CnsGlobalDataMgr.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Cuneus::RTS::Typing;

Meson_Cuneus_BEGIN

GlobalDataMgr::GlobalDataMgr(void)
{
	m_plstData = new TArrayList< TPointer< DataType > >();
}

GlobalDataMgr::~GlobalDataMgr(void)
{
	delete m_plstData;
}


TArrayList< TPointer< DataType > >* GlobalDataMgr::GetDataList( void )
{
	return m_plstData;
}

TPointer< DataType > GlobalDataMgr::GetDataItem( int p_nIndex )
{
	return (*m_plstData)[ p_nIndex ];
}

int GlobalDataMgr::AddDataItem( TPointer< DataType > p_pDataType )
{
	int n = (int) m_plstData->Size();

	(*m_plstData).Add( p_pDataType );
	return n;
}


void GlobalDataMgr::SetDataItem( int p_nIndex, TPointer< DataType > p_pDataType )
{
	(*m_plstData)[ p_nIndex ] = p_pDataType;
}


Meson_Cuneus_END
