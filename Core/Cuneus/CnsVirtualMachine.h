//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::VirtualMachine
//	This is the Cuneus Virtual Machine main class.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	26/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#pragma once

#include <Types.h>
#include <Configuration.h>
#include <String8.h>

#include "CnsNamespaces.h"
#include "CnsByteCodeMgr.h"
#include "CnsScheduler.h"
#include "CnsCuneusInterpreter.h"
#include "CnsGarbageCollector.h"




Meson_Cuneus_BEGIN

class CuneusVirtualMachine
{
public:
	static const char* m_szVM_ByteCodeMgr;
	static const char* m_szVM_GarbageCollector;
	static const char* m_szVM_Scheduler;
	static const char* m_szVM_Interpreter;

	static const char* m_szVM_Heap_Min;
	static const char* m_szVM_Heap_Max;
	static const char* m_szVM_Stack_Size;
	static const char* m_szVM_Instruction_Quantum;
	static const char* m_szVM_Time_Quantum;

protected:
	Meson::Common::Config::Configuration m_config;

	Meson::Cuneus::IByteCodeMgr* m_pByteCodeMgr;
	Meson::Cuneus::IScheduler* m_pScheduler;
	Meson::Cuneus::ICuneusInterpreter* m_pInterpreter;
	Meson::Cuneus::IGarbageCollector* m_pGarbageCollector;

	

public:
	CuneusVirtualMachine( void );
	~CuneusVirtualMachine( void );

	void Startup( void );
	void Shutdown( void );

	void SetDefaultConfiguration( void );
	Meson::Common::Config::Configuration *GetConfiguration( void );

	void Load( Meson::Common::Text::String p_strScriptName );
	Meson::Cuneus::RTS::Reflection::TypeMethod* Load( Meson::Common::Text::String p_strScriptName, Meson::Common::Text::String p_strStartMethodName );

	void Execute( Meson::Cuneus::RTS::Reflection::TypeMethod* p_pMethod, int p_nPriority = 1 );

	dword Run( int p_nCount );
	dword Run( double p_dfTimespan );


	Meson::Common::TPointer<Meson::Cuneus::RTS::Typing::DataType> Invoke( Meson::Cuneus::RTS::Reflection::TypeMethod* p_pMethod, 
		Meson::Common::Collections::TList< Meson::Common::TPointer<Meson::Cuneus::RTS::Typing::DataType> >& p_lstParam,
		int p_nStackSize = 1024 );

	//Invoke( const char *p_szMethod );

	//void GarbageCollect( void );
};

Meson_Cuneus_END
