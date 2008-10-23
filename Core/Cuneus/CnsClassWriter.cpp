#include "CnsClassWriter.h"

#include "CnsClassConstants.h"
#include "CnsAllDataTypes.h"
#include "CnsTypeCodes.h"

#include <iostream>

using namespace Meson::Common;
using namespace Meson::Common::Text;
//using namespace Meson::Cuneus::RTS::Reflection;
using namespace Meson::Cuneus::RTS::Typing;

using namespace Meson::Cuneus::Compiler::ParserNodes;

Meson_Cuneus_RTS_BEGIN

ClassWriter::ClassWriter( void )
{
}

ClassWriter::~ClassWriter( void )
{
}


dword ClassWriter::InsertStringConstant( String& p_str )
{
	dword dw;
	
	if( !m_mapString.ContainsKey( p_str, dw ) )
	{
		dw = (dword) m_mapString.Size();
		m_mapString.Insert( p_str, dw );
	}
	return dw;
}

dword ClassWriter::GetStringConstant( String& p_str )
{
	dword dw = 0xFFFFFFFF;
	m_mapString.ContainsKey( p_str, dw );

	return dw;
}


dword ClassWriter::GetByteCodeOffset( void )
{
	return (dword) m_lstByteCode.Size();
}

dword ClassWriter::AddByteCode( byte p_byCode )
{
	dword dw = (dword) m_lstByteCode.Size();
	m_lstByteCode.Add( p_byCode );

	return dw;
}

dword ClassWriter::AddByteCodeW( word p_wCode )
{
	dword dw = (dword) m_lstByteCode.Size();
	m_lstByteCode.Add( (byte)p_wCode & 0xFF );
	m_lstByteCode.Add( (byte)( ( p_wCode >> 8 ) & 0xFF ) );
	return dw;
}

dword ClassWriter::AddByteCodeD( dword p_dwCode )
{
	dword dw = (dword) m_lstByteCode.Size();
	m_lstByteCode.Add( (byte)( p_dwCode & 0xFF ) );
	m_lstByteCode.Add( (byte)( ( p_dwCode >> 8 ) & 0xFF  ) );
	m_lstByteCode.Add( (byte)( ( p_dwCode >> 16 ) & 0xFF ) );
	m_lstByteCode.Add( (byte)( ( p_dwCode >> 24 ) & 0xFF ) );
	return dw;
}

dword ClassWriter::AddByteCodeP( byte* p_pbyCode, dword p_dwLen )
{
	dword dw = (dword) m_lstByteCode.Size();
	
	while( p_dwLen > 0 )
	{
		m_lstByteCode.Add( *p_pbyCode );
		p_pbyCode++;
		p_dwLen--;
	}
	return dw;
}

dword ClassWriter::AddByteCodeN( int p_nRelativeAddress )
{
	return AddByteCodeD( *((dword*)&p_nRelativeAddress) );
}

dword ClassWriter::Skip( int p_nNum )
{
	while( p_nNum-- )
		AddByteCodeD( 0 );
	
	return GetByteCodeOffset();
}


void ClassWriter::SetByteCode( dword p_dwAddress, byte p_byCode )
{
	m_lstByteCode[ p_dwAddress ] = p_byCode;
}

void ClassWriter::SetByteCodeW( dword p_dwAddress, word p_wCode )
{
	m_lstByteCode[ p_dwAddress ] = ( (byte)p_wCode & 0xFF );
	m_lstByteCode[ p_dwAddress + 1 ] = ( (byte)( ( p_wCode >> 8 ) & 0xFF ) );
}

void ClassWriter::SetByteCodeD( dword p_dwAddress, dword p_dwCode )
{
	m_lstByteCode[ p_dwAddress ] = ( (byte)( p_dwCode & 0xFF ) );
	m_lstByteCode[ p_dwAddress + 1 ] = ( (byte)( ( p_dwCode >> 8 ) & 0xFF ) );
	m_lstByteCode[ p_dwAddress + 2 ] = ( (byte)( ( p_dwCode >> 16 ) & 0xFF ) );
	m_lstByteCode[ p_dwAddress + 3 ] = ( (byte)( ( p_dwCode >> 24 ) & 0xFF ) );
}

void ClassWriter::SetByteCodeN( dword p_dwAddress, int p_nRelativeAddress )
{
	dword dwCode = *((dword*)&p_nRelativeAddress);

	m_lstByteCode[ p_dwAddress ] = ( (byte)( dwCode & 0xFF ) );
	m_lstByteCode[ p_dwAddress + 1 ] = ( (byte)( ( dwCode >> 8 ) & 0xFF ) );
	m_lstByteCode[ p_dwAddress + 2 ] = ( (byte)( ( dwCode >> 16 ) & 0xFF ) );
	m_lstByteCode[ p_dwAddress + 3 ] = ( (byte)( ( dwCode >> 24 ) & 0xFF ) );
}



void ClassWriter::SetByteCodeP( dword p_dwAddress, byte* p_pbyCode, dword p_dwLen )
{
	while( p_dwLen > 0 )
	{
		m_lstByteCode[ p_dwAddress ] = *p_pbyCode;
		p_dwAddress++;
		p_pbyCode++;
		p_dwLen--;
	}
}

int ClassWriter::GetAddressDifference( dword p_dwAddress )
{
	return (int) ( (dword) m_lstByteCode.Size() - p_dwAddress );
}

dword ClassWriter::AddConstant( Meson::Cuneus::RTS::Typing::DataType* p_pDataType )
{
	dword dw = (dword) m_lstConstants.Size();

	m_lstConstants.Add( p_pDataType );

	return dw;
}

Meson::Cuneus::RTS::Typing::DataType* ClassWriter::GetConstant( dword p_dwIndex )
{
	return m_lstConstants[ p_dwIndex ];
}



dword ClassWriter::AddField( Variable* p_pField )
{
	dword dw = (dword) m_lstField.Size();
	m_lstField.Add( p_pField );

	return dw;
}

Variable* ClassWriter::GetField( dword p_dwIndex )
{
	Variable* pField = NULL;

	if( m_lstField.Size() > p_dwIndex )
		return m_lstField[ p_dwIndex ];

	return pField;
}


dword ClassWriter::AddMethod( MethodNode* p_pMethod )
{
	dword dw = (dword) m_lstMethod.Size();
	m_lstMethod.Add( p_pMethod );

	return dw;
}

MethodNode* ClassWriter::GetMethod( dword p_dwIndex )
{
	MethodNode* pMethod = NULL;

	if( m_lstMethod.Size() > p_dwIndex )
		return m_lstMethod[ p_dwIndex ];

	return pMethod;
}



dword ClassWriter::AddClass( ClassNode* p_pClass )
{
	dword dw = (dword) m_lstClass.Size();
	m_lstClass.Add( p_pClass );

	return dw;
}

ClassNode* ClassWriter::GetClass( dword p_dwIndex )
{
	ClassNode* pClass = NULL;

	if( m_lstClass.Size() > p_dwIndex )
		return m_lstClass[ p_dwIndex ];

	return pClass;
}


dword ClassWriter::AddNamespace( NamespaceNode* p_pNamespace )
{
	dword dw = (dword) m_lstNamespace.Size();
	m_lstNamespace.Add( p_pNamespace );

	return dw;
}

NamespaceNode* ClassWriter::GetNamespace( dword p_dwIndex )
{
	NamespaceNode* pNamespace = NULL;

	if( m_lstNamespace.Size() > p_dwIndex )
		return m_lstNamespace[ p_dwIndex ];

	return pNamespace;
}

//--- Writer Helper Functions -----------------------------------------------------------------------------

void WriteString( Meson::Common::IO::IOutputStream& p_outStream, String& p_str )
{
	//p_outStream << (dword) p_str.Size();
	p_outStream << p_str;
}

void WriteDataType( Meson::Common::IO::IOutputStream& p_outStream, DataType* p_pDataType )
{
	p_outStream << p_pDataType->GetTypeCode();
	p_pDataType->Store( p_outStream );
	std::cout << p_pDataType->ToString() << std::endl;
}


void WriteField( Meson::Common::IO::IOutputStream& p_outStream, Variable* p_pVariable )
{
	p_outStream << p_pVariable->GetTypeCode();

	//if( p_pVariable->GetTypeCode() == CNS_TC_ARRAY )
	//{
	//	ArrayNode* pArray = (ArrayNode*) p_pVariable->GetType();
	//	p_outStream << pArray->GetTypeCode();
	//}

	p_outStream << p_pVariable->GetModifierCode();
	p_outStream << p_pVariable->GetQualifiedName();

	std::cout << p_pVariable->ToString() << std::endl;
}

void WriteMethod( Meson::Common::IO::IOutputStream& p_outStream, MethodNode* p_pMethod )
{
	p_outStream << p_pMethod->GetTypeCode();
	p_outStream << p_pMethod->GetModifierCode();
	p_outStream << p_pMethod->GetQualifiedName();
	p_outStream << p_pMethod->GetByteCodeOffset();


	ParserNode* pParams = p_pMethod->GetChild( 0 );
	dword dwNum = (dword) pParams->NumberOfChildren();
	std::cout << dwNum << std::endl;
	
	if( dwNum > 0 )
	{
		p_outStream << dwNum;
		for( dword dw=0; dw < dwNum; dw++ )
		{
			p_outStream << (*pParams)[ dw ]->GetTypeCode();
			p_outStream << (*pParams)[ dw ]->GetName();
		}
	}
	else
		p_outStream << (dword)0;

	std::cout << p_pMethod->ToString() << std::endl;
}



//--- Writer Helper Functions -----------------------------------------------------------------------------

bool ClassWriter::Write( Meson::Common::IO::IOutputStream& p_outStream )
{
	bool bWritten = false;
	dword dwLoop = 0;

	p_outStream << CNS_CNX_MAGIC;

	//Namespaces
	dword dwSize = (dword)m_lstNamespace.Size();
	p_outStream << dwSize;
	
	for( dwLoop=0; dwLoop < dwSize; dwLoop++ )
	{
		 WriteString( p_outStream, m_lstNamespace[ dwLoop ]->GetQualifiedName() );
	}

	//Constants
	dwSize = (dword)m_lstConstants.Size();
	p_outStream << dwSize;
	
	std::cout << "Constants" << std::endl;

	for( dwLoop=0; dwLoop < dwSize; dwLoop++ )
	{
		WriteDataType( p_outStream, m_lstConstants[ dwLoop ] );
	}

	//Fields
	dwSize = (dword)m_lstField.Size();
	p_outStream << dwSize;
	
	std::cout << "Fields" << std::endl;

	for( dwLoop=0; dwLoop < dwSize; dwLoop++ )
	{
		WriteField( p_outStream, m_lstField[ dwLoop ] );
	}

	//Methods
	dwSize = (dword)m_lstMethod.Size();
	p_outStream << dwSize;
	
	std::cout << "Methods" << std::endl;

	for( dwLoop=0; dwLoop < dwSize; dwLoop++ )
	{
		WriteMethod( p_outStream, m_lstMethod[ dwLoop ] );
	}

	//Classes
	dwSize = 0;
	p_outStream << dwSize;

	//Byte Code
	dwSize = (dword)m_lstByteCode.Size();
	p_outStream << dwSize;
	for( dwLoop=0; dwLoop < dwSize; dwLoop++ )
	{
		p_outStream << (char) m_lstByteCode.GetElement( dwLoop );
	}

	return true;
}

Meson_Cuneus_RTS_END
