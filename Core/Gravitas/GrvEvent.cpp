#include "GrvEvent.h"

#include "GrvGravitasEngine.h"

using namespace Meson::Common::Memory;

using namespace Meson::Gravitas;

Meson_Gravitas_Event_BEGIN

MemoryManager* GetEventMemoryManager(void)
{
	MemoryManager* pMemoryManagerGravitas = GetGravitasMemoryManager();
	static MemoryManager s_memoryManagerEvents
		= MemoryManager("Events", pMemoryManagerGravitas, 0, 0);
	return &s_memoryManagerEvents;
}

Meson_Gravitas_Event_END
