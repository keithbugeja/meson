#pragma once

#include "String8.h"

#include "GrvNamespaces.h"
#include "GrvSpace.h"

Meson_Gravitas_Space_BEGIN

//----------------------------------------------------------------------------------------------
/// \class ISpaceFactory
/// Abstract factory interface for space instances. Implementations of this interface
/// generate specific space implementations and are normally loaded through the engine's
/// plugin mechanism.
//----------------------------------------------------------------------------------------------
class GRAVITAS_API ISpaceFactory
{
public:
	virtual ~ISpaceFactory(void) {};

	//----------------------------------------------------------------------------------------------
	/// Returns the name of the space factory.
	/// \returns the name of the space factory.
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetName(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Returns a managed pointer to a newly created instance of a space associated with this
	/// factory.
	/// \returns a managed pointer to a newly created instance of a space.
	//----------------------------------------------------------------------------------------------
	virtual SpacePtr CreateSpace(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Creates and returns a new space instance with the given ID.
	/// \param p_strId ID to assign to the space instance.
	/// \returns a new space instance.
	//----------------------------------------------------------------------------------------------
	virtual SpacePtr CreateSpace(const Meson::Common::Text::String& p_strId) = 0;
};

Meson_Gravitas_Space_END
