#include "CnsNativeMethodMgr.h"


using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Cuneus::RTS::Reflection;

Meson_Cuneus_BEGIN

NativeMethodMgr::NativeMethodMgr( void )
{
}

NativeMethodMgr::~NativeMethodMgr( void )
{
}


dword NativeMethodMgr::AddMethod( TPointer< TypeMethod > p_pMethod )
{
	dword dw = (dword) m_lstMethods.Size();

	m_lstMethods.Add( p_pMethod );
	m_mapMethods.Insert( p_pMethod->GetName(), p_pMethod );

	return dw;
}


dword NativeMethodMgr::AddMethod( Meson::Common::Text::String& p_strMethodName, Meson::Cuneus::RTS::Reflection::TypeMethod* p_pMethod )
{
	dword dw = (dword) m_lstMethods.Size();

	TPointer< Meson::Cuneus::RTS::Reflection::TypeMethod > pMethod( p_pMethod );

	p_pMethod->SetName( p_strMethodName );
	m_lstMethods.Add( pMethod );
	m_mapMethods.Insert( p_pMethod->GetName(), pMethod );

	return dw;
}


TPointer< TypeMethod > NativeMethodMgr::GetMethod( dword p_dwIndex )
{
	return m_lstMethods[ p_dwIndex ];
}

TPointer< TypeMethod > NativeMethodMgr::GetMethod( String& p_strMethodName )
{
	return m_mapMethods[ p_strMethodName ];
}


dword NativeMethodMgr::RegisterMethod( String& p_strMethodName, void* p_pMethod, dword p_dwReturnTypeCode )
{
	if( p_pMethod == NULL )
		throw new MesonException( "Cannot register null pointing methods!", __FILE__, __LINE__ );

	TPointer< TypeMethod > pMethod;

	pMethod = new TypeMethod();
	pMethod->SetName( p_strMethodName );
	pMethod->SetNativeAddress( p_pMethod );
	pMethod->SetTypeCode( p_dwReturnTypeCode );

	return AddMethod( pMethod );
}


Meson_Cuneus_END