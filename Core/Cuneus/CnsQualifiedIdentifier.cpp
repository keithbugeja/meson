#include "CnsQualifiedIdentifier.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"

using namespace Meson::Cuneus::Compiler;

using namespace Meson::Common::Text;

Meson_Cuneus_Compiler_ParserNodes_BEGIN

QualifiedIdentifier::QualifiedIdentifier( void ) 
	: ParserNode( "QualifiedIdentifier" ) 
{
}

QualifiedIdentifier::QualifiedIdentifier( Meson::Common::Text::String p_strName ) 
	: ParserNode( p_strName )
{
}

String QualifiedIdentifier::ToString( void )
{
	String str;

	if( m_plstChildNode )
	{
		size_t unSize = m_plstChildNode->Size();
		for( size_t un = 0; un < unSize; un++ )
		{
			str += (*m_plstChildNode)[ un ]->ToString();
			if( un < (unSize - 1) )
				str += '.';
		}
	}

	return str;
}

bool QualifiedIdentifier::TypeCheck( SemanticAnalyser* p_pAnalyser ) 
{ 
	return p_pAnalyser->Check( this ); 
}

bool QualifiedIdentifier::Generate( CodeGenerator* p_pGenerator )
{
	return p_pGenerator->Generate( this );
}

Meson_Cuneus_Compiler_ParserNodes_END
