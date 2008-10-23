//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <d3d9.h>
#include "VstShaderProgramFactory.h"
#include "DX9Definitions.h"
#include "DX9VertexProgram.h"
#include "DX9PixelProgram.h"

Meson_Vistas_BEGIN
	class DX9HLSLProgramFactory : public IShaderProgramFactory
	{
	public:
		//----------------------------------------------------------------------------------------------
		DX9HLSLProgramFactory(LPDIRECT3DDEVICE9 p_pD3DDevice) 
			: m_strName("hlsl"),
			  m_strExtension("hlsl"),
			  m_pD3DDevice(p_pD3DDevice)
		{
			InitialiseAvailableProfiles();
		}
		//----------------------------------------------------------------------------------------------
		void InitialiseAvailableProfiles(void)
		{
			// Update pixel shader profile list
			m_strPixelProfile = D3DXGetPixelShaderProfile(m_pD3DDevice);

			if (m_strPixelProfile == "ps_1_1")
			{
				m_availablePixelProfilesList.Add("ps_1_1");
			} 
			else if (m_strPixelProfile == "ps_1_2")
			{
				m_availablePixelProfilesList.Add("ps_1_1");
				m_availablePixelProfilesList.Add("ps_1_2");
			} 
			else if (m_strPixelProfile == "ps_1_3")
			{
				m_availablePixelProfilesList.Add("ps_1_1");
				m_availablePixelProfilesList.Add("ps_1_2");
				m_availablePixelProfilesList.Add("ps_1_3");
			} 
			else if (m_strPixelProfile == "ps_1_4")
			{
				m_availablePixelProfilesList.Add("ps_1_1");
				m_availablePixelProfilesList.Add("ps_1_2");
				m_availablePixelProfilesList.Add("ps_1_3");
				m_availablePixelProfilesList.Add("ps_1_4");
			} 
			else if (m_strPixelProfile == "ps_2_0")
			{
				m_availablePixelProfilesList.Add("ps_1_1");
				m_availablePixelProfilesList.Add("ps_1_2");
				m_availablePixelProfilesList.Add("ps_1_3");
				m_availablePixelProfilesList.Add("ps_1_4");
				m_availablePixelProfilesList.Add("ps_2_0");
			} 
			else if (m_strPixelProfile == "ps_2_a")
			{
				m_availablePixelProfilesList.Add("ps_1_1");
				m_availablePixelProfilesList.Add("ps_1_2");
				m_availablePixelProfilesList.Add("ps_1_3");
				m_availablePixelProfilesList.Add("ps_1_4");
				m_availablePixelProfilesList.Add("ps_2_0");
				m_availablePixelProfilesList.Add("ps_2_a");
			} 
			else if (m_strPixelProfile == "ps_2_b")
			{
				m_availablePixelProfilesList.Add("ps_1_1");
				m_availablePixelProfilesList.Add("ps_1_2");
				m_availablePixelProfilesList.Add("ps_1_3");
				m_availablePixelProfilesList.Add("ps_1_4");
				m_availablePixelProfilesList.Add("ps_2_0");
				m_availablePixelProfilesList.Add("ps_2_a");
				m_availablePixelProfilesList.Add("ps_2_b");
			} 
			else if (m_strPixelProfile == "ps_3_0")
			{
				m_availablePixelProfilesList.Add("ps_1_1");
				m_availablePixelProfilesList.Add("ps_1_2");
				m_availablePixelProfilesList.Add("ps_1_3");
				m_availablePixelProfilesList.Add("ps_1_4");
				m_availablePixelProfilesList.Add("ps_2_0");
				m_availablePixelProfilesList.Add("ps_2_a");
				m_availablePixelProfilesList.Add("ps_2_b");
				m_availablePixelProfilesList.Add("ps_3_0");
			}

			// Update vertex shader profile list
			m_strVertexProfile = D3DXGetVertexShaderProfile(m_pD3DDevice);

			if (m_strVertexProfile == "vs_1_1")
			{
				m_availableVertexProfilesList.Add("vs_1_1");
			} 
			else if (m_strVertexProfile == "vs_2_0")
			{
				m_availableVertexProfilesList.Add("vs_1_1");
				m_availableVertexProfilesList.Add("vs_2_0");
			} 
			else if (m_strVertexProfile == "vs_2_a")
			{
				m_availableVertexProfilesList.Add("vs_1_1");
				m_availableVertexProfilesList.Add("vs_2_0");
				m_availableVertexProfilesList.Add("vs_2_a");
			} 
			else if (m_strVertexProfile == "vs_3_0")
			{
				m_availableVertexProfilesList.Add("vs_1_1");
				m_availableVertexProfilesList.Add("vs_2_0");
				m_availableVertexProfilesList.Add("vs_2_a");
				m_availableVertexProfilesList.Add("vs_3_0");
			} 
		}
		//----------------------------------------------------------------------------------------------
		const Meson::Common::Text::String& GetName(void) const
		{
			return m_strName;
		}
		//----------------------------------------------------------------------------------------------
		const Meson::Common::Text::String& GetShaderProfile(const IShaderProgram::ShaderProgramType p_shaderProgramType)
		{
			if (p_shaderProgramType == IShaderProgram::SPT_VERTEX)
				return m_strVertexProfile;
			else if (p_shaderProgramType == IShaderProgram::SPT_PIXEL)
				return m_strPixelProfile;

			throw new Meson::Common::MesonException("Unknown shader program type.", __FILE__, __LINE__ );
		}
		//----------------------------------------------------------------------------------------------
		ShaderProfileEnumerator GetAvailableShaderProfiles(const IShaderProgram::ShaderProgramType p_shaderProgramType)
		{
			if (p_shaderProgramType == IShaderProgram::SPT_VERTEX)
				return m_availableVertexProfilesList.GetEnumerator();
			else if (p_shaderProgramType == IShaderProgram::SPT_PIXEL)
				return m_availablePixelProfilesList.GetEnumerator();

			throw new Meson::Common::MesonException("Unknown shader program type.", __FILE__, __LINE__ );
		}
		//----------------------------------------------------------------------------------------------
		bool IsShaderProfileSupported(const IShaderProgram::ShaderProgramType p_shaderProgramType, const Meson::Common::Text::String& p_strProfile)
		{
			if (p_shaderProgramType == IShaderProgram::SPT_VERTEX)
				return (m_availableVertexProfilesList.IndexOf(p_strProfile) != -1);
			else if (p_shaderProgramType == IShaderProgram::SPT_PIXEL)
				return (m_availablePixelProfilesList.IndexOf(p_strProfile) != -1);

			throw new Meson::Common::MesonException("Unknown shader program type.", __FILE__, __LINE__ );
		}
		//----------------------------------------------------------------------------------------------
		const Meson::Common::Text::String& GetShaderProgramExtension(void) const
		{
			return m_strExtension;
		}
		//----------------------------------------------------------------------------------------------
		IShaderProgram* CreateShaderProgram(const IShaderProgram::ShaderProgramType p_eShaderProgramType, const Meson::Common::Text::String& p_strProgramName)
		{
			switch( p_eShaderProgramType )
			{
			case IShaderProgram::SPT_PIXEL:
				return new DX9PixelProgram(m_pD3DDevice, this, p_strProgramName);

			case IShaderProgram::SPT_VERTEX:
				return new DX9VertexProgram(m_pD3DDevice, this, p_strProgramName);
			}

			return NULL;
		}
		//----------------------------------------------------------------------------------------------
		IShaderProgram* CreateShaderProgramFromFile(const Meson::Common::Text::String& p_strFilename, const IShaderProgram::ShaderProgramType p_eShaderProgramType, const Meson::Common::Text::String& p_strProgramName)
		{
			switch( p_eShaderProgramType )
			{
			case IShaderProgram::SPT_PIXEL:
				return new DX9PixelProgram(m_pD3DDevice, this, p_strFilename, p_strProgramName);

			case IShaderProgram::SPT_VERTEX:
				return new DX9VertexProgram(m_pD3DDevice, this, p_strFilename, p_strProgramName);
			}

			return NULL;
		}
		//----------------------------------------------------------------------------------------------
		void DestroyShaderProgram(IShaderProgram* p_pShaderProgram)
		{
			ShaderProgramAdapter* pShaderProgramAdapter = static_cast<ShaderProgramAdapter*>(p_pShaderProgram);
			SAFE_DELETE(pShaderProgramAdapter);
		}
		//----------------------------------------------------------------------------------------------
	protected:
		LPDIRECT3DDEVICE9 m_pD3DDevice;
		
		Meson::Common::Collections::TArrayList<Meson::Common::Text::String> m_availablePixelProfilesList,
																			m_availableVertexProfilesList;
		Meson::Common::Text::String m_strName,
									m_strExtension,
									m_strPixelProfile,
									m_strVertexProfile;
	};
Meson_Vistas_END
