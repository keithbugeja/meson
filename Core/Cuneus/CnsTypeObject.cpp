#include "CnsTypeObject.h"
#include "CnsTypeClass.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Cuneus::RTS::Reflection;

Meson_Cuneus_RTS_Typing_BEGIN

TypeObject::TypeObject( TypeClass* p_pTypeClass )
	: DataType( p_pTypeClass->GetName(), 0 )
	, m_pTypeClass( p_pTypeClass )
{
	m_plstData = new TArrayList< TPointer < DataType > >();
	m_plstRef = new TArrayList< TPointer < TypeRef > >();
}

TypeObject::~TypeObject( void )
{
	delete m_plstData;
	delete m_plstRef;
}

dword TypeObject::GetTypeCode( void )
{
	return m_pTypeClass->GetTypeCode();
}

void TypeObject::SetTypeCode( dword p_dwTypeCode )
{
	m_pTypeClass->SetTypeCode( p_dwTypeCode );
}

TypeClass* TypeObject::GetClass( void )
{
	return m_pTypeClass;
}

dword TypeObject::GetSize( void )
{
	return m_pTypeClass->GetSize();
}

TypeObject* TypeObject::Clone( bool p_bShallow )
{
	return NULL;
}


String TypeObject::ToString( void )
{
	String str = m_pTypeClass->ToString();
	str += '@';
	str += (long long)this;
	return str;
}

TArrayList< TPointer< DataType > >* TypeObject::GetDataList( void )
{
	return m_plstData;
}

TArrayList< TPointer< TypeRef > >* TypeObject::GetRefList( void )
{
	return m_plstRef;
}

TPointer< DataType > TypeObject::GetDataItem( int p_nIndex )
{
	return (*m_plstData)[ p_nIndex ];
}

TPointer< TypeRef > TypeObject::GetRefItem( int p_nIndex )
{
	return (*m_plstRef)[ p_nIndex ];
}

int TypeObject::AddDataItem( TPointer< DataType > p_pTypeData )
{
	int n = (int) m_plstData->Size();
	m_plstData->Add( p_pTypeData );
	return n;
}

int TypeObject::AddRefItem( TPointer< TypeRef  > p_pTypeRef )
{
	int n = (int)m_plstRef->Size();
	m_plstRef->Add( p_pTypeRef );
	return n;
}

void TypeObject::SetDataItem( int p_nIndex, TPointer< DataType > p_typeData )
{
	(*m_plstData)[ p_nIndex ] = p_typeData;
}

void TypeObject::SetRefItem( int p_nIndex, TPointer< TypeRef > p_typeRef )
{
	(*m_plstRef)[ p_nIndex ] = p_typeRef;
}

Meson_Cuneus_RTS_Typing_END
