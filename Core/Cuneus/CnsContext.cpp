//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::VCPU::Context
//	This class represents the execution contexts and contains the information required by the
//	Virtual Machine's interpreter to execute the task.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	27/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#include "CnsContext.h"

using namespace Meson::Cuneus::RTS::Typing;
using namespace Meson::Cuneus::Util;

Meson_Cuneus_VCPU_BEGIN

Context::Context( int p_nStackSize ) : Stack( p_nStackSize )
{

}

Context::~Context( void )
{
}

void Context::Reset( void )
{
	Registers.Reset();
	Flags.Reset();
	DataList.Clear();
	Stack.Clear();
}

dword Context::EnterMethodFrame( dword p_dwSize )
{
	dword dwMark = (dword)DataList.Size();

	for( dword dw = 0; dw < p_dwSize; dw++ )
	{
		DataList.Add( DataType() );
	}

	return dwMark;
}

void  Context::LeaveMethodFrame( dword p_dwPosition )
{
	DataList.RemoveRange( p_dwPosition, DataList.Size() - p_dwPosition );
}

Meson_Cuneus_VCPU_END
