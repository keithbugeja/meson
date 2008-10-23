#include "CnsCodeGenerator.h"

#include "CnsCuneusCompiler.h"
#include "CnsOpcodes.h"
#include "CnsTypeCodes.h"

#include "CnsTypeField.h"
#include "CnsTypeMethod.h"


#include <Pointer.h>

#include <iostream>

Meson_Cuneus_Compiler_BEGIN

using namespace Meson::Common;

using namespace Meson::Cuneus::RTS;
using namespace Meson::Cuneus::RTS::Typing;
using namespace Meson::Cuneus::RTS::Reflection;

using namespace ParserNodes;



CodeGenerator::CodeGenerator( CuneusCompiler* p_pCompiler )
{
	m_pDataTypeMgr = CuneusEngine::GetInstance()->GetDataTypeMgr();
	m_pCompiler = p_pCompiler;
	m_pSymbolTable = p_pCompiler->GetSymbolTable();
	m_byRegIndex = 0;
	m_byPushCount = 0;
}

CodeGenerator::~CodeGenerator( void )
{
}

ClassWriter* CodeGenerator::GetClassWriter( void )
{
	return m_pClassWriter;
}

void CodeGenerator::SetClassWriter( ClassWriter*  p_pClassWriter )
{
	m_pClassWriter = p_pClassWriter;
}

byte CodeGenerator::IncRegIndex( void )
{
	if( m_byRegIndex == 31 )
	{
		m_pClassWriter->AddByteCode( CNS_PUSHALL );
		std::cout << "PushAll" << std::endl;
		m_byRegIndex = 0;
	}
	else
		m_byRegIndex++;

	return m_byRegIndex;
}

byte CodeGenerator::DecRegIndex( void )
{
	if( m_byRegIndex > 0 )
		m_byRegIndex--;
	else
	{
		m_pClassWriter->AddByteCode( CNS_POPALL );
		std::cout << "PopAll" << std::endl;
		m_byRegIndex = 31;
	}

	return m_byRegIndex;
}

byte CodeGenerator::GetRegIndex( void )
{
	return m_byRegIndex;
}

byte CodeGenerator::GetPreviousRegIndex( void )
{
	return m_byRegIndex - 1;
}

byte CodeGenerator::GetNextRegIndex( void )
{
	return m_byRegIndex + 1;
}


dword CodeGenerator::MarkAddress( void )
{
	dword dw = (dword) m_lstAddress.Size();

	m_lstAddress.Add( m_pClassWriter->GetByteCodeOffset() );
	m_pClassWriter->AddByteCodeD( 0 );

	return dw;
}

void CodeGenerator::WriteRelativeAddress( dword p_dwAddressIndex, int p_nRelativeAddress )
{
	dword dwAddress = m_lstAddress[ p_dwAddressIndex ];
	m_pClassWriter->SetByteCodeD( dwAddress, *((dword*)&p_nRelativeAddress) );
}

//*********************************************************************************************************

bool CodeGenerator::Generate( IntegerLiteral* p_pLiteral )
{
	dword dw = m_pClassWriter->AddConstant( p_pLiteral->GetDataType() );

	m_pClassWriter->AddByteCode( CNS_LDGC );
	m_pClassWriter->AddByteCode( GetRegIndex() );
	m_pClassWriter->AddByteCodeD( dw );

	std::cout << "LDGC " << (int)GetRegIndex() << ", " << dw << std::endl;

	return true;
}

bool CodeGenerator::Generate( RealLiteral* p_pLiteral )
{
	dword dw = m_pClassWriter->AddConstant( p_pLiteral->GetDataType() );
	
	m_pClassWriter->AddByteCode( CNS_LDGC );
	m_pClassWriter->AddByteCode( GetRegIndex() );
	m_pClassWriter->AddByteCodeD( dw );

	return true;
}

bool CodeGenerator::Generate( CharLiteral* p_pLiteral )
{
	dword dw = m_pClassWriter->AddConstant( p_pLiteral->GetDataType() );

	m_pClassWriter->AddByteCode( CNS_LDGC );
	m_pClassWriter->AddByteCode( GetRegIndex() );
	m_pClassWriter->AddByteCodeD( dw );

	return true;
}

bool CodeGenerator::Generate( StringLiteral* p_pLiteral )
{
	dword dw = m_pClassWriter->AddConstant( p_pLiteral->GetDataType() );
	
	m_pClassWriter->AddByteCode( CNS_LDGC );
	m_pClassWriter->AddByteCode( GetRegIndex() );
	m_pClassWriter->AddByteCodeD( dw );

	return true;
}

bool CodeGenerator::Generate( BoolLiteral* p_pLiteral )
{
	dword dw = m_pClassWriter->AddConstant( p_pLiteral->GetDataType() );
	
	m_pClassWriter->AddByteCode( CNS_LDGC );
	m_pClassWriter->AddByteCode( GetRegIndex() );
	m_pClassWriter->AddByteCodeD( dw );

	m_pClassWriter->AddByteCode( ( ( p_pLiteral->GetName() == "true" ) ? CNS_STT : CNS_CLT ) );

	return true;
}

bool CodeGenerator::Generate( RefLiteral* p_pLiteral )
{
	dword dw = m_pClassWriter->AddConstant( p_pLiteral->GetDataType() );

	m_pClassWriter->AddByteCode( CNS_LDGC );
	m_pClassWriter->AddByteCode( GetRegIndex() );
	m_pClassWriter->AddByteCodeD( dw );

	return true;
}


//*********************************************************************************************************

bool CodeGenerator::Generate( ParserNodes::ExpressionNode* p_pNode )
{
	if( !p_pNode )
		return false;

	int nSize = (int) p_pNode->NumberOfChildren();
	for( int n = 0; n < nSize; n++ )
	{
		if( !(*p_pNode)[ n ]->Generate( this ) )
			return false;
	}

	return true;
}



bool CodeGenerator::Generate( ParserNodes::UnaryOperator* p_pNode )
{
	return false;
}

bool CodeGenerator::Generate( ParserNodes::UnaryMinus* p_pNode )
{
	p_pNode->GetChild(0)->Generate( this );
	m_pClassWriter->AddByteCode( CNS_NEG );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	return true;
}


bool CodeGenerator::Generate( ParserNodes::UnaryLogicalNot* p_pNode )
{
	p_pNode->GetChild(0)->Generate( this );
	m_pClassWriter->AddByteCode( CNS_NOT );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	return true;
}

bool CodeGenerator::Generate( ParserNodes::UnaryInc* p_pNode )

{
	p_pNode->GetChild(0)->Generate( this );
	m_pClassWriter->AddByteCode( CNS_INC );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	return true;
}

bool CodeGenerator::Generate( ParserNodes::UnaryDec* p_pNode )
{
	p_pNode->GetChild(0)->Generate( this );
	m_pClassWriter->AddByteCode( CNS_DEC );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	return true;
}

bool CodeGenerator::Generate( ParserNodes::UnaryBitwiseNot* p_pNode )
{
	p_pNode->GetChild(0)->Generate( this );
	m_pClassWriter->AddByteCode( CNS_NOT );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	return true;
}

bool CodeGenerator::Generate( ParserNodes::UnaryPlus* p_pNode )
{
	p_pNode->GetChild(0)->Generate( this );
	return true;
}




bool CodeGenerator::Generate( ParserNodes::BinaryOperator* p_pNode )
{
	return false;
}


bool CodeGenerator::Generate( ParserNodes::OpLogicOr* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_OR );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "LOR " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpBitwiseOr* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_OR );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "OR " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpBitwiseXor* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_XOR );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "XOR " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpPlus* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_ADD );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "Add " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpMinus* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_SUB );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "SUB " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpLogicAnd* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_AND );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "LAND " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpBitwiseAnd* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_AND );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "AND " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpShl* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_SHL );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "SHL " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpShr* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_SHR );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "SHR " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpUShr* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_USHR );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "USHR " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpMul* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_MUL );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "MUL " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpDiv* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_DIV );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "DIV " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}

bool CodeGenerator::Generate( ParserNodes::OpMod* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_MOD );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "Mod " << (int)GetPreviousRegIndex() << ", " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}





bool CodeGenerator::Generate( ParserNodes::TernaryOperator* p_pNode )
{
//TODO:----------------------------------------------------------------------------------------------------
	std::cout << "?: Operator" << std::endl;

	//Condition
	p_pNode->GetChild(0)->Generate( this );
	
	dword dwIfStartAddress = m_pClassWriter->GetByteCodeOffset();

	// Jump out of if condition false
	m_pClassWriter->AddByteCode( CNS_JRF );
	dword ThenIndex = MarkAddress();

	// Then Block
	p_pNode->GetChild(1)->Generate( this );


	//Block
	if( p_pNode->NumberOfChildren() > 2 )
	{
		m_pClassWriter->AddByteCode( CNS_JR );
		dword FinishIndex = MarkAddress();

		int nRel = m_pClassWriter->GetAddressDifference( dwIfStartAddress );
		WriteRelativeAddress( ThenIndex, nRel );
std::cout << ": END " << nRel << std::endl;

		p_pNode->GetChild(2)->Generate( this );


		nRel = m_pClassWriter->GetAddressDifference( dwIfStartAddress );
		WriteRelativeAddress( FinishIndex, nRel );
std::cout << "? END " << nRel << std::endl;
	}
	else
	{
		int nRel = m_pClassWriter->GetAddressDifference( dwIfStartAddress );
		WriteRelativeAddress( ThenIndex, nRel );
std::cout << "? END " << nRel << std::endl;
	}

	return true;
}

bool CodeGenerator::Generate( ParserNodes::RelationalOperator* p_pNode )
{
	return false;
}


bool CodeGenerator::Generate( ParserNodes::RelOpEqEq* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_EQ );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "EQ " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}


bool CodeGenerator::Generate( ParserNodes::RelOpNotEq* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_NE );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "NE " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}


bool CodeGenerator::Generate( ParserNodes::RelOpLtEq* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_LE );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "LE " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}


bool CodeGenerator::Generate( ParserNodes::RelOpGtEq* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_GE );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "GE " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}


bool CodeGenerator::Generate( ParserNodes::RelOpLt* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_LT );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "LT " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}


bool CodeGenerator::Generate( ParserNodes::RelOpGt* p_pNode )
{
	//LHS
	p_pNode->GetChild(0)->Generate( this );
	
	IncRegIndex();

	//RHS
	p_pNode->GetChild(1)->Generate( this );

	//LHS op RHS
	m_pClassWriter->AddByteCode( CNS_GT );
	m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	std::cout << "GT " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << std::endl;

	DecRegIndex();

	return true;
}




bool CodeGenerator::Generate( ParserNodes::TypeCast* p_pNode )
{
	//TODO: ----------

	//p_pNode->
	return true;
}

//bool CodeGenerator::Generate( ParserNodes::TypeNode* p_pNode )
//{
//	return true;
//}

bool CodeGenerator::Generate( ParserNodes::Identifier* p_pNode )
{
	return true;
}

bool CodeGenerator::Generate( ParserNodes::QualifiedIdentifier* p_pNode )
{
	//SymbolTableEntry* pEntry = m_pSymbolTable->Lookup( p_pNode->GetChild(0)->GetQualifiedName() );

	//if( pEntry->IsLocal() )
	//{
	//	m_pClassWriter->AddByteCode( CNS_LDLD );
	//	m_pClassWriter->AddByteCode( GetRegIndex() );
	//	m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );

	//	std::cout << "LDLD " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;
	//}
	//else
	//{
	//	m_pClassWriter->AddByteCode( CNS_LDGD );
	//	m_pClassWriter->AddByteCode( GetRegIndex() );
	//	m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );

	//	std::cout << "LDGD " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;
	//}

	return true;
}

bool CodeGenerator::Generate( ParserNodes::VarNode* p_pNode )
{
	SymbolTableEntry* pEntry = m_pSymbolTable->Lookup( p_pNode->GetQualifiedName() );

	switch( pEntry->GetEntryType() )
	{
		case SymbolTableEntryType::Global:
			m_pClassWriter->AddByteCode( CNS_LDGD );
			m_pClassWriter->AddByteCode( GetRegIndex() );
			m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );

			std::cout << "LDGD " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;
			break;

		case SymbolTableEntryType::Local:
			m_pClassWriter->AddByteCode( CNS_LDLD );
			m_pClassWriter->AddByteCode( GetRegIndex() );
			m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );

			std::cout << "LDLD " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;
			break;

		case SymbolTableEntryType::Param:
			m_pClassWriter->AddByteCode( CNS_LDP );
			m_pClassWriter->AddByteCode( GetRegIndex() );
			m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );

			std::cout << "LDP " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;
			break;
	}

	return true;
}



bool CodeGenerator::Generate( ParserNodes::ArrayNode* p_pNode )
{
	/*ParserNode pNode = p_pNode;

	while( pNode->GetTypeCode() == CNS_TC_ARRAY)
	{
		pNode->GetChild(0)-Generate( this );*/

	p_pNode->GetChild(0)->Generate( this );

	m_pClassWriter->AddByteCode( CNS_NEWARRAY );
	m_pClassWriter->AddByteCode( GetRegIndex() );
	m_pClassWriter->AddByteCodeD( p_pNode->GetTypeCode() );
	m_pClassWriter->AddByteCode( GetRegIndex() );

	//std::cout << "NEWARRAY " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;
	std::cout << "NEWARRAY " << (int)GetRegIndex() << ", " << p_pNode->GetTypeCode() << ", " << (int)GetRegIndex() << std::endl;

	

	return true;
}

bool CodeGenerator::Generate( ParserNodes::ElementNode* p_pNode )
{
	//Index
	p_pNode->GetChild(0)->Generate( this );

	IncRegIndex();

	SymbolTableEntry* pEntry = m_pSymbolTable->Lookup( p_pNode->GetName() );

	switch( pEntry->GetEntryType() )
	{
		case SymbolTableEntryType::Global:
			m_pClassWriter->AddByteCode( CNS_LDGD );
			m_pClassWriter->AddByteCode( GetRegIndex() );
			m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );

			std::cout << "LDGD " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;
			
			m_pClassWriter->AddByteCode( CNS_GETELM );
			m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
			m_pClassWriter->AddByteCode( GetRegIndex() );
			m_pClassWriter->AddByteCode( GetPreviousRegIndex() );

			std::cout << "CNS_GETELM " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << ", " << (int)GetPreviousRegIndex() << std::endl;

			break;

		case SymbolTableEntryType::Local:
			m_pClassWriter->AddByteCode( CNS_LDLD );
			m_pClassWriter->AddByteCode( GetRegIndex() );
			m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );

			std::cout << "LDLD " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;
			
			m_pClassWriter->AddByteCode( CNS_GETELM );
			m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
			m_pClassWriter->AddByteCode( GetRegIndex() );
			m_pClassWriter->AddByteCode( GetPreviousRegIndex() );

			std::cout << "CNS_GETELM " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << ", " << (int)GetPreviousRegIndex() << std::endl;

			break;

		case SymbolTableEntryType::Param:
			m_pClassWriter->AddByteCode( CNS_LDP );
			m_pClassWriter->AddByteCode( GetRegIndex() );
			m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );

			std::cout << "LDP " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;

			m_pClassWriter->AddByteCode( CNS_GETELM );
			m_pClassWriter->AddByteCode( GetPreviousRegIndex() );
			m_pClassWriter->AddByteCode( GetRegIndex() );
			m_pClassWriter->AddByteCode( GetPreviousRegIndex() );

			std::cout << "CNS_GETELM " << (int)GetPreviousRegIndex() << ", " << (int)GetRegIndex() << ", " << (int)GetPreviousRegIndex() << std::endl;
			break;
	}


	std::cout << "ELM " << std::endl;

	DecRegIndex();

	return true;
}

//*********************************************************************************************************

bool CodeGenerator::Generate( ParserNodes::Assignment* p_pNode )
{
	
	p_pNode->GetChild(1)->Generate( this );
	IncRegIndex();
	
	
	//LHS op RHS
	SymbolTableEntry* pEntry = m_pSymbolTable->Lookup( p_pNode->GetChild(0)->GetQualifiedName() );

	if( p_pNode->GetChild( 0 )->GetTypeCode() != CNS_TC_ARRAY )
	{
		p_pNode->GetChild(0)->Generate( this );

		DecRegIndex();

		switch( pEntry->GetEntryType() )
		{
			case SymbolTableEntryType::Global:
				m_pClassWriter->AddByteCode( CNS_STGD );
				m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );
				m_pClassWriter->AddByteCode( GetRegIndex() );
				
				std::cout << "STGD " << pEntry->GetTargetOffset() << ", " << (int)GetRegIndex() << std::endl;
				break;

			case SymbolTableEntryType::Local:
				m_pClassWriter->AddByteCode( CNS_STLD );
				m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );
				m_pClassWriter->AddByteCode( GetRegIndex() );

				std::cout << "STLD " << pEntry->GetTargetOffset() << ", " << (int)GetRegIndex() << std::endl;
				break;

			case SymbolTableEntryType::Param:
				m_pClassWriter->AddByteCode( CNS_STP );
				m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );
				m_pClassWriter->AddByteCode( GetRegIndex() );
				
				std::cout << "STP " << pEntry->GetTargetOffset() << ", " << (int)GetRegIndex() << std::endl;
				break;
		}
	}
	else
	{
		// Array Element

		switch( pEntry->GetEntryType() )
		{
			case SymbolTableEntryType::Global:
				m_pClassWriter->AddByteCode( CNS_LDGD );
				m_pClassWriter->AddByteCode( GetRegIndex() );
				m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );

				std::cout << "LDGD " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;
				
				IncRegIndex();

				pEntry->GetNode()->GetChild( 0 )->Generate( this );

				DecRegIndex();

				m_pClassWriter->AddByteCode( CNS_SETELM );
				m_pClassWriter->AddByteCode( GetRegIndex() );
				m_pClassWriter->AddByteCode( GetNextRegIndex() );
				m_pClassWriter->AddByteCode( GetPreviousRegIndex() );

				std::cout << "CNS_SETELM " << (int)GetRegIndex() << ", " << (int)GetNextRegIndex() << ", " << (int)GetPreviousRegIndex() << std::endl;

				break;

			case SymbolTableEntryType::Local:
				m_pClassWriter->AddByteCode( CNS_LDLD );
				m_pClassWriter->AddByteCode( GetRegIndex() );
				m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );

				std::cout << "LDLD " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;
				
				IncRegIndex();

				pEntry->GetNode()->GetChild( 0 )->Generate( this );

				DecRegIndex();

				m_pClassWriter->AddByteCode( CNS_SETELM );
				m_pClassWriter->AddByteCode( GetRegIndex() );
				m_pClassWriter->AddByteCode( GetNextRegIndex() );
				m_pClassWriter->AddByteCode( GetPreviousRegIndex() );

				std::cout << "CNS_SETELM " << (int)GetRegIndex() << ", " << (int)GetNextRegIndex() << ", " << (int)GetPreviousRegIndex() << std::endl;

				
				break;

			case SymbolTableEntryType::Param:
				m_pClassWriter->AddByteCode( CNS_LDP );
				m_pClassWriter->AddByteCode( GetRegIndex() );
				m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );

				std::cout << "LDP " << (int)GetRegIndex() << ", " << pEntry->GetTargetOffset() << std::endl;

				IncRegIndex();

				pEntry->GetNode()->GetChild( 0 )->Generate( this );

				DecRegIndex();

				m_pClassWriter->AddByteCode( CNS_SETELM );
				m_pClassWriter->AddByteCode( GetRegIndex() );
				m_pClassWriter->AddByteCode( GetNextRegIndex() );
				m_pClassWriter->AddByteCode( GetPreviousRegIndex() );

				std::cout << "CNS_SETELM " << (int)GetRegIndex() << ", " << (int)GetNextRegIndex() << ", " << (int)GetPreviousRegIndex() << std::endl;
				break;
		}

		DecRegIndex();
	}

	

	return true;
}

bool CodeGenerator::Generate( ParserNodes::IfNode* p_pNode )
{
	std::cout << "If Condition" << std::endl;

	//Condition
	p_pNode->GetChild(0)->Generate( this );
	
	dword dwIfStartAddress = m_pClassWriter->GetByteCodeOffset();
std::cout << "IF ADDRESS " << dwIfStartAddress << std::endl;

	// Jump out of while if condition false
	m_pClassWriter->AddByteCode( CNS_JRF );
	dword ThenIndex = MarkAddress();

	// Then Block
	p_pNode->GetChild(1)->Generate( this );


	dword dwThenAddress = m_pClassWriter->GetByteCodeOffset();
	//Block
	if( p_pNode->NumberOfChildren() > 2 )
	{
		
		m_pClassWriter->AddByteCode( CNS_JR );
		dword FinishIndex = MarkAddress();

		int nRel = m_pClassWriter->GetAddressDifference( dwIfStartAddress );
		WriteRelativeAddress( ThenIndex, nRel );
std::cout << "THEN END " << nRel << std::endl;

		p_pNode->GetChild(2)->Generate( this );


		nRel = m_pClassWriter->GetAddressDifference( dwThenAddress );
		WriteRelativeAddress( FinishIndex, nRel );
std::cout << "IF END " << nRel << std::endl;
	}
	else
	{
		int nRel = m_pClassWriter->GetAddressDifference( dwIfStartAddress );
		WriteRelativeAddress( ThenIndex, nRel );
std::cout << "IF END " << nRel << std::endl;
	}

	return true;
}


bool CodeGenerator::Generate( ParserNodes::WhileNode* p_pNode )
{
	
	dword dwWhileStartAddress = m_pClassWriter->GetByteCodeOffset();

	std::cout << "While Address:" << dwWhileStartAddress << std::endl;
	std::cout << "While Condition" << std::endl;

	//Condition
	p_pNode->GetChild(0)->Generate( this );
	
	// Jump out of while if condition false
	dword dwJumpAddress = m_pClassWriter->GetByteCodeOffset();
	m_pClassWriter->AddByteCode( CNS_JRF );
	dword FinishIndex = MarkAddress();

std::cout << "JRF" << std::endl;

	//Block
	if( p_pNode->NumberOfChildren() > 1 )
		p_pNode->GetChild(1)->Generate( this );


	// Jump to condition
	int nRelAdress = -m_pClassWriter->GetAddressDifference( dwWhileStartAddress );
	m_pClassWriter->AddByteCode( CNS_JR );
	m_pClassWriter->AddByteCodeN( nRelAdress );
std::cout << "JR " << nRelAdress << std::endl;

	// End of While Block
	int nRel = m_pClassWriter->GetAddressDifference( dwJumpAddress );
	WriteRelativeAddress( FinishIndex, nRel );
std::cout << "WHILE END " << nRel << std::endl;

	return true;
}


bool CodeGenerator::Generate( ParserNodes::LabelNode* p_pNode )
{
	SymbolTableEntry* pEntry = m_pSymbolTable->Lookup( p_pNode->GetQualifiedName() );
	pEntry->SetTargetOffset( m_pClassWriter->GetByteCodeOffset() );
std::cout << "LABEL " << p_pNode->GetName() << std::endl;
	int nChildren = (int) p_pNode->NumberOfChildren();
	if( nChildren )
	{
		GotoNode* pGoto;
		for( int n=0; n<nChildren; n++ )
		{
			pGoto = ((GotoNode*)(p_pNode->GetChild(n)));
			int nRel = m_pClassWriter->GetAddressDifference( pGoto->GetAddress() );
			WriteRelativeAddress( pGoto->GetAddressReference(), nRel );
		}
	}

	return true;
}


bool CodeGenerator::Generate( ParserNodes::ReturnNode* p_pNode )
{
	if( p_pNode->NumberOfChildren() > 0 )
	{
		p_pNode->GetChild(0)->Generate(this);
	}

	dword dwStackItems = p_pNode->GetLocalVars();

	if( dwStackItems )
	{
		m_pClassWriter->AddByteCode( CNS_RETN );
		//m_pClassWriter->AddByteCode( GetRegIndex() );
		m_pClassWriter->AddByteCodeD( dwStackItems );
		std::cout << "Method RETN " << p_pNode->GetLocalVars() << std::endl;
	}
	else
	{
		m_pClassWriter->AddByteCode( CNS_RET );
		//m_pClassWriter->AddByteCode( GetRegIndex() );
		std::cout << "Method RET " << p_pNode->GetLocalVars() << std::endl;
	}
	
	return true;
}


bool CodeGenerator::Generate( ParserNodes::GotoNode* p_pNode )
{
	dword dwGotoAddress = m_pClassWriter->GetByteCodeOffset();

	m_pClassWriter->AddByteCode( CNS_JR );

std::cout << "JR (GOTO) " << std::endl;
	SymbolTableEntry* pEntry = m_pSymbolTable->Lookup( p_pNode->GetQualifiedName() );
	if( pEntry )
	{
		LabelNode* pLabel = (LabelNode*) pEntry->GetNode();

		if( pEntry->GetTargetOffset() != 0xFFFFFFFF )
		{
			// Available
			int nRel = -m_pClassWriter->GetAddressDifference( pEntry->GetTargetOffset() ) + 1;
			m_pClassWriter->AddByteCodeN( nRel );
std::cout << "GOTO " << nRel << std::endl;
		}
		else
		{
			// Foward Reference
			p_pNode->SetAddressReference( MarkAddress() );
			p_pNode->SetAddress( dwGotoAddress );
			pLabel->AddChild( p_pNode );
		}
	}

	return true;
}







void GenerateNativeCall()
{
}





bool CodeGenerator::Generate( ParserNodes::CallNode* p_pNode )
{
	
	SymbolTableEntry* pEntry = m_pSymbolTable->Lookup( p_pNode->GetQualifiedName() );

std::cout << "START CALL FRAME " << pEntry->GetName() << std::endl;

	MethodNode* pMethod = (MethodNode*) pEntry->GetNode();

std::cout << pMethod->GetChild(0)->ToString() << std::endl;

	if( pMethod->IsNative() )
	{
		
		dword dwChildren = (dword)p_pNode->NumberOfChildren();
		if( dwChildren > 0 )
		{
			for( int n = 0; n < (int)(dwChildren); n++ )
			{
	std::cout << "PARAM " << (*p_pNode)[ n ]->ToString() << std::endl;
				(*p_pNode)[ n ]->Generate( this );
				m_pClassWriter->AddByteCode( CNS_PUSH );
				m_pClassWriter->AddByteCode( GetRegIndex() );
	std::cout << "PARAM PUSH" << (int)GetRegIndex() << std::endl;
			}
		}
		
		m_pClassWriter->AddByteCode( CNS_CALLNATIVE );
		m_pClassWriter->AddByteCode( GetRegIndex() );
		m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );
		std::cout << "CALL NATIVE " << GetRegIndex() << "," << pEntry->GetTargetOffset() << "(" << pEntry->GetName() << ")"  << std::endl;
	}
	else
	{
		m_pClassWriter->AddByteCode( CNS_PUSH );
		m_pClassWriter->AddByteCode( CNS_FP );

		dword dwChildren = (dword)p_pNode->NumberOfChildren();
		if( dwChildren > 0 )
		{
			for( int n = (int)(dwChildren - 1); n >= 0; n-- )
			{
	std::cout << "PARAM " << (*p_pNode)[ n ]->ToString() << std::endl;
				(*p_pNode)[ n ]->Generate( this );
				m_pClassWriter->AddByteCode( CNS_PUSH );
				m_pClassWriter->AddByteCode( GetRegIndex() );
	std::cout << "PARAM PUSH" << (int)GetRegIndex() << std::endl;
			}
		}

		m_pClassWriter->AddByteCode( CNS_CALL );
		int nRel = -m_pClassWriter->GetAddressDifference( pMethod->GetByteCodeOffset() ) + 1;
		m_pClassWriter->AddByteCodeN( nRel );
	std::cout << "CALL " << nRel << std::endl;
		
		if( dwChildren > 0 )
		{
			m_pClassWriter->AddByteCode( CNS_POPN );
			m_pClassWriter->AddByteCodeD( (dword)p_pNode->NumberOfChildren() );
	std::cout << "POPN " << p_pNode->NumberOfChildren() << std::endl;
		}

		m_pClassWriter->AddByteCode( CNS_POP );
		m_pClassWriter->AddByteCode( CNS_FP );

	std::cout << "END CALL FRAME " << pMethod->GetName() << std::endl;
	}

	return true;
}

//*********************************************************************************************************

bool CodeGenerator::Generate( ParserNodes::Variable* p_pNode )
{
	dword dwIndex = m_pClassWriter->AddField( p_pNode );
	
	SymbolTableEntry* pEntry = m_pSymbolTable->Lookup( p_pNode->GetQualifiedName() );
	pEntry->SetTargetOffset( dwIndex );

/*	ParserNode* pNode = p_pNode->GetChild(0);
	if( pNode )
		pNode->Generate( this )*/;

	return true;
}

bool CodeGenerator::Generate( ParserNodes::BlockVar* p_pNode )
{
	//ParserNodes::BlockNode* pBlockNode = ( (ParserNodes::BlockNode*) p_pNode->GetParent());
	//
	//
	//pEntry->SetTargetOffset( pBlockNode->GetLocalVars() );


	ParserNode* pNode = p_pNode->GetChild(0);
	if( pNode )
	{
		pNode->Generate( this );
		SymbolTableEntry* pEntry = m_pSymbolTable->Lookup( p_pNode->GetQualifiedName() );

		m_pClassWriter->AddByteCode( CNS_STLD );
		m_pClassWriter->AddByteCodeD( pEntry->GetTargetOffset() );
		m_pClassWriter->AddByteCode( GetRegIndex() );

		std::cout << "STLD " << pEntry->GetTargetOffset() << ", " << (int)GetRegIndex() << std::endl;
	}

	return true;
}


bool CodeGenerator::Generate( ParserNodes::BlockNode* p_pNode )
{
	if( !p_pNode )
		return false;

	int nSize = (int) p_pNode->NumberOfChildren();
	for( int n = 0; n < nSize; n++ )
	{
		if( !(*p_pNode)[ n ]->Generate( this ) )
			return false;
	}

	//std::cout << "BLOCKVARS " << p_pNode->GetLocalVars() << std::endl;

	return true;
}



bool CodeGenerator::Generate( ParserNodes::MethodNode* p_pNode )
{
	if( !p_pNode )
		return false;

	m_pClassWriter->AddMethod( p_pNode );
	

	if( p_pNode->IsNative() )
	{
		p_pNode->SetByteCodeOffset( 0 );
		return true;
	}

	p_pNode->SetByteCodeOffset( m_pClassWriter->GetByteCodeOffset() );

	std::cout << "Method Offset" << p_pNode->GetByteCodeOffset() << std::endl;

	if( p_pNode->GetLocalVars() > 0 )
	{
		m_pClassWriter->AddByteCode( CNS_LVARS );
		m_pClassWriter->AddByteCodeD( p_pNode->GetLocalVars() );
		std::cout << "LVARS " << p_pNode->GetLocalVars() << std::endl;
	}

	//Parameters
	ParserNode* pParam = p_pNode->GetChild(0);
	BlockNode*  pBlock = (BlockNode*)p_pNode->GetChild(1);

	pBlock->Generate( this );

	dword dwStackItems = p_pNode->GetLocalVars();

	if( dwStackItems )
	{
		m_pClassWriter->AddByteCode( CNS_RETN );
		//m_pClassWriter->AddByteCode( GetRegIndex() );
		m_pClassWriter->AddByteCodeD( dwStackItems );
		std::cout << "Method RETN " << p_pNode->GetLocalVars() << std::endl;
	}
	else
	{
		m_pClassWriter->AddByteCode( CNS_RET );
		//m_pClassWriter->AddByteCode( GetRegIndex() );
		std::cout << "Method RET " << p_pNode->GetLocalVars() << std::endl;
	}

	return true;
}

bool CodeGenerator::Generate( ParserNodes::ClassNode* p_pNode )
{
	m_pClassWriter->AddClass( p_pNode );

	int nSize = (int) p_pNode->NumberOfChildren();
	for( int n = 0; n < nSize; n++ )
	{
		if( !(*p_pNode)[n]->Generate( this ) )
			return false;
	}

	return true;
}

bool CodeGenerator::Generate( ParserNodes::InterfaceNode* p_pNode )
{

	return true;
}


//*********************************************************************************************************

bool CodeGenerator::Generate( ParserNodes::Imports* p_pNode )
{
	return true;
}

bool CodeGenerator::Generate( ParserNodes::NamespaceNode* p_pNode )
{
	m_pClassWriter->AddNamespace( p_pNode );

	int nSize = (int) p_pNode->NumberOfChildren();
	for( int n = 0; n < nSize; n++ )
	{
		if( !(*p_pNode)[n]->Generate( this ) )
			return false;
	}

	return true;
}

bool CodeGenerator::Generate( ParserNodes::CompilationUnit* p_pUnit )
{
	if( !p_pUnit )
		return false;

	int nSize = (int) p_pUnit->NumberOfChildren();
	for( int n = 0; n < nSize; n++ )
	{
		if( !(*p_pUnit)[ n ]->Generate( this ) )
			return false;
	}

	return true;
}


bool CodeGenerator::Generate( ParserNode* p_pNode )
{
	if( !p_pNode )
		return false;

	int nSize = (int) p_pNode->NumberOfChildren();
	for( int n = 0; n < nSize; n++ )
	{
		if( !(*p_pNode)[ n ]->Generate( this ) )
			return false;
	}

	return true;
}


Meson_Cuneus_Compiler_END
