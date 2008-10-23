
#include "CoreByteCodeManagersPlugIn.h"
#include "CnsCuneusEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::PlugIn;

Meson_Cuneus_BEGIN

CoreByteCodeManagersPlugIn::CoreByteCodeManagersPlugIn( void ) : m_strName("BasicByteCodeManager PlugIn"), m_version( 0, 0, 1 ), m_pByteCodeMgr( NULL )
{
}

CoreByteCodeManagersPlugIn::~CoreByteCodeManagersPlugIn( void )
{
}


const String &CoreByteCodeManagersPlugIn::GetName( void ) const
{
	return m_strName;
}

const Version &CoreByteCodeManagersPlugIn::GetVersion( void ) const
{
	return m_version;
}

void CoreByteCodeManagersPlugIn::Register( SharedObjectMap* p_pSharedObjectMap )
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	if( m_pByteCodeMgr == NULL )
	{
		m_pByteCodeMgr = new BasicByteCodeMgr();

		CuneusEngine *pCuneusEngine = (CuneusEngine*)m_pSharedObjectMap->GetRegisteredObject("CuneusEngine");
		MESON_ASSERT( pCuneusEngine != NULL, "CuneusEngine is not a registered shared object!" );

		//pCuneusEngine->ByteCodeManagers().Add( "BasicByteCodeManager", m_pByteCodeMgr );
		CuneusEngine::SetInstance( pCuneusEngine );

		pCuneusEngine->SetByteCodeMgr( m_pByteCodeMgr );

		pCuneusEngine->Logger().Out << "Registered Basic Bytecode Manager...\n";
	}
}


void CoreByteCodeManagersPlugIn::Unregister( void )
{
	CuneusEngine::GetInstance()->Logger().Out << "Unregistering Basic Bytecode Manager...\n";

	if( m_pByteCodeMgr != NULL )
	{
		//CuneusEngine::GetInstance()->GetByteCodeMgr().Remove( "BasicByteCodeManager" );
		CuneusEngine::SetInstance(NULL);

		delete m_pByteCodeMgr;
		m_pByteCodeMgr = NULL;
	}
}

void CoreByteCodeManagersPlugIn::Initialise( void )
{
}

void CoreByteCodeManagersPlugIn::Shutdown( void )
{
}

Meson_Cuneus_END
