//----------------------------------------------------------------------------------------------
//	Meson::Vistas::
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstVistasObject.h"

Meson_Vistas_BEGIN
	
	/** The ResourceHandle masks implementation specific objects
		such as pointers or handles allotted by APIs like DirectX
		or OpenGL.
	 */
	typedef void* ResourceHandle;
	
	/** The Resource class is the base class for all resource objects
		which wrap implementation specific objects. The resource class
		provides reference counting for use with smart-pointers, naming
		for object identitification and type naming for identification of
		specific object families.
	 */
	class Resource : public VistasObject
	{
		VISTAS_IMPLEMENT_RTTI(Resource, VistasObject);
		
	protected:
		Resource(const Meson::Common::Text::String& p_strId);
		Resource(void);

	public:
		//----------------------------------------------------------------------------------------------
		/** Virtual destructor
		 */
		//----------------------------------------------------------------------------------------------
		virtual ~Resource(void) {};

		//----------------------------------------------------------------------------------------------
		/** Returns the resource type.

			\return A string reference to the name of the type.
		 */
		//----------------------------------------------------------------------------------------------
		const Meson::Common::Text::String& GetResourceType(void) const;

		//----------------------------------------------------------------------------------------------
		/** Returns the resource handle.

			\return A resource handle object which hides the actual 
					implementation specific handle or pointer.
		 */
		//----------------------------------------------------------------------------------------------
		ResourceHandle GetResourceHandle(void);

		//----------------------------------------------------------------------------------------------
		/** Sets the resource handle. Note that the resource 
			handle should <b>not</b> be set by users of sub-
			classes; it is only the derived classes and other
			members of a specific implementation that should
			set the resource handle.

			\param p_resourceHandle A resource handle object.
		 */
		//----------------------------------------------------------------------------------------------
		void SetResourceHandle(ResourceHandle p_resourceHandle);

	protected:
		ResourceHandle m_resourceHandle;

		Meson::Common::Text::String m_strResourceType;
	};
	
Meson_Vistas_END