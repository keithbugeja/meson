//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	05/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <BufferList.h>
#include <String8.h>

#include "CnsNamespaces.h"

Meson_Cuneus_Compiler_BEGIN

// Forward Declaration
class SemanticAnalyser;
class CodeGenerator;

namespace ParserNodes
{
	class TypeNode;
};

class ParserNode
{

protected:
	Meson::Common::Text::String m_strName;
	Meson::Common::Collections::TBufferList< ParserNode* >* m_plstChildNode;
	ParserNodes::TypeNode* m_pType;
	ParserNode* m_pParent;

public:
	ParserNode( void );
	ParserNode( Meson::Common::Text::String p_strName );
	virtual ~ParserNode( void );
	
	Meson::Common::Text::String GetName( void );
	void SetName( Meson::Common::Text::String p_strName );

	Meson::Common::Text::String GetQualifiedName( void );

	ParserNodes::TypeNode* GetType( void ) { return m_pType; }

	void SetType( ParserNodes::TypeNode* p_pType ) { m_pType = p_pType; }

	dword GetTypeCode( void );
	void  SetTypeCode( dword p_dwTypeCode );

	ParserNode* GetParent( void );
	void SetParent( ParserNode* p_pParent );

	virtual void IncLocalVars( void );
	
	virtual dword GetLocalVars( void );

	bool HasChildren( void );
	bool HasNoChildren( void );

	size_t NumberOfChildren( void );

	void AddChild( ParserNode* p_pParserNode );
	ParserNode* RemoveChild( int p_nIndex );
	ParserNode* RemoveLastChild( void );

	ParserNode* GetChild( int p_nIndex );
	ParserNode* SetChild( int p_nIndex, ParserNode* p_pParserNode );

	void Merge( ParserNode* p_pParserNode );

	ParserNode* operator[]( int p_nIndex );

	virtual Meson::Common::Text::String ToString( void );


	virtual bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	virtual bool Generate( CodeGenerator* p_pGenerator );

	//virtual bool Generate( CodeGenerator* p_pGenerator, int p_nIndex );
};

Meson_Cuneus_Compiler_END
