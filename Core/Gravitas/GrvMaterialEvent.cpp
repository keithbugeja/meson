#include "GrvMaterialEvent.h"

using namespace Meson::Common::Memory;

using namespace Meson::Gravitas::Event;
using namespace Meson::Gravitas::Materials;

MemoryManager* Meson::Gravitas::Event::GetMaterialEventMemoryManager(void)
{
	MemoryManager* pMemoryManagerEvents = GetEventMemoryManager();
	static MemoryManager s_memoryManagerMaterialEvents
		= MemoryManager("Material Events", pMemoryManagerEvents, sizeof(MaterialEvent), 256);
	return &s_memoryManagerMaterialEvents;
}

MaterialEvent::MaterialEvent(void)
	: Material1(NULL)
	, Material2(NULL)
	, MaterialCombined(NULL)
{
}

MaterialEvent::MaterialEvent(
		const Material* p_pMaterial1,
		const Material* p_pMaterial2,
		Material* p_pMaterialCombined)
	: Material1(p_pMaterial1)
	, Material2(p_pMaterial2)
	, MaterialCombined(p_pMaterialCombined)
{
}

MaterialEvent::~MaterialEvent(void)
{
}

void* MaterialEvent::operator new(size_t p_unSize)
{
	return GetMaterialEventMemoryManager()->AllocatePooled(p_unSize);
}

void MaterialEvent::operator delete(void* p_pObject, size_t p_unSize)
{
	::operator delete(p_pObject);
}							
