//----------------------------------------------------------------------------------------------
//	Meson::Vistas::DX9RenderDevice
//	Direct3D9-based renderer implementation for Vistas.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <d3dx9math.h>
#include <d3dx9core.h>
#include <d3d9.h>

#include "VstRenderDeviceAdapter.h"
#include "VstVistasEngine.h"

#include "DX9Types.h"
#include "DX9AdapterManager.h"
#include "DX9AdapterDevice.h"
#include "DX9WindowRenderTarget.h"
#include "DX9TextureRenderTarget.h"
#include "DX9HardwareResourceFactory.h"
#include "DX9HLSLProgramFactory.h"

#define DX9RD_FFP_OFF

Meson_Vistas_BEGIN
	class DX9RenderDevice : public RenderDeviceAdapter
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** These methods convert Meson and Vistas objects to their Direct3D9 counterparts.
		 */
		//----------------------------------------------------------------------------------------------
		static D3DMATRIX GetD3DMatrix(const Meson::Common::Maths::Matrix4f& p_matrix);
		static D3DMATRIX GetD3DMatrix(float* p_pfMatrix);

		static D3DVECTOR GetD3DVector(const Meson::Common::Maths::Vector3f& p_vector);
		static D3DVECTOR GetD3DVector(float* p_pfVector);

		static D3DCOLORVALUE GetD3DColourValue(const Colour& p_colour);
		static D3DCOLOR  GetD3DColour(const Colour& p_colour);
		//----------------------------------------------------------------------------------------------

	protected:
		//----------------------------------------------------------------------------------------------
		/** Called on initialisation of the render device, this method creates
			the default entries in the device configuration.
		 */
		void SetDefaultConfiguration(void);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		/** This method loads the device capability structures for the selected	
			adapter.
		 */
		//----------------------------------------------------------------------------------------------
		void SetDeviceCapabilities(void);

		//----------------------------------------------------------------------------------------------
		/** This method wraps Direct3D's SetRenderState function and checks for
			actual state changes before committing a value.
		 */
		void SetD3DRenderState(D3DRENDERSTATETYPE p_d3drsType, DWORD p_dwValue);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		/** This method wraps Direct3D's SetSamplerState function and checks for
			actual state changes before committing a value.
		 */
		void SetD3DSamplerState(DWORD p_dwSampler, D3DSAMPLERSTATETYPE p_d3dssType, DWORD p_dwValue);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		/** Sets a Direct3D light object from a Vistas light object.
		 */
		void SetD3DLight(uint p_uiLightIndex, const Light* p_pLight);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		/** Sets a Direct3D sampler object from a Vistas sampler object.
		 */
		void SetD3DSampler(uint p_uiSamplerIndex, const SamplerState* p_pSamplerState);
		//----------------------------------------------------------------------------------------------

	public:
		//----------------------------------------------------------------------------------------------
		DX9RenderDevice(void);
		~DX9RenderDevice(void);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		const Meson::Common::Text::String& GetName(void) const;
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		WindowRenderTarget* Startup(void);
		void Shutdown(void);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		bool BeginScene(void);
		void EndScene(void);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		void SetColourMask(IRenderDevice::ColourMask p_eColourMask);
		//ColourMask GetColourMask(void);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		void DrawPrimitive(PrimitiveType p_ePrimitiveType, uint p_uiStartVertex, uint p_uiCount);
		void DrawIndexedPrimitive(PrimitiveType p_ePrimitiveType, uint p_uiStartVertex, uint p_uiStartIndex, uint p_uiIndexLowerBound, uint p_uiIndexUpperBound, uint p_uiPrimitiveCount );
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		void SetStreamSource(uint p_uiStream, VertexBuffer* p_pVertexBuffer);
		void SetVertexDeclaration(VertexDeclaration* p_vertexDeclaration);
		void SetVertexChannel(VertexChannel* p_pVertexChannel);
		void SetIndexBuffer(IndexBuffer* p_pIndexBuffer);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		void ClearDepthBuffer(float p_fDepthValue);
		void ClearStencilBuffer(uint p_uiStencilValue);
		void ClearColourBuffer(const Colour& p_colour);
		void ClearBuffers(const Colour& p_colour, float p_fDepthValue, uint p_uiStencilValue);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		void SetAlphaRenderState(AlphaRenderState* p_pAlphaRenderState);
		void SetBiasRenderState(BiasRenderState* p_pBiasRenderState);
		void SetCullRenderState(CullRenderState* p_pCullRenderState);
		void SetDepthRenderState(DepthRenderState* p_pDepthRenderState);
		void SetLightRenderState(LightRenderState* p_pLightRenderState);
		void SetMaterialRenderState(MaterialRenderState* p_pMaterialRenderState);
		void SetSamplerRenderState(SamplerRenderState* p_pSamplerRenderState);
		void SetStencilRenderState(StencilRenderState* p_pStencilRenderState);
		void SetTargetRenderState(TargetRenderState* p_pTargetRenderState);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		void SetRenderTarget(const Meson::Common::Text::String& p_strName);
		WindowRenderTarget* CreateWindowRenderTarget(const Meson::Common::Text::String& p_strName, uint p_uiWidth, uint p_uiHeight, bool p_bWindowed, Meson::Common::Config::Configuration* p_pConfiguration);
		TextureRenderTarget* CreateTextureRenderTarget(const Meson::Common::Text::String& p_strName, TextureBuffer* p_pTextureBuffer, bool p_bCreateDepthStencilSurface);
		MultipleTextureRenderTarget* CreateMultipleTextureRenderTarget(const Meson::Common::Text::String& p_strName);
		void DestroyRenderTarget(const Meson::Common::Text::String& p_strName);
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		void SetProjectionMatrix(const Meson::Common::Maths::Matrix4f& p_projectionMatrix);
		void SetWorldMatrix(const Meson::Common::Maths::Matrix4f& p_worldMatrix);
		void SetViewMatrix(const Meson::Common::Maths::Matrix4f& p_viewMatrix);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		void MakePerspectiveProjection(float p_fLeft, float p_fRight, float p_fTop, float p_fBottom, float p_fNear, float p_fFar, Meson::Common::Maths::Matrix4f* p_pMatrixOut );
		void MakeOrthographicProjection(float p_fLeft, float p_fRight, float p_fTop, float p_fBottom, float p_fNear, float p_fFar, Meson::Common::Maths::Matrix4f* p_pMatrixOut );
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		void SetViewport(Viewport* p_pViewport);
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		void SetSamplerState(uint p_uiSamplerIndex, const SamplerState* p_pSamplerState);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		void SetTexture(uint p_uiTextureIndex, TextureBuffer* p_pTextureBuffer);
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		void SetLight(uint p_uiLightIndex, const Light* p_pLight);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		void ResetDevice(void);
		bool IsDeviceLost(void);
		void OnDeviceLost(void);
		void OnDeviceReset(void);
		//----------------------------------------------------------------------------------------------

	protected:
		/** Direct3D object shared across all instances of render device.
		 */
		static LPDIRECT3D9		m_pD3D9;

		HRESULT					m_hResult;
		HINSTANCE				m_hInstance;
		D3DDISPLAYMODE			m_displayMode;
		D3DADAPTER_IDENTIFIER9	m_adapterIdentifier;

		Meson::Common::Text::String	m_strName;

		/** These state tables map a generic vistas constant onto its Direct3D
			equivalent.
		 */
		static DWORD m_d3dAlphaCompareFunction[AlphaRenderState::ARSCMP_COUNT];
		static DWORD m_d3dAlphaBlend[AlphaRenderState::ARSB_COUNT];
		static DWORD m_d3dCullMode[CullRenderState::CRSFW_COUNT];
		static DWORD m_d3dDepthCompareFunction[DepthRenderState::DRSCMP_COUNT];
		static DWORD m_d3dFilterType[Meson::Vistas::SamplerState::FT_COUNT];
		static DWORD m_d3dPrimitiveType[IRenderDevice::PT_COUNT];
		//static DWORD m_d3dShadeMode[LightRenderState::LRSST_COUNT];
		static DWORD m_d3dStencilCompareFunction[StencilRenderState::SRSCMP_COUNT];
		static DWORD m_d3dStencilOperation[StencilRenderState::SRSOP_COUNT];
		static DWORD m_d3dWrapType[Meson::Vistas::SamplerState::WT_COUNT];

		/** The adapter device holds the Direct3D Device instance, shared across 
			all users of the DX9RenderDevice.
		 */
		DX9AdapterDevice*			m_pAdapterDevice;
		DX9AdapterManager*			m_pAdapterManager;

		/** The hardware resource factory implementation
		 */
		DX9HardwareResourceFactory*	m_pHardwareResourceFactory;
		DX9HLSLProgramFactory*		m_pHLSLProgramFactory;

		DX9WindowRenderTarget*		m_pPrimaryWindow;
		Meson::Common::Collections::TArrayList<DX9WindowRenderTarget*>	m_secondaryWindowList;
	};
Meson_Vistas_END
