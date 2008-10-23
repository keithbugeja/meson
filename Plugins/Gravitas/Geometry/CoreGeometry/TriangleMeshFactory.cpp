#include "TriangleMeshFactory.h"
#include "TriangleMesh.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Geometry;

TriangleMeshFactory::TriangleMeshFactory(void)
	: m_strName("TriangleMesh")
{
}

TriangleMeshFactory:: ~TriangleMeshFactory(void)
{
}

const String& TriangleMeshFactory::GetName(void)
{
	return m_strName;
}

GeometryPtr TriangleMeshFactory::CreateGeometry(void)
{
	TriangleMesh* pTriangleMesh = new TriangleMesh();
	GravitasEngine::GetInstance()->Logger().Out << "TriangleMesh geometry created.\n";
	return GeometryPtr(pTriangleMesh);
}

GeometryPtr TriangleMeshFactory::CreateGeometry(const String& p_strId)
{
	TriangleMesh* pTriangleMesh = new TriangleMesh(p_strId);
	GravitasEngine::GetInstance()->Logger().Out
		<< "TriangleMesh geometry created with ID '" + p_strId + "'.\n";
	return GeometryPtr(pTriangleMesh);
}
