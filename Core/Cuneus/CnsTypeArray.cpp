#include "CnsTypeArray.h"

#include "CnsTypeCodes.h"
#include "CnsAllDataTypes.h"

#include "CnsCuneusEngine.h"

#include <MesonException.h>

Meson_Cuneus_RTS_Typing_BEGIN

TypeArray::TypeArray( dword p_dwSize, dword p_dwElementTypeCode ) 
	: DataType( "Array", CNS_TC_ARRAY )
	, m_dwSize( p_dwSize )
	, m_dwElementTypeCode( p_dwElementTypeCode )
{
	m_dwElementSize = (dword)CuneusEngine::GetInstance()->GetDataTypeMgr()->GetDataTypeSize( p_dwElementTypeCode );

	m_pbyData = new byte[ m_dwElementSize * m_dwSize ];

	m_bAutoRelease = true;
}

TypeArray::TypeArray( dword p_dwSize, dword p_dwElementTypeCode, void* p_pbyData )
	: DataType( "Array", CNS_TC_ARRAY )
	, m_dwSize( p_dwSize )
	, m_dwElementTypeCode( p_dwElementTypeCode )
{
	m_dwElementSize = (dword)CuneusEngine::GetInstance()->GetDataTypeMgr()->GetDataTypeSize( p_dwElementTypeCode );

	m_pbyData = (byte*)p_pbyData;

	m_bAutoRelease = false;
}

TypeArray::~TypeArray(void)
{
	if( m_pbyData && m_bAutoRelease )
		delete m_pbyData;
}

dword TypeArray::GetSize( void )
{
	return m_dwSize;
}

dword TypeArray::GetElementTypeCode( void )
{
	return m_dwElementTypeCode;
}


Meson::Common::TPointer<DataType> TypeArray::GetElement( dword p_dwIndex )
{
	Meson::Common::TPointer<DataType> pDataType;

	pDataType = CuneusEngine::GetInstance()->GetDataTypeMgr()->CreateDataType( m_dwElementTypeCode );
	
	pDataType->SetData( m_pbyData + (m_dwElementSize * p_dwIndex) );

	return pDataType;
}

void TypeArray::SetElement( dword p_dwIndex, Meson::Common::TPointer<DataType>& p_dataType )
{
	p_dataType->GetData( m_pbyData + (m_dwElementSize * p_dwIndex) );
}


Meson_Cuneus_RTS_Typing_END
