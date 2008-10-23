//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::VCPU::CuneusInterpreter
//	This is the actual Virtual Machine's byte code interpreter implementation.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	27/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include <CnsNamespaces.h>
#include <CnsCuneusinterpreter.h>

#include "Executor.h"

Meson_Cuneus_BEGIN


//union Instruction
//{
//	byte  op[4];
//	dword Bytecode;
//};



class Interpreter: public ICuneusInterpreter
{
private:
	Executor m_executor;

public:
	Interpreter( void );
	~Interpreter( void );

	void Startup( Meson::Common::Config::Configuration &p_config );
	void Shutdown( void );

	dword Execute( Meson::Cuneus::VCPU::Context *p_pContext );
	dword ExecuteInstructions( Meson::Cuneus::VCPU::Context *p_pContext, dword p_dwInstructions );
	dword ExecuteTimespan( Meson::Cuneus::VCPU::Context *p_pContext, dword p_dwTimespan );
};

Meson_Cuneus_END
