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
#include "VstShaderProgram.h"
#include "VstShaderProgramFactory.h"

Meson_Vistas_BEGIN
	
	typedef Meson::Common::Collections::TEnumerator<IShaderProgram*>& ShaderProgramEnumerator;

	/**
	 */
	class ShaderProgramCatalogue
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** Construct a new shader program catalogue.
		 */
		//----------------------------------------------------------------------------------------------
		ShaderProgramCatalogue(const Meson::Common::Text::String& p_strName);

		//----------------------------------------------------------------------------------------------
		/** Destruct catalogue.
		 */
		//----------------------------------------------------------------------------------------------
		~ShaderProgramCatalogue(void);

		//----------------------------------------------------------------------------------------------
		/** Returns the name of the catalogue.
		 */
		//----------------------------------------------------------------------------------------------
		const Meson::Common::Text::String& GetName(void) const;

		//----------------------------------------------------------------------------------------------
		/** Adds a shader program to the catalogue.
		 */
		//----------------------------------------------------------------------------------------------
		void AddShaderProgram(IShaderProgram* p_pShaderProgram);

		//----------------------------------------------------------------------------------------------
		/** Removes shader program from catalogue.
		 */
		//----------------------------------------------------------------------------------------------
		void RemoveShaderProgram(const Meson::Common::Text::String& p_strName);
		
		//----------------------------------------------------------------------------------------------
		/** Clears the catalogue of all shader programs.
		 */
		//----------------------------------------------------------------------------------------------
		void RemoveAllShaderPrograms(void);

		//----------------------------------------------------------------------------------------------
		/** Returns the named shader program.
		 */
		//----------------------------------------------------------------------------------------------
		IShaderProgram* GetShaderProgram(const Meson::Common::Text::String& p_strName);

		//----------------------------------------------------------------------------------------------
		/** Returns whether the specified shader program is contained within the catalogue.
		 */
		//----------------------------------------------------------------------------------------------
		bool ContainsShaderProgram(const Meson::Common::Text::String& p_strName);

		//----------------------------------------------------------------------------------------------
		/** Returns whether the specified shader program is contained within the catalogue.
		 */
		//----------------------------------------------------------------------------------------------
		bool ContainsShaderProgram(const Meson::Common::Text::String& p_strName, IShaderProgram*& p_pShaderProgram);

		//----------------------------------------------------------------------------------------------
		/** Returns an enumerator of all shader programs within the catalogue.
		 */
		//----------------------------------------------------------------------------------------------
		ShaderProgramEnumerator GetShaderProgramEnumeration(void);

	protected:
		Meson::Common::Text::String m_strName;

		Meson::Common::Collections::THashMap<Meson::Common::Text::String, IShaderProgram*> m_shaderProgramMap;
	};

Meson_Vistas_END