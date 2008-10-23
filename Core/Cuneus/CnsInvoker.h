#pragma once

#include "CnsNamespaces.h"
#include "CnsDataType.h"
#include "CnsTypeMethod.h"
#include "CnsContext.h"

#include <ArrayList.h>
#include <Pointer.h>


Meson_Cuneus_RTS_BEGIN


class Invoker
{

public:
	typedef Meson::Common::Collections::TArrayList< Meson::Common::TPointer< Typing::DataType > > Parameters;

public:
	Invoker( void );
	~Invoker( void );

	Meson::Common::TPointer< Typing::DataType > Invoke( Meson::Cuneus::VCPU::Context* p_pContext, Parameters p_params, Reflection::TypeMethod p_method );
	Meson::Common::TPointer< Typing::DataType > InvokeObject( Meson::Cuneus::VCPU::Context* p_pContext, Meson::Common::TPointer< Typing::DataType > p_pObject, Parameters p_params, Reflection::TypeMethod p_method );

	void InvokeObject( Meson::Cuneus::VCPU::Context* p_pContext, byte p_byRegObj, word p_wMethodIndex );

	Meson::Common::TPointer< Typing::DataType > InvokeVirtual( Meson::Cuneus::VCPU::Context* p_pContext, Parameters p_params,  Reflection::TypeMethod p_method );

	//Meson::Common::TPointer< Typing::DataType > InvokeNative( Meson::Cuneus::VCPU::Context* p_pContext, Parameters& p_params, Reflection::TypeMethod p_method );
	
	Meson::Common::TPointer< Typing::DataType > Invoker::InvokeNative( Meson::Cuneus::VCPU::Context* p_pContext, Reflection::TypeMethod& p_method );
	
	void InvokeNative( Meson::Cuneus::VCPU::Context* p_pContext, word p_wMethodIndex, byte p_byRegObj );
	
};

Meson_Cuneus_RTS_END