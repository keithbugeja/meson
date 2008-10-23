#pragma once

//----------------------------------------------------------------------------------------------
//	Used to implements a Listener pattern for the dispatching of Reading and Updating of Properties
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	05/03/2008	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------


#include "Namespaces.h"


Meson_Common_Properties_BEGIN



// Forward Reference
class Property;
class PropertyValue;

class IPropertyListener
{
public:
	virtual ~IPropertyListener( void ) {}

	// Get Events
	virtual void OnGetting( Property& p_property ) {}

	// Set Events
	virtual void OnSetting( Property& p_property ) {}
	virtual void OnSet( Property& p_property ) {}

	// Get Indexed Event
	virtual void OnGettingIndexed( PropertyValue& p_propertyValue, size_t p_unIndex ) {}

	// Set Indexed Event
	virtual void OnSettingIndexed( PropertyValue& p_propertyValue, size_t p_unIndex ) {}
};


Meson_Common_Properties_END
