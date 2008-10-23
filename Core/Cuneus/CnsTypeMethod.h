#pragma once

#include "CnsAccessEntity.h"
#include "CnsTypeParam.h"

#include <ArrayList.h>
#include <InputStream.h>
#include <OutputStream.h>

Meson_Cuneus_RTS_Reflection_BEGIN

class TypeMethod : public AccessEntity
{
private:
	dword m_dwByteCodeOffset;
	void* m_pNativeAddress;

	Meson::Common::Collections::TArrayList< Meson::Common::TPointer< TypeParam > > m_lstParams;

	//dword m_dwFrameSize;
	//dword m_dwReturnTypeCode;

public:
	TypeMethod( void );
	~TypeMethod( void );

	dword GetByteCodeOffset( void );
	void  SetByteCodeOffset( dword p_dwOffset );

	void* GetNativeAddress( void );
	void  SetNativeAddress( void* p_pAddress );

	void AddParam( Meson::Common::TPointer< TypeParam > m_pParam );
	Meson::Common::TPointer< TypeParam > GetParam( dword p_dwIndex );
	dword GetArity( void );

	//dword GetFrameSize( void );
	//void  SetFrameSize( dword p_dwFrameSize );

	//dword GetReturnTypeCode( void );
	//void  SetReturnTypeCode( dword p_dwReturnTypeCode );

	void Load( Meson::Common::IO::IInputStream& p_stream );
	void Store( Meson::Common::IO::IOutputStream& p_stream );
};

Meson_Cuneus_RTS_Reflection_END
