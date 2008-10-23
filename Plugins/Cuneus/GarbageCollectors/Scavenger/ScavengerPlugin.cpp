#include "ScavengerPlugIn.h"
#include "CnsCuneusEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::PlugIn;

Meson_Cuneus_BEGIN

ScavengerPlugIn::ScavengerPlugIn( void ) : m_strName("Scavenger PlugIn"), m_version( 0, 0, 1 ), m_pGarbageCollector( NULL )
{
}

ScavengerPlugIn::~ScavengerPlugIn( void )
{
}

const String &ScavengerPlugIn::GetName( void ) const
{
	return m_strName;
}

const Version &ScavengerPlugIn::GetVersion( void ) const
{
	return m_version;
}

void ScavengerPlugIn::Register( SharedObjectMap* p_pSharedObjectMap )
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	if( m_pGarbageCollector == NULL )
	{
		m_pGarbageCollector = new Scavenger();

		CuneusEngine *pCuneusEngine = (CuneusEngine*)m_pSharedObjectMap->GetRegisteredObject("CuneusEngine");
		MESON_ASSERT( pCuneusEngine != NULL, "CuneusEngine is not a registered shared object!" );

		//pCuneusEngine->GarbageCollectors().Add( "Scavenger", m_pGarbageCollector );
		pCuneusEngine->SetGarbageCollector( m_pGarbageCollector );

		CuneusEngine::SetInstance( pCuneusEngine );
		pCuneusEngine->Logger().Out << "Registered Scavenger...\n";
	}
}


void ScavengerPlugIn::Unregister( void )
{
	CuneusEngine::GetInstance()->Logger().Out << "Unregistering Scavenger...\n";

	if( m_pGarbageCollector != NULL )
	{
		//CuneusEngine::GetInstance()->GarbageCollectors().Remove( "Scavenger" );
		CuneusEngine::SetInstance(NULL);

		delete m_pGarbageCollector;
		m_pGarbageCollector = NULL;
	}
}

void ScavengerPlugIn::Initialise( void )
{
}

void ScavengerPlugIn::Shutdown( void )
{
}

Meson_Cuneus_END
