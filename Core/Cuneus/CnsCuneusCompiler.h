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

#include <InputStream.h>
#include <OutputStream.h>

#include "CnsErrorTracker.h"
#include "CnsLexer.h"
#include "CnsTokenStream.h"
#include "CnsCuneusParser.h"
#include "CnsSemanticAnalyser.h"
#include "CnsCodeGenerator.h"
#include "CnsCuneusEngine.h"

Meson_Cuneus_Compiler_BEGIN


enum TokenValue 
{
	Undefined = 0,

	KeywordAbstract,	KeywordBase,	KeywordBoolean,			KeywordBreak,		KeywordByte,		KeywordCase,	KeywordCatch,		KeywordChar, 
	KeywordClass,
	KeywordContinue,	KeywordDelete,	KeywordDo,				KeywordDouble,		KeywordDword,		KeywordElse,	KeywordExtends,	KeywordFalse, KeywordFinal,	KeywordFloat,		KeywordFor,
	KeywordGet,			KeywordGoto,	KeywordIf,				KeywordImport,		KeywordImplements,	KeywordInstanceof,	KeywordInt,		KeywordInterface,	KeywordLong,
	KeywordNamespace,
	KeywordNative,		KeywordNew,		KeywordNull,	KeywordPrivate,			KeywordProtected,	KeywordPublic,		KeywordQword,	KeywordReturn,		KeywordShort,
	KeywordStatic,		KeywordString,	KeywordSwitch,	KeywordSynchronized,	KeywordThis,		KeywordThrow,		KeywordTrue, KeywordTry,		KeywordUnmanaged,	KeywordVoid,	KeywordWhile,
	KeywordWord,

	OperatorArrayDefn,	OperatorAdd,	OperatorAddEq,		OperatorAnd,	OperatorBitAnd,	OperatorBitAndEq,	OperatorBitNot,	OperatorBitOr,	OperatorBitOrEq,
	OperatorBitXor,	OperatorCaretEq,	OperatorDec,	OperatorDiv,	OperatorDivEq,		OperatorEq,		OperatorEqEq,	OperatorGt,	
	OperatorGtEq,	OperatorInc,		OperatorLt,		OperatorLtEq,	OperatorMod,		OperatorModEq,	OperatorMul,	OperatorMulEq,
	OperatorNot,	OperatorNotEq,		OperatorOr,		OperatorShl,	OperatorShlEq,		OperatorShr,	OperatorShrEq,	OperatorSub,
	OperatorSubEq,	OperatorUShr,		OperatorUShrEq
};

class CUNEUS_API CuneusCompiler
{
protected:
	SourceInfo m_sourceInfo;
	ErrorTracker m_errorTracker;
	Lexer m_lexer;
	TokenStream m_tokenStream;
	SymbolTable m_symbolTable;
	CuneusParser m_parser;
	SemanticAnalyser m_analyser;
	CodeGenerator m_generator;

public:
	CuneusCompiler( void );
	~CuneusCompiler( void );

	inline TokenStream* GetTokenStream( void ) { return &m_tokenStream; }
	inline ErrorTracker* GetErrorTracker( void ) { return &m_errorTracker; }
	inline SymbolTable* GetSymbolTable( void ) { return &m_symbolTable; }

	void Init( void );

	//bool Compile( Meson::Common::IO::IInputStream* p_pInputStream );
	bool CuneusCompiler::Compile( Meson::Common::IO::IInputStream* p_pInputStream, Meson::Common::IO::IOutputStream* p_pOutputStream );
};

Meson_Cuneus_Compiler_END
