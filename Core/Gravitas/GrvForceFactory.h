#pragma once

#include "String8.h"

#include "GrvNamespaces.h"
#include "GrvForce.h"

Meson_Gravitas_Force_BEGIN

//----------------------------------------------------------------------------------------------
/// \class IForceFactory
/// Abstract factory interface for force generator instances. Implementations of this interface
/// generate specific force generator implementations and are normally used through the engine's
/// plugin mechanism.
//----------------------------------------------------------------------------------------------
class IForceFactory
{
public:
	virtual ~IForceFactory(void) {};

	//----------------------------------------------------------------------------------------------
	/// Returns the name of the force factory.
	/// \returns the name of the force factory.
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetName(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Creates and returns a new force instance.
	/// \returns a new force instance.
	//----------------------------------------------------------------------------------------------
	virtual ForcePtr CreateForce(void) = 0;

	//----------------------------------------------------------------------------------------------
	/// Creates and returns a new force instance with the given ID.
	/// \param p_strId ID to assign to the force instance.
	/// \returns a new force instance.
	//----------------------------------------------------------------------------------------------
	virtual ForcePtr CreateForce(const Meson::Common::Text::String& p_strId) = 0;
};

Meson_Gravitas_Force_END
