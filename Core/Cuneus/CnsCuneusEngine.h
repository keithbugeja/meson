#pragma once

#include <Namespaces.h>
#include <Engine.h>
#include <ArrayList.h>
#include <PlugInTypeManager.h>

#include "CnsNamespaces.h"
#include "CnsByteCodeMgr.h"
#include "CnsScheduler.h"
#include "CnsCuneusInterpreter.h"
#include "CnsGarbageCollector.h"

#include "CnsDataTypeMgr.h"
#include "CnsVirtualMachine.h"
#include "CnsGlobalDataMgr.h"

#include "CnsNativeMethodMgr.h"
#include "CnsSymbolMgr.h"
#include "CnsConstantsMgr.h"

Meson_Cuneus_BEGIN

//----------------------------------------------------------------------------------------------
//	Cuneus Engine
//----------------------------------------------------------------------------------------------
class CUNEUS_API CuneusEngine : public Meson::Common::TEngine<CuneusEngine>
{
	friend class Meson::Common::TEngine<CuneusEngine>;

private:
	Meson::Common::Text::String  m_strName;
	Meson::Common::Version m_version;
	Meson::Common::Collections::TArrayList< Meson::Cuneus::CuneusVirtualMachine* > m_lstVMs;

	GlobalDataMgr m_globalDataMgr;
	NativeMethodMgr m_methodMgr;
	SymbolMgr m_symbolMgr;

	ConstantsMgr m_constantsMgr;

//public:
	Meson::Cuneus::IByteCodeMgr* m_pByteCodeManager;
	Meson::Cuneus::IGarbageCollector*  m_pGarbageCollector;
	Meson::Cuneus::IScheduler* m_pScheduler;
	Meson::Cuneus::ICuneusInterpreter* m_pCuneusInterpreter;


	//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::IByteCodeMgr > m_ByteCodeManager;
	//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::IGarbageCollector > m_GarbageCollectorsManager;
	//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::IScheduler > m_SchedulersManager;
	//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::ICuneusInterpreter > m_CuneusInterpretersManager;

	Meson::Cuneus::RTS::Typing::DataTypeMgr *m_pDataTypeMgr;
	//Meson::Cuneus::RTS::Reflection::Builder *m_pBuilder;

protected:
	CuneusEngine(void);
	~CuneusEngine(void);

	

public:
	const Meson::Common::Text::String& GetName(void);
	const Meson::Common::Version& GetVersion(void);



	//----------------------------------------------------------------------------------------------
	//	Cuneus Engine Managers
	//----------------------------------------------------------------------------------------------
	//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::IByteCodeMgr >& ByteCodeManagers( void );	
	//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::IGarbageCollector >& GarbageCollectors();
	//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::IScheduler >& Schedulers( void );
	//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::ICuneusInterpreter >& CuneusInterpreters( void );

//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::IByteCodeMgr > m_pByteCodeManager;
	Meson::Cuneus::IByteCodeMgr* GetByteCodeMgr( void ) { return m_pByteCodeManager; }
	void SetByteCodeMgr( Meson::Cuneus::IByteCodeMgr* p_pByteCodeManager ) { m_pByteCodeManager = p_pByteCodeManager; }

	Meson::Cuneus::IGarbageCollector* GetGarbageCollector( void ) { return m_pGarbageCollector; }
	void SetGarbageCollector( Meson::Cuneus::IGarbageCollector* p_pGarbageCollector ) { m_pGarbageCollector = p_pGarbageCollector; }

	Meson::Cuneus::IScheduler* GetScheduler( void ) { return m_pScheduler; }
	void SetScheduler( Meson::Cuneus::IScheduler* p_pScheduler ) { m_pScheduler = p_pScheduler; }

	Meson::Cuneus::ICuneusInterpreter* GetCuneusInterpreter( void ) { return m_pCuneusInterpreter; }
	void SetCuneusInterpreter( Meson::Cuneus::ICuneusInterpreter* p_pCuneusInterpreter ) { m_pCuneusInterpreter = p_pCuneusInterpreter; }


	Meson::Cuneus::RTS::Typing::DataTypeMgr* GetDataTypeMgr( void );
	Meson::Cuneus::ConstantsMgr* GetConstantsMgr( void );
	Meson::Cuneus::GlobalDataMgr* GetGlobalDataMgr( void );
	Meson::Cuneus::SymbolMgr* GetSymbolMgr( void );

	//Meson::Cuneus::RTS::Reflection::Builder *GetReflectionBuilder( void );
	//Meson::Cuneus::RTS::Reflection::Namespace *GetGlobalNamespace( void );

	//----------------------------------------------------------------------------------------------
	//	Cuneus Engine Virtual Machine Creation Destruction methods
	//----------------------------------------------------------------------------------------------
	Meson::Cuneus::CuneusVirtualMachine *CreateVM( void );
	void DestroyVM( Meson::Cuneus::CuneusVirtualMachine *p_pVM );

	Meson::Cuneus::CuneusVirtualMachine *GetVM( int p_nIndex = 0 );


	//dword RegisterNativeMethod( Meson::Common::Text::String& p_strMethodName, void* p_pMethod, dword p_dwReturnTypeCode );
	//Meson::Common::TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > GetNativeMethod( dword p_dwIndex );
	//Meson::Common::TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > GetNativeMethod( Meson::Common::Text::String& p_strMethodName );


	dword RegisterMethod( Meson::Common::Text::String& p_strMethodName, Meson::Cuneus::RTS::Reflection::TypeMethod* p_pMethod );
	void BindNativeMethod( Meson::Common::Text::String& p_strMethodName, void* p_pNativeAddress );
	void BindNativeMethod( dword p_dwMethodIndex, void* p_pNativeAddress );
	Meson::Common::TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > GetMethod( dword p_dwIndex );
	Meson::Common::TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > GetMethod( Meson::Common::Text::String& p_strMethodName );


	dword AddConstant( Meson::Cuneus::RTS::Typing::DataType* p_pDataType );
	Meson::Common::TPointer< Meson::Cuneus::RTS::Typing::DataType > GetConstant( dword p_dwIndex );
	void SetConstant( dword p_dwIndex, Meson::Cuneus::RTS::Typing::DataType* p_pDataType );
};

Meson_Cuneus_END
