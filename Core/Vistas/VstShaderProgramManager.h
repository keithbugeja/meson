//----------------------------------------------------------------------------------------------
//	Meson::Vistas::
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Singleton.h"

#include "VstTypes.h"
#include "VstShaderProgram.h"
#include "VstShaderProgramFactory.h"
#include "VstShaderProgramCatalogue.h"

Meson_Vistas_BEGIN

	/**
	 */
	class VISTAS_API ShaderProgramManager
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** Constructor
		 */
		//----------------------------------------------------------------------------------------------
		ShaderProgramManager(void);

		//----------------------------------------------------------------------------------------------
		/** Destructor
		 */
		//----------------------------------------------------------------------------------------------
		~ShaderProgramManager(void); 

		//----------------------------------------------------------------------------------------------
		/**	Registers a shader program factory implementation with shader program manager.

			\param p_pShaderProgramFactory Pointer to an IShaderProgramFactory implementation.
		 */
		//----------------------------------------------------------------------------------------------
		void RegisterShaderProgramFactory(IShaderProgramFactory* p_pShaderProgramFactory);

		//----------------------------------------------------------------------------------------------
		/** Unregisters the specified factory.

			\param p_strName Name of factory to unregister.
		 */
		//----------------------------------------------------------------------------------------------
		void UnregisterShaderProgramFactory(const Meson::Common::Text::String& p_strName);
		
		//----------------------------------------------------------------------------------------------
		/** Unregisters all the shader program factories.
		 */
		//----------------------------------------------------------------------------------------------
		void UnregisterAllShaderProgramFactories(void);

		//----------------------------------------------------------------------------------------------
		/** Selects the active shader program factory.

			\param p_strName Name of shader program factory.
		 */
		//----------------------------------------------------------------------------------------------
		void SetShaderProgramFactory(const Meson::Common::Text::String& p_strName);

		//----------------------------------------------------------------------------------------------
		/** Checks whether shader program factory is currently active.

			\param p_strName Name of shader program factory.
		 */
		//----------------------------------------------------------------------------------------------
		bool IsActiveShaderProgramFactory(const Meson::Common::Text::String& p_strName);

		//----------------------------------------------------------------------------------------------
		/** Returns the latest shader profiles for the given shader program type.

			\param p_shaderProgramType ShaderProgramType under which to query for supported profiles.
			\return A string with the supported profile.
		 */
		//----------------------------------------------------------------------------------------------
		const Meson::Common::Text::String& GetShaderProfile(const IShaderProgram::ShaderProgramType p_shaderProgramType);

		//----------------------------------------------------------------------------------------------
		/** Returns a list of the supported shader profiles for the given shader program type.

			\param p_shaderProgramType ShaderProgramType under which to query for supported profiles.
			\return An enumeration with the supported profiles.
		 */
		//----------------------------------------------------------------------------------------------
		ShaderProfileEnumerator GetAvailableShaderProfiles(const IShaderProgram::ShaderProgramType p_shaderProgramType);

		//----------------------------------------------------------------------------------------------
		/** Returns whether a given profile is supported for a specific program type.

			\param p_shaderProgramType ShaderProgramType under which to test for profile support.
			\param p_strProfile Profile to test.

			\return <b>true</b> if profile is supported, <b>false</b> otherwise.
		 */
		//----------------------------------------------------------------------------------------------
		bool IsShaderProfileSupported(const IShaderProgram::ShaderProgramType p_shaderProgramType, const Meson::Common::Text::String& p_strProfile);

		//----------------------------------------------------------------------------------------------
		/** Returns the file extension for the active shader program factory.

			\return The shader program file extension for the active program factory.
		 */
		//----------------------------------------------------------------------------------------------
		const Meson::Common::Text::String& GetShaderProgramExtension(void) const;

		//----------------------------------------------------------------------------------------------
		/** Creates a named shader program within the given catalogue. The program type is chosen 
			through the ShaderProgramType enumeration. 

			\param p_strFilename		Name of shader program file to load.
			\param p_shaderProgramType	Type of program to create (i.e. Vertex, Pixel, etc.)
			\param p_strCatalogueName	Name of catalogue within which to put the new program.
			\param p_strProgramName		Name of new program.

			\return Returns the newly created shader program.
		 */
		//----------------------------------------------------------------------------------------------
		IShaderProgram* CreateShaderProgramFromFile(Meson::Common::Text::String p_strFilename, const IShaderProgram::ShaderProgramType p_shaderProgramType, const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName);

		//----------------------------------------------------------------------------------------------
		/** Creates a named shader program within the given catalogue. The program type is chosen 
			through the ShaderProgramType enumeration. 

			\param p_shaderProgramType	Type of program to create (i.e. Vertex, Pixel, etc.)
			\param p_strCatalogueName	Name of catalogue within which to put the new program.
			\param p_strProgramName		Name of new program.

			\return Returns the newly created shader program.
		 */
		//----------------------------------------------------------------------------------------------
		IShaderProgram* CreateShaderProgram(const IShaderProgram::ShaderProgramType p_shaderProgramType, const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName);
		
		//----------------------------------------------------------------------------------------------
		/** Returns the named shader program from the specified catalogue.

			\param p_strCatalogueName	Name of catalogue where to look for shader program.
			\param p_strProgramName		Name of shader program.

			\return Returns the shader program.
		 */
		//----------------------------------------------------------------------------------------------
		IShaderProgram* GetShaderProgram(const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName);

		//----------------------------------------------------------------------------------------------
		/** Checks whether the given shader program is present in the specified catalogue.

			\param p_strCatalogueName	Name of catalogue where to look for shader program.
			\param p_strProgramName		Name of shader program.

			\return True if the program exists, false otherwise.
		 */
		//----------------------------------------------------------------------------------------------
		bool ContainsShaderProgram(const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName);

		//----------------------------------------------------------------------------------------------
		/** Checks whether the given shader program is present in the specified catalogue.

			\param p_strCatalogueName	Name of catalogue where to look for shader program.
			\param p_strProgramName		Name of shader program.
			\param p_pShaderProgram		Set to shader program if found in catalogue.

			\return True if the program exists, false otherwise.
		 */
		//----------------------------------------------------------------------------------------------
		bool ContainsShaderProgram(const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName, IShaderProgram*& p_pShaderProgram);
		
		//----------------------------------------------------------------------------------------------
		/**	Destroys the shader program within the specified catalogue.

			\param p_strCatalogueName	Name of catalogue containing program.
			\param p_strProgramName		Name of program to destroy.
		 */
		//----------------------------------------------------------------------------------------------
		void DestroyShaderProgram(const Meson::Common::Text::String& p_strCatalogueName, const Meson::Common::Text::String& p_strProgramName);

		//----------------------------------------------------------------------------------------------
		/**	Destroys all programs within all catalogues. The catalogues containing the programs are
			also destroyed.
		 */
		//----------------------------------------------------------------------------------------------
		void DestoryAllShaderPrograms(void);

		//----------------------------------------------------------------------------------------------
		/** Checks whether the given shader program catalogue exists.

			\param p_strCatalogueName Name of program catalogue.

			\return True if catalogue exists, false otherwise.
		 */
		//----------------------------------------------------------------------------------------------
		bool ContainsCatalogue(const Meson::Common::Text::String& p_strCatalogueName);

	protected:
		IShaderProgramFactory* m_pShaderProgramFactory;

		Meson::Common::Collections::THashMap<Meson::Common::Text::String, IShaderProgramFactory*> m_shaderProgramFactoryMap;
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, ShaderProgramCatalogue*> m_shaderProgramCatalogueMap;
	};

Meson_Vistas_END