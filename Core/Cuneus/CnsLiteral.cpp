#include "CnsLiteral.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;
using namespace Meson::Cuneus::RTS::Typing;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

Literal::Literal( void )
	: ParserNode( "Literal" )
	, m_pDataType( NULL )
{
}

Literal::Literal( Meson::Common::Text::String p_strName )
	: ParserNode( p_strName )
	, m_pDataType( NULL )
{
}

Literal::~Literal( void )
{
	if( m_pDataType )
	{
		delete m_pDataType;
	}
}

DataType* Literal::GetDataType( void )
{
	return m_pDataType;
}

void Literal::SetDataType( DataType* p_pDataType )
{
	m_pDataType = p_pDataType;
}


bool Literal::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool Literal::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



Meson::Common::Text::String Literal::ToString( void )
{
	String str;

	if( m_strName.Size() > 0 )
		str += m_strName;
	
	return str;
}

//*********************************************************************************************************

bool IntegerLiteral::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool IntegerLiteral::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//*********************************************************************************************************

bool RealLiteral::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool RealLiteral::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//*********************************************************************************************************

bool CharLiteral::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool CharLiteral::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//*********************************************************************************************************

bool StringLiteral::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool StringLiteral::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//*********************************************************************************************************

bool BoolLiteral::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool BoolLiteral::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

//*********************************************************************************************************

bool RefLiteral::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool RefLiteral::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}



Meson_Cuneus_Compiler_ParserNodes_END

