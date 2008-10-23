#pragma once

//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Geometry::GeometryManager
//	Gravitas geometry manager.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		09/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Singleton.h"
#include "LookaheadInputStream.h"

#include "GrvNamespaces.h"
#include "GrvGeometryFactory.h"

Meson_Gravitas_Geometry_BEGIN

struct GeometryManagerState;

//----------------------------------------------------------------------------------------------
/// \class GeometryManager
/// The physics engine's geometry manager provides type name mapping services for geometry
/// instances and maintains a registry of geometry factories. The geometry type name identifiers
/// are used for fast indexing into the collision detector matrix that provides access to
/// appropriate collision algorithms for any given geometry pair.
//----------------------------------------------------------------------------------------------
class GeometryManager
	: public Meson::Common::TSingleton<GeometryManager>
{
	friend class Meson::Common::TSingleton<GeometryManager>;

private: // private variables
	GeometryManagerState *m_pGeometryManagerState;

private: // private methods
	GeometryManager(void);
	~GeometryManager(void);

public:
	//----------------------------------------------------------------------------------------------
	/// Returns a short integer-based identifier for the given geometry type name. Identifiers for
	/// new type names are assigned on the fly.
	/// \param p_strTypeName Type name of a geometry instance.
	/// \returns a short integer-based identifier for the given geometry type name.
	//----------------------------------------------------------------------------------------------
	short GetGeometryTypeId(const Meson::Common::Text::String& p_strTypeName);

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of registered geometry factories.
	/// \returns an enumerator of registered geometry factories.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<IGeometryFactory *>& GetGeometryFactories(void);

	//----------------------------------------------------------------------------------------------
	/// Returns a registered geometry factory using the given type name.
	/// \returns a registered geometry factory using the given type name.
	//----------------------------------------------------------------------------------------------
	IGeometryFactory* GetGeometryFactory(const Meson::Common::Text::String& p_strName);
	
	//----------------------------------------------------------------------------------------------
	/// Registers the given geometry factory implementation with the manager.
	/// \param p_pGeometryFactory Geometry factory implementation to register.
	//----------------------------------------------------------------------------------------------
	void RegisterGeometryFactory(IGeometryFactory* p_pGeometryFactory);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the given geometry factory implementation from the manager.
	/// \param p_pGeometryFactory Geometry factory implementation to unregister.
	//----------------------------------------------------------------------------------------------
	void UnregisterGeometryFactory(IGeometryFactory* p_pGeometryFactory);

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of registered geometry instances.
	/// \returns an enumerator of registered geometry instances.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<GeometryPtr>& GetGeometries(void);

	//----------------------------------------------------------------------------------------------
	/// Returns a registered geometry instance using the given ID.
	/// \returns a registered geometry instance using the given ID.
	//----------------------------------------------------------------------------------------------
	GeometryPtr GetGeometry(const Meson::Common::Text::String& p_strId);

	//----------------------------------------------------------------------------------------------
	/// Registers the given geometry instance with the manager.
	/// \param p_pGeometry Managed pointer to a geometry instance to register.
	//----------------------------------------------------------------------------------------------
	void RegisterGeometry(GeometryPtr p_pGeometry);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the given geometry instance with the manager.
	/// \param p_pGeometry Managed pointer to a geometry instance to unregister.
	//----------------------------------------------------------------------------------------------
	void UnregisterGeometry(GeometryPtr p_pGeometry);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the all the registered geometry instances from the manager.
	//----------------------------------------------------------------------------------------------
	void UnregisterAllGeometries(void);

	//----------------------------------------------------------------------------------------------
	/// Loads and registers a geometry instance from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        single geometry definition.
	//----------------------------------------------------------------------------------------------
	void LoadGeometry(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);

	//----------------------------------------------------------------------------------------------
	/// Loads and registers a collection of geometry instances from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        collection of geometry definitions.
	//----------------------------------------------------------------------------------------------
	void LoadGeometries(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);

	//----------------------------------------------------------------------------------------------
	/// Loads and registers a collection of geometry instances from the given file.
	/// \param p_strFilename Name of a file containing a collection of geometry definitions.
	//----------------------------------------------------------------------------------------------
	void LoadGeometries(const Meson::Common::Text::String& p_strFilename);
};

Meson_Gravitas_Geometry_END
