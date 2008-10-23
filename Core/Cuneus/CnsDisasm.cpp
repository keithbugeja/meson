#include "CnsDisasm.h"

#include <Converter.h>

#include "CnsOpcodes.h"
#include "CnsDecoder.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Cuneus::VCPU;
using namespace Meson::Cuneus::VCPU;


Meson_Cuneus_Util_BEGIN

Disasm::Disasm( void )
{
}

Disasm::Disasm( Registers* p_pRegisters, Flags* p_pFlags, DataTypeStack* p_pStack)
{
	m_pRegisters = p_pRegisters;
	m_pFlags = p_pFlags;
//	m_pStack = p_pStack;
}

Disasm::~Disasm(void)
{
}

String Disasm::Register( byte p_byReg )
{
	String str( 10 );

	if( p_byReg < 32  )
	{
			str += 'R';
			str += p_byReg;
	}
	else if( p_byReg == 32  )
	{
		str += "PC";
	}
	else if( p_byReg == 33  )
	{
		str += "SP";
	}
	else if( p_byReg == 34  )
	{
		str += "FP";
	}

	return str;
}




void Disasm::SetByteCodeBase( byte* p_pByteCodeBase )
{
	m_pByteCodeBase = p_pByteCodeBase;
}



dword Disasm::Disassemble( dword p_dwOffset, String& p_str )
{
	byte* pByteCode = m_pByteCodeBase + p_dwOffset;

	byte	byOper1, 
			byOper2,
			byOper3,
			byLen = 0;

	word*	pwIndex;
	dword*	pdwIndex;
	int*    pnRelative;

	p_str.Clear();

	Converter::ToHex( p_dwOffset, p_str );
	p_str += '\t';

	switch( pByteCode[ byLen ] )
	{
		case CNS_NOP:
			p_str += "NOP";
			byLen++;
			break;

		//Load Global Constant
		case CNS_LDGC:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);
			
			//REG[ byOper1] = m_pConstantsMgr->GetConstant( *pdwIndex );

			p_str += "LDGC\t";
			p_str += Register( byOper1 );
			p_str += ", (";
			p_str += (*pdwIndex);
			p_str += ')';

			byLen += 6;

			break;

		//Load Global Constant
		case CNS_LDCC:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);
			
			//REG[ byOper1] = m_pConstantsMgr->GetConstant( *pdwIndex );

			p_str += "LDCC\t";
			p_str += Register( byOper1 );
			p_str += ", (";
			p_str += (*pdwIndex);
			p_str += ')';

			byLen += 6;

			break;

		//Load Global Data
		case CNS_LDGD:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);
			
			//REG[ byOper1] = m_pGlobalDataMgr->GetDataItem( *pdwIndex );
			p_str += "LDGD\t";
			p_str += Register( byOper1 );
			p_str += ", (";
			p_str += (*pdwIndex);
			p_str += ')';

			byLen += 6;

			break;


		// Load Local Param
		case CNS_LDLD:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);
			
			//REG[ byOper1 ] = STACK.Peek( FP->GetValue() + *pdwIndex );
			p_str += "LDLD\t";
			p_str += Register( byOper1 );
			p_str += ", (";
			p_str += (*pdwIndex);
			p_str += ')';

			byLen += 6;

			break;

		// Load Local Param
		case CNS_LDP:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);
			
			//REG[ byOper1 ] = STACK.Peek( FP->GetValue() - *pdwIndex - 2 );
			p_str += "LDP\t";
			p_str += Register( byOper1 );
			p_str += ", (";
			p_str += (*pdwIndex);
			p_str += ')';

			byLen += 6;

			break;

		// Load Object Data
		case CNS_LDOD:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];
			
			//REG[ byOper1] = ( (TypeObject*) &REG[ byOper2 ] )->GetDataItem( byOper3 );
			//REG[ byOper1] = ( (TypeObject&) *REG[ byOper2 ] ).GetDataItem( byOper3 );
			p_str += "LDOD\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );
			
			byLen += 4;
			break;



		//Store Global Data
		case CNS_STGD:
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			byOper1 = pByteCode[ byLen + 5 ];
			//m_pGlobalDataMgr->SetDataItem( *pdwIndex, REG[ byOper1] );
			p_str += "STGD\t(";
			p_str += (*pdwIndex);
			p_str += "), ";
			p_str += Register( byOper1 );

			byLen += 6;
			break;

		// Load Local Param
		case CNS_STLD:
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			byOper1 = pByteCode[ byLen + 5 ];
			
			//STACK.Poke( ( FP->GetValue() + *pdwIndex ), REG[ byOper1 ] );
			p_str += "STLD\t(";
			p_str += (*pdwIndex);
			p_str += "), ";
			p_str += Register( byOper1 );

			byLen += 6;
			break;

		// Load Local Param
		case CNS_STP:
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			byOper1 = pByteCode[ byLen + 5 ];
			
			//*STACK.Peek( FP->GetValue() - *pdwIndex - 1 ) = *REG[ byOper1 ];
			//STACK.Poke( FP->GetValue() - *pdwIndex - 2, REG[ byOper1 ] );

			p_str += "STP\t(";
			p_str += (*pdwIndex);
			p_str += "), ";
			p_str += Register( byOper1 );

			byLen += 6;
			break;


		
		// Store Object Data
		case CNS_STOD:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];
			
			//( (TypeObject*) &REG[ byOper1 ] )->SetDataItem( byOper2, REG[ byOper3] );
			p_str += "STOD\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;

			break;



		case CNS_COPY:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//*REG[ byOper1] = *REG[ byOper2 ];

			p_str += "COPY\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );

			byLen += 3;
			break;


		case CNS_NEG:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//REG[ byOper1] = -(*REG[ byOper2 ]);
			
			p_str += "NEG\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			
			byLen += 3;

			break;

		case CNS_ADD:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			//REG[ byOper1] = *REG[ byOper2 ] + REG[ byOper3 ];
			p_str += "ADD\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;

		case CNS_SUB:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			//REG[ byOper1] = *REG[ byOper2 ] - REG[ byOper3 ];
			p_str += "SUB\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;

		case CNS_MUL:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			//REG[ byOper1] = *REG[ byOper2 ] * REG[ byOper3 ];
			p_str += "MUL\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );
			byLen += 4;
			break;

		case CNS_DIV:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			//REG[ byOper1] = *REG[ byOper2 ] / REG[ byOper3 ];
			p_str += "DIV\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;

		case CNS_MOD:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			//REG[ byOper1] = *REG[ byOper2 ] % REG[ byOper3 ];
			p_str += "MOD\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;

		case CNS_INC:
			byOper1 = pByteCode[ byLen + 1 ];
			//++*REG[ byOper1];

			p_str += "INC\t";
			p_str += Register( byOper1 );

			byLen += 2;
			break;

		case CNS_DEC:
			byOper1 = pByteCode[ byLen + 1 ];
			//--*REG[ byOper1];

			p_str += "DEC\t";
			p_str += Register( byOper1 );

			byLen += 2;
			break;

		case CNS_NOT:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//REG[ byOper1] = ~*REG[ byOper2 ];
			
			p_str += "NOT\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );

			byLen += 3;
			break;

		case CNS_AND:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			//REG[ byOper1] = *REG[ byOper2 ] & REG[ byOper3 ];
			p_str += "AND\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;

		case CNS_OR:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			//REG[ byOper1] = *REG[ byOper2 ] | REG[ byOper3 ];
			p_str += "OR\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;

		case CNS_XOR:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			//REG[ byOper1] = *REG[ byOper2 ] ^ REG[ byOper3 ];
			p_str += "XOR\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;

		case CNS_SHL:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			//REG[ byOper1] = *REG[ byOper2 ] << REG[ byOper3 ];
			p_str += "SHL\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;

		case CNS_SHR:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

//TODO: Sign Extension
			//REG[ byOper1] = *REG[ byOper2 ] >> REG[ byOper3 ];
			p_str += "SHR\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;

		case CNS_USHR:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];

			//REG[ byOper1] = *REG[ byOper2 ] >> REG[ byOper3 ];
			p_str += "USHR\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;


		case CNS_XCHG:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			/*{
				TPointer< DataType > tmp;
				tmp = REG[ byOper1 ];
				REG[ byOper1 ] = REG[ byOper2 ];
				REG[ byOper2 ] = tmp;
			}*/

			p_str += "XCHG\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			
			byLen += 3;
			break;

		case CNS_PUSH:
			byOper1 = pByteCode[ byLen + 1 ];
			//STACK.Push( REG[ byOper1 ] );
			p_str += "PUSH\t";
			p_str += Register( byOper1 );
		
			byLen += 2;
			break;

		case CNS_PUSHALL:
			//for(int n=0; n<0x20; n++)
			//{
			//	STACK.Push( REG[ n ] );
			//}
			p_str += "PUSHALL";
			
			byLen++;
			break;

		case CNS_POP:
			byOper1 = pByteCode[ byLen + 1 ];
			//REG[ byOper1 ] = STACK.Pop();
			p_str += "POP\t";
			p_str += Register( byOper1 );

			byLen += 2;
			break;

		case CNS_POPALL:
		/*	for(int n=0; n<0x20; n++)
			{
				REG[ n ] = STACK.Pop();
			}*/

			p_str += "POPALL";

			byLen++;
			break;

		case CNS_PEEK:
			byOper1 = pByteCode[ byLen + 1 ];
			//REG[ byOper1 ] = STACK.Peek();
			p_str += "PEEK\t";
			p_str += Register( byOper1 );

			byLen += 2;
			break;

		case CNS_CMP:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			/*{
				int n = REG[ byOper1]->Compare( REG[ byOper2 ] );
				FLAGS.eq = ( n == 0 );
				FLAGS.lt = ( n == -1 );
				FLAGS.gt = ( n == 1 );
			}*/
			p_str += "CMP\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );

			byLen += 3;
			break;


		case CNS_EQ:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) == 0;
			p_str += "EQ\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			
			byLen += 3;
			break;

		case CNS_NE:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) != 0;
			p_str += "NE\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );

			byLen += 3;
			break;

		case CNS_LT:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) < 0;
			p_str += "LT\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );

			byLen += 3;
			break;

		case CNS_GT:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) > 0;
			p_str += "GT\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );

			byLen += 3;
			break;

		case CNS_LE:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) <= 0;
			p_str += "LE\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );

			byLen += 3;
			break;

		case CNS_GE:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];

			//FLAGS.tru = (*REG[ byOper1]).Compare(REG[ byOper2 ]) >= 0;
			p_str += "GE\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );

			byLen += 3;
			break;

		case CNS_HALT:
			p_str += "HALT";
			byLen++;
			break;

		case CNS_RET:
			//PC = (TPointer<TypeDWord>&) STACK.Pop();
			//byOper1 = pByteCode[ byLen + 1 ];

			p_str += "RET";
			//p_str += Register( byOper1 );

			byLen++;
			break;

		case CNS_RETN:
			//byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			
			/*STACK.PopN( *pdwIndex );
			PC = (TPointer<TypeDWord>&) STACK.Pop();*/

			p_str += "RETN\t";
			//p_str += Register( byOper1 );
			//p_str += ", ";
			p_str += (*pdwIndex);
	
			byLen += 5;
			break;

		case CNS_JRT:
			pnRelative = (int*) (pByteCode + byLen + 1);
			p_str += "JRT\t";
			p_str += (*pnRelative);
			p_str += " [";
			Converter::ToHex( (dword)(p_dwOffset + (*pnRelative)), p_str );
			p_str += "]";

			byLen += 5;
			break;

		case CNS_JRF:
			pnRelative = (int*) (pByteCode + byLen + 1);
			p_str += "JRF\t";
			p_str += (*pnRelative);
			p_str += " [";
			Converter::ToHex( (dword)(p_dwOffset + (*pnRelative)), p_str );
			p_str += "]";

			byLen += 5;
			break;

		case CNS_JR:
			pnRelative = (int*) (pByteCode + byLen + 1);
			p_str += "JR\t";
			p_str += (*pnRelative);
			p_str += " [";
			Converter::ToHex( (dword)(p_dwOffset + (*pnRelative)), p_str );
			p_str += "]";

			byLen += 5;
			break;

		case CNS_CALL:
			pnRelative = (int*) (pByteCode + byLen + 1);
			
			p_str += "CALL\t";
			p_str += (*pnRelative);
			p_str += " [";
			Converter::ToHex( (dword)(p_dwOffset + (*pnRelative)), p_str );
			p_str += "]";
			
			byLen += 5;
			break;

		case CNS_POPN:
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			
			p_str += "POPN\t";
			p_str += (*pdwIndex);

			byLen+=5;
			break;

		case CNS_CAST:
			p_str += "CAST(undefined)";
			byLen++;
			break;

		case CNS_CALLOBJECT:
			byOper1 = pByteCode[ byLen + 1 ];
			pwIndex = (word*) (pByteCode + byLen + 2);

			/*m_invoker.InvokeObject( m_pContext, byOper1, *pwIndex );*/
			p_str += "CALLOBJECT\t";
			p_str += Register( byOper1 );
			p_str += ", (";
			p_str += (*pwIndex);
			p_str += ')';
			byLen += 4;
			break;

		case CNS_CALLNATIVE:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);

			p_str += "CALLNATIVE\t";
			p_str += Register( byOper1 );
			p_str += ", (";
			p_str += (*pdwIndex);
			p_str += ')';

			byLen += 6;
			break;

		case CNS_CALLVIRTUAL:
			p_str += "CALLVIRTUAL\t";

			byLen += 1;
			break;

		case CNS_LVARS:
			pdwIndex = (dword*) (pByteCode + byLen + 1);
			p_str += "LVARS\t";
			p_str += (*pdwIndex);
			byLen += 5;
			break;

		case CNS_STT:
			p_str += "STT";
			byLen++;
			break;

		case CNS_CLT:
			p_str += "CLT";
			byLen++;
			break;

		case CNS_NEWARRAY:
			byOper1 = pByteCode[ byLen + 1 ];
			pdwIndex = (dword*) (pByteCode + byLen + 2);
			byOper2 = pByteCode[ byLen + 6 ];

			p_str += "NEWARRAY\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += (*pdwIndex);
			p_str += ", ";
			p_str += Register( byOper2 );

			byLen += 7;
			break;


		case CNS_GETELM:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];
			
			//REG[ byOper1 ] = ( (TypeArray*) (DataType*) REG[ byOper2 ] )->GetElement( *( (TypeDWord*) (DataType*) REG[ byOper3 ] ) );
			p_str += "GETELM\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;

		case CNS_SETELM:
			byOper1 = pByteCode[ byLen + 1 ];
			byOper2 = pByteCode[ byLen + 2 ];
			byOper3 = pByteCode[ byLen + 3 ];
			
			//( (TypeArray*) (DataType*) REG[ byOper1 ] )->SetElement( *( (TypeDWord*) (DataType*) REG[ byOper2 ] ), REG[ byOper3 ] );
			p_str += "SETELM\t";
			p_str += Register( byOper1 );
			p_str += ", ";
			p_str += Register( byOper2 );
			p_str += ", ";
			p_str += Register( byOper3 );

			byLen += 4;
			break;


		default:
			p_str += "UNDEFINED\t";
			
			byLen++;
	}

	return p_dwOffset + byLen;
}




//String Disasm::Disassemble( byte* p_pbyCode )
//{
//	return String();
//}



//String Disasm::Disassemble( byte* p_pbyCode )
//{
//	String str(100);
//	str.Clear();
//
//	switch( *p_pbyCode )
//	{
//	case CNS_NOP:
//		str += "NOP ";
//		break;
//
//	case CNS_SET:
//		str += "SET ";
//		str += Register( *(p_pbyCode+1) );
//		str += ", ";
//		str += ( m_pFlags->dbl ) ? *((qword*)p_pbyCode) : *((dword*)p_pbyCode) ;
//		break;
//
//	case CNS_COPY:
//		str += "COPY ";
//		str += Register( *(p_pbyCode+1) );
//		str += ", ";
//		str += Register( *(p_pbyCode+2) );
//		break;
//	}
//
//	return str;
//}



//String Disasm::Disassemble( byte* p_pbyCode )
//{
//	String str(100);
//
//	str += m_arrMnemonic[ (*p_pbyCode) & 0x7F ];
//
//	switch( *p_pbyCode )
//	{
//	case CNS_NOP:
//		break;
//
//	case CNS_SET:
//		str += Register( *(p_pbyCode+1) );
//		str += ", 0x";
//		//str += ( m_pFlags->dbl ) ? *((qword*)p_pbyCode) : *((dword*)p_pbyCode) ;
//		if( m_pFlags->dbl )
//		{
//			Converter::ToHex( *((qword*)(p_pbyCode+2)), str );
//		}
//		else
//		{
//			Converter::ToHex( *((dword*)(p_pbyCode+2)), str );
//		}
//
//		break;
//
//	case CNS_COPY:
//		Arity( 2, (p_pbyCode+1), str );
//
//		/*str += Register( *(p_pbyCode+1) );
//		str += ", ";
//		str += Register( *(p_pbyCode+2) );*/
//		break;
//
//	case CNS_NEG:
//		Arity( 2, (p_pbyCode+1), str );
//		break;
//
//	case CNS_NEGF:
//		Arity( 2, (p_pbyCode+1), str );
//		break;
//
//	case CNS_ADD:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_ADDF:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_SUB:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_SUBF:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_MUL:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_MULF:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_DIV:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_DIVF:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_MOD:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_MODF:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_INC:
//		Arity( 1, (p_pbyCode+1), str );
//		break;
//
//	case CNS_INCF:
//		Arity( 1, (p_pbyCode+1), str );
//		break;
//
//	case CNS_DEC:
//		Arity( 1, (p_pbyCode+1), str );
//		break;
//
//	case CNS_DECF:
//		Arity( 1, (p_pbyCode+1), str );
//		break;
//
//	case CNS_NOT:
//		Arity( 2, (p_pbyCode+1), str );
//		break;
//
//	case CNS_AND:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_OR:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_XOR:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_SHL:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_SHR:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_SAR:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_ROL:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_ROR:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_PUSHV:
//		str += "0x";
//		if( m_pFlags->dbl )
//		{
//			Converter::ToHex( *((qword*)(p_pbyCode+2)), str );
//		}
//		else
//		{
//			Converter::ToHex( *((dword*)(p_pbyCode+2)), str );
//		}
//		break;
//
//	case CNS_PUSH:
//		Arity( 1, (p_pbyCode+1), str );
//		break;
//
//	case CNS_PUSHALL:
//		break;
//
//	case CNS_POP:
//		Arity( 1, (p_pbyCode+1), str );
//		break;
//
//	case CNS_POPALL:
//		break;
//
//	case CNS_PEEK:
//		Arity( 1, (p_pbyCode+1), str );
//		break;
//
//	case CNS_DUP:
//		break;
//
//	case CNS_SWAP:
//		Arity( 2, (p_pbyCode+1), str );
//		break;
//
//	case CNS_XCHG:
//		Arity( 2, (p_pbyCode+1), str );
//		break;
//
//	case CNS_HALT:
//		break;
//
//	case CNS_CMP:
//		Arity( 2, (p_pbyCode+1), str );
//		break;
//
//	case CNS_CMPF:
//		Arity( 2, (p_pbyCode+1), str );
//		break;
//
//	case CNS_CAST:
//		switch( (*(p_pbyCode+1)) & 0x1F )
//		{
//			case CNS_i2b :
//				str += ".i2b \t";
//				break;
//
//			case CNS_i2c :
//				str += ".i2c \t";
//				break;
//
//			case CNS_i2w :
//				str += ".i2w \t";
//				break;
//
//			case CNS_i2s :
//				str += ".i2s \t";
//				break;
//
//			case CNS_i2dw:
//				str += ".i2dw\t";
//				break;
//
//			case CNS_i2q :
//				str += ".i2q \t";
//				break;
//
//			case CNS_i2l :
//				str += ".i2l \t";
//				break;
//
//			case CNS_i2f :
//				str += ".i2f \t";
//				break;
//
//			case CNS_i2d :
//				str += ".i2d \t";
//				break;
//
//
//			case CNS_dw2b:
//				str += ".dw2b\t";
//				break;
//
//			case CNS_dw2c:
//				str += ".dw2c\t";
//				break;
//
//			case CNS_dw2w:
//				str += ".dw2w\t";
//				break;
//
//			case CNS_dw2s:
//				str += ".dw2s\t";
//				break;
//
//			case CNS_dw2i:
//				str += ".dw2i\t";
//				break;
//
//			case CNS_dw2q:
//				str += ".dw2q\t";
//				break;
//
//			case CNS_dw2l:
//				str += ".dw2l\t";
//				break;
//
//			case CNS_dw2f:
//				str += ".dw2f\t";
//				break;
//
//			case CNS_dw2d:
//				str += ".dw2d\t";
//				break;
//
//
//			case CNS_l2b :
//				str += ".l2b \t";
//				break;
//
//			case CNS_l2c :
//				str += ".l2c \t";
//				break;
//
//			case CNS_l2w :
//				str += ".l2w \t";
//				break;
//
//			case CNS_l2s :
//				str += ".l2s \t";
//				break;
//
//			case CNS_l2dw:
//				str += ".l2dw\t";
//				break;
//
//			case CNS_l2i :
//				str += ".l2i \t";
//				break;
//
//			case CNS_l2q :
//				str += ".l2q \t";
//				break;
//
//			case CNS_l2f :
//				str += ".l2f \t";
//				break;
//
//			case CNS_l2d :
//				str += ".l2d \t";
//				break;
//
//
//			case CNS_q2b :
//				str += ".q2b \t";
//				break;
//
//			case CNS_q2c :
//				str += ".q2c \t";
//				break;
//
//			case CNS_q2w :
//				str += ".q2w \t";
//				break;
//
//			case CNS_q2s :
//				str += ".q2s \t";
//				break;
//
//			case CNS_q2dw:
//				str += ".q2dw\t";
//				break;
//
//			case CNS_q2i :
//				str += ".q2i \t";
//				break;
//
//			case CNS_q2l :
//				str += ".q2l \t";
//				break;
//
//			case CNS_q2f :
//				str += ".q2f \t";
//				break;
//
//			case CNS_q2d :
//				str += ".q2d \t";
//				break;
//
//
//			case CNS_f2b :
//				str += ".f2b \t";
//				break;
//
//			case CNS_f2c :
//				str += ".f2c \t";
//				break;
//
//			case CNS_f2w :
//				str += ".f2w \t";
//				break;
//
//			case CNS_f2s :
//				str += ".f2s \t";
//				break;
//
//			case CNS_f2dw:
//				str += ".f2dw\t";
//				break;
//
//			case CNS_f2i :
//				str += ".f2i \t";
//				break;
//
//			case CNS_f2q :
//				str += ".f2q \t";
//				break;
//
//			case CNS_f2l :
//				str += ".f2l \t";
//				break;
//
//			case CNS_f2d :
//				str += ".f2d \t";
//				break;
//
//
//			case CNS_d2b :
//				str += ".d2b \t";
//				break;
//
//			case CNS_d2c :
//				str += ".d2c \t";
//				break;
//
//			case CNS_d2w :
//				str += ".d2w \t";
//				break;
//
//			case CNS_d2s :
//				str += ".d2s \t";
//				break;
//
//			case CNS_d2dw:
//				str += ".d2dw\t";
//				break;
//
//			case CNS_d2i :
//				str += ".d2i \t";
//				break;
//
//			case CNS_d2q :
//				str += ".d2q \t";
//				break;
//
//			case CNS_d2l :
//				str += ".d2l \t";
//				break;
//
//			case CNS_d2f :
//				str += ".d2f \t";
//				break;
//
//		}
//
//		Arity( 2, (p_pbyCode+2), str );
//		break;
//
//	case CNS_BIT_SET:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_BIT_RES:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_BIT_TGL:
//		Arity( 3, (p_pbyCode+1), str );
//		break;
//
//	case CNS_BIT_TST:
//		Arity( 2, (p_pbyCode+1), str );
//		break;
//
////TODO: Check ------------------------------------------------------------
//
//	case CNS_JMP:
//		Converter::ToHex( *((dword*)(p_pbyCode+1)), str );
//		break;
//
//	case CNS_JRE:
//		Converter::ToHex( *((word*)(p_pbyCode+1)), str );
//		break;
//
//	case CNS_CALL: 
//		Converter::ToHex( *((dword*)(p_pbyCode+1)), str );
//		break;
//
//	case CNS_JMPC:
//		Compare( p_pbyCode+1, str );
//		Converter::ToHex( *((dword*)(p_pbyCode+2)), str );
//		break;
//	
//	case CNS_JREC:
//		Compare( p_pbyCode+1, str );
//		Converter::ToHex( *((word*)(p_pbyCode+2)), str );
//		break;
//	
//	case CNS_CALLC:
//		Compare( p_pbyCode+1, str );
//		Converter::ToHex( *((dword*)(p_pbyCode+2)), str );
//		break;
//
//	case CNS_RET:
//		break;
//
//	case CNS_RETC:
//		Compare( p_pbyCode+1, str );
//		break;
//
////TODO: Check ------------------------------------------------------------
//
//	}
//
//	return str;
//}



Meson_Cuneus_Util_END
