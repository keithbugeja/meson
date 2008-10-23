#pragma once

//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::CuneusCompiler
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	07/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "CnsNamespaces.h"
#include "CnsDataType.h"
#include "CnsParserNode.h"

#include <String8.h>
#include <StringMap.h>
#include <Pointer.h>
#include <ArrayList.h>

Meson_Cuneus_Compiler_BEGIN

namespace SymbolTableEntryType
{
	enum SymbolTableEntryType { None, Global, Local, Param };
}


class SymbolTableEntry
{
protected:
	Meson::Common::Text::String m_strName;

	//Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > m_pDataType;

	dword m_dwTypeCode;
	dword m_dwOffset;
	dword m_dwScope;

	ParserNode* m_pNode;

	SymbolTableEntryType::SymbolTableEntryType m_eEntryType;

public:
	SymbolTableEntry( Meson::Common::Text::String p_strName );
	~SymbolTableEntry( void );

	Meson::Common::Text::String& GetName( void );

	/*Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > GetType( void );
	void SetType( Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType >  p_pDataType );*/

	dword GetTypeCode( void );
	void  SetTypeCode( dword p_dwTypeCode );

	dword GetTargetOffset( void );
	void SetTargetOffset( dword p_dwOffset );

	dword GetScope( void );
	void SetScope( dword p_dwScope );

	ParserNode* GetNode( void );
	void SetNode( ParserNode* p_pNode );

	ParserNode* GetParentNode( void );

	SymbolTableEntryType::SymbolTableEntryType GetEntryType( void );
	void SetEntryType( SymbolTableEntryType::SymbolTableEntryType p_eValue );

	void ToString( Meson::Common::Text::String& p_string );
};

//---------------------------------------------------------------------------------------------------------

class SymbolTable
{
protected:
	Meson::Common::Collections::TStringMap< SymbolTableEntry* > m_mapEntry;

public:
	SymbolTable( void );
	~SymbolTable( void );

public:
	bool Contains( Meson::Common::Text::String p_strName, SymbolTableEntry*& p_pEntry);

	SymbolTableEntry* Lookup( Meson::Common::Text::String p_strName );

	SymbolTableEntry* Lookup( Meson::Common::Collections::TArrayList< Meson::Common::Text::String > p_lstName );

	SymbolTableEntry* Insert( Meson::Common::Text::String p_strName );

	SymbolTableEntry* operator[]( Meson::Common::Text::String p_strName );

	SymbolTableEntry* operator[]( Meson::Common::Collections::TArrayList< Meson::Common::Text::String > p_lstName );

	void ToString( Meson::Common::Text::String& p_string );
};

Meson_Cuneus_Compiler_END
