#pragma once

#include "GrvEvent.h"
#include "GrvMaterial.h"

Meson_Gravitas_Event_BEGIN

//------------------------------------------------------------------------------------------
/// Returns a pointer to the memory manager for Gravitas material events.
/// \returns a pointer to the memory manager for Gravitas material events.
//------------------------------------------------------------------------------------------
Meson::Common::Memory::MemoryManager* GetMaterialEventMemoryManager(void);

//------------------------------------------------------------------------------------------
/// \class MaterialEvent
/// Material event implementation for allowing custom material combinations. A listener
/// registere for this event is given the opportunity to compute a custom combination of
/// the materials involved in a collision.
//------------------------------------------------------------------------------------------
class MaterialEvent :
	public IEvent
{
public: // public variables

	//------------------------------------------------------------------------------------------
	/// Constant pointer to a a constant material for the first body.
	//------------------------------------------------------------------------------------------
	const Meson::Gravitas::Materials::Material* const Material1;

	//------------------------------------------------------------------------------------------
	/// Constant pointer to a a constant material for the second body.
	//------------------------------------------------------------------------------------------
	const Meson::Gravitas::Materials::Material* const Material2;

	//------------------------------------------------------------------------------------------
	/// Constant pointer to a material with combined properties.
	//------------------------------------------------------------------------------------------
	Meson::Gravitas::Materials::Material* const MaterialCombined;

public: // public methods

	//------------------------------------------------------------------------------------------
	/// Constructs a material event.
	//------------------------------------------------------------------------------------------
	MaterialEvent(void);

	//------------------------------------------------------------------------------------------
	/// Constructs a material event with the given pointers to the materials of the first and
	/// second bodies, and a pointer to a combined material.
	/// \param p_pMaterial1 Pointer to a a constant material for the first body.
	/// \param p_pMaterial2 Pointer to a a constant material for the second body.
	/// \param p_pMaterialCombined Pointer to a material with combined properties.
	//------------------------------------------------------------------------------------------
	MaterialEvent(
		const Meson::Gravitas::Materials::Material* p_pMaterial1,
		const Meson::Gravitas::Materials::Material* p_pMaterial2,
		Meson::Gravitas::Materials::Material* p_pMaterialCombined);

	//------------------------------------------------------------------------------------------
	/// Destroys the material event.
	//------------------------------------------------------------------------------------------
	~MaterialEvent(void);

	//------------------------------------------------------------------------------------------
	/// Allocates memory for the object prior to construction.
	/// \param p_unSize Size of the required memory buffer in bytes.
	/// \returns a pointer to the allocated buffer.
	//------------------------------------------------------------------------------------------
	void* operator new(size_t p_unSize);

	//------------------------------------------------------------------------------------------
	/// Deallocates the object's memory following destruction.
	/// \param p_pObject Pointer to the memory buffer to deallocate.
	/// \param p_unSize Size of the memory buffer in bytes.
	//------------------------------------------------------------------------------------------
	void operator delete(void* p_pObject, size_t p_unSize);
};

//------------------------------------------------------------------------------------------
/// Managed pointer to a material event.
//------------------------------------------------------------------------------------------
typedef Meson::Common::TPointer<MaterialEvent> MaterialEventPtr;

Meson_Gravitas_Event_END
