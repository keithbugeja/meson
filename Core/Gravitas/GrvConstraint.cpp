#include "GrvConstraint.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common::Memory;

using namespace Meson::Gravitas;

Meson_Gravitas_Constraint_BEGIN

MemoryManager* GetConstraintMemoryManager(void)
{
	MemoryManager* pMemoryManagerGravitas = GetGravitasMemoryManager();
	static MemoryManager s_memoryManagerConstraints
		= MemoryManager("Constraints", pMemoryManagerGravitas, 0, 0);
	return &s_memoryManagerConstraints;
}

Meson_Gravitas_Constraint_END
