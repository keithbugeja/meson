#include "InterpreterPlugIn.h"
#include "CnsCuneusEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::PlugIn;

Meson_Cuneus_BEGIN

InterpreterPlugIn::InterpreterPlugIn( void ) : m_strName("Cuneus Interpreter PlugIn"), m_version( 0, 0, 1 ), m_pInterpreter( NULL )
{
}

InterpreterPlugIn::~InterpreterPlugIn( void )
{
}

const String &InterpreterPlugIn::GetName( void ) const
{
	return m_strName;
}

const Version &InterpreterPlugIn::GetVersion( void ) const
{
	return m_version;
}

void InterpreterPlugIn::Register( SharedObjectMap* p_pSharedObjectMap )
{
	m_pSharedObjectMap = p_pSharedObjectMap;

	if( m_pInterpreter == NULL )
	{
		m_pInterpreter = new Interpreter();

		CuneusEngine *pCuneusEngine = (CuneusEngine*)m_pSharedObjectMap->GetRegisteredObject("CuneusEngine");
		MESON_ASSERT( pCuneusEngine != NULL, "CuneusEngine is not a registered shared object!" );

		//pCuneusEngine->CuneusInterpreters().Add( "CuneusInterpreter", m_pInterpreter );
		pCuneusEngine->SetCuneusInterpreter( m_pInterpreter );

		CuneusEngine::SetInstance( pCuneusEngine );
		pCuneusEngine->Logger().Out << "Registered Cuneus Interpreter...\n";
	}
}


void InterpreterPlugIn::Unregister( void )
{
	CuneusEngine::GetInstance()->Logger().Out << "Unregistering Cuneus Interpreter...\n";

	if( m_pInterpreter != NULL )
	{
		//CuneusEngine::GetInstance()->CuneusInterpreters().Remove( "CuneusInterpreter" );
		CuneusEngine::SetInstance(NULL);

		delete m_pInterpreter;
		m_pInterpreter = NULL;
	}
}

void InterpreterPlugIn::Initialise( void )
{
}

void InterpreterPlugIn::Shutdown( void )
{
}

Meson_Cuneus_END
