//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Material:MaterialManager
//	Gravitas space manager.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		10/07/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Singleton.h"
#include "LookaheadInputStream.h"

#include "GrvNamespaces.h"
#include "GrvMaterial.h"

Meson_Gravitas_Materials_BEGIN

struct MaterialManagerState;

//----------------------------------------------------------------------------------------------
/// \class MaterialManager
/// The physics engine's material manager provides registry and query services for materials.
//----------------------------------------------------------------------------------------------
class MaterialManager
	: public Meson::Common::TSingleton<MaterialManager>
{
	friend class Meson::Common::TSingleton<MaterialManager>;

private: // private variables
	MaterialManagerState *m_pMaterialManagerState;

private: // private methods
	MaterialManager(void);
	~MaterialManager(void);

public:
	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of materials registered with the manager.
	/// \returns an enumerator of materials registered with the manager.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<MaterialPtr> &GetMaterials(void);

	//----------------------------------------------------------------------------------------------
	/// Returns a material indexed by the given ID. If the material is not found a MesonException is
	/// thrown.
	/// \param p_strId ID of the material to retrieve.
	/// \returns a managed pointer to the material.
	//----------------------------------------------------------------------------------------------
	MaterialPtr GetMaterial(const Meson::Common::Text::String& p_strId);

	//----------------------------------------------------------------------------------------------
	/// Registers the given material with the manager.
	/// \param p_pMaterial Managed pointer to a material to registor.
	//----------------------------------------------------------------------------------------------
	void RegisterMaterial(MaterialPtr p_pMaterial);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the given material from the manager.
	/// \param p_pMaterial Managed pointer to a material to unregistor.
	//----------------------------------------------------------------------------------------------
	void UnregisterMaterial(MaterialPtr p_pMaterial);

	//----------------------------------------------------------------------------------------------
	/// Unregisters all the registered materials from the manager.
	//----------------------------------------------------------------------------------------------
	void UnregisterAllMaterials(void);

	//----------------------------------------------------------------------------------------------
	/// Loads a material from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        single material definition.
	//----------------------------------------------------------------------------------------------
	void LoadMaterial(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);

	//----------------------------------------------------------------------------------------------
	/// Loads a collection of materials from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        a collection of material definitions.
	//----------------------------------------------------------------------------------------------
	void LoadMaterials(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);

	//----------------------------------------------------------------------------------------------
	/// Loads a collection of materials from the given file.
	/// \param p_strFilename Name of a file containing a collection of material definitions.
	//----------------------------------------------------------------------------------------------
	void LoadMaterials(const Meson::Common::Text::String& p_strFilename);
};

Meson_Gravitas_Materials_END
