
#include "CnsCuneusEngine.h"

using namespace Meson::Common;
using namespace Meson::Cuneus;
using namespace Meson::Cuneus::RTS::Reflection;
using namespace Meson::Cuneus::RTS::Typing;

Meson_Cuneus_BEGIN

//----------------------------------------------------------------------------------------------
///	Constructor for CuneusEngine.
//----------------------------------------------------------------------------------------------
CuneusEngine::CuneusEngine(void) : m_strName("CuneusEngine"), m_version(0,0,1), m_lstVMs(4)
{
	m_pDataTypeMgr = DataTypeMgr::GetInstance();
	//m_pBuilder = Builder::GetInstance();
}

//----------------------------------------------------------------------------------------------
///	Destructor for CuneusEngine.
//----------------------------------------------------------------------------------------------
CuneusEngine::~CuneusEngine(void)
{
	FreePointers( m_lstVMs );
}

//----------------------------------------------------------------------------------------------
/// Returns the Engine name.
/// \return String containing the Engine name
//----------------------------------------------------------------------------------------------
const String& CuneusEngine::GetName(void)
{
	return m_strName;
}

//----------------------------------------------------------------------------------------------
/// Returns the Engine version.
/// \return Version object containing Engine version information
//----------------------------------------------------------------------------------------------
const Version& CuneusEngine::GetVersion(void)
{
	return m_version;
}

//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::IGarbageCollector >& CuneusEngine::GarbageCollectors( void )
//{
//	return m_GarbageCollectorsManager;
//}

//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::IByteCodeMgr >& CuneusEngine::ByteCodeManagers( void )
//{
//	return m_ByteCodeManager;
//}

//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::IScheduler >& CuneusEngine::Schedulers( void )
//{
//	return m_SchedulersManager;
//}

//Meson::Common::PlugIn::TPlugInTypeManager< Meson::Cuneus::ICuneusInterpreter >& CuneusEngine::CuneusInterpreters( void )
//{
//	return m_CuneusInterpretersManager;
//}


DataTypeMgr* CuneusEngine::GetDataTypeMgr( void )
{
	return m_pDataTypeMgr;
}

ConstantsMgr* CuneusEngine::GetConstantsMgr( void )
{
	return &m_constantsMgr;
}
GlobalDataMgr* CuneusEngine::GetGlobalDataMgr( void )
{
	return &m_globalDataMgr;
}

SymbolMgr* CuneusEngine::GetSymbolMgr( void )
{
	return &m_symbolMgr;
}

//Builder *CuneusEngine::GetReflectionBuilder( void )
//{
//	return m_pBuilder;
//}

//Namespace *CuneusEngine::GetGlobalNamespace( void )
//{
//	return m_pBuilder->GetGlobalNamespace();
//}

CuneusVirtualMachine* CuneusEngine::CreateVM( void )
{
	CuneusVirtualMachine *pVM = new CuneusVirtualMachine();
	m_lstVMs += pVM;
	return pVM;
}

void CuneusEngine::DestroyVM( CuneusVirtualMachine *p_pVM )
{
	m_lstVMs.Remove( p_pVM );
	delete p_pVM;
}

CuneusVirtualMachine* CuneusEngine::GetVM( int p_nIndex )
{
	return m_lstVMs[ p_nIndex ];
}




dword CuneusEngine::RegisterMethod( Meson::Common::Text::String& p_strMethodName, TypeMethod* p_pMethod )
{
	return m_methodMgr.AddMethod( p_strMethodName, p_pMethod );
}

void CuneusEngine::BindNativeMethod( Meson::Common::Text::String& p_strMethodName, void* p_pNativeAddress )
{
	GetMethod( p_strMethodName )->SetNativeAddress( p_pNativeAddress );
}

void CuneusEngine::BindNativeMethod( dword p_dwMethodIndex, void* p_pNativeAddress )
{
	GetMethod( p_dwMethodIndex )->SetNativeAddress( p_pNativeAddress );
}

Meson::Common::TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > CuneusEngine::GetMethod( dword p_dwIndex )
{
	return m_methodMgr.GetMethod( p_dwIndex );
}

Meson::Common::TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > CuneusEngine::GetMethod( Meson::Common::Text::String& p_strMethodName )
{
	return m_methodMgr.GetMethod( p_strMethodName );
}







dword CuneusEngine::AddConstant( Meson::Cuneus::RTS::Typing::DataType* p_pDataType )
{
	return m_constantsMgr.AddConstant( p_pDataType );
}

TPointer< Meson::Cuneus::RTS::Typing::DataType > CuneusEngine::GetConstant( dword p_dwIndex )
{
	return m_constantsMgr.GetConstant( p_dwIndex );
}

void CuneusEngine::SetConstant( dword p_dwIndex, Meson::Cuneus::RTS::Typing::DataType* p_pDataType )
{
	m_constantsMgr.SetConstant( p_dwIndex, p_pDataType );
}


Meson_Cuneus_END
