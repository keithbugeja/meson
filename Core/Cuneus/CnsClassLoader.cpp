#include "CnsClassLoader.h"
#include "CnsCuneusEngine.h"
#include "CnsDataTypeMgr.h"

#include "CnsLoaderConstants.h"
#include "CnsOpcodes.h"
#include "CnsDisasm.h"

#include <Pointer.h>
#include <MesonException.h>


#include <iostream>

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Cuneus::RTS::Typing;
using namespace Meson::Cuneus::RTS::Reflection;

Meson_Cuneus_RTS_BEGIN

ClassLoader::ClassLoader(void)
{
}

ClassLoader::~ClassLoader(void)
{
}


//Meson::Common::Text::String ClassLoader::ReadString( Meson::Common::IO::IInputStream& p_stream )
//{
//	char cBuffer[ 1024 ];
//	dword dwData;
//
//	p_stream >> dwData;
//	p_stream.Read( cBuffer, dwData );
//
//	String str( (int)dwData );
//	str.Append( cBuffer, dwData );
//
//	return str;
//}


void LoadField( Meson::Common::IO::IInputStream& p_stream, CuneusEngine* p_pEngine )
{
	//p_pEngine->GetGlobalDataMgr()->AddDataItem()

	dword dwData;
	String str;
	
	//Type Code
	p_stream >> dwData;
	TPointer< DataType> pDataType( p_pEngine->GetDataTypeMgr()->CreateDataType( dwData ) );
	p_pEngine->GetGlobalDataMgr()->AddDataItem( pDataType );

	//Modifier Code
	p_stream >> dwData;

	// Name
	p_stream >> str;

	//p_outStream << p_pVariable->GetTypeCode();
	//p_outStream << p_pVariable->GetModifierCode();
	//p_outStream << p_pVariable->GetQualifiedName();

	std::cout << str << std::endl;
}

void LoadMethod( Meson::Common::IO::IInputStream& p_stream, CuneusEngine* p_pEngine )
{
	String strName;

	dword	dwData = 0,
			dwSize = 0,
			dwLoop = 0;

	TypeMethod* pMethod = new TypeMethod();

	p_stream >> dwData;
	pMethod->SetTypeCode( dwData );
	
	p_stream >> dwData;
	pMethod->SetModifierCode( dwData );

	p_stream >> strName;
	pMethod->SetName( strName );

	p_stream >> dwData;
	if( !pMethod->GetModifier().m_bNative )
	{
		pMethod->SetByteCodeOffset(dwData);
	}
	
	p_pEngine->RegisterMethod( strName, pMethod );

	p_stream >> dwSize;

	TPointer< TypeParam > pParam;

	for( dword dw=0; dw < dwSize; dw++ )
	{
		pParam = new TypeParam();

		// TypeCode
		p_stream >> dwData;
		pParam->SetTypeCode( dwData );

		p_stream >> strName;
		pParam->SetName( strName );

		pMethod->AddParam( pParam );
	}

	std::cout << pMethod->ToString() << std::endl;
}


bool ClassLoader::Load( Meson::Common::IO::IInputStream& p_stream )
{

	CuneusEngine* pEngine = CuneusEngine::GetInstance();
	String strData;

	dword	dwData = 0,
			dwSize = 0,
			dwLoop = 0;

	
	// Magic
	p_stream >> dwData;

	if( dwData != CNS_LDR_MAGIC )
		throw new Meson::Common::MesonException( "Stream's Magic is not 'CNS;'!", __FILE__, __LINE__ );


	// Namespaces
	p_stream >> dwSize;

	for( dwLoop=0; dwLoop < dwSize; dwLoop++ )
	{
		p_stream >> strData;
		//pEngine->GetSymbolMgr()-> SymbolIndex( strData );
		std::cout << strData << std::endl;
		//WriteString( p_outStream, m_lstNamespace[ dwLoop ]->GetQualifiedName() );
	}
	
	// Constants
	p_stream >> dwSize;
	DataType* pDataType;
	for( dwLoop=0; dwLoop < dwSize; dwLoop++ )
	{
		p_stream >> dwData;
		
		pDataType =	pEngine->GetDataTypeMgr()->CreateDataType( dwData );
		pDataType->Load( p_stream );
		pEngine->GetConstantsMgr()->AddConstant( pDataType );
		

		std::cout << pDataType->ToString() << std::endl;
		//WriteString( p_outStream, m_lstNamespace[ dwLoop ]->GetQualifiedName() );
	}

	// Fields
	p_stream >> dwSize;

	for( dwLoop=0; dwLoop < dwSize; dwLoop++ )
	{
		LoadField( p_stream, pEngine );
		/*p_stream >> dwData;
		TPointer< DataType> pDataType( pEngine->GetDataTypeMgr()->CreateDataType( dwData ) );
		pEngine->GetConstantsMgr()->AddConstant( pDataType );
		pDataType->Load( p_stream );

		std::cout << pDataType->ToString() << std::endl;*/
		//WriteString( p_outStream, m_lstNamespace[ dwLoop ]->GetQualifiedName() );
	}

	// Methods
	p_stream >> dwSize;

	for( dwLoop=0; dwLoop < dwSize; dwLoop++ )
	{
		LoadMethod( p_stream, pEngine );
	}

	//Classes
	p_stream >> dwSize;

	//Byte Code
	p_stream >> dwSize;
	byte* pByte = new byte[ dwSize ];

	p_stream.Read( pByte, dwSize );
	pEngine->GetByteCodeMgr()->AppendByteCode( pByte, dwSize );

	Meson::Cuneus::Util::Disasm disasm;
	disasm.SetByteCodeBase( pByte );

	String str;
	for( dword dw=0; dw < dwSize; )
	{
		dw = disasm.Disassemble( dw, str );
		std::cout << str << std::endl;
	}


	memset( pByte, CNS_HALT, 6 );
	pEngine->GetByteCodeMgr()->AppendByteCode( pByte, 6 );
	delete pByte;

	return true;
}


/*
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
		p_outStream << m_lstByteCode[ dwLoop ];
	}

	return true;
}
*/

Meson_Cuneus_RTS_END
