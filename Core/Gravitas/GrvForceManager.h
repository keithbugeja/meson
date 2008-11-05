//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Forces::ForceManager
//	Gravitas force manager.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		24/07/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Singleton.h"
#include "LookaheadInputStream.h"

#include "GrvNamespaces.h"
#include "GrvForceFactory.h"

Meson_Gravitas_Force_BEGIN

struct ForceManagerState;

//----------------------------------------------------------------------------------------------
/// \class ForceManager
/// The physics engine's force manager provides registry and query services for force factory
/// implementations.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API ForceManager
	: public Meson::Common::TSingleton<ForceManager>
{
	friend class Meson::Common::TSingleton<ForceManager>;

private: // private variables
	ForceManagerState* m_pForceManagerState;

private: // private methods
	ForceManager(void);
	~ForceManager(void);

public:
	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of force factories registered with the manager.
	/// \returns an enumerator of force factories registered with the manager.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<IForceFactory*>& GetForceFactories(void);

	//----------------------------------------------------------------------------------------------
	/// Returns a force factory indexed by the given name. If the factory is not found a
	/// MesonException is thrown.
	/// \param p_strName Name of the force factory to retrieve.
	/// \returns a pointer to the force factory implementation.
	//----------------------------------------------------------------------------------------------
	IForceFactory* GetForceFactory(const Meson::Common::Text::String& p_strName);

	//----------------------------------------------------------------------------------------------
	/// Registers the given force factory implementation with the manager.
	/// \param p_pForceFactory Pointer to a force factory to registor.
	//----------------------------------------------------------------------------------------------
	void RegisterForceFactory(Meson::Gravitas::Force::IForceFactory* p_pForceFactory);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the given force factory implementation from the manager.
	/// \param p_pForceFactory Pointer to a force factory to unregistor.
	//----------------------------------------------------------------------------------------------
	void UnregisterForceFactory(Meson::Gravitas::Force::IForceFactory *p_pForceFactory);

	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of registered force instances.
	/// \returns an enumerator of registered force instances.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<ForcePtr>& GetForces(void);

	//----------------------------------------------------------------------------------------------
	/// Returns a registered force instance using the given ID.
	/// \returns a registered force instance using the given ID.
	//----------------------------------------------------------------------------------------------
	ForcePtr GetForce(const Meson::Common::Text::String& p_strId);

	//----------------------------------------------------------------------------------------------
	/// Registers the given force instance with the manager.
	/// \param p_pForce Managed pointer to a force instance to register.
	//----------------------------------------------------------------------------------------------
	void RegisterForce(ForcePtr p_pForce);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the given force instance with the manager.
	/// \param p_pForce Managed pointer to a force instance to unregister.
	//----------------------------------------------------------------------------------------------
	void UnregisterForce(ForcePtr p_pForce);

	//----------------------------------------------------------------------------------------------
	/// Unregisters all the registered force instances from the manager.
	//----------------------------------------------------------------------------------------------
	void UnregisterAllForces(void);

	//----------------------------------------------------------------------------------------------
	/// Loads and registers a force instance from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        single force definition.
	//----------------------------------------------------------------------------------------------
	void LoadForce(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);

	//----------------------------------------------------------------------------------------------
	/// Loads and registers a collection of force instances from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        collection of force definitions.
	//----------------------------------------------------------------------------------------------
	void LoadForces(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);

	//----------------------------------------------------------------------------------------------
	/// Loads and registers a collection of force instances from the given file.
	/// \param p_strFilename Name of a file containing a collection of force definitions.
	//----------------------------------------------------------------------------------------------
	void LoadForces(const Meson::Common::Text::String& p_strFilename);
};

Meson_Gravitas_Force_END
