#include "CnsTypeMethod.h"

#include "CnsCuneusEngine.h"
#include "CnsClassLoader.h"
#include "CnsDataTypeMgr.h"


using namespace Meson::Cuneus::RTS::Typing;


Meson_Cuneus_RTS_Reflection_BEGIN

TypeMethod::TypeMethod( void )
	:	m_dwByteCodeOffset( 0 )
	,	m_lstParams()
{
}

TypeMethod::~TypeMethod( void )
{
}

dword TypeMethod::GetByteCodeOffset( void )
{
	return m_dwByteCodeOffset;
}

void TypeMethod::SetByteCodeOffset( dword p_dwOffset )
{
	m_dwByteCodeOffset = p_dwOffset;
}


void* TypeMethod::GetNativeAddress( void )
{
	return m_pNativeAddress;
}

void  TypeMethod::SetNativeAddress( void* p_pAddress )
{
	m_pNativeAddress = p_pAddress;
}


void TypeMethod::AddParam( Meson::Common::TPointer< TypeParam > m_pParam )
{
	m_lstParams.Add( m_pParam );
}

Meson::Common::TPointer< TypeParam > TypeMethod::GetParam( dword p_dwIndex )
{
	return m_lstParams[ p_dwIndex ];
}

dword TypeMethod::GetArity( void )
{
	return (dword) m_lstParams.Size();
}

//dword TypeMethod::GetFrameSize( void )
//{
//	return m_dwFrameSize;
//}
//
//void  TypeMethod::SetFrameSize( dword p_dwFrameSize )
//{
//	m_dwFrameSize = p_dwFrameSize;
//}
//
//dword TypeMethod::GetReturnTypeCode( void )
//{
//	return m_dwReturnTypeCode;
//}
//
//void  TypeMethod::SetReturnTypeCode( dword p_dwReturnTypeCode )
//{
//	m_dwReturnTypeCode = p_dwReturnTypeCode;
//}


void TypeMethod::Load( Meson::Common::IO::IInputStream& p_stream )
{
	//Load Name
	//Load Return
	//Load Params
	//Load ByteCode Length
	//Load ByteCode


	//CuneusEngine::GetInstance()->ByteCodeManagers()[0]->GetByteCodePointer( m_dwByteCodeOffset );
}

void TypeMethod::Store( Meson::Common::IO::IOutputStream& p_stream )
{
	p_stream.Write( m_strName.ToArray(), m_strName.Length() );
	//p_stream.Write( m_dataType.GetName().ToArray(), m_dataType.GetName().Length() );

	CuneusEngine::GetInstance()->GetByteCodeMgr()->GetByteCodePointer( m_dwByteCodeOffset );
}


Meson_Cuneus_RTS_Reflection_END
