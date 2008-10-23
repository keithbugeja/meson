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

Meson_Vistas_BEGIN
	/**
	 */
	typedef Meson::Common::Collections::TEnumerator<Meson::Common::Text::String>& ShaderProfileEnumerator;

	/**
	 */
	class IShaderProgramFactory
	{
	public:
		virtual ~IShaderProgramFactory() { };
		virtual const Meson::Common::Text::String& GetName(void) const = 0;
	
		virtual const Meson::Common::Text::String& GetShaderProfile(const IShaderProgram::ShaderProgramType p_shaderProgramType) = 0;
		virtual ShaderProfileEnumerator GetAvailableShaderProfiles(const IShaderProgram::ShaderProgramType p_shaderProgramType) = 0;
		virtual bool IsShaderProfileSupported(const IShaderProgram::ShaderProgramType p_shaderProgramType, const Meson::Common::Text::String& p_strProfile) = 0;

		virtual const Meson::Common::Text::String& GetShaderProgramExtension(void) const = 0; 

		virtual IShaderProgram* CreateShaderProgram(const IShaderProgram::ShaderProgramType p_shaderProgramType, const Meson::Common::Text::String& p_strProgramName) = 0;
		virtual IShaderProgram* CreateShaderProgramFromFile(const Meson::Common::Text::String& p_strFilename, const IShaderProgram::ShaderProgramType p_shaderProgramType, const Meson::Common::Text::String& p_strProgramName) = 0;

		virtual void DestroyShaderProgram(IShaderProgram* p_pShaderProgram) = 0;
	};

Meson_Vistas_END