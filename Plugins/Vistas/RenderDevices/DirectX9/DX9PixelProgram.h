//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <d3d9.h>
#include <d3dx9shader.h> 
#include "VstShaderProgramAdapter.h"

Meson_Vistas_BEGIN
	class DX9PixelProgram : public ShaderProgramAdapter
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** Constructor for a Direct3D9 pixel shader program.

			\param p_pD3DDevice		Direct3D9 device.
			\param p_pCreator		Factory that created the object.
			\param p_pProgramName	Pixel shader program name.
		 */
		//----------------------------------------------------------------------------------------------
		DX9PixelProgram(LPDIRECT3DDEVICE9 p_pD3DDevice, IShaderProgramFactory* p_pCreator, const Meson::Common::Text::String& p_strProgramName);
		
		//----------------------------------------------------------------------------------------------
		/** Constructor for a Direct3D9 pixel shader program.

			\param p_pD3DDevice		Direct3D9 device.
			\param p_pCreator		Factory that created the object.
			\param p_strFilename	Program filename.
			\param p_pProgramName	Pixel shader program name.
		 */
		//----------------------------------------------------------------------------------------------
		DX9PixelProgram(LPDIRECT3DDEVICE9 p_pD3DDevice, IShaderProgramFactory* p_pCreator, const Meson::Common::Text::String& p_strFilename, const Meson::Common::Text::String& p_strProgramName);	
		
		//----------------------------------------------------------------------------------------------
		/** Vertex program destructor.
		 */
		//----------------------------------------------------------------------------------------------
		~DX9PixelProgram(void);

		//----------------------------------------------------------------------------------------------
		/** Returns the type of shader program, in this case IShaderProgram::SPT_PIXEL.
		 */
		//----------------------------------------------------------------------------------------------
		ShaderProgramType GetType(void) const;

		//----------------------------------------------------------------------------------------------
		/** Loads the value of a shader program constant.

			\param p_pShaderProgramConstant Constant to load.
		 */
		//----------------------------------------------------------------------------------------------
		void LoadConstant(ShaderProgramConstant* p_pShaderProgramConstant);
		
		//----------------------------------------------------------------------------------------------
		/** Loads the vertex shader program operating parameters.
		 */
		//----------------------------------------------------------------------------------------------
		void LoadParameters(void);
		void LoadRenderConstant(RenderConstantType::RenderConstantType p_renderConstantType);
		void LoadVariable(const Meson::Common::Text::String& p_strVariableName);
		void LoadFreeVariable(ShaderProgramVariable* p_pShaderVariable);

		//----------------------------------------------------------------------------------------------
		/** Bind the pixel shader program to the respective unit.
		 */
		//----------------------------------------------------------------------------------------------
		void Bind(void);

		//----------------------------------------------------------------------------------------------
		/** Unbinds the pixel shader object from the respective unit.
		 */
		//----------------------------------------------------------------------------------------------
		void Unbind(void);

		//----------------------------------------------------------------------------------------------
		/** Compiles the pixel shader HLSL program.
		 */
		//----------------------------------------------------------------------------------------------
		bool Compile(void);
		bool Compile(Meson::Common::Text::String& p_strErrors);

	protected:
		LPDIRECT3DDEVICE9 m_pD3DDevice;

		LPD3DXCONSTANTTABLE m_pD3DConstantTable;
		LPDIRECT3DPIXELSHADER9 m_pD3DPixelShader;
	};
Meson_Vistas_END