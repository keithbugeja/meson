//----------------------------------------------------------------------------------------------
//	Meson::Gravitas::Space::SpaceManager
//	Gravitas space manager.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		20/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Singleton.h"
#include "LookaheadInputStream.h"

#include "GrvNamespaces.h"
#include "GrvSpaceFactory.h"

Meson_Gravitas_Space_BEGIN

struct SpaceManagerState;

//----------------------------------------------------------------------------------------------
/// \class SpaceManager
/// The physics engine's space manager provides registry and query services for space factory
/// implementations.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API SpaceManager
	: public Meson::Common::TSingleton<SpaceManager>
{
	friend class Meson::Common::TSingleton<SpaceManager>;

private: // private variables
	SpaceManagerState *m_pSpaceManagerState;

private: // private methods
	SpaceManager(void);
	~SpaceManager(void);

public:
	//----------------------------------------------------------------------------------------------
	/// Returns an enumerator of space factories registered with the manager.
	/// \returns an enumerator of space factories registered with the manager.
	//----------------------------------------------------------------------------------------------
	Meson::Common::Collections::TEnumerator<ISpaceFactory*>& GetSpaceFactories(void);

	//----------------------------------------------------------------------------------------------
	/// Returns a space factory indexed by the given name. If the factory is not found a
	/// MesonException is thrown.
	/// \param p_strName Name of the space factory to retrieve.
	/// \returns a pointer to the space factory implementation.
	//----------------------------------------------------------------------------------------------
	ISpaceFactory* GetSpaceFactory(const Meson::Common::Text::String& p_strName);

	//----------------------------------------------------------------------------------------------
	/// Registers the given space factory implementation with the manager.
	/// \param p_pSpaceFactory Pointer to a space factory to registor.
	//----------------------------------------------------------------------------------------------
	void RegisterSpaceFactory(Meson::Gravitas::Space::ISpaceFactory* p_pSpaceFactory);

	//----------------------------------------------------------------------------------------------
	/// Unregisters the given space factory implementation from the manager.
	/// \param p_pSpaceFactory Pointer to a space factory to unregistor.
	//----------------------------------------------------------------------------------------------
	void UnregisterSpaceFactory(Meson::Gravitas::Space::ISpaceFactory *p_pSpaceFactory);

	//----------------------------------------------------------------------------------------------
	/// Unregisters all the space factory implementation from the manager.
	//----------------------------------------------------------------------------------------------
	void UnregisterAllSpaceFactories(void);

	//----------------------------------------------------------------------------------------------
	/// Loads a space instance from the given input stream.
	/// \param p_pLookaheadInputStream An input stream with lookahead capabilities containing a
	///        space definition.
	//----------------------------------------------------------------------------------------------
	SpacePtr LoadSpace(Meson::Common::IO::LookaheadInputStreamPtr p_pLookaheadInputStream);
};

Meson_Gravitas_Space_END
