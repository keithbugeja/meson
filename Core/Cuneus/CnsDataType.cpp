#include "CnsDataType.h"

#include "CnsTypes.h"
#include "CnsAllDataTypes.h"
#include "CnsTypeCodes.h"

#include <MesonException.h>

using namespace Meson::Common::Text;

Meson_Cuneus_RTS_Typing_BEGIN

DataType::DataType( void )
	: Entity()
{
}


DataType::DataType( Meson::Common::Text::String p_strName, dword p_dwTypeCode ) 
	: Entity( p_strName, p_dwTypeCode )
{
}

bool DataType::operator==( const DataType& p_dataType )
{
	return ( this == &p_dataType );
}


DataType* DataType::CreateInstance( void )
{
	return NULL;
}

Meson::Common::TPointer< DataType > DataType::Clone( void )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

void DataType::GetData( byte* p_pbyData )
{
}

void DataType::SetData( byte* p_pbyData )
{
}

//OperationBin( Plus );

	//switch( m_dwTypeCode )
	//{
	//case CNS_TC_VOID:
	//case CNS_TC_BOOLEAN:
	//case CNS_TC_BYTE:
	//case CNS_TC_CHAR:
	//case CNS_TC_WORD:
	//case CNS_TC_SHORT:
	//case CNS_TC_DWORD:
	//case CNS_TC_INTEGER:
	//case CNS_TC_QWORD:
	//case CNS_TC_LONG:
	//case CNS_TC_FLOAT:
	//case CNS_TC_DOUBLE:
	//case CNS_TC_REFERENCE:
	//}


DataType* DataType::Cast( Meson::Common::TPointer< DataType > p_pDataType )
{
	switch( p_pDataType->GetTypeCode() )
	{
		case CNS_TC_VOID:
			return NULL;
		case CNS_TC_BOOLEAN:
			return (TypeBool*) (DataType*) p_pDataType;
		case CNS_TC_BYTE:
			return (TypeByte*) (DataType*) p_pDataType;
		case CNS_TC_CHAR:
			return (TypeChar*) (DataType*) p_pDataType;
		case CNS_TC_WORD:
			return (TypeWord*) (DataType*) p_pDataType;
		case CNS_TC_SHORT:
			return (TypeShort*) (DataType*) p_pDataType;
		case CNS_TC_DWORD:
			return (TypeDWord*) (DataType*) p_pDataType;
		case CNS_TC_INTEGER:
			return (TypeInt*) (DataType*) p_pDataType;
		case CNS_TC_QWORD:
			return (TypeQWord*) (DataType*) p_pDataType;
		case CNS_TC_LONG:
			return (TypeLong*) (DataType*) p_pDataType;
		case CNS_TC_FLOAT:
			return (TypeFloat*) (DataType*) p_pDataType;
		case CNS_TC_DOUBLE:
			return (TypeDouble*) (DataType*) p_pDataType;
		case CNS_TC_REFERENCE:
			return (TypeRef*) (DataType*) p_pDataType;
	}
	return NULL;
}


Meson::Common::TPointer< DataType > DataType::operator+( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}


Meson::Common::TPointer< DataType > DataType::operator-( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

Meson::Common::TPointer< DataType > DataType::operator*( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

Meson::Common::TPointer< DataType > DataType::operator/( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

Meson::Common::TPointer< DataType > DataType::operator%( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

Meson::Common::TPointer< DataType > DataType::operator&( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

Meson::Common::TPointer< DataType > DataType::operator|( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

Meson::Common::TPointer< DataType > DataType::operator^( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

Meson::Common::TPointer< DataType > DataType::operator>>( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

Meson::Common::TPointer< DataType > DataType::operator<<( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

Meson::Common::TPointer< DataType > DataType::operator-( void )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

Meson::Common::TPointer< DataType > DataType::operator~( void )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}



Meson::Common::TPointer< DataType > DataType::operator++( void )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation Inc not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

Meson::Common::TPointer< DataType > DataType::operator--( void )
{
	 Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation Dec not supported on DataType", __FILE__, __LINE__ );
	 throw pExcept;
}


int DataType::Compare( Meson::Common::TPointer< DataType >& p_pDataType )
{
	Meson::Common::MesonException* pExcept = new Meson::Common::MesonException( " Operation not supported on DataType", __FILE__, __LINE__ );
	throw pExcept;
}

void DataType::Store( Meson::Common::IO::IOutputStream& p_outStream )
{
}

void DataType::Load(  Meson::Common::IO::IInputStream& p_inStream )
{
}


Meson_Cuneus_RTS_Typing_END


