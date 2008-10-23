//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::VCPU::CuneusInterpreter
//	This is the actual Virtual Machine's byte code interpreter implementation.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	27/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#include "Interpreter.h"

#include <CnsCuneusEngine.h>
#include <CnsVirtualMachine.h>
#include <stdlib.h>

#include "CnsContext.h"

using namespace Meson::Cuneus::VCPU;

//----------------------------------------------------------------------------------------------
// Defines
//----------------------------------------------------------------------------------------------
#define REGS  (*m_pRegisters)
#define FLAGS (*m_pFlags)
#define STACK (*m_pStack)


Meson_Cuneus_BEGIN

Interpreter::Interpreter(void)
{
}

Interpreter::~Interpreter(void)
{
}

void Interpreter::Startup( Meson::Common::Config::Configuration &p_config )
{
	CuneusEngine* pCuneusEngine = CuneusEngine::GetInstance();

	String strName = p_config.GetString( CuneusVirtualMachine::m_szVM_ByteCodeMgr );
	m_executor.SetByteCodeMgr( pCuneusEngine->GetByteCodeMgr() );

	strName = p_config.GetString( CuneusVirtualMachine::m_szVM_GarbageCollector );
	m_executor.SetGarbageCollector( pCuneusEngine->GetGarbageCollector() );
	m_executor.SetConstantsMgr( pCuneusEngine->GetConstantsMgr() );
	m_executor.SetGlobalDataMgr( pCuneusEngine->GetGlobalDataMgr() );
}

void Interpreter::Shutdown( void ) 
{
}


dword Interpreter::Execute( Context *p_pContext )
{
	if( p_pContext == NULL ) 
		return 0;

	dword dwInstructions = 0;
	m_executor.SetContext( p_pContext );

	
	while( m_executor.Execute() != -1 )
	{
		dwInstructions++;
	}

	return dwInstructions;
}

dword Interpreter::ExecuteInstructions( Context *p_pContext, dword p_dwInstructions )
{
	if( ( p_pContext == NULL ) || ( p_dwInstructions == 0 ) )
		return 0;

	dword dwInstructions = 0;
	m_executor.SetContext( p_pContext );

	while( dwInstructions <= p_dwInstructions )
	{
		if( m_executor.Execute() == -1 )
			return -1;

		dwInstructions++;
	}


	return dwInstructions;
}

dword Interpreter::ExecuteTimespan( Context *p_pContext, dword p_dwTimespan )
{
	if( ( p_pContext == NULL ) || ( p_dwTimespan == 0 ) )
		return 0;

	return 1;
}

Meson_Cuneus_END
