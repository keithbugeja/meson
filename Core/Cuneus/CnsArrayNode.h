//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::ParserNodes::
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	12/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#pragma once

#include "CnsTypeNode.h"
#include "CnsVariable.h"

Meson_Cuneus_Compiler_ParserNodes_BEGIN

class ArrayNode 
	: public ParserNode
{
public:
	ArrayNode( void );
	ArrayNode( ArrayNode* p_pArrayNode );
	ArrayNode( TypeNode* p_pTypeNode );
	~ArrayNode( void );

	bool IsArray( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );

	Meson::Common::Text::String ToString( void );
};


//----------------------------------------------------------------------------------------------


class ElementNode
	: public ParserNode
{
protected:
	Variable* m_pVariable;
	int m_nIndex;

public:
	ElementNode( void );
	~ElementNode( void );

	int GetIndex( void );
	void SetIndex( int p_nIndex );

	Variable* GetVariable( void );

	bool TypeCheck( SemanticAnalyser* p_pAnalyser );

	bool Generate( CodeGenerator* p_pGenerator );

};


Meson_Cuneus_Compiler_ParserNodes_END