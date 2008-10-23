#include "CnsInvoker.h"
#include "CnsAllDataTypes.h"
#include "CnsTypeCodes.h"
#include "CnsTypeClass.h"


using namespace Meson::Common;
using namespace Meson::Cuneus::RTS::Typing;
using namespace Meson::Cuneus::RTS::Reflection;
using namespace Meson::Cuneus::VCPU;

Meson_Cuneus_RTS_BEGIN

Invoker::Invoker( void )
{
}

Invoker::~Invoker( void )
{
}


TPointer< DataType > Invoker::Invoke( Context* p_pContext, Parameters p_params, TypeMethod p_method )
{
	TPointer< DataType > pDataType;

	pDataType = NULL;

	return pDataType;
}

TPointer< DataType > Invoker::InvokeObject( Context* p_pContext, TPointer< DataType > p_pObject, Parameters p_params, TypeMethod p_method )
{
	TPointer< DataType > pDataType;
	
	TypeObject* pObject = (TypeObject*) (DataType*) p_pObject;
	TPointer< TypeObject > ptrObject( pObject );
	
	pDataType = NULL;

	//p_method.GetByteCodeOffset()


	//Push( 10 );
	//Push( 10 );
	//Push( Rn );
	//Push( PC );
	//


	//p_pContext->Stack.Push();

////Push Object
//			STACK.Push( REG[ byOper1] );
//			byLen += 3;
//
//			//Return Address
//			m_pContext->Registers.PC = m_pContext->Registers.PC + byLen;
//			STACK.Push( (TPointer<DataType>&) m_pContext->Registers.PC );

	return pDataType;
}


void Invoker::InvokeObject( Meson::Cuneus::VCPU::Context* p_pContext, byte p_byRegObj, word p_wMethodIndex )
{
	//Push Object
	p_pContext->Stack.Push( p_pContext->Registers.R[ p_byRegObj ] );

	//Return Address
	p_pContext->Registers.PC = p_pContext->Registers.PC + 4;
	p_pContext->Stack.Push( (TPointer<DataType>&) p_pContext->Registers.PC );

	
	TypeObject* pObject = (TypeObject*) (DataType*) p_pContext->Registers.R[ p_byRegObj ];
	TypeMethod* pMethod = pObject->GetClass()->GetMethod( p_wMethodIndex );

	*p_pContext->Registers.PC = pMethod->GetByteCodeOffset();

}


TPointer< DataType > Invoker::InvokeVirtual( Context* p_pContext, Parameters p_params,  TypeMethod p_method )
{
	TPointer< DataType > pDataType;

	pDataType = NULL;

	return pDataType;
}




/*
TPointer< DataType > Invoker::InvokeNative( Context* p_pContext, Parameters& p_params, TypeMethod p_method )
{
	TPointer< DataType > pDataType;
	pDataType = NULL;


	DataType dt;
	dword dwSize = (dword)p_params.Size();
	
	void* pAddress = p_method.GetNativeAddress();
	int nStackSize = 0;

	for( dword dw=0; dw < dwSize; dw++ )
	{
		switch( p_params[dw]->GetTypeCode() )
		{
			case CNS_TC_VOID: break;

			case CNS_TC_BOOLEAN:
				{
					nStackSize+=1;
					bool value = ((TypeBool&)*p_params[dw]).GetValue();
					_asm
					{
						xor  eax, eax
						mov  al, [value]
						push eax
					}
				}
				break;

			case CNS_TC_BYTE:
				{
					nStackSize+=1;
					byte value = ((TypeByte&)*p_params[dw]).GetValue();
					_asm
					{
						xor  eax, eax
						mov  al, [value]
						push eax
					}
				}
				break;

			case CNS_TC_CHAR:
				{
					nStackSize+=1;
					char value = ((TypeChar&)*p_params[dw]).GetValue();
					_asm
					{
						xor  eax, eax
						mov  al, [value]
						push eax
					}
				}
				break;

			case CNS_TC_WORD:
				{
					nStackSize+=1;
					word value = ((TypeWord&)*p_params[dw]).GetValue();
					_asm
					{
						xor  eax, eax
						mov  ax, [value]
						push eax
					}
				}
				break;

			case CNS_TC_SHORT:
				{
					nStackSize+=1;
					short value = ((TypeShort&)*p_params[dw]).GetValue();
					_asm
					{
						xor  eax, eax
						mov  ax, [value]
						push eax
					}
				}
				break;

			case CNS_TC_DWORD:
				{
					nStackSize+=1;
					dword value = ((TypeDWord&)*p_params[dw]).GetValue();
					_asm
					{
						xor  eax, eax
						mov  eax, [value]
						push eax
					}
				}
				break;

			case CNS_TC_INTEGER:
				{
					nStackSize+=1;
					int value = ((TypeInt&)*p_params[dw]).GetValue();
					_asm
					{
						mov  eax, [value]
						push eax
					}
				}
				break;

			case CNS_TC_QWORD:
				{
					nStackSize+=2;
					qword value = ((TypeQWord&)*p_params[dw]).GetValue();
					_asm
					{
						mov  eax, dword ptr [value]
						push eax
						mov  eax, dword ptr [value + 4]
						push eax
					}
				}
				break;

			case CNS_TC_LONG:
				{
					nStackSize+=2;
					long long value = ((TypeLong&)*p_params[dw]).GetValue();
					_asm
					{
						mov  eax, dword ptr [value+4]
						push eax
						mov  eax, dword ptr [value]
						push eax
					}
				}
				break;

			case CNS_TC_FLOAT:
				{
					nStackSize+=1;
					float value = ((TypeFloat&)*p_params[dw]).GetValue();
					// Push float on stack
					_asm
					{
						sub esp, 4
						fld  dword ptr [value]
						fstp dword ptr [esp]
					}
				}
				break;

			case CNS_TC_DOUBLE:
				{
					nStackSize+=2;
					double value = ((TypeDouble&)*p_params[dw]).GetValue();
					// Push double on stack
					_asm
					{
						sub esp, 8
						fld  qword ptr [value]
						fstp qword ptr [esp]
					}
				}
				break;

			case CNS_TC_REFERENCE:
				{
					nStackSize+=1;
					void* value = ((TypeRef&)*p_params[dw]).GetValue();
					_asm
					{
						mov  eax, [value]
						push eax
					}
				}
				break;

		}
	}


	switch( p_method.GetReturnTypeCode() )
	{
		case CNS_TC_VOID:
			pDataType = NULL;
			_asm
			{
				call [pAddress]
			}
			break;

		case CNS_TC_BOOLEAN:
			{
				bool b = false;
			
				_asm
				{
					call [pAddress]
					mov [b], al
				}

				pDataType = new TypeBool( b );
			}
			break;

		case CNS_TC_BYTE:
			{
				byte by = 0;
			
				_asm
				{
					call [pAddress]
					mov [by], al
				}

				pDataType = new TypeByte( by );
			}
			break;

		case CNS_TC_CHAR:
			{
				char c = 0;
			
				_asm
				{
					call [pAddress]
					mov [c], al
				}

				pDataType = new TypeChar( c );
			}
			break;

		case CNS_TC_WORD:
			{
				word w = 0;
			
				_asm
				{
					call [pAddress]
					mov [w], ax
				}

				pDataType = new TypeWord( w );
			}
			break;

		case CNS_TC_SHORT:
			{
				short s = 0;
			
				_asm
				{
					call [pAddress]
					mov [s], ax
				}

				pDataType = new TypeShort( s );
			}
			break;

		case CNS_TC_DWORD:
			{
				dword dw = 0;
			
				_asm
				{
					call [pAddress]
					mov [dw], eax
				}

				pDataType = new TypeDWord( dw );
			}
			break;

		case CNS_TC_INTEGER:
			{
				int n = 0;
			
				_asm
				{
					call [pAddress]
					mov [n], eax
				}

				pDataType = new TypeInt( n );
			}
			break;

		case CNS_TC_QWORD:
			{
				qword qw = 0;
			
				_asm
				{
					call [pAddress]
					mov dword ptr [qw], eax
					mov dword ptr [qw+4], edx
				}

				pDataType = new TypeQWord( qw );
			}
			break;

		case CNS_TC_LONG:
			{
				long long ll = 0;
			
				_asm
				{
					call [pAddress]
					mov dword ptr [ll], eax
					mov dword ptr [ll+4], edx
				}

				pDataType = new TypeLong( ll );
			}
			break;

		case CNS_TC_DOUBLE:
			{
				double df = 0;
			
				_asm
				{
					call [pAddress]
					fstp qword ptr [ df ]
				}

				pDataType = new TypeDouble( df );
			}
			break;

		case CNS_TC_FLOAT:
			{
				float f = 0;
			
				_asm
				{
					call [pAddress]
					fstp dword ptr [ f ]
				}

				pDataType = new TypeFloat( f );
			}
			break;

		case CNS_TC_REFERENCE:
			{
				void* p = 0;
			
				_asm
				{
					call [pAddress]
					mov [p], eax
				}

				pDataType = new TypeRef( p );
			}
			break;
	}

	//for( ; nStackSize; nStackSize-- )
	//{
	//	_asm pop eax;
	//}

	_asm
	{
		mov eax, [nStackSize]
		shl eax, 2
		add esp, eax
	}


	return pDataType;
}

*/



TPointer< DataType > Invoker::InvokeNative( Context* p_pContext, TypeMethod& p_method )
{
	TPointer< DataType > pDataType;
	pDataType = NULL;


	DataType dt;
	dword dwSize = p_method.GetArity();
	
	void* pAddress = p_method.GetNativeAddress();
	int nStackSize = 0;

	for( dword dw=0; dw < dwSize; dw++ )
	{
		pDataType = p_pContext->Stack.Pop();
		switch( pDataType->GetTypeCode() )
		{
			case CNS_TC_VOID: break;

			case CNS_TC_BOOLEAN:
				{
					nStackSize+=1;
					bool value = ((TypeBool&)*pDataType).GetValue();
					_asm
					{
						xor  eax, eax
						mov  al, [value]
						push eax
					}
				}
				break;

			case CNS_TC_BYTE:
				{
					nStackSize+=1;
					byte value = ((TypeByte&)*pDataType).GetValue();
					_asm
					{
						xor  eax, eax
						mov  al, [value]
						push eax
					}
				}
				break;

			case CNS_TC_CHAR:
				{
					nStackSize+=1;
					char value = ((TypeChar&)*pDataType).GetValue();
					_asm
					{
						xor  eax, eax
						mov  al, [value]
						push eax
					}
				}
				break;

			case CNS_TC_WORD:
				{
					nStackSize+=1;
					word value = ((TypeWord&)*pDataType).GetValue();
					_asm
					{
						xor  eax, eax
						mov  ax, [value]
						push eax
					}
				}
				break;

			case CNS_TC_SHORT:
				{
					nStackSize+=1;
					short value = ((TypeShort&)*pDataType).GetValue();
					_asm
					{
						xor  eax, eax
						mov  ax, [value]
						push eax
					}
				}
				break;

			case CNS_TC_DWORD:
				{
					nStackSize+=1;
					dword value = ((TypeDWord&)*pDataType).GetValue();
					_asm
					{
						xor  eax, eax
						mov  eax, [value]
						push eax
					}
				}
				break;

			case CNS_TC_INTEGER:
				{
					nStackSize+=1;
					int value = ((TypeInt&)*pDataType).GetValue();
					_asm
					{
						mov  eax, [value]
						push eax
					}
				}
				break;

			case CNS_TC_QWORD:
				{
					nStackSize+=2;
					qword value = ((TypeQWord&)*pDataType).GetValue();
					_asm
					{
						mov  eax, dword ptr [value]
						push eax
						mov  eax, dword ptr [value + 4]
						push eax
					}
				}
				break;

			case CNS_TC_LONG:
				{
					nStackSize+=2;
					long long value = ((TypeLong&)*pDataType).GetValue();
					_asm
					{
						mov  eax, dword ptr [value+4]
						push eax
						mov  eax, dword ptr [value]
						push eax
					}
				}
				break;

			case CNS_TC_FLOAT:
				{
					nStackSize+=1;
					float value = ((TypeFloat&)*pDataType).GetValue();
					// Push float on stack
					_asm
					{
						sub esp, 4
						fld  dword ptr [value]
						fstp dword ptr [esp]
					}
				}
				break;

			case CNS_TC_DOUBLE:
				{
					nStackSize+=2;
					double value = ((TypeDouble&)*pDataType).GetValue();
					// Push double on stack
					_asm
					{
						sub esp, 8
						fld  qword ptr [value]
						fstp qword ptr [esp]
					}
				}
				break;

			case CNS_TC_REFERENCE:
				{
					nStackSize+=1;
					void* value = ((TypeRef&)*pDataType).GetValue();
					_asm
					{
						mov  eax, [value]
						push eax
					}
				}
				break;

			case CNS_TC_STRING:
				{
					nStackSize+=1;
					void* value = &((TypeString&)*pDataType).GetValue();
					_asm
					{
						mov  eax, [value]
						push eax
					}
				}
				break;

		}
	}


	switch( p_method.GetTypeCode() )
	{
		case CNS_TC_VOID:
			pDataType = NULL;
			_asm
			{
				call [pAddress]
			}
			break;

		case CNS_TC_BOOLEAN:
			{
				bool b = false;
			
				_asm
				{
					call [pAddress]
					mov [b], al
				}

				pDataType = new TypeBool( b );
			}
			break;

		case CNS_TC_BYTE:
			{
				byte by = 0;
			
				_asm
				{
					call [pAddress]
					mov [by], al
				}

				pDataType = new TypeByte( by );
			}
			break;

		case CNS_TC_CHAR:
			{
				char c = 0;
			
				_asm
				{
					call [pAddress]
					mov [c], al
				}

				pDataType = new TypeChar( c );
			}
			break;

		case CNS_TC_WORD:
			{
				word w = 0;
			
				_asm
				{
					call [pAddress]
					mov [w], ax
				}

				pDataType = new TypeWord( w );
			}
			break;

		case CNS_TC_SHORT:
			{
				short s = 0;
			
				_asm
				{
					call [pAddress]
					mov [s], ax
				}

				pDataType = new TypeShort( s );
			}
			break;

		case CNS_TC_DWORD:
			{
				dword dw = 0;
			
				_asm
				{
					call [pAddress]
					mov [dw], eax
				}

				pDataType = new TypeDWord( dw );
			}
			break;

		case CNS_TC_INTEGER:
			{
				int n = 0;
			
				_asm
				{
					call [pAddress]
					mov [n], eax
				}

				pDataType = new TypeInt( n );
			}
			break;

		case CNS_TC_QWORD:
			{
				qword qw = 0;
			
				_asm
				{
					call [pAddress]
					mov dword ptr [qw], eax
					mov dword ptr [qw+4], edx
				}

				pDataType = new TypeQWord( qw );
			}
			break;

		case CNS_TC_LONG:
			{
				long long ll = 0;
			
				_asm
				{
					call [pAddress]
					mov dword ptr [ll], eax
					mov dword ptr [ll+4], edx
				}

				pDataType = new TypeLong( ll );
			}
			break;

		case CNS_TC_DOUBLE:
			{
				double df = 0;
			
				_asm
				{
					call [pAddress]
					fstp qword ptr [ df ]
				}

				pDataType = new TypeDouble( df );
			}
			break;

		case CNS_TC_FLOAT:
			{
				float f = 0;
			
				_asm
				{
					call [pAddress]
					fstp dword ptr [ f ]
				}

				pDataType = new TypeFloat( f );
			}
			break;

		case CNS_TC_REFERENCE:
			{
				void* p = 0;
			
				_asm
				{
					call [pAddress]
					mov [p], eax
				}

				pDataType = new TypeRef( p );
			}
			break;

		case CNS_TC_STRING:
			{
				String* p = 0;
			
				_asm
				{
					call [pAddress]
					mov [p], eax
				}

				pDataType = new TypeString( *p );
			}
			break;

	}

	_asm
	{
		mov eax, [nStackSize]
		shl eax, 2
		add esp, eax
	}


	return pDataType;
}



void Invoker::InvokeNative( Meson::Cuneus::VCPU::Context* p_pContext, word p_wMethodIndex, byte p_byRegObj )
{
	//Return Address
	p_pContext->Registers.PC = p_pContext->Registers.PC + 4;
	p_pContext->Stack.Push( (TPointer<DataType>&) p_pContext->Registers.PC );

	
	TypeObject* pObject = (TypeObject*) (DataType*) p_pContext->Registers.R[ p_byRegObj ];
	TypeMethod* pMethod = pObject->GetClass()->GetMethod( p_wMethodIndex );

	
	p_pContext->Registers.R[ p_byRegObj ] = InvokeNative( p_pContext, *pMethod );
}


Meson_Cuneus_RTS_END
