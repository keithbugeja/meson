#include "CnsCuneusCompiler.h"
#include "CnsClassWriter.h"

#include <iostream>
#include <OutputStream.h>

Meson_Cuneus_Compiler_BEGIN

using namespace Meson::Cuneus::RTS;

CuneusCompiler::CuneusCompiler( void ) 
	: m_sourceInfo()
	, m_errorTracker( &m_sourceInfo )
	, m_lexer( &m_sourceInfo )
	, m_tokenStream( &m_lexer )
	, m_parser( this )
	, m_analyser( &m_parser )
	, m_generator( this )
{
	Init();
}

CuneusCompiler::~CuneusCompiler( void )
{
}

void CuneusCompiler::Init( void )
{
	m_lexer.Init( NULL );

	m_lexer.AddKeyword( "abstract",     KeywordAbstract );
	m_lexer.AddKeyword( "base",         KeywordBase );
	m_lexer.AddKeyword( "boolean",      KeywordBoolean );
	m_lexer.AddKeyword( "break",        KeywordBreak );
	m_lexer.AddKeyword( "byte",         KeywordByte );
	m_lexer.AddKeyword( "case",         KeywordCase );
	m_lexer.AddKeyword( "catch",        KeywordCatch );
	m_lexer.AddKeyword( "char",         KeywordChar );
	m_lexer.AddKeyword( "class",        KeywordClass );
	m_lexer.AddKeyword( "continue",     KeywordContinue );
	m_lexer.AddKeyword( "delete",       KeywordDelete );
	m_lexer.AddKeyword( "do",           KeywordDo );
	m_lexer.AddKeyword( "double",       KeywordDouble );
	m_lexer.AddKeyword( "dword",        KeywordDword );
	m_lexer.AddKeyword( "else",         KeywordElse );
	m_lexer.AddKeyword( "extends",      KeywordExtends );
	m_lexer.AddKeyword( "false",        KeywordFalse );
	m_lexer.AddKeyword( "final",        KeywordFinal );
	m_lexer.AddKeyword( "float",        KeywordFloat );
	m_lexer.AddKeyword( "for",          KeywordFor );
	m_lexer.AddKeyword( "get",          KeywordGet );
	m_lexer.AddKeyword( "goto",         KeywordGoto );
	m_lexer.AddKeyword( "if",           KeywordIf );
	m_lexer.AddKeyword( "using",        KeywordImport );
	m_lexer.AddKeyword( "implements",   KeywordImplements );
	m_lexer.AddKeyword( "instanceof",   KeywordInstanceof );
	m_lexer.AddKeyword( "int",          KeywordInt );
	m_lexer.AddKeyword( "interface",    KeywordInterface );
	m_lexer.AddKeyword( "long",         KeywordLong );
	m_lexer.AddKeyword( "namespace",    KeywordNamespace );
	m_lexer.AddKeyword( "native",       KeywordNative );
	m_lexer.AddKeyword( "new",          KeywordNew );
	m_lexer.AddKeyword( "null",         KeywordNull );
	m_lexer.AddKeyword( "private",      KeywordPrivate );
	m_lexer.AddKeyword( "protected",    KeywordProtected );
	m_lexer.AddKeyword( "public",       KeywordPublic );
	m_lexer.AddKeyword( "qword",        KeywordQword );
	m_lexer.AddKeyword( "return",       KeywordReturn );
	m_lexer.AddKeyword( "short",        KeywordShort );
	m_lexer.AddKeyword( "static",       KeywordStatic );
	m_lexer.AddKeyword( "string",       KeywordString );
	m_lexer.AddKeyword( "switch",       KeywordSwitch );
	m_lexer.AddKeyword( "synchronized", KeywordSynchronized );
	m_lexer.AddKeyword( "this",         KeywordThis );
	m_lexer.AddKeyword( "throw",        KeywordThrow );
	m_lexer.AddKeyword( "true",         KeywordTrue );
	m_lexer.AddKeyword( "try",          KeywordTry );
	m_lexer.AddKeyword( "unmanaged",    KeywordUnmanaged );
	m_lexer.AddKeyword( "void",         KeywordVoid );
	m_lexer.AddKeyword( "while",        KeywordWhile );
	m_lexer.AddKeyword( "word",         KeywordWord );

	m_lexer.AddSymbol( "[]",  OperatorArrayDefn );
	m_lexer.AddSymbol( "+",   OperatorAdd );
	m_lexer.AddSymbol( "+=",  OperatorAddEq );
	m_lexer.AddSymbol( "&&",  OperatorAnd );
	m_lexer.AddSymbol( "&",   OperatorBitAnd );
	m_lexer.AddSymbol( "&=",  OperatorBitAndEq );
	m_lexer.AddSymbol( "~",   OperatorBitNot );
	m_lexer.AddSymbol( "|",   OperatorBitOr );
	m_lexer.AddSymbol( "|=",  OperatorBitOrEq );
	m_lexer.AddSymbol( "^",   OperatorBitXor );
	m_lexer.AddSymbol( "^=",  OperatorCaretEq );
	m_lexer.AddSymbol( "--",  OperatorDec );
	m_lexer.AddSymbol( "/",   OperatorDiv );
	m_lexer.AddSymbol( "/=",  OperatorDivEq );
	m_lexer.AddSymbol( "=",   OperatorEq );
	m_lexer.AddSymbol( "==",  OperatorEqEq );
	m_lexer.AddSymbol( ">",   OperatorGt );
	m_lexer.AddSymbol( ">=",  OperatorGtEq );
	m_lexer.AddSymbol( "++",  OperatorInc );
	m_lexer.AddSymbol( "<",   OperatorLt );
	m_lexer.AddSymbol( "<=",  OperatorLtEq );
	m_lexer.AddSymbol( "%",   OperatorMod );
	m_lexer.AddSymbol( "%=",  OperatorModEq );
	m_lexer.AddSymbol( "*",   OperatorMul );
	m_lexer.AddSymbol( "*=",  OperatorMulEq );
	m_lexer.AddSymbol( "!",   OperatorNot );
	m_lexer.AddSymbol( "!=",  OperatorNotEq );
	m_lexer.AddSymbol( "||",  OperatorOr );
	m_lexer.AddSymbol( "<<",  OperatorShl );
	m_lexer.AddSymbol( "<<=", OperatorShlEq );
	m_lexer.AddSymbol( ">>",  OperatorShr );
	m_lexer.AddSymbol( ">>=", OperatorShrEq );
	m_lexer.AddSymbol( "-",   OperatorSub );
	m_lexer.AddSymbol( "-=",  OperatorSubEq );
	m_lexer.AddSymbol( ">>>", OperatorUShr );
	m_lexer.AddSymbol( ">>>=",OperatorUShrEq );

	m_parser.Init();
}


bool CuneusCompiler::Compile( Meson::Common::IO::IInputStream* p_pInputStream, Meson::Common::IO::IOutputStream* p_pOutputStream )
{
	m_lexer.SetInputStream( p_pInputStream );
	m_tokenStream.Init();

	ParserNode* pParseTree = m_parser.Parse();

	if( pParseTree )
	{
		std::cout << pParseTree->ToString() << std::endl;
		m_analyser.Check( (ParserNodes::CompilationUnit*) pParseTree );
	
		ClassWriter writer;
		m_generator.SetClassWriter( &writer );
		if( m_generator.Generate( (ParserNodes::CompilationUnit*) pParseTree ) )
		{

			if( writer.Write( *p_pOutputStream ) )
				std::cout << "Finished." << std::endl;
			else
				std::cout << "Error occured while writing cnx file" << std::endl;

			p_pOutputStream->Flush();
		}
	}


	if( !pParseTree )
		m_errorTracker.OutputErrors();

	return true;
}

Meson_Cuneus_Compiler_END

