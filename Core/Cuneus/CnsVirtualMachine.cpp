//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::VirtualMachine
//	This is the Cuneus Virtual Machine main class.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	26/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include <StopWatch.h>
#include <FileInputStream.h>

#include "CnsVirtualMachine.h"
#include "CnsCuneusEngine.h"
#include "CnsDataType.h"
#include "CnsContext.h"
#include "CnsClassLoader.h"

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Config;
using namespace Meson::Common::System;
using namespace Meson::Common::Collections;

using namespace Meson::Cuneus::VCPU;

using namespace Meson::Cuneus::RTS;
using namespace Meson::Cuneus::RTS::Typing;
using namespace Meson::Cuneus::RTS::Reflection;

Meson_Cuneus_BEGIN

const char* CuneusVirtualMachine::m_szVM_ByteCodeMgr = "VM.ByteCodeManager";
const char* CuneusVirtualMachine::m_szVM_GarbageCollector = "VM.GarbageCollector";
const char* CuneusVirtualMachine::m_szVM_Scheduler = "VM.Scheduler";
const char* CuneusVirtualMachine::m_szVM_Interpreter = "VM.Interpreter";

const char* CuneusVirtualMachine::m_szVM_Heap_Min = "VM.Heap.Min";
const char* CuneusVirtualMachine::m_szVM_Heap_Max = "VM.Heap.Max";
const char* CuneusVirtualMachine::m_szVM_Stack_Size = "VM.Stack.Size";

const char* CuneusVirtualMachine::m_szVM_Instruction_Quantum = "VM.Instruction.Quantum";
const char* CuneusVirtualMachine::m_szVM_Time_Quantum = "VM.Time.Quantum";

CuneusVirtualMachine::CuneusVirtualMachine( void )
{
	m_pByteCodeMgr = NULL;
	m_pScheduler = NULL;
	m_pInterpreter = NULL;
	m_pGarbageCollector = NULL;

	SetDefaultConfiguration();
}

CuneusVirtualMachine::~CuneusVirtualMachine( void )
{
}

void CuneusVirtualMachine::Startup( void )
{
	CuneusEngine* pCE = CuneusEngine::GetInstance();

	//m_pByteCodeMgr = pCE->ByteCodeManagers()[ *m_config.Get( m_szVM_ByteCodeMgr )->GetString() ];
	//m_pScheduler = pCE->Schedulers()[ *m_config.Get( m_szVM_Scheduler )->GetString() ];
	//m_pInterpreter = pCE->CuneusInterpreters()[ *m_config.Get( m_szVM_Interpreter )->GetString() ];
	//m_pGarbageCollector = pCE->GarbageCollectors()[ *m_config.Get( m_szVM_GarbageCollector )->GetString() ];

	m_pByteCodeMgr = pCE->GetByteCodeMgr();
	m_pScheduler = pCE->GetScheduler();
	m_pInterpreter = pCE->GetCuneusInterpreter();
	m_pGarbageCollector = pCE->GetGarbageCollector();

	m_pByteCodeMgr->Startup( m_config );
	m_pScheduler->Startup( m_config );
	m_pInterpreter->Startup( m_config );
	m_pGarbageCollector->Startup( m_config );


}

void CuneusVirtualMachine::Shutdown( void )
{
	m_pByteCodeMgr->Shutdown();
	m_pScheduler->Shutdown();
	m_pInterpreter->Shutdown();
	m_pGarbageCollector->Shutdown();
}

void CuneusVirtualMachine::SetDefaultConfiguration( void )
{
	m_config.Set( m_szVM_ByteCodeMgr, "BasicByteCodeManager" );
	m_config.Set( m_szVM_GarbageCollector, "Scavenger" );
	m_config.Set( m_szVM_Scheduler, "WeightedRoundRobin" );
	m_config.Set( m_szVM_Interpreter, "CuneusInterpreter" );

	m_config.SetDWord( m_szVM_Heap_Min,   65536 );
	m_config.SetDWord( m_szVM_Heap_Max,  524288 );
	m_config.SetDWord( m_szVM_Stack_Size, 65536 );

	m_config.SetDWord( m_szVM_Instruction_Quantum, 100 );
}

Configuration* CuneusVirtualMachine::GetConfiguration( void )
{
	return &m_config;
}

void CuneusVirtualMachine::Load( Meson::Common::Text::String p_strScriptName )
{
	FileInputStream iStream( p_strScriptName );
	
	ClassLoader ldr;

	ldr.Load( iStream );

	iStream.Close();
}


TypeMethod* CuneusVirtualMachine::Load( Meson::Common::Text::String p_strScriptName, Meson::Common::Text::String p_strStartMethodName )
{
	FileInputStream iStream( p_strScriptName );
	
	ClassLoader ldr;

	ldr.Load( iStream );

	iStream.Close();

	CuneusEngine* pCE = CuneusEngine::GetInstance();
	return pCE->GetMethod( p_strStartMethodName );
}


void CuneusVirtualMachine::Execute( Meson::Cuneus::RTS::Reflection::TypeMethod* p_pMethod, int p_nPriority )
{
	m_pScheduler->CreateTask( p_pMethod, p_nPriority );
}

dword CuneusVirtualMachine::Run( int p_nCount )
{
	if( p_nCount <= 0 )
		return 0;

	dword dwInstructionCount = 0;
	dword dwInstructionResult = 0;
	
	dword dwQuantum = m_config.GetWord( m_szVM_Instruction_Quantum );

	while( (int)dwInstructionCount < p_nCount )
	{
		Context* pContext = m_pScheduler->NextTask();
		
		dwQuantum = min( dwQuantum, p_nCount - dwInstructionCount );
		dwInstructionResult = m_pInterpreter->ExecuteInstructions( pContext, dwQuantum );
		if( dwInstructionResult != -1 )
			dwInstructionCount += m_pInterpreter->ExecuteInstructions( pContext, dwQuantum );
		else
			return -1;
	}

	return dwInstructionCount;
}

dword CuneusVirtualMachine::Run( double p_dfTimespan )
{
	if( p_dfTimespan <= 0.0 )
		return 0;

	dword dwInstructionCount = 0;
	
	dword dwQuantum = m_config.GetWord( m_szVM_Time_Quantum );

	StopWatch stopwatch;

	stopwatch.Reset();

	while( true )
	{
		Context* pContext = m_pScheduler->NextTask();
		dwInstructionCount += m_pInterpreter->ExecuteTimespan( pContext, dwQuantum );

		//stopwatch.GetElapsedTime();
	}

	return dwInstructionCount;
}


TPointer< DataType > CuneusVirtualMachine::Invoke( TypeMethod* p_pMethod, TList< TPointer< DataType > >& p_lstParam, int p_nStackSize )
{
	Context* pContext = new Context( p_nStackSize );

	// Terminating PC condition
	

	pContext->Registers.PC = TPointer<TypeDWord>( new TypeDWord( p_pMethod->GetByteCodeOffset() ) );
	

	p_lstParam.Reverse();

	TEnumerator< TPointer< DataType > >& eDataType = p_lstParam.GetEnumerator( true );
	
	while( eDataType.HasMoreElements() )
	{
		pContext->Stack.Push( eDataType.NextElement() );
	}
	pContext->Stack.Push( TPointer<DataType>( new TypeDWord( 0xFFFFFFFF ) ) );
	pContext->Registers.FP = TPointer<TypeDWord>( new TypeDWord( pContext->Stack.GetTopPos() ) );
	
	m_pInterpreter->Execute( pContext );

	return pContext->Registers.R[0];
}


Meson_Cuneus_END
