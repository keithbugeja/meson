#include "GrvEnvironment.h"

#include "LinkedList.h"
#include "ArrayList.h"
#include "Pair.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common::Collections;

using namespace Meson::Gravitas;
using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Collision;
using namespace Meson::Gravitas::Constraint;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Space;

Environment::Environment(void)
	: Id(GravitasEngine::GetInstance()->GenerateGuid())
	, Space(NULL)
	, Constraints(NULL)
	, CollisionFilter(NULL)
	, KineticIntegrator(NULL)
	, ConstraintSolver(NULL)
{
}

Environment::Environment(const String& p_strId)
	: Id(p_strId)
	, Space(NULL)
	, Constraints(NULL)
	, CollisionFilter(NULL)
	, KineticIntegrator(NULL)
	, ConstraintSolver(NULL)
{
}

Environment::Environment(const Environment& p_environment)
: Space(p_environment.Space)
	, Constraints(p_environment.Constraints)
	, CollisionFilter(p_environment.CollisionFilter)
	, KineticIntegrator(p_environment.KineticIntegrator)
	, ConstraintSolver(p_environment.ConstraintSolver)
{
}

Environment::~Environment(void)
{
}

