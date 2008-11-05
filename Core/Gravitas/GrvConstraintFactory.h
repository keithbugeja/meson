#pragma once

#include "String8.h"

#include "GrvNamespaces.h"
#include "GrvConstraint.h"

Meson_Gravitas_Constraint_BEGIN

//------------------------------------------------------------------------------------------
/// \class IConstraintFactory
/// Abstract factory interface for creating constraints.
//------------------------------------------------------------------------------------------
class GRAVITAS_API IConstraintFactory
{
public:
	virtual ~IConstraintFactory(void) {};

	//------------------------------------------------------------------------------------------
	/// Returns the name of the factory.
	/// \returns the name of the factory.
	//------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetName(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Creates a constraint instance and returns a managed pointer to it.
	/// \returns a managed pointer to a new constraint instance.
	//------------------------------------------------------------------------------------------
	virtual ConstraintPtr CreateConstraint(void) = 0;

	//------------------------------------------------------------------------------------------
	/// Creates a constraint instance with the given ID and returns a managed pointer to it.
	/// \param p_strId ID to assign to the constraint instance.
	/// \returns a managed pointer to a new constraint instance.
	//------------------------------------------------------------------------------------------
	virtual ConstraintPtr CreateConstraint(const Meson::Common::Text::String& p_strId) = 0;
};

Meson_Gravitas_Constraint_END
