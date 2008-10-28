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
#include "VstShaderProgram.h"
#include "VstShaderProgramVariable.h"
#include "VstShaderProgramConstant.h"


Meson_Vistas_BEGIN

	/** Adapter class for all shader program types.
	 */
	class ShaderProgramAdapter : public IShaderProgram
	{
	protected:
		ShaderProgramAdapter(IShaderProgramFactory* p_pCreator, Meson::Common::Text::String p_strId);
		ShaderProgramAdapter(IShaderProgramFactory* p_pCreator);

	public:
		//----------------------------------------------------------------------------------------------
		/** Virtual destructor for derived class cascaded destruction.
		 */
		//----------------------------------------------------------------------------------------------
		virtual ~ShaderProgramAdapter();

		//----------------------------------------------------------------------------------------------
		/**	Returns the factory that created the shader program. Note that the creator must be set
			by derived classes.

			\return Pointer to the IShaderProgramFactory implementation that created the shader
					program.
		 */
		//----------------------------------------------------------------------------------------------
		IShaderProgramFactory* GetCreator(void);

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

			TODO: Fine grain loading of shader parameters by adding LoadVariable and LoadConstant 
				  functions to uniquely upload constants.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void LoadParameters(void) = 0;
		virtual void LoadVariable(const Meson::Common::Text::String& p_strVariableName) = 0;
		virtual void LoadRenderConstant(RenderConstantType::RenderConstantType p_renderConstantType) = 0;

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
		/** Compiles the shader program into a usable resource which can be uploaded to the GPU. Must
			be implemented.
		 */
		//----------------------------------------------------------------------------------------------
		virtual bool Compile(void) = 0;
		virtual bool Compile(Meson::Common::Text::String& p_strErrors) = 0;

		//----------------------------------------------------------------------------------------------
		/** Returns whether the given shader program is bound to its respective unit. 

			\return <b>true</b> if bound, <b>false</b> otherwise.
		 */
		//----------------------------------------------------------------------------------------------
		bool IsBound(void) const;

		//----------------------------------------------------------------------------------------------
		/** Returns whether the given shader program is compiled into a usable resource. 

			\return <b>true</b> if compiled, <b>false</b> otherwise.
		 */
		//----------------------------------------------------------------------------------------------
		bool IsCompiled(void) const;

		//----------------------------------------------------------------------------------------------
		/** Sets the source code of the shader program. Note that once the program source code is
			set, it must then be compiled before it can be used.

			\param p_strSourceCode Shader program source code.
		 */
		//----------------------------------------------------------------------------------------------
		void SetSource(const Meson::Common::Text::String& p_strSourceCode);

		//----------------------------------------------------------------------------------------------
		/** Sets the entry point to the program when it is executed. Although this is usually equivalent 
			to the <i>main</i> function in the C programming language, it is dependent on the 
			shading language being used and the respective implementation.

			/param p_strEntryPoint Program entry point.
		 */
		//----------------------------------------------------------------------------------------------
		void SetEntryPoint(const Meson::Common::Text::String& p_strEntryPoint);

		//----------------------------------------------------------------------------------------------
		/** Sets the profile under which the shader program will operate. For instance, in a HLSL 
			implementation, the profile would be anything from "vs_1_1" to "vs_4_0" (vertex shader) 
			or "ps_1_1" to "ps_4_0" (pixel shaders). The available profile set may be acquired 
			at runtime from the ShaderProgramManager.

			\param p_strProfile Shader program profile.
		 */
		//----------------------------------------------------------------------------------------------
		void SetProfile(const Meson::Common::Text::String& p_strProfile);

		//----------------------------------------------------------------------------------------------
		/** Adds a user-defined variable which may be used as a constant during shader program
			execution. 

			\param p_pVariable User defined variable.
		 */
		//----------------------------------------------------------------------------------------------
		void AddVariable(ShaderProgramVariable* p_pVariable);

		//----------------------------------------------------------------------------------------------
		/** Returns the named user-defined variable bound with the shader program.

			\param p_strVariableName Name of user-defined variable.
			\return Pointer to the user-defined variable.
		 */
		//----------------------------------------------------------------------------------------------
		ShaderProgramVariable* GetVariable(const Meson::Common::Text::String& p_strVariableName);
		
		//----------------------------------------------------------------------------------------------
		/** Removes the binding between the named user-defined variable and the shader program.

			\param p_strVariableName Name of user-defined variable to loose.
		 */
		//----------------------------------------------------------------------------------------------
		void RemoveVariable(const Meson::Common::Text::String& p_strVariableName);

		//----------------------------------------------------------------------------------------------
		/** Adds a render constant to the list of constants the program may peruse during execution.
			
			\param p_renderConstantType Type of render constant to add to shader program.
		 */
		//----------------------------------------------------------------------------------------------
		void AddRenderConstant(RenderConstantType::RenderConstantType p_eRenderConstantType);

		//----------------------------------------------------------------------------------------------
		/** Removes the render constant binding with the shader program.
			
			\param p_renderConstantType Type of render constant to unbind from shader program.
		 */
		//----------------------------------------------------------------------------------------------
		void RemoveRenderConstant(RenderConstantType::RenderConstantType p_eRenderConstantType);

	protected:
		bool m_bIsBound,
			 m_bIsCompiled;

		IShaderProgramFactory* m_pCreator;

		Meson::Common::Collections::THashMap<Meson::Common::Text::String, ShaderProgramVariable*> m_shaderVariableMap;
		Meson::Common::Collections::TArrayList<RenderConstantType::RenderConstantType> m_renderConstantList;

		Meson::Common::Text::String m_strId,
									m_strSourceCode,
									m_strEntryPoint,
									m_strProfile;
	};

Meson_Vistas_END