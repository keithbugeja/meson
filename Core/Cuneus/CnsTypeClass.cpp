#include "CnsTypeClass.h"
#include "CnsTypeObject.h"
#include "CnsClassLoader.h"
#include "CnsLoaderConstants.h"
#include "CnsCuneusEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Cuneus::RTS::Typing;

Meson_Cuneus_RTS_Reflection_BEGIN

TypeClass::TypeClass(void)
{
}

TypeClass::~TypeClass(void)
{
}

dword TypeClass::GetSize( void )
{
	return 0;
}

Entity* TypeClass::Clone( bool p_bShallow )
{
	return NULL;
}

String TypeClass::ToString( void )
{
	return String();
}

TArrayList< TypeField >* TypeClass::GetDataList( void )
{
	return m_plstClassData;
}

TypeField TypeClass::GetDataItem( int p_nIndex )
{
	return (*m_plstClassData)[ p_nIndex ];
}

void TypeClass::SetDataItem( int p_nIndex, TypeField p_dataType )
{
	(*m_plstClassData)[ p_nIndex ] = p_dataType;
}



TPointer< DataType > TypeClass::CreateObject( void )
{
	TPointer< DataType > pObject;
	TPointer< DataType > pDataType;

	pObject = new TypeObject( this );

	if( m_plstClassData )
	{
		DataTypeMgr *pDtMgr = CuneusEngine::GetInstance()->GetDataTypeMgr();

		size_t unLen = m_plstClassData->Size();
		for( size_t un = 0; un < unLen; un++ )
		{
			pDataType = pDtMgr->CreateDataType( (*m_plstClassData)[ un ].GetTypeCode() );
			((TypeObject&)(*pObject)).AddDataItem( pDataType );

			//TODO: Ref Items?
		}
	}

	return pObject;
}


Meson::Common::Collections::TArrayList< Meson::Common::TPointer< TypeMethod > >& TypeClass::GetMethodList( void )
{
	return m_lstMethod;
}

int TypeClass::AddMethod( TypeMethod* p_pMethod )
{
	int nIndex = (int)m_lstMethod.Size();

	TPointer< TypeMethod > pMethod( p_pMethod );
	m_lstMethod.Add( pMethod );

	return nIndex;
}

TypeMethod* TypeClass::GetMethod( int p_nIndex )
{
	return m_lstMethod[ p_nIndex ];
}

void TypeClass::SetMethod( int p_nIndex, TypeMethod* p_pMethod )
{
	TPointer< TypeMethod > pMethod( p_pMethod );
	m_lstMethod[ p_nIndex ] = pMethod;
}


void TypeClass::Load( Meson::Common::IO::IInputStream& p_stream )
{
	byte  byData;
	dword dwData;

	// Load Name
	//SetName( ClassLoader::ReadString( p_stream ) );
	
	//Load Num of Items
	p_stream >> dwData;

	//Foreach Load Item
	for( int n = 0; n < (int)dwData; n++ )
	{
		p_stream >> (char&)byData;
		switch( byData )
		{
			case CNS_LDR_FIELD:
				
				break;

			case CNS_LDR_METHOD:

				break;
		}
	}


	
}

void TypeClass::Store( Meson::Common::IO::IOutputStream& p_stream )
{
	dword dwData = (dword)m_strName.Length();
	p_stream << dwData;
	p_stream.Write( m_strName.ToArray(), dwData );

	if( m_plstClassData != NULL )
	{
		dwData = (dword)m_plstClassData->Size();
		p_stream << dwData;
		
		for( int n = 0; n < (int)dwData; n++ )
		{
			(*m_plstClassData)[ n ].Store(p_stream);
		}
	}
}



Meson_Cuneus_RTS_Reflection_END
