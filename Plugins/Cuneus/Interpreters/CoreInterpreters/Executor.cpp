
#include "Executor.h"

#include <math.h>

#include <CnsCuneusEngine.h>
#include <CnsOpcodes.h>
#include <CnsDecoder.h>
#include <CnsDisasm.h>
#include <CnsMemoryHandle.h>
#include <Converter.h>

#include <CnsAllDataTypes.h>
#include <iostream>


using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Cuneus;
using namespace Meson::Cuneus::VCPU;
using namespace Meson::Cuneus::Util;
using namespace Meson::Cuneus::RTS::Typing;

Meson_Cuneus_BEGIN

Executor::Executor(void) : m_invoker(), m_logger( CuneusEngine::GetInstance()->Logger() )
{
	m_pByteCodeMgr = NULL;
	m_pGarbageCollector = NULL;
	m_pContext = NULL;
}

Executor::~Executor(void)
{
}

void Executor::SetByteCodeMgr( IByteCodeMgr* p_pByteCodeMgr )
{
	m_pByteCodeMgr = p_pByteCodeMgr;
}

void Executor::SetGarbageCollector( IGarbageCollector* p_pGarbageCollector )
{
	m_pGarbageCollector = p_pGarbageCollector;
}

void Executor::SetConstantsMgr( Meson::Cuneus::ConstantsMgr*  p_pConstantsMgr )
{
	m_pConstantsMgr = p_pConstantsMgr;
}


void Executor::SetGlobalDataMgr( GlobalDataMgr* p_pGlobalDataMgr )
{
	m_pGlobalDataMgr = p_pGlobalDataMgr;
}

void Executor::SetContext( Context* p_pContext )
{
	m_pContext = p_pContext;
}


dword Executor::EnterMethodFrame( dword p_dwSize )
{
	dword dwMark = (dword)(m_pContext->DataList).Size();

	for( dword dw = 0; dw < p_dwSize; dw++ )
	{
		(m_pContext->DataList).Add( DataType() );
	}

	return dwMark;
}

void  Executor::LeaveMethodFrame( dword p_dwPosition )
{
	(m_pContext->DataList).RemoveRange( p_dwPosition, (m_pContext->DataList).Size() - p_dwPosition );
}



#define REG m_pContext->Registers.R
#define FP  m_pContext->Registers.FP
#define SP  m_pContext->Registers.SP
#define PC  m_pContext->Registers.PC

#define FLAGS m_pContext->Flags
#define STACK m_pContext->Stack



int Executor::Execute( void )
{
	if( PC->GetValue() == 0xFFFFFFFF )
		return -1;

	byte* pByteCode = m_pByteCodeMgr->GetByteCodePointer( (size_t)PC->GetValue() );
	byte* pMarker = pByteCode;

	if( (*pByteCode > 0x7F) && !m_pContext->Flags.brk )
	{
		m_pContext->Flags.brk = 1;
		return -1;
	}
	m_pContext->Flags.brk = 0;


	byte	byOper1, 
			byOper2,
			byOper3,
			byLen = 0;

	word*	pwIndex;
	dword*	pdwIndex;
	int*    pnRelative;


	String str;

	Disasm disasm;
	disasm.SetByteCodeBase( m_pByteCodeMgr->GetByteCodePointer(0) );
	disasm.Disassemble( *PC, str );
	std::cout << str << std::endl;

	switch( pByteCode[ byLen ] )
	{
		case CNS_NOP:
			byLen++;
			break;

		//Load Global Constant
		case CNS_LDGC:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);
			
			REG[ byOper1] = m_pConstantsMgr->GetConstant( *pdwIndex )->Clone();

			byLen += 6;

			break;

		//Load Global Constant
		case CNS_LDCC:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);
			
			//REG[ byOper1] = m_pConstantsMgr->GetConstant( *pdwIndex );

			byLen += 6;

			break;

		//Load Global Data
		case CNS_LDGD:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);
			
			REG[ byOper1] = m_pGlobalDataMgr->GetDataItem( *pdwIndex );

			byLen += 6;

			break;


		// Load Local Param
		case CNS_LDLD:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);
			
			REG[ byOper1 ] = STACK.Peek( FP->GetValue() + *pdwIndex );

			byLen += 6;

			break;

		// Load Local Param
		case CNS_LDP:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);
			
			REG[ byOper1 ] = STACK.Peek( FP->GetValue() - *pdwIndex - 2 );

			byLen += 6;

			break;

		// Load Object Data
		case CNS_LDOD:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];
			
			//REG[ byOper1] = ( (TypeObject*) &REG[ byOper2 ] )->GetDataItem( byOper3 );
			REG[ byOper1] = ( (TypeObject&) *REG[ byOper2 ] ).GetDataItem( byOper3 );
			
			byLen += 4;
			break;



		//Store Global Data
		case CNS_STGD:
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			byOper1 = pByteCode[ byLen + 5 ];
			m_pGlobalDataMgr->SetDataItem( *pdwIndex, REG[ byOper1] );

			byLen += 6;
			break;

		// Load Local Param
		case CNS_STLD:
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			byOper1 = pByteCode[ byLen + 5 ];
			
			STACK.Poke( ( FP->GetValue() + *pdwIndex ), REG[ byOper1 ] );

			byLen += 6;
			break;

		// Load Local Param
		case CNS_STP:
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			byOper1 = pByteCode[ byLen + 5 ];
			
			//*STACK.Peek( FP->GetValue() - *pdwIndex - 1 ) = *REG[ byOper1 ];
			STACK.Poke( FP->GetValue() - *pdwIndex - 2, REG[ byOper1 ] );

			byLen += 6;
			break;


		
		// Store Object Data
		case CNS_STOD:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];
			
			( (TypeObject*) &REG[ byOper1 ] )->SetDataItem( byOper2, REG[ byOper3] );

			byLen += 4;

			break;



		case CNS_COPY:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			*REG[ byOper1] = *REG[ byOper2 ];
			byLen += 3;
			break;


		case CNS_NEG:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			REG[ byOper1] = -(*REG[ byOper2 ]);
			
			byLen += 3;
			break;

		case CNS_ADD:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			REG[ byOper1] = *REG[ byOper2 ] + REG[ byOper3 ];

			byLen += 4;
			break;

		case CNS_SUB:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			REG[ byOper1] = *REG[ byOper2 ] - REG[ byOper3 ];
			byLen += 4;
			break;

		case CNS_MUL:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			REG[ byOper1] = *REG[ byOper2 ] * REG[ byOper3 ];
			byLen += 4;
			break;

		case CNS_DIV:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			REG[ byOper1] = *REG[ byOper2 ] / REG[ byOper3 ];
			byLen += 4;
			break;

		case CNS_MOD:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			REG[ byOper1] = *REG[ byOper2 ] % REG[ byOper3 ];
			byLen += 4;
			break;

		case CNS_INC:
			byOper1 = pByteCode[ byLen + 1 ];
			++(*REG[ byOper1]);
			byLen += 2;
			break;

		case CNS_DEC:
			byOper1 = pByteCode[ byLen + 1 ];
			--(*REG[ byOper1]);
			byLen += 2;
			break;

		case CNS_NOT:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			REG[ byOper1] = ~*REG[ byOper2 ];
			byLen += 3;
			break;

		case CNS_AND:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			REG[ byOper1] = *REG[ byOper2 ] & REG[ byOper3 ];
			byLen += 4;
			break;

		case CNS_OR:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			REG[ byOper1] = *REG[ byOper2 ] | REG[ byOper3 ];
			byLen += 4;
			break;

		case CNS_XOR:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			REG[ byOper1] = *REG[ byOper2 ] ^ REG[ byOper3 ];
			byLen += 4;
			break;

		case CNS_SHL:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			REG[ byOper1] = *REG[ byOper2 ] << REG[ byOper3 ];
			byLen += 4;
			break;

		case CNS_SHR:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

//TODO: Sign Extension
			REG[ byOper1] = *REG[ byOper2 ] >> REG[ byOper3 ];

			byLen += 4;
			break;

		case CNS_USHR:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			REG[ byOper1] = *REG[ byOper2 ] >> REG[ byOper3 ];
			byLen += 4;
			break;


		case CNS_XCHG:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			{
				TPointer< DataType > tmp;
				tmp = REG[ byOper1 ];
				REG[ byOper1 ] = REG[ byOper2 ];
				REG[ byOper2 ] = tmp;
			}
			byLen += 3;
			break;

		case CNS_PUSH:
			byOper1 = pByteCode[ byLen + 1 ];
			STACK.Push( REG[ byOper1 ] );
			byLen += 2;
			break;

		case CNS_PUSHALL:
			for(int n=0; n<0x20; n++)
			{
				STACK.Push( REG[ n ] );
			}
			//STACK.Push( (TPointer<DataType>&) FP );
			//STACK.Push( (TPointer<DataType>&) SP );
			//STACK.Push( (TPointer<DataType>&) PC );
			byLen++;
			break;

		case CNS_POP:
			byOper1 = pByteCode[ byLen + 1 ];
			REG[ byOper1 ] = STACK.Pop();
			byLen += 2;
			break;

		case CNS_POPALL:
			for(int n=0; n<0x20; n++)
			{
				REG[ n ] = STACK.Pop();
			}
			//FP = (TPointer<TypeDWord>&) STACK.Pop();
			//SP = (TPointer<TypeDWord>&) STACK.Pop();
			//PC = (TPointer<TypeDWord>&) STACK.Pop();

			byLen++;
			break;

		case CNS_PEEK:
			byOper1 = pByteCode[ byLen + 1 ];
			REG[ byOper1 ] = STACK.Peek();
			byLen += 2;
			break;

		case CNS_CMP:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			{
				int n = REG[ byOper1]->Compare( REG[ byOper2 ] );
				FLAGS.eq = ( n == 0 );
				FLAGS.lt = ( n == -1 );
				FLAGS.gt = ( n == 1 );
			}
			byLen += 3;
			break;


		case CNS_EQ:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]) == (*REG[ byOper2 ]);
			
			FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) == 0;

			byLen += 3;
			break;

		case CNS_NE:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]) != (*REG[ byOper2 ]);
			FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) != 0;
			byLen += 3;
			break;

		case CNS_LT:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]) < (*REG[ byOper2 ]);
			FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) < 0;
			byLen += 3;
			break;

		case CNS_GT:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]) > (*REG[ byOper2 ]);
			FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) > 0;
			byLen += 3;
			break;

		case CNS_LE:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]) <= (*REG[ byOper2 ]);
			FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) <= 0;
			byLen += 3;
			break;

		case CNS_GE:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]) >= (*REG[ byOper2 ]);
			FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) >= 0;
			byLen += 3;
			break;

		case CNS_HALT:
			return -1;
			//break;

		case CNS_RET:
			//byOper1 = pByteCode[ byLen + 1 ];
			PC = (TPointer<TypeDWord>&) STACK.Pop();
	//STACK.Push( REG[ byOper1 ] );
			return byLen++;

		case CNS_RETN:
			//byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			
			STACK.PopN( *pdwIndex );
			PC = (TPointer<TypeDWord>&) STACK.Pop();
	//STACK.Push( REG[ byOper1 ] );

			return byLen + 5;

		case CNS_JRT:
			if( FLAGS.tru )
			{
				pnRelative = (int*) (pByteCode + byLen + 1);
				*PC = *PC + *pnRelative;
				return byLen + 5;
			}
			byLen += 5;
			break;

		case CNS_JRF:
			if( !FLAGS.tru )
			{
				pnRelative = (int*) (pByteCode + byLen + 1);
				*PC = *PC + *pnRelative;
				return byLen + 5;
			}
			byLen += 5;
			break;

		case CNS_JR:
			pnRelative = (int*) (pByteCode + byLen + 1);
			*PC = *PC + *pnRelative;
			return byLen + 5;

		case CNS_CALL:
			pnRelative = (int*) (pByteCode + byLen + 1);
			
			STACK.Push( TPointer<DataType> ( new TypeDWord( *PC + 5 ) ) );
			
			FP = TPointer<TypeDWord> ( new TypeDWord( STACK.GetTopPos() ) );

			*PC = *PC + *pnRelative;
			return byLen + 5;

		case CNS_POPN:
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			STACK.PopN( *pdwIndex );
			byLen+=5;
			break;

		case CNS_CAST:
			break;

		case CNS_CALLOBJECT:
			byOper1 = pByteCode[ byLen + 1 ];
			pwIndex = (word*) (pByteCode + byLen + 2);

			m_invoker.InvokeObject( m_pContext, byOper1, *pwIndex );
			byLen += 4;
			break;

		case CNS_CALLNATIVE:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);

			{
				Meson::Cuneus::RTS::Reflection::TypeMethod* pMethod = CuneusEngine::GetInstance()->GetMethod( *pdwIndex );
				REG[ byOper1 ] = m_invoker.InvokeNative( m_pContext, *pMethod );
			}

			byLen += 6;
			break;

		case CNS_CALLVIRTUAL:
			byLen += 1;
			break;

		case CNS_LVARS:
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			STACK.LocalVars( *pdwIndex );
			byLen += 5;
			break;

		case CNS_STT:
			FLAGS.tru = 1;
			byLen++;
			break;

		case CNS_CLT:
			FLAGS.tru = 0;
			byLen++;
			break;

		case CNS_NEWARRAY:
			{
				byOper1 = pByteCode[ byLen + 1 ];
				pdwIndex = (dword*) (pByteCode + byLen + 2);
				byOper2 = pByteCode[ byLen + 6 ];

				TPointer<TypeArray> pArray( new TypeArray( (dword)((TypeLong*)(DataType*)REG[byOper2])->GetValue(), *pdwIndex ) );

				REG[ byOper1 ] = pArray;
			}

			byLen += 7;
			break;

		//case CNS_DISPOSEARRAY:

		case CNS_GETELM:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];
			
			REG[ byOper1 ] = ( (TypeArray*) (DataType*) REG[ byOper2 ] )->GetElement( *( (TypeDWord*) (DataType*) REG[ byOper3 ] ) );

			byLen += 4;
			break;

		case CNS_SETELM:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];
			
			( (TypeArray*) (DataType*) REG[ byOper1 ] )->SetElement( *( (TypeDWord*) (DataType*) REG[ byOper2 ] ), REG[ byOper3 ] );

			byLen += 4;
			break;


		//case CNS_LENGHT:

		default:
			byLen++;
	}

	(*PC) = (*PC) + (dword)(byLen);

	return byLen;
}

//---------------------------------------------------------------------------------------------------------

//int Executor::Execute( void )
//{
//	byte* pByteCode = m_pByteCodeMgr->GetByteCodePointer( (size_t)((m_pContext->Registers).PC->GetValue()) );
//	byte* pMarker = pByteCode;
//
//	if( (*pByteCode > 0x7F) && !m_pContext->Flags.brk )
//	{
//		m_pContext->Flags.brk = 1;
//		return -1;
//	}
//	m_pContext->Flags.brk = 0;
//
//
//	byte	byOper1, 
//			byOper2,
//			byOper3,
//			byLen = 0;
//
//	word*	pwIndex;
//	dword*	pdwIndex;
//
//	switch( pByteCode[ byLen ] )
//	{
//		case CNS_NOP:
//			byLen++;
//			break;
//
//		//Load Global Constant
//		case CNS_LDGC:
//
//			break;
//
//		//Load Global Data
//		case CNS_LDGD:
//			byOper1 = pByteCode[ byLen + 1 ];
//			pwIndex = (word*) pByteCode + byLen + 2;
//			
//			REG[ byOper1] = m_pGlobalDataMgr->GetDataItem( *pwIndex );
//
//			byLen += 4;
//
//			break;
//
//		//Load Global Reference
//		case CNS_LDGR:
//			break;
//
//		//Store Global Data
//		case CNS_STGD:
//			pwIndex = (word*) pByteCode + byLen + 1;
//			byOper1 = pByteCode[ byLen + 3 ];
//			m_pGlobalDataMgr->SetDataItem( *pwIndex, REG[ byOper1] );
//
//			byLen += 4;
//			break;
//		
//		//Store Global Reference
//		case CNS_STGR:
//			break;
//
//		// Load Class Constant
//		case CNS_LDCC:
//			break;
//
//		// Load Object Data
//		case CNS_LDOD:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//			
//			//REG[ byOper1] = ( (TypeObject*) &REG[ byOper2 ] )->GetDataItem( byOper3 );
//			REG[ byOper1] = ( (TypeObject&) *REG[ byOper2 ] ).GetDataItem( byOper3 );
//			
//			byLen += 4;
//			break;
//
//		// Load Object Reference
//		case CNS_LDOR:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//			
//			REG[ byOper1] = ( (TypeObject*) &REG[ byOper2 ] )->GetRefItem( byOper3 );
//
//			byLen += 4;
//			break;
//
//		// Store Object Data
//		case CNS_STOD:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//			
//			( (TypeObject*) &REG[ byOper1 ] )->SetDataItem( byOper2, REG[ byOper3] );
//
//			byLen += 4;
//
//			break;
//
//		// Store Object Reference
//		case CNS_STOR:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//			
//			( (TypeObject*) &REG[ byOper1 ] )->SetRefItem( byOper2, (TPointer<TypeRef>&) REG[ byOper3] );
//
//			byLen += 4;
//
//			break;
//
//		case CNS_COPY:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//
//			REG[ byOper1] = REG[ byOper2 ];
//			byLen += 3;
//			break;
//
//
//		case CNS_NEG:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//
//			REG[ byOper1] = -(*REG[ byOper2 ]);
//			
//			byLen += 3;
//			break;
//
//		case CNS_ADD:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//
//			REG[ byOper1] = *REG[ byOper2 ] + *REG[ byOper3 ];
//			byLen += 4;
//			break;
//
//		case CNS_SUB:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//
//			REG[ byOper1] = *REG[ byOper2 ] - *REG[ byOper3 ];
//			byLen += 4;
//			break;
//
//		case CNS_MUL:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//
//			REG[ byOper1] = *REG[ byOper2 ] * *REG[ byOper3 ];
//			byLen += 4;
//			break;
//
//		case CNS_DIV:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//
//			REG[ byOper1] = *REG[ byOper2 ] / *REG[ byOper3 ];
//			byLen += 4;
//			break;
//
//		case CNS_MOD:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//
//			REG[ byOper1] = *REG[ byOper2 ] % *REG[ byOper3 ];
//			byLen += 4;
//			break;
//
//		case CNS_INC:
//			byOper1 = pByteCode[ byLen + 1 ];
//			++*REG[ byOper1];
//			byLen += 2;
//			break;
//
//		case CNS_DEC:
//			byOper1 = pByteCode[ byLen + 1 ];
//			--*REG[ byOper1];
//			byLen += 2;
//			break;
//
//		case CNS_NOT:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//
//			REG[ byOper1] = ~*REG[ byOper2 ];
//			byLen += 3;
//			break;
//
//		case CNS_AND:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//
//			REG[ byOper1] = *REG[ byOper2 ] & *REG[ byOper3 ];
//			byLen += 4;
//			break;
//
//		case CNS_OR:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//
//			REG[ byOper1] = *REG[ byOper2 ] | *REG[ byOper3 ];
//			byLen += 4;
//			break;
//
//		case CNS_XOR:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//
//			REG[ byOper1] = *REG[ byOper2 ] ^ *REG[ byOper3 ];
//			byLen += 4;
//			break;
//
//		case CNS_SHL:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//
//			REG[ byOper1] = *REG[ byOper2 ] << *REG[ byOper3 ];
//			byLen += 4;
//			break;
//
//		case CNS_SHR:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			byOper3 = pByteCode[ byLen + 3 ];
//
//			REG[ byOper1] = *REG[ byOper2 ] >> *REG[ byOper3 ];
//			byLen += 4;
//			break;
//
//
//		case CNS_XCHG:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			{
//				TPointer< DataType > tmp;
//				tmp = REG[ byOper1 ];
//				REG[ byOper1 ] = REG[ byOper2 ];
//				REG[ byOper2 ] = tmp;
//			}
//			byLen += 3;
//			break;
//
//		case CNS_PUSH:
//			byOper1 = pByteCode[ byLen + 1 ];
//			STACK.Push( REG[ byOper1 ] );
//			byLen += 2;
//			break;
//
//		case CNS_PUSHALL:
//			for(int n=0; n<0x20; n++)
//			{
//				STACK.Push( REG[ n ] );
//			}
//			STACK.Push( (TPointer<DataType>&) m_pContext->Registers.FP );
//			STACK.Push( (TPointer<DataType>&) m_pContext->Registers.SP );
//			STACK.Push( (TPointer<DataType>&) m_pContext->Registers.PC );
//			byLen++;
//			break;
//
//		case CNS_POP:
//			byOper1 = pByteCode[ byLen + 1 ];
//			REG[ byOper1 ] = STACK.Pop();
//			byLen += 2;
//			break;
//
//		case CNS_POPALL:
//			for(int n=0; n<0x20; n++)
//			{
//				REG[ n ] = STACK.Pop();
//			}
//			m_pContext->Registers.FP = (TPointer<TypeDWord>&) STACK.Pop();
//			m_pContext->Registers.SP = (TPointer<TypeDWord>&) STACK.Pop();
//			m_pContext->Registers.PC = (TPointer<TypeDWord>&) STACK.Pop();
//
//			byLen++;
//			break;
//
//		case CNS_PEEK:
//			byOper1 = pByteCode[ byLen + 1 ];
//			REG[ byOper1 ] = STACK.Peek();
//			byLen += 2;
//			break;
//
//		case CNS_CMP:
//			byOper1 = pByteCode[ byLen + 1 ];
//			byOper2 = pByteCode[ byLen + 2 ];
//			{
//				int n = REG[ byOper1]->Compare( *REG[ byOper2 ] );
//				FLAGS.eq = ( n == 0 );
//				FLAGS.lt = ( n == -1 );
//				FLAGS.gt = ( n == 1 );
//			}
//			byLen += 3;
//			break;
//
//		//case CNS_JREQ:
//		//	pwIndex = (word*) pByteCode + byLen + 2;
//
//		//	if( m_pContext->Flags.eq )
//		//		*m_pContext->Registers.PC = *m_pContext->Registers.PC + (short)(*pwIndex);
//		//	byLen += 3;
//		//	break;
//
//
//		//case CNS_JRNE:
//		//	pwIndex = (word*) pByteCode + byLen + 2;
//
//		//	if( m_pContext->Flags.lt && m_pContext->Flags.gt )
//		//		*m_pContext->Registers.PC = *m_pContext->Registers.PC + (short)(*pwIndex);
//		//	byLen += 3;
//		//	break;
//
//		//case CNS_JRLT:
//		//	pwIndex = (word*) pByteCode + byLen + 2;
//
//		//	if( m_pContext->Flags.lt )
//		//		*m_pContext->Registers.PC = *m_pContext->Registers.PC + (short)(*pwIndex);
//		//	byLen += 3;
//		//	break;
//
//		//case CNS_JRGT:
//		//	pwIndex = (word*) pByteCode + byLen + 2;
//
//		//	if( m_pContext->Flags.gt )
//		//		*m_pContext->Registers.PC = *m_pContext->Registers.PC + (short)(*pwIndex);
//		//	byLen += 3;
//		//	break;
//
//		//case CNS_JRLE:
//		//	pwIndex = (word*) pByteCode + byLen + 2;
//
//		//	if( m_pContext->Flags.lt || m_pContext->Flags.eq )
//		//		*m_pContext->Registers.PC = *m_pContext->Registers.PC + (short)(*pwIndex);
//		//	byLen += 3;
//		//	break;
//
//		//case CNS_JRGE:
//		//	pwIndex = (word*) pByteCode + byLen + 2;
//
//		//	if( m_pContext->Flags.gt || m_pContext->Flags.eq )
//		//		*m_pContext->Registers.PC = *m_pContext->Registers.PC + (short)(*pwIndex);
//		//	byLen += 3;
//		//	break;
//
//		case CNS_HALT:
//			//TODO: HALT
//			break;
//
//		case CNS_RET:
//			byOper1 = pByteCode[ byLen + 1 ];
//			REG[ byOper1 ] = STACK.Pop();
//			m_pContext->Registers.PC = (TPointer<TypeDWord>&) STACK.Pop();
//
//			byLen += 2;
//			break;
//
//
//		case CNS_INVOKEOBJECT:
//			byOper1 = pByteCode[ byLen + 1 ];
//			pwIndex = (word*) pByteCode + byLen + 3;
//
//			m_invoker.InvokeObject( m_pContext, byOper1, *pwIndex );
//			byLen += 4;
//			break;
//
//		case CNS_INVOKENATIVE:
//			pwIndex = (word*) pByteCode + byLen + 2;
//			byOper1 = pByteCode[ byLen + 3 ];
//
//			m_invoker.InvokeNative( m_pContext, *pwIndex, byOper1 );
//
//			byLen += 4;
//			break;
//	}
//
//	return byLen;
//}



//int Executor::Execute( void )
//{
//	byte* pByteCode = m_pByteCodeMgr->GetByteCodePointer( m_pRegisters->PC );
//	byte* pMarker = pByteCode;
//
//	if( (*pByteCode > 0x7F) && !m_pFlags->brk )
//	{
//		m_pFlags->brk = 1;
//		return -1;
//	}
//	m_pFlags->brk = 0;
//
//	Meson::Cuneus::Util::Disasm disasm( m_pRegisters, m_pFlags, m_pStack );
//
//	String str(100);
//	Converter::ToHex( m_pRegisters->PC, str );
//	str += '\t';
//	str += disasm.Disassemble( pByteCode );
//	m_logger.Out << str << "\r\n";
//
//	
//
//
//	byte	byOper1, 
//			byOper2,
//			byOper3;
//
//	switch( *pByteCode )
//	{
//		case CNS_NOP:
//			pByteCode++;
//			break;
//
//		case CNS_SET:
//			pByteCode++;
//
//			byOper1 = *pByteCode;
//			pByteCode++;
//
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = *((qword*)pByteCode);
//				pByteCode += sizeof( qword );
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F ] = *((dword*)pByteCode);
//				//32 - PC
//				//33 - SP
//				//34 - FP
//				if( (byOper1 & 0x3F) == 32 )
//					return 0;
//				pByteCode += sizeof( dword );
//			}
//			break;
//
//		case CNS_COPY:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->L[ byOper1 & 0x0F ] = m_pRegisters->L[ byOper2 & 0x0F ];
//			}
//			else
//			{
//				m_pRegisters->R[ byOper1 & 0x3F] = m_pRegisters->R[ byOper2 & 0x3F];
//			}
//			break;
//
//		case CNS_NEG:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->L[ byOper1 & 0x0F ].LL = -m_pRegisters->L[ byOper2 & 0x0F ].LL;
//			}
//			else
//			{
//				m_pRegisters->R[ byOper1 & 0x3F].N = -m_pRegisters->R[ byOper2 & 0x3F].N;
//			}
//			break;
//
//		case CNS_NEGF:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->L[ byOper1 & 0x0F ].D = -m_pRegisters->L[ byOper2 & 0x0F ].D;
//			}
//			else
//			{
//				m_pRegisters->R[ byOper1 & 0x3F].F = -m_pRegisters->R[ byOper2 & 0x3F].F;
//			}
//			break;
//
//
//		case CNS_ADD:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pRegisters->l[ byOper2 & 0x0F ] + m_pRegisters->l[ byOper3 & 0x0F ];
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pRegisters->r[ byOper2 & 0x3F] + m_pRegisters->r[ byOper3 & 0x3F];
//			}
//			break;
//
//		case CNS_ADDF:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->L[ byOper1 & 0x0F ].D = m_pRegisters->L[ byOper2 & 0x0F ].D + m_pRegisters->L[ byOper3 & 0x0F ].D;
//			}
//			else
//			{
//				m_pRegisters->R[ byOper1 & 0x3F].F = m_pRegisters->R[ byOper2 & 0x3F].F + m_pRegisters->R[ byOper3 & 0x3F].F;
//			}
//			break;
//
//		case CNS_SUB:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pRegisters->l[ byOper2 & 0x0F ] - m_pRegisters->l[ byOper3 & 0x0F ];
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pRegisters->r[ byOper2 & 0x3F] - m_pRegisters->r[ byOper3 & 0x3F];
//			}
//			break;
//
//		case CNS_SUBF:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->L[ byOper1 & 0x0F ].D = m_pRegisters->L[ byOper2 & 0x0F ].D - m_pRegisters->L[ byOper3 & 0x0F ].D;
//			}
//			else
//			{
//				m_pRegisters->R[ byOper1 & 0x3F].F = m_pRegisters->R[ byOper2 & 0x3F].F - m_pRegisters->R[ byOper3 & 0x3F].F;
//			}
//			break;
//
//
//		case CNS_MUL:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pRegisters->l[ byOper2 & 0x0F ] * m_pRegisters->l[ byOper3 & 0x0F ];
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pRegisters->r[ byOper2 & 0x3F] * m_pRegisters->r[ byOper3 & 0x3F];
//			}
//			break;
//
//		case CNS_MULF:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->L[ byOper1 & 0x0F ].D = m_pRegisters->L[ byOper2 & 0x0F ].D * m_pRegisters->L[ byOper3 & 0x0F ].D;
//			}
//			else
//			{
//				m_pRegisters->R[ byOper1 & 0x3F].F = m_pRegisters->R[ byOper2 & 0x3F].F * m_pRegisters->R[ byOper3 & 0x3F].F;
//			}
//			break;
//
//		case CNS_DIV:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pRegisters->l[ byOper2 & 0x0F ] / m_pRegisters->l[ byOper3 & 0x0F ];
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pRegisters->r[ byOper2 & 0x3F] / m_pRegisters->r[ byOper3 & 0x3F];
//			}
//			break;
//
//		case CNS_DIVF:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->L[ byOper1 & 0x0F ].D = m_pRegisters->L[ byOper2 & 0x0F ].D / m_pRegisters->L[ byOper3 & 0x0F ].D;
//			}
//			else
//			{
//				m_pRegisters->R[ byOper1 & 0x3F].F = m_pRegisters->R[ byOper2 & 0x3F].F / m_pRegisters->R[ byOper3 & 0x3F].F;
//			}
//			break;
//
//		case CNS_MOD:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pRegisters->l[ byOper2 & 0x0F ] % m_pRegisters->l[ byOper3 & 0x0F ];
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pRegisters->r[ byOper2 & 0x3F] % m_pRegisters->r[ byOper3 & 0x3F];
//			}
//			break;
//
//		case CNS_MODF:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				double dTmp = m_pRegisters->L[ byOper2 & 0x0F ].D / m_pRegisters->L[ byOper3 & 0x0F ].D;
//				dTmp = dTmp - floor( dTmp );
//				m_pRegisters->L[ byOper1 & 0x0F ].D = dTmp;
//			}
//			else
//			{
//				float fTmp = m_pRegisters->R[ byOper2 & 0x3F].F / m_pRegisters->R[ byOper3 & 0x3F].F;
//				fTmp = fTmp - floor( fTmp );
//				m_pRegisters->R[ byOper1 & 0x3F].F = fTmp;
//			}
//			break;
//
//		case CNS_INC:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ]++;
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F]++;
//			}
//			break;
//
//		case CNS_INCF:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->L[ byOper1 & 0x0F ].D++;
//			}
//			else
//			{
//				m_pRegisters->R[ byOper1 & 0x3F].F++;
//			}
//			break;
//
//		case CNS_DEC:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ]--;
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F]--;
//			}
//			break;
//
//		case CNS_DECF:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->L[ byOper1 & 0x0F ].D--;
//			}
//			else
//			{
//				m_pRegisters->R[ byOper1 & 0x3F].F--;
//			}
//			break;
//
//		case CNS_NOT:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->L[ byOper1 & 0x0F ].LL = ~m_pRegisters->L[ byOper2 & 0x0F ].LL;
//			}
//			else
//			{
//				m_pRegisters->R[ byOper1 & 0x3F].N = ~m_pRegisters->R[ byOper2 & 0x3F].N;
//			}
//			break;
//
//		case CNS_AND:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pRegisters->l[ byOper2 & 0x0F ] & m_pRegisters->l[ byOper3 & 0x0F ];
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pRegisters->r[ byOper2 & 0x3F] & m_pRegisters->r[ byOper3 & 0x3F];
//			}
//			break;
//
//		case CNS_OR:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pRegisters->l[ byOper2 & 0x0F ] | m_pRegisters->l[ byOper3 & 0x0F ];
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pRegisters->r[ byOper2 & 0x3F] | m_pRegisters->r[ byOper3 & 0x3F];
//			}
//			break;
//
//		case CNS_XOR:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pRegisters->l[ byOper2 & 0x0F ] ^ m_pRegisters->l[ byOper3 & 0x0F ];
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pRegisters->r[ byOper2 & 0x3F] ^ m_pRegisters->r[ byOper3 & 0x3F];
//			}
//			break;
//
//		case CNS_SHL:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pRegisters->l[ byOper2 & 0x0F ] << m_pRegisters->l[ byOper3 & 0x0F ];
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pRegisters->r[ byOper2 & 0x3F] << m_pRegisters->r[ byOper3 & 0x3F];
//			}
//			break;
//
//		case CNS_SHR:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pRegisters->l[ byOper2 & 0x0F ] >> m_pRegisters->l[ byOper3 & 0x0F ];
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pRegisters->r[ byOper2 & 0x3F] >> m_pRegisters->r[ byOper3 & 0x3F];
//			}
//			break;
//
//		case CNS_SAR:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				qword llAmount = m_pRegisters->l[ byOper3 & 0x0F ];
//				qword llTemp = m_pRegisters->l[ byOper2 & 0x0F ];
//
//				if( llTemp & 0x8000000000000000 )
//				{
//					qword llMask = -1 << ( 64 - llAmount );
//					m_pRegisters->l[ byOper1 & 0x0F ] = llMask | ( llTemp >> llAmount );
//				}
//				else
//				{
//					m_pRegisters->l[ byOper1 & 0x0F ] = ( llTemp >> llAmount );
//				}
//			}
//			else
//			{
//				dword lAmount = m_pRegisters->r[ byOper3 & 0x3F ];
//				dword lTemp = m_pRegisters->r[ byOper2 & 0x3F ];
//
//				if( lTemp & 0x8000000000000000 )
//				{
//					dword lMask = -1 << ( 32 - lAmount );
//					m_pRegisters->r[ byOper1 & 0x3F ] = lMask | ( lTemp >> lAmount );
//				}
//				else
//				{
//					m_pRegisters->r[ byOper1 & 0x3F ] = ( lTemp >> lAmount );
//				}
//			}
//			break;
//
//		case CNS_ROL:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				qword llAmount = m_pRegisters->l[ byOper3 & 0x0F ];
//				qword llCompliment = 64 - llAmount;
//
//				qword llMask = -1 << llCompliment;
//				qword llTemp = m_pRegisters->l[ byOper2 & 0x0F ] & llMask;
//
//				llTemp >>= llCompliment;
//
//				m_pRegisters->l[ byOper1 & 0x0F ] = ( llTemp ) | ( m_pRegisters->l[ byOper2 & 0x0F ] << llAmount );
//			}
//			else
//			{
//				dword lAmount = m_pRegisters->r[ byOper3 & 0x3F ];
//				dword lCompliment = 32 - lAmount;
//
//				dword lMask = -1 << lCompliment;
//				dword lTemp = m_pRegisters->r[ byOper2 & 0x3F ] & lMask;
//
//				lTemp >>= lCompliment;
//
//				m_pRegisters->r[ byOper1 & 0x3F ] = ( lTemp ) | ( m_pRegisters->r[ byOper2 & 0x3F ] << lAmount );
//			}
//			break;
//
//		case CNS_ROR:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				qword llAmount = m_pRegisters->l[ byOper3 & 0x0F ];
//				qword llCompliment = 64 - llAmount;
//
//				qword llMask = -1 >> llCompliment;
//				qword llTemp = m_pRegisters->l[ byOper2 & 0x0F ] & llMask;
//
//				llTemp <<= llCompliment;
//
//				m_pRegisters->l[ byOper1 & 0x0F ] = ( llTemp ) | ( m_pRegisters->l[ byOper2 & 0x0F ] >> llAmount );
//			}
//			else
//			{
//				dword lAmount = m_pRegisters->r[ byOper3 & 0x3F ];
//				dword lCompliment = 32 - lAmount;
//
//				dword lMask = -1 >> lCompliment;
//				dword lTemp = m_pRegisters->r[ byOper2 & 0x3F ] & lMask;
//
//				lTemp <<= lCompliment;
//
//				m_pRegisters->r[ byOper1 & 0x3F ] = ( lTemp ) | ( m_pRegisters->r[ byOper2 & 0x3F ] >> lAmount );
//			}
//			break;
//
//		case CNS_PUSHV:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//
//			if( m_pFlags->dbl )
//			{
//				qword qwTemp = *((qword*)pByteCode);
//				pByteCode += sizeof( qword );
//				m_pStack->PushQWord( qwTemp );
//			}
//			else
//			{
//				dword dwTemp = *((dword*)pByteCode);
//				pByteCode += sizeof( dword );
//				m_pStack->PushDWord( dwTemp );
//			}
//			break;
//
//		case CNS_PUSH:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pStack->PushQWord( m_pRegisters->l[ byOper1 & 0x0F ] );
//			}
//			else
//			{
//				m_pStack->PushDWord( m_pRegisters->r[ byOper1 & 0x3F] );
//			}
//			break;
//
//		case CNS_PUSHALL:
//			pByteCode++;
//			m_pStack->Push( m_pFlags, sizeof(dword) );
//			m_pStack->Push( m_pRegisters->r, sizeof(dword)*35 );
//			break;
//
//		case CNS_POP:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pStack->PopQWord();
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pStack->PopDWord();
//			}
//			break;
//
//		case CNS_POPALL:
//			pByteCode++;
//			m_pStack->Pop( m_pFlags, sizeof(dword) );
//			m_pStack->Pop( m_pRegisters->r, sizeof(dword)*35 );
//			break;
//
//		case CNS_PEEK:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = m_pStack->PeekQWord();
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F] = m_pStack->PeekDWord();
//			}
//			break;
//
//		case CNS_DUP:
//			pByteCode++;
//			if( m_pFlags->dbl )
//				m_pStack->DupQWord();
//			else
//				m_pStack->DupDWord();
//			break;
//
//		case CNS_SWAP:
//
//		case CNS_XCHG:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				qword qwTemp = m_pRegisters->L[ byOper1 & 0x0F ].LL;
//				m_pRegisters->L[ byOper1 & 0x0F ].LL = m_pRegisters->L[ byOper2 & 0x0F ].LL;
//				m_pRegisters->L[ byOper2 & 0x0F ].LL = qwTemp;
//			}
//			else
//			{
//				dword dwTemp = m_pRegisters->R[ byOper1 & 0x3F].N;
//				m_pRegisters->R[ byOper1 & 0x3F].N = m_pRegisters->R[ byOper2 & 0x3F].N;
//				m_pRegisters->R[ byOper2 & 0x3F].N = dwTemp;
//			}
//			break;
//		
//		case CNS_HALT:
//			return -1;
//
//		case CNS_CMP:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				long long llOper1 = m_pRegisters->L[ byOper1 & 0x0F ].LL;
//				long long llOper2 = m_pRegisters->L[ byOper2 & 0x0F ].LL;
//				if( llOper1 < llOper2 )
//					m_pFlags->lt = 1;
//				else if( llOper1 > llOper2 )
//					m_pFlags->gt = 1;
//				else
//					m_pFlags->eq = 1;
//			}
//			else
//			{
//				long lOper1 = m_pRegisters->R[ byOper1 & 0x3F].N;
//				long lOper2 = m_pRegisters->R[ byOper2 & 0x3F].N;
//				if( lOper1 < lOper2 )
//					m_pFlags->lt = 1;
//				else if( lOper1 > lOper2 )
//					m_pFlags->gt = 1;
//				else
//					m_pFlags->eq = 1;
//			}
//			break;
//
//		case CNS_CMPF:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				double dfOper1 = m_pRegisters->L[ byOper1 & 0x0F ].D;
//				double dfOper2 = m_pRegisters->L[ byOper2 & 0x0F ].D;
//				if( dfOper1 < dfOper2 )
//					m_pFlags->lt = 1;
//				else if( dfOper1 > dfOper2 )
//					m_pFlags->gt = 1;
//				else
//					m_pFlags->eq = 1;
//			}
//			else
//			{
//				float fOper1 = m_pRegisters->R[ byOper1 & 0x3F].F;
//				float fOper2 = m_pRegisters->R[ byOper2 & 0x3F].F;
//				if( fOper1 < fOper2 )
//					m_pFlags->lt = 1;
//				else if( fOper1 > fOper2 )
//					m_pFlags->gt = 1;
//				else
//					m_pFlags->eq = 1;
//			}
//			break;
//
//		case CNS_CAST:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//
//			switch( byOper1 & 0x1F )
//			{
//				case CNS_i2b :
//					m_pRegisters->r[ byOper2 & 0x3F] = (byte) m_pRegisters->R[ byOper3 & 0x3F].N;
//					break;
//
//				case CNS_i2c :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (char) m_pRegisters->R[ byOper3 & 0x3F].N;
//					break;
//
//				case CNS_i2w :
//					m_pRegisters->r[ byOper2 & 0x3F] = (word) m_pRegisters->R[ byOper3 & 0x3F].N;
//					break;
//
//				case CNS_i2s :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (short) m_pRegisters->R[ byOper3 & 0x3F].N;
//					break;
//
//				case CNS_i2dw:
//					m_pRegisters->r[ byOper2 & 0x3F] = (dword) m_pRegisters->R[ byOper3 & 0x3F].N;
//					break;
//
//				case CNS_i2q :
//					m_pRegisters->l[ byOper2 & 0x3F] = (qword) m_pRegisters->R[ byOper3 & 0x3F].N;
//					break;
//
//				case CNS_i2l :
//					m_pRegisters->L[ byOper2 & 0x0F].LL = (long long) m_pRegisters->R[ byOper3 & 0x3F].N;
//					break;
//
//				case CNS_i2f :
//					m_pRegisters->R[ byOper2 & 0x3F].F = (float) m_pRegisters->r[ byOper3 & 0x3F];
//					break;
//
//				case CNS_i2d :
//					m_pRegisters->L[ byOper2 & 0x0F].D = (double) m_pRegisters->r[ byOper3 & 0x3F];
//					break;
//
//
//				case CNS_dw2b:
//					m_pRegisters->r[ byOper2 & 0x3F] = (byte) m_pRegisters->r[ byOper3 & 0x3F];
//					break;
//
//				case CNS_dw2c:
//					m_pRegisters->R[ byOper2 & 0x3F].N = (char) m_pRegisters->r[ byOper3 & 0x3F];
//					break;
//
//				case CNS_dw2w:
//					m_pRegisters->r[ byOper2 & 0x3F] = (word) m_pRegisters->r[ byOper3 & 0x3F];
//					break;
//
//				case CNS_dw2s:
//					m_pRegisters->R[ byOper2 & 0x3F].N = (short) m_pRegisters->r[ byOper3 & 0x3F];
//					break;
//
//				case CNS_dw2i:
//					m_pRegisters->R[ byOper2 & 0x3F].N = (long) m_pRegisters->r[ byOper3 & 0x3F];
//					break;
//
//				case CNS_dw2q:
//					m_pRegisters->l[ byOper2 & 0x0F] = (qword) m_pRegisters->r[ byOper3 & 0x3F];
//					break;
//
//				case CNS_dw2l:
//					m_pRegisters->L[ byOper2 & 0x0F].LL = (long long) m_pRegisters->r[ byOper3 & 0x3F];
//					break;
//
//				case CNS_dw2f:
//					m_pRegisters->R[ byOper2 & 0x3F].F = (float) m_pRegisters->r[ byOper3 & 0x3F];
//					break;
//
//				case CNS_dw2d:
//					m_pRegisters->L[ byOper2 & 0x0F].D = (double) m_pRegisters->r[ byOper3 & 0x3F];
//					break;
//
//
//				case CNS_l2b :
//					m_pRegisters->r[ byOper2 & 0x3F] = (byte) m_pRegisters->L[ byOper3 & 0x0F].LL;
//					break;
//
//				case CNS_l2c :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (char) m_pRegisters->L[ byOper3 & 0x0F].LL;
//					break;
//
//				case CNS_l2w :
//					m_pRegisters->r[ byOper2 & 0x3F] = (word) m_pRegisters->L[ byOper3 & 0x0F].LL;
//					break;
//
//				case CNS_l2s :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (short) m_pRegisters->L[ byOper3 & 0x0F].LL;
//					break;
//
//				case CNS_l2dw:
//					m_pRegisters->r[ byOper2 & 0x3F] = (dword) m_pRegisters->L[ byOper3 & 0x0F].LL;
//					break;
//
//				case CNS_l2i :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (long) m_pRegisters->L[ byOper3 & 0x0F].LL;
//					break;
//
//				case CNS_l2q :
//					m_pRegisters->l[ byOper2 & 0x0F] = (qword) m_pRegisters->L[ byOper3 & 0x0F].LL;
//					break;
//
//				case CNS_l2f :
//					m_pRegisters->R[ byOper2 & 0x3F].F = (float) m_pRegisters->L[ byOper3 & 0x0F].LL;
//					break;
//
//				case CNS_l2d :
//					m_pRegisters->L[ byOper2 & 0x3F].D = (double) m_pRegisters->L[ byOper3 & 0x0F].LL;
//					break;
//
//
//				case CNS_q2b :
//					m_pRegisters->r[ byOper2 & 0x3F] = (byte) m_pRegisters->l[ byOper3 & 0x0F];
//					break;
//
//				case CNS_q2c :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (char) m_pRegisters->l[ byOper3 & 0x0F];
//					break;
//
//				case CNS_q2w :
//					m_pRegisters->r[ byOper2 & 0x3F] = (word) m_pRegisters->l[ byOper3 & 0x0F];
//					break;
//
//				case CNS_q2s :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (short) m_pRegisters->l[ byOper3 & 0x0F];
//					break;
//
//				case CNS_q2dw:
//					m_pRegisters->r[ byOper2 & 0x3F] = (dword) m_pRegisters->l[ byOper3 & 0x0F];
//					break;
//
//				case CNS_q2i :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (int) m_pRegisters->l[ byOper3 & 0x0F];
//					break;
//
//				case CNS_q2l :
//					m_pRegisters->L[ byOper2 & 0x0F].LL = (long long) m_pRegisters->l[ byOper3 & 0x0F];
//					break;
//
//				case CNS_q2f :
//					m_pRegisters->R[ byOper2 & 0x3F].F = (float) m_pRegisters->l[ byOper3 & 0x0F];
//					break;
//
//				case CNS_q2d :
//					m_pRegisters->L[ byOper2 & 0x0F].D = (double) m_pRegisters->l[ byOper3 & 0x0F];
//					break;
//
//
//				case CNS_f2b :
//					m_pRegisters->r[ byOper2 & 0x3F] = (byte) m_pRegisters->R[ byOper3 & 0x3F].F;
//					break;
//
//				case CNS_f2c :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (char) m_pRegisters->R[ byOper3 & 0x3F].F;
//					break;
//
//				case CNS_f2w :
//					m_pRegisters->r[ byOper2 & 0x3F] = (word) m_pRegisters->R[ byOper3 & 0x3F].F;
//					break;
//
//				case CNS_f2s :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (short) m_pRegisters->R[ byOper3 & 0x3F].F;
//					break;
//
//				case CNS_f2dw:
//					m_pRegisters->r[ byOper2 & 0x3F] = (dword) m_pRegisters->R[ byOper3 & 0x3F].F;
//					break;
//
//				case CNS_f2i :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (long) m_pRegisters->R[ byOper3 & 0x3F].F;
//					break;
//
//				case CNS_f2q :
//					m_pRegisters->l[ byOper2 & 0x0F] = (qword) m_pRegisters->R[ byOper3 & 0x3F].F;
//					break;
//
//				case CNS_f2l :
//					m_pRegisters->L[ byOper2 & 0x0F].LL = (long long) m_pRegisters->R[ byOper3 & 0x3F].F;
//					break;
//
//				case CNS_f2d :
//					m_pRegisters->L[ byOper2 & 0x0F].D = (double) m_pRegisters->R[ byOper3 & 0x3F].F;
//					break;
//
//				case CNS_d2b :
//					m_pRegisters->r[ byOper2 & 0x3F] = (byte) m_pRegisters->L[ byOper3 & 0x0F].D;
//					break;
//
//				case CNS_d2c :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (char) m_pRegisters->L[ byOper3 & 0x0F].D;
//					break;
//
//				case CNS_d2w :
//					m_pRegisters->r[ byOper2 & 0x3F] = (word) m_pRegisters->L[ byOper3 & 0x0F].D;
//					break;
//
//				case CNS_d2s :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (short) m_pRegisters->L[ byOper3 & 0x0F].D;
//					break;
//
//				case CNS_d2dw:
//					m_pRegisters->r[ byOper2 & 0x3F] = (dword) m_pRegisters->L[ byOper3 & 0x0F].D;
//					break;
//
//				case CNS_d2i :
//					m_pRegisters->R[ byOper2 & 0x3F].N = (long) m_pRegisters->L[ byOper3 & 0x0F].D;
//					break;
//
//				case CNS_d2q :
//					m_pRegisters->l[ byOper2 & 0x0F] = (qword) m_pRegisters->L[ byOper3 & 0x0F].D;
//					break;
//
//				case CNS_d2l :
//					m_pRegisters->L[ byOper2 & 0x0F].LL = (long long) m_pRegisters->L[ byOper3 & 0x0F].D;
//					break;
//
//				case CNS_d2f :
//					m_pRegisters->R[ byOper2 & 0x3F].F = (float) m_pRegisters->L[ byOper3 & 0x0F].D;
//					break;
//
//			}
//
//			//if( m_pFlags->dbl )
//			//{
//			//	m_pRegisters->l[ byOper1 & 0x0F ] = m_pRegisters->l[ byOper2 & 0x0F ] + m_pRegisters->l[ byOper3 & 0x0F ];
//			//}
//			//else
//			//{
//			//	m_pRegisters->r[ byOper1 & 0x3F] = m_pRegisters->r[ byOper2 & 0x3F] + m_pRegisters->r[ byOper3 & 0x3F];
//			//}
//
//			break;
//
//		case CNS_BIT_SET:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				qword llAmount = m_pRegisters->l[ byOper3 & 0x0F ];
//
//				qword llMask = 1 << llAmount;
//				qword llTemp = m_pRegisters->l[ byOper2 & 0x0F ] | llMask;
//	
//				m_pRegisters->l[ byOper1 & 0x0F ] = llTemp;
//			}
//			else
//			{
//				dword lAmount = m_pRegisters->r[ byOper3 & 0x3F ];
//
//				dword lMask = 1 << lAmount;
//				dword lTemp = m_pRegisters->r[ byOper2 & 0x3F ] | lMask;
//
//				m_pRegisters->r[ byOper1 & 0x3F ] = lTemp;
//			}
//			break;
//
//		case CNS_BIT_RES:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				qword llAmount = m_pRegisters->l[ byOper3 & 0x0F ];
//
//				qword llMask = 1 << llAmount;
//				qword llTemp = m_pRegisters->l[ byOper2 & 0x0F ] & ~llMask;
//	
//				m_pRegisters->l[ byOper1 & 0x0F ] = llTemp;
//			}
//			else
//			{
//				dword lAmount = m_pRegisters->r[ byOper3 & 0x3F ];
//
//				dword lMask = 1 << lAmount;
//				dword lTemp = m_pRegisters->r[ byOper2 & 0x3F ] & ~lMask;
//
//				m_pRegisters->r[ byOper1 & 0x3F ] = lTemp;
//			}
//			break;
//
//		case CNS_BIT_TGL:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//			byOper3 = *pByteCode;
//			pByteCode++;
//			if( m_pFlags->dbl )
//			{
//				qword llAmount = m_pRegisters->l[ byOper3 & 0x0F ];
//
//				qword llMask = 1 << llAmount;
//				qword llTemp = m_pRegisters->l[ byOper2 & 0x0F ] ^ llMask;
//	
//				m_pRegisters->l[ byOper1 & 0x0F ] = llTemp;
//			}
//			else
//			{
//				dword lAmount = m_pRegisters->r[ byOper3 & 0x3F ];
//
//				dword lMask = 1 << lAmount;
//				dword lTemp = m_pRegisters->r[ byOper2 & 0x3F ] ^ lMask;
//
//				m_pRegisters->r[ byOper1 & 0x3F ] = lTemp;
//			}
//			break;
//
//		case CNS_BIT_TST:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			byOper2 = *pByteCode;
//			pByteCode++;
//
//			if( m_pFlags->dbl )
//			{
//				qword llAmount = m_pRegisters->l[ byOper2 & 0x0F ];
//
//				qword llMask = 1 << llAmount;
//				bool bTemp = (m_pRegisters->l[ byOper1 & 0x0F ] & llMask) > 0;
//	
//				m_pFlags->eq = bTemp;
//			}
//			else
//			{
//				dword lAmount = m_pRegisters->r[ byOper2 & 0x3F ];
//
//				dword lMask = 1 << lAmount;
//				bool bTemp = (m_pRegisters->r[ byOper1 & 0x3F ] & lMask) > 0;
//
//				m_pFlags->eq = bTemp;
//			}
//			break;
//
//		case CNS_JMP:
//			m_pRegisters->PC = *((dword*)(pByteCode+1));
//			return 5;
//
//		case CNS_JRE:
//			m_pRegisters->PC += *((short*)(pByteCode+1));
//			return 3;
//
//		case CNS_CALL:
//			m_pStack->PushDWord( m_pRegisters->PC + 5 );
//			m_pRegisters->PC = *((dword*)(pByteCode+1));
//			return 5;
//
//		case CNS_JMPC:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			
//			if( m_pFlags->cmp & byOper1 )
//			{
//				m_pRegisters->PC = *((dword*)(pByteCode));
//				return 6;
//			}
//			pByteCode+=4;
//
//			break;
//
//		case CNS_JREC:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			
//			if( m_pFlags->cmp & byOper1 )
//			{
//				m_pRegisters->PC += *((short*)(pByteCode+1));
//				return 4;
//			}
//			pByteCode+=2;
//
//			break;
//
//		case CNS_CALLC:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			
//			if( m_pFlags->cmp & byOper1 )
//			{
//				m_pStack->PushDWord( m_pRegisters->PC + 5 );
//				m_pRegisters->PC = *((dword*)(pByteCode+1));
//				return 6;
//			}
//			pByteCode+=4;
//
//			break;
//
//		case CNS_RET:
//			m_pRegisters->PC = m_pStack->PopDWord();
//			return 1;
//
//		case CNS_RETC:
//			pByteCode++;
//			byOper1 = *pByteCode;
//			pByteCode++;
//			
//			if( m_pFlags->cmp & byOper1 )
//			{
//				m_pRegisters->PC = m_pStack->PopDWord();
//				return 2;
//			}
//			break;
//
//
//		case CNS_GET:
//			pByteCode++;
//
//			byOper1 = *pByteCode;
//			pByteCode++;
//
//			if( m_pFlags->dbl )
//			{
//				m_pRegisters->l[ byOper1 & 0x0F ] = ( (qword)( (*m_pData)[ *((dword*)pByteCode) ] ) << 32 ) | (*m_pData)[ *((dword*)pByteCode) + 1 ];
//				pByteCode += sizeof( dword );
//			}
//			else
//			{
//				m_pRegisters->r[ byOper1 & 0x3F ] = (*m_pData)[ *((dword*)pByteCode) ];
//				//32 - PC
//				//33 - SP
//				//34 - FP
//				if( (byOper1 & 0x3F) == 32 )
//					return 0;
//				pByteCode += sizeof( dword );
//			}
//			break;
//
//
//		case CNS_PUT:
//
//		case CNS_XTND:
//			pByteCode++;
//			m_pFlags->dbl = 1;
//			break;
//
//		default:
//			pByteCode++;
//	}
//
//	m_pFlags->dbl = 0;
//	m_pRegisters->PC += (dword)(pByteCode - pMarker);
//
//	str.Clear();
//	m_pRegisters->Dump( str );
//	m_logger.Out << str << "\r\n";
//
//	return 0;
//}


Meson_Cuneus_END
