#pragma once

#include <String8.h>
#include <ArrayList.h>
#include <StringMap.h>
#include <InputStream.h>
#include <OutputStream.h>

#include "CnsNamespaces.h"

#include "CnsParserNodes.h"

#include "CnsDataType.h"

//#include "CnsTypeNamespace.h"
//#include "CnsTypeClass.h"
//#include "CnsTypeField.h"
//#include "CnsTypeMethod.h"


Meson_Cuneus_RTS_BEGIN

class ClassWriter
{
private:
	Meson::Common::Collections::TStringMap< dword > m_mapString;
	Meson::Common::Collections::TBufferList< byte > m_lstByteCode;

	Meson::Common::Collections::TArrayList< Meson::Cuneus::Compiler::ParserNodes::NamespaceNode* > m_lstNamespace;
	Meson::Common::Collections::TArrayList< Meson::Cuneus::Compiler::ParserNodes::Variable* > m_lstField;
	Meson::Common::Collections::TArrayList< Meson::Cuneus::Compiler::ParserNodes::MethodNode* > m_lstMethod;
	Meson::Common::Collections::TArrayList< Meson::Cuneus::Compiler::ParserNodes::ClassNode* > m_lstClass;

	Meson::Common::Collections::TArrayList< Meson::Cuneus::RTS::Typing::DataType* > m_lstConstants;

public:
	ClassWriter( void );
	virtual ~ClassWriter( void );

	dword InsertStringConstant( Meson::Common::Text::String& p_str );
	dword GetStringConstant( Meson::Common::Text::String& p_str );

	dword GetByteCodeOffset( void );
	dword AddByteCode( byte p_byCode );
	dword AddByteCodeW( word p_wCode );
	dword AddByteCodeD( dword p_dwCode );
	dword AddByteCodeP( byte* p_pbyCode, dword p_dwLen );
	dword AddByteCodeN( int p_nRelativeAddress );

	dword Skip( int p_nNum );

	void SetByteCode( dword p_dwAddress, byte p_byCode );
	void SetByteCodeW( dword p_dwAddress, word p_wCode );
	void SetByteCodeD( dword p_dwAddress, dword p_dwCode );
	void SetByteCodeN( dword p_dwAddress, int p_nRelativeAddress );
	void SetByteCodeP( dword p_dwAddress, byte* p_pbyCode, dword p_dwLen );

	int GetAddressDifference( dword p_dwAddress );

	dword AddConstant( Meson::Cuneus::RTS::Typing::DataType* p_pDataType );
	Meson::Cuneus::RTS::Typing::DataType* GetConstant( dword p_dwIndex );

	dword AddField( Meson::Cuneus::Compiler::ParserNodes::Variable* p_pField );
	Meson::Cuneus::Compiler::ParserNodes::Variable* GetField( dword p_dwIndex );

	dword AddMethod( Meson::Cuneus::Compiler::ParserNodes::MethodNode* p_pMethod );
	Meson::Cuneus::Compiler::ParserNodes::MethodNode* GetMethod( dword p_dwIndex );

	dword AddClass( Meson::Cuneus::Compiler::ParserNodes::ClassNode* p_pClass );
	Meson::Cuneus::Compiler::ParserNodes::ClassNode* GetClass( dword p_dwIndex );

	dword AddNamespace( Meson::Cuneus::Compiler::ParserNodes::NamespaceNode* p_pNamespace );
	Meson::Cuneus::Compiler::ParserNodes::NamespaceNode* GetNamespace( dword p_dwIndex );


	bool Write( Meson::Common::IO::IOutputStream& p_outStream );
};

Meson_Cuneus_RTS_END
