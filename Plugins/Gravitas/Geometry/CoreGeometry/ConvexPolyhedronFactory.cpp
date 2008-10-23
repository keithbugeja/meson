#include "ConvexPolyhedronFactory.h"
#include "ConvexPolyhedron.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

ConvexPolyhedronFactory::ConvexPolyhedronFactory(void)
	: m_strName("ConvexPolyhedron")
{
}

ConvexPolyhedronFactory:: ~ConvexPolyhedronFactory(void)
{
}

const String& ConvexPolyhedronFactory::GetName(void)
{
	return m_strName;
}

GeometryPtr ConvexPolyhedronFactory::CreateGeometry(void)
{
	ConvexPolyhedron* pConvextPolyhedron = new ConvexPolyhedron();
	GravitasEngine::GetInstance()->Logger().Out << "ConvextPolyhedron geometry created.\n";
	return GeometryPtr(pConvextPolyhedron);
}

GeometryPtr ConvexPolyhedronFactory::CreateGeometry(const String& p_strId)
{
	ConvexPolyhedron* pConvextPolyhedron = new ConvexPolyhedron(p_strId);
	GravitasEngine::GetInstance()->Logger().Out <<
		"ConvextPolyhedron geometry created with ID '" + p_strId + "'.\n";
	return GeometryPtr(pConvextPolyhedron);
}
