#include "GrvInstrumentationDevice.h"

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Instrumentation;

const Colour Colour::Black = Colour((Real)0.0, (Real) 0.0, (Real) 0.0);
const Colour Colour::Blue = Colour((Real) 0.0, (Real) 0.0, (Real) 1.0);
const Colour Colour::Green = Colour((Real) 0.0, (Real) 1.0, (Real) 0.0);
const Colour Colour::Cyan = Colour((Real) 0.0, (Real) 1.0, (Real) 1.0);
const Colour Colour::Red = Colour((Real) 1.0, (Real) 0.0, (Real) 0.0);
const Colour Colour::Magenta = Colour((Real) 1.0, (Real) 0.0, (Real) 1.0);
const Colour Colour::Yellow = Colour((Real) 1.0, (Real) 1.0, (Real) 0.0);
const Colour Colour::White = Colour((Real) 1.0, (Real) 1.0, (Real) 1.0);

Colour::Colour(void)
	: R((Real) 1.0)
	, G((Real) 1.0)
	, B((Real) 1.0)
{
}

Colour::Colour(const Colour& p_colour)
	: R(p_colour.R)
	, G(p_colour.G)
	, B(p_colour.B)
{
}

Colour::Colour(Real p_rRed, Real p_rGreen, Real p_rBlue)
	: R(p_rRed)
	, G(p_rGreen)
	, B(p_rBlue)
{
}

Colour& Colour::operator=(const Colour& p_colour)
{
	R = p_colour.R;
	G = p_colour.G;
	B = p_colour.B;
	return *this;
}

bool Colour::operator==(const Colour& p_colour) const
{
	return R == p_colour.R
		&& G == p_colour.G
		&& B == p_colour.B;
}
