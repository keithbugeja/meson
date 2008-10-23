//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::VCPU::Registers
//	This class defines and contains the Virtual Machine Registers.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	23/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------
#include "CnsRegisters.h"
#include <string.h>
#include <Converter.h>

using namespace Meson::Common;
using namespace Meson::Common::Text;

using namespace Meson::Cuneus::RTS::Typing;

Meson_Cuneus_VCPU_BEGIN

Registers::Registers(void)
{
	Reset();
}

Registers::Registers( Registers &p_vmRegisters )
{
	Copy( p_vmRegisters );
}

Registers::~Registers(void)
{
}

void Registers::Reset( void )
{
	//memset( R, 0, sizeof( Registers ) );

	TypeDWord* value = new TypeDWord( 0 );

	for( int n=0; n<32; n++)
	{
		R[n] = value;
	}
		
	FP = value;
	SP = value;
	PC = value;
}

void Registers::Copy( Registers &p_vmRegisters )
{
	memcpy( R, p_vmRegisters.R, sizeof( Registers ) );
}

Registers &Registers::operator=( Registers &p_vmRegisters )
{
	Copy( p_vmRegisters );
	return *this;
}

bool Registers::operator==( Registers &p_vmRegisters )
{
	return ( memcmp( this->R, p_vmRegisters.R,  sizeof( Registers ) ) == 0 );
}

bool Registers::operator!=( Registers &p_vmRegisters )
{
	return ( memcmp( this->R, p_vmRegisters.R,  sizeof( Registers ) ) != 0 );
}

void Registers::Dump( Meson::Common::Text::String& p_str )
{
	for( int n=0; n<32; n++)
	{
		p_str += 'R';
		if( n<10 )
			p_str += '0';
		p_str += n;
		p_str += "=";
		p_str += R[n]->ToString();
		
		n++;
		p_str += 'R';
		if( n<10 )
			p_str += '0';
		p_str += n;
		p_str += "=";
		p_str += R[n]->ToString();
	}

}

Meson_Cuneus_VCPU_END
