//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstRenderTypes.h"
#include "VstVistasObject.h"
#include "VstShaderProgramVariable.h"
#include "VstShaderProgramConstant.h"


Meson_Vistas_BEGIN

	/** Base class for all shader program types.
	 */
	class IShaderProgram : public VistasObject
	{
	public:
		/** This enumeration lists the shader program types which
			are currently supported.
		 */
		enum ShaderProgramType
		{	
			SPT_VERTEX,
			SPT_PIXEL,
			SPT_COUNT
		};

	protected:
		IShaderProgram(void) : VistasObject() { }
		IShaderProgram(const Meson::Common::Text::String& p_strId) : VistasObject(p_strId) { }

	public:
		//----------------------------------------------------------------------------------------------
		/**	Returns the type of shader program of the underlying implementation. Must be implemented
			by derived classes.

			\return Type of shader program.
		 */
		//----------------------------------------------------------------------------------------------
		virtual ShaderProgramType GetType(void) const = 0;

		//----------------------------------------------------------------------------------------------
		/** Loads the shader program parameters into the respective environment. User variables, 
			renderer constants and sampler units are automatically loaded and bound. Must be 
			implemented by derived classes.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void LoadParameters(void) = 0;
		virtual void LoadVariable(const Meson::Common::Text::String& p_strVariableName) = 0;
		virtual void LoadRenderConstant(RenderConstantType::RenderConstantType p_renderConstantType) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Loads a user variable in the respective environment. Free variables do not require binding, 
			but need to be explicitly uploaded by the user before a program is executed.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void LoadFreeVariable(ShaderProgramVariable* p_pShaderVariable) = 0;

		//----------------------------------------------------------------------------------------------
		/** Binds the shader program to its respective unit; for instance, a vertex program will be
			bound to the vertex shading unit, replacing any previously bound programs. Must be 
			implemented by derived classes.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Bind(void) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Frees the shader program unit, forcing rendering to fall back to the fixed-function 
			pipeline. Must be implemented by derived classes.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Unbind(void) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Returns whether the given shader program is bound to its respective unit. 

			\return <b>true</b> if bound, <b>false</b> otherwise.
		 */
		//----------------------------------------------------------------------------------------------
		virtual bool IsBound(void) const = 0;

		//----------------------------------------------------------------------------------------------
		/** Compiles the shader program into a usable resource which can be uploaded to the GPU. Must
			be implemented.
		 */
		//----------------------------------------------------------------------------------------------
		virtual bool Compile(void) = 0;
		virtual bool Compile(Meson::Common::Text::String& p_strErrors) = 0;

		//----------------------------------------------------------------------------------------------
		/** Returns whether the given shader program is compiled into a usable resource. 

			\return <b>true</b> if compiled, <b>false</b> otherwise.
		 */
		//----------------------------------------------------------------------------------------------
		virtual bool IsCompiled(void) const = 0;

		//----------------------------------------------------------------------------------------------
		/** Sets the source code of the shader program. Note that once the program source code is
			set, it must then be compiled before it can be used.

			\param p_strSourceCode Shader program source code.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetSource(const Meson::Common::Text::String& p_strSourceCode) = 0;

		//----------------------------------------------------------------------------------------------
		/** Sets the entry point to the program when it is executed. Although this is usually equivalent 
			to the <i>main</i> function in the C programming language, it is dependent on the 
			shading language being used and the respective implementation.

			\param p_strEntryPoint Program entry point.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetEntryPoint(const Meson::Common::Text::String& p_strEntryPoint) = 0;

		//----------------------------------------------------------------------------------------------
		/** Sets the profile under which the shader program will operate. For instance, in a HLSL 
			implementation, the profile would be anything from "vs_1_1" to "vs_4_0" (vertex shader) 
			or "ps_1_1" to "ps_4_0" (pixel shaders). The available profile set may be acquired 
			at runtime from the ShaderProgramManager.

			\param p_strProfile Shader program profile.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetProfile(const Meson::Common::Text::String& p_strProfile) = 0;

		//----------------------------------------------------------------------------------------------
		/** Adds a user-defined variable which may be used as a constant during shader program
			execution. 

			\param p_pVariable User-defined variable.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void AddVariable( ShaderProgramVariable* p_pVariable ) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Returns the named user-defined variable bound with the shader program.

			\param p_strVariableName Name of user-defined variable.
			\return Pointer to the user-defined variable.
		 */
		//----------------------------------------------------------------------------------------------
		virtual ShaderProgramVariable* GetVariable(const Meson::Common::Text::String& p_strVariableName) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Removes the binding between the named user-defined variable and the shader program.

			\param p_strVariableName Name of user-defined variable to loose.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void RemoveVariable(const Meson::Common::Text::String& p_strVariableName) = 0;

		//----------------------------------------------------------------------------------------------
		/** Adds a render constant to the list of constants the program may peruse during execution.
			
			\param p_renderConstantType Type of render constant to add to shader program.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void AddRenderConstant(RenderConstantType::RenderConstantType p_renderConstantType) = 0;

		//----------------------------------------------------------------------------------------------
		/** Removes the render constant binding with the shader program.
			
			\param p_renderConstantType Type of render constant to unbind from shader program.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void RemoveRenderConstant(RenderConstantType::RenderConstantType p_renderConstantType) = 0;
	};

Meson_Vistas_END