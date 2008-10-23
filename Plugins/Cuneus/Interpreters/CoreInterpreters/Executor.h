#pragma once

#include <CnsTypes.h>
#include <CnsNamespaces.h>
#include <CnsContext.h>
#include <CnsStack.h>
#include <CnsByteCodeMgr.h>
#include <CnsGarbageCollector.h>
#include <CnsConstantsMgr.h>
#include <CnsGlobalDataMgr.h>
#include <CnsInvoker.h>
#include <ArrayList.h>

#include <Logger.h>

Meson_Cuneus_BEGIN

class Executor
{
protected:
	Meson::Cuneus::VCPU::Context* m_pContext;

	Meson::Cuneus::IByteCodeMgr* m_pByteCodeMgr;
	Meson::Cuneus::IGarbageCollector* m_pGarbageCollector;

	Meson::Cuneus::ConstantsMgr*  m_pConstantsMgr;
	Meson::Cuneus::GlobalDataMgr* m_pGlobalDataMgr;

	Meson::Cuneus::RTS::Invoker m_invoker;

	Meson::Common::Logging::Logger& m_logger;

public:
	Executor( void );
	~Executor( void );


	void SetByteCodeMgr( Meson::Cuneus::IByteCodeMgr* p_pByteCodeMgr );
	void SetGarbageCollector( Meson::Cuneus::IGarbageCollector* p_pGarbageCollector );
	void SetConstantsMgr( Meson::Cuneus::ConstantsMgr*  p_pConstantsMgr);
	void SetGlobalDataMgr( Meson::Cuneus::GlobalDataMgr* p_pGlobalDataMgr );

	void SetContext( Meson::Cuneus::VCPU::Context* p_pContext );

	dword EnterMethodFrame( dword p_dwSize );
	void  LeaveMethodFrame( dword p_dwPosition );

	int Execute( void );
};

Meson_Cuneus_END
