
#include "CnsDataTypeMgr.h"
#include "CnsDataType.h"
#include "CnsAllDataTypes.h"
#include "CnsTypeCodes.h"


using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::Text;


Meson_Cuneus_RTS_Typing_BEGIN


DataTypeMgr::DataTypeMgr(void) 
	: m_lstDataType(), 
	m_mapDataType()
{
	Startup();
}

DataTypeMgr::~DataTypeMgr(void)
{
}

void DataTypeMgr::Startup( void )
{
	m_lstDataType.Clear();
	m_mapDataType.Clear();

	RegisterType( "void",    0 );
	RegisterType( "boolean", sizeof( bool ) );
	RegisterType( "byte",    sizeof( byte ) ); 
	RegisterType( "char",    sizeof( char ) );
	RegisterType( "word",    sizeof( word ) );
	RegisterType( "short",   sizeof( short ) );
	RegisterType( "dword",   sizeof( dword ) );
	RegisterType( "int",     sizeof( int ) );
	RegisterType( "qword",   sizeof( qword ) );
	RegisterType( "long",    sizeof( long long ) );
	RegisterType( "float",   sizeof( float ) );
	RegisterType( "double",  sizeof( double ) );
	RegisterType( "reference", sizeof( void* ) );
	RegisterType( "String",  sizeof( Meson::Common::Text::String* ) );
}

void DataTypeMgr::Shutdown(void )
{

}


void DataTypeMgr::RegisterType( String p_strName, dword p_dwSize )
{
	TPointer<DataType> pDataType;
	
	if( m_mapDataType.ContainsKey( p_strName ) )
		return;
	
	pDataType = new DataType( p_strName, (dword)m_lstDataType.Size() );
	//pDataType->SetSize?
	m_lstDataType.Add( pDataType );
	m_mapDataType.Insert( p_strName, pDataType );
}




void DataTypeMgr::RegisterType( DataType& p_dataType )
{
	RegisterType( p_dataType.GetName(), p_dataType.GetSize() );
}


void DataTypeMgr::RegisterType( DataType* p_pDataType )
{

}

TPointer<DataType> DataTypeMgr::Get( dword p_dwTypeCode )
{
	TPointer<DataType> pDataType;
	pDataType = m_lstDataType[ p_dwTypeCode ];
	return pDataType;
}

TPointer<DataType> DataTypeMgr::Get( String p_strName )
{
	TPointer<DataType> pDataType;

	if( m_mapDataType.ContainsKey( p_strName, pDataType ) )
		return pDataType;

	throw new MesonException( "Type '" + p_strName + "' does not exist!", __FILE__, __LINE__ );	
}


dword DataTypeMgr::GetTypeCode( Meson::Common::Text::String p_strName )
{
	TPointer<DataType> pDataType;

	if( m_mapDataType.ContainsKey( p_strName, pDataType ) )
		return pDataType->GetTypeCode();

	throw new MesonException( "Type '" + p_strName + "' does not exist!", __FILE__, __LINE__ );	
}


//Meson::Common::TPointer< DataType > DataTypeMgr::CreateDataType( dword p_dwTypeCode )
DataType* DataTypeMgr::CreateDataType( dword p_dwTypeCode )
{
	//TPointer<DataType> pDataType;
	DataType* pDataType = NULL;

	//pDataType = m_lstDataType[ p_dwTypeCode ];
	//pDataType = ((DataType*)pDataType)->CreateInstance();

	switch( p_dwTypeCode )
	{
	case CNS_TC_VOID:
		pDataType = new DataType();
		return pDataType;

	case CNS_TC_BOOLEAN:
		pDataType = new TypeBool();
		return pDataType;
	
	case CNS_TC_BYTE:
		pDataType = new TypeByte();
		return pDataType;

	case CNS_TC_CHAR:
		pDataType = new TypeChar();
		return pDataType;

	case CNS_TC_WORD:
		pDataType = new TypeWord();
		return pDataType;

	case CNS_TC_SHORT:
		pDataType = new TypeShort();
		return pDataType;

	case CNS_TC_DWORD:
		pDataType = new TypeDWord();
		return pDataType;

	case CNS_TC_INTEGER:
		pDataType = new TypeInt();
		return pDataType;

	case CNS_TC_QWORD:
		pDataType = new TypeQWord();
		return pDataType;

	case CNS_TC_LONG:
		pDataType = new TypeLong();
		return pDataType;

	case CNS_TC_FLOAT:
		pDataType = new TypeFloat();
		return pDataType;

	case CNS_TC_DOUBLE:
		pDataType = new TypeDouble();
		return pDataType;

	case CNS_TC_REFERENCE:
		pDataType = new TypeRef();
		return pDataType;

	case CNS_TC_STRING:
		pDataType = new TypeString();
		return pDataType;


	}

	return pDataType;
}



size_t DataTypeMgr::GetDataTypeSize( dword p_dwTypeCode )
{
	switch( p_dwTypeCode )
	{
	case CNS_TC_VOID:
		return 0;

	case CNS_TC_BOOLEAN:
		return sizeof(bool);
	
	case CNS_TC_BYTE:
		return sizeof(byte);

	case CNS_TC_CHAR:
		return sizeof(char);

	case CNS_TC_WORD:
		return sizeof(word);

	case CNS_TC_SHORT:
		return sizeof(short);

	case CNS_TC_DWORD:
		return sizeof(dword);

	case CNS_TC_INTEGER:
		return sizeof(int);

	case CNS_TC_QWORD:
		return sizeof(qword);

	case CNS_TC_LONG:
		return sizeof(long long);

	case CNS_TC_FLOAT:
		return sizeof(float);

	case CNS_TC_DOUBLE:
		return sizeof(double);

	case CNS_TC_REFERENCE:
		return sizeof(void*);

	case CNS_TC_STRING:
		return sizeof(String&);


	}

	return 8;
}

Meson_Cuneus_RTS_Typing_END
