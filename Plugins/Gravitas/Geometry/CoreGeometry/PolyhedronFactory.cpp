#include "PolyhedronFactory.h"
#include "Polyhedron.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

PolyhedronFactory::PolyhedronFactory(void)
	: m_strName("Polyhedron")
{
}

PolyhedronFactory:: ~PolyhedronFactory(void)
{
}

const String& PolyhedronFactory::GetName(void)
{
	return m_strName;
}

GeometryPtr PolyhedronFactory::CreateGeometry(void)
{
	Polyhedron* pPolyhedron = new Polyhedron();
	GravitasEngine::GetInstance()->Logger().Out << "Polyhedron geometry created.\n";
	return GeometryPtr(pPolyhedron);
}
