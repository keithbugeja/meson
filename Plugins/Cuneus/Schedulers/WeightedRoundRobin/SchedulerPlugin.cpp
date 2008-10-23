#include "SchedulerPlugIn.h"
#include "CnsCuneusEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::PlugIn;

Meson_Cuneus_BEGIN

SchedulerPlugIn::SchedulerPlugIn( void ) : m_strName("Weighted Round Robin Scheduler PlugIn"), m_version( 0, 0, 1 ), m_pScheduler( NULL )
{
}

SchedulerPlugIn::~SchedulerPlugIn( void )
{
}

const String &SchedulerPlugIn::GetName( void ) const
{
	return m_strName;
}

const Version &SchedulerPlugIn::GetVersion( void ) const
{
	return m_version;
}

void SchedulerPlugIn::Register( SharedObjectMap* p_pSharedObjectMap )
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	if( m_pScheduler == NULL )
	{
		m_pScheduler = new Scheduler();

		CuneusEngine *pCuneusEngine = (CuneusEngine*)m_pSharedObjectMap->GetRegisteredObject("CuneusEngine");
		MESON_ASSERT( pCuneusEngine != NULL, "CuneusEngine is not a registered shared object!" );

		//pCuneusEngine->Schedulers().Add( "WeightedRoundRobin", m_pScheduler );
		pCuneusEngine->SetScheduler( m_pScheduler );

		CuneusEngine::SetInstance( pCuneusEngine );
		pCuneusEngine->Logger().Out << "Registered Weighted Round Robin Scheduler...\n";
	}
}


void SchedulerPlugIn::Unregister( void )
{
	CuneusEngine::GetInstance()->Logger().Out << "Unregistering Weighted Round Robin Scheduler...\n";

	if( m_pScheduler != NULL )
	{
		//CuneusEngine::GetInstance()->Schedulers().Remove( "WeightedRoundRobin" );
		CuneusEngine::SetInstance(NULL);

		delete m_pScheduler;
		m_pScheduler = NULL;
	}
}

void SchedulerPlugIn::Initialise( void )
{
}

void SchedulerPlugIn::Shutdown( void )
{
}

Meson_Cuneus_END
