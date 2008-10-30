#include "GrvSpace.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common::Memory;
using namespace Meson::Common::Maths;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Geometry;

Meson_Gravitas_Space_BEGIN

MemoryManager* GetSpaceMemoryManager(void)
{
	MemoryManager* pMemoryManagerGravitas = GetGravitasMemoryManager();
	static MemoryManager s_memoryManagerSpaces
		= MemoryManager("Spaces", pMemoryManagerGravitas, 0, 0);
	return &s_memoryManagerSpaces;
}

BodyRayIntersection::BodyRayIntersection(void)
	: Ray(NULL)
	, Body()
	, Position()
{
}

BodyRayIntersection::BodyRayIntersection(const BodyRayIntersection& p_bodyRayIntersection)
	: Ray(p_bodyRayIntersection.Ray)
	, Body(p_bodyRayIntersection.Body)
	, Position(p_bodyRayIntersection.Position)
{
}

BodyRayIntersection::BodyRayIntersection(
	const Meson::Gravitas::Geometry::Ray& p_ray, BodyPtr p_pBody, const TVector3<Real>& p_ptPosition)
	: Ray(&p_ray)
	, Body(p_pBody)
	, Position(p_ptPosition)
{
}

BodyRayIntersection& BodyRayIntersection::operator=(const BodyRayIntersection& p_bodyRayIntersection)
{
	Ray = p_bodyRayIntersection.Ray;
	Body = p_bodyRayIntersection.Body;
	Position = p_bodyRayIntersection.Position;
	return *this;
}

bool BodyRayIntersection::operator==(const BodyRayIntersection& p_bodyRayIntersection) const
{
	return Ray == p_bodyRayIntersection.Ray
		&& Body == p_bodyRayIntersection.Body
		&& Position == p_bodyRayIntersection.Position;
}

bool BodyRayIntersection::operator<(const BodyRayIntersection& p_bodyRayIntersection) const
{
	return (Position - Ray->Source).LengthSquared()
		< (p_bodyRayIntersection.Position - p_bodyRayIntersection.Ray->Source).LengthSquared();
}

bool BodyRayIntersection::operator>(const BodyRayIntersection& p_bodyRayIntersection) const
{
	return (Position - Ray->Source).LengthSquared()
		> (p_bodyRayIntersection.Position - p_bodyRayIntersection.Ray->Source).LengthSquared();
}

bool BodyRayIntersection::operator<=(const BodyRayIntersection& p_bodyRayIntersection) const
{
	return (Position - Ray->Source).LengthSquared()
		<= (p_bodyRayIntersection.Position - p_bodyRayIntersection.Ray->Source).LengthSquared();
}

bool BodyRayIntersection::operator>=(const BodyRayIntersection& p_bodyRayIntersection) const
{
	return (Position - Ray->Source).LengthSquared()
		>= (p_bodyRayIntersection.Position - p_bodyRayIntersection.Ray->Source).LengthSquared();
}

Meson_Gravitas_Space_END
