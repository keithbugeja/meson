//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::IRenderDevice
//	Interface class for rendering APIs.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Stack.h"
#include "VstRenderDevice.h"

Meson_Vistas_BEGIN

	class RenderConstant;

	class RenderDeviceAdapter : public IRenderDevice
	{
	public:
		//----------------------------------------------------------------------------------------------
		RenderDeviceAdapter(void);
		virtual ~RenderDeviceAdapter(void);
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual const Meson::Common::Text::String& GetName(void) const = 0;
		virtual Meson::Common::Config::Configuration* GetConfiguration(void);
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual WindowRenderTarget* Startup(void);
		virtual void Shutdown(void);
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual bool BeginScene(void) = 0;
		virtual void EndScene(void) = 0;
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		virtual void SetColourMask(IRenderDevice::ColourMask p_eColourMask);
		virtual IRenderDevice::ColourMask GetColourMask(void);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		virtual void DrawPrimitive(PrimitiveType p_ePrimitiveType, uint p_uiStartVertex, uint p_uiCount) = 0;
		virtual void DrawIndexedPrimitive(PrimitiveType p_ePrimitiveType, uint p_uiStartVertex, uint p_uiStartIndex, 
			uint p_uiIndexLowerBound, uint p_uiIndexUpperBound, uint p_uiPrimitiveCount ) = 0;
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual void SetStreamSource(uint p_uiStream, VertexBuffer* p_pVertexBuffer) = 0;
		virtual void SetVertexDeclaration(VertexDeclaration* p_pVertexDeclaration) = 0;
		virtual void SetVertexChannel(VertexChannel* p_pVertexChannel) = 0;
		virtual void SetIndexBuffer(IndexBuffer* p_pIndexBuffer) = 0;
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual void SwitchRenderContext(RenderContextPtr p_pOldContext, RenderContextPtr p_pNewContext);
		virtual void SetRenderContext(RenderContextType p_eRenderContextType, RenderContextPtr p_pRenderContext);
		virtual RenderContextPtr GetRenderContext(RenderContextType p_eRenderContextType);
		virtual void RestoreRenderContext(void);

		virtual void SetRenderState(IRenderState* p_pRenderState);
		virtual IRenderState* GetRenderState(IRenderState::RenderStateType p_eRenderStateType);

		virtual void PushRenderState(IRenderState::RenderStateType p_eRenderStateType);
		virtual void PopRenderState(IRenderState::RenderStateType p_eRenderStateType);

		virtual void SetAlphaRenderState(AlphaRenderState* p_pAlphaRenderState);
		virtual AlphaRenderState* GetAlphaRenderState(void);
		virtual void SetBiasRenderState(BiasRenderState* p_pBiasRenderState);
		virtual BiasRenderState* GetBiasRenderState(void);
		virtual void SetTargetRenderState(TargetRenderState* p_pTargetRenderState);
		virtual TargetRenderState* GetTargetRenderState(void);
		virtual void SetDepthRenderState(DepthRenderState* p_pDepthRenderState);
		virtual DepthRenderState* GetDepthRenderState(void);
		virtual void SetStencilRenderState(StencilRenderState* p_pStencilRenderState);
		virtual StencilRenderState* GetStencilRenderState(void);
		virtual void SetMaterialRenderState(MaterialRenderState* p_pMaterialRenderState);
		virtual MaterialRenderState* GetMaterialRenderState(void);
		virtual void SetSamplerRenderState(SamplerRenderState* p_pSamplerRenderState);
		virtual SamplerRenderState* GetSamplerRenderState(void);
		virtual void SetCullRenderState(CullRenderState* p_pCullRenderState);
		virtual CullRenderState* GetCullRenderState(void);
		virtual void SetLightRenderState(LightRenderState* p_pLightRenderState);
		virtual LightRenderState* GetLightRenderState(void);
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual void ClearDepthBuffer(float p_fDepthValue) = 0;
		virtual void ClearStencilBuffer(uint p_uiStencilValue) = 0;
		virtual void ClearColourBuffer(const Colour& p_colour) = 0;
		virtual void ClearBuffers(const Colour& p_colour, float p_fDepthValue, uint p_uiStencilValue) = 0;
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual void SetRenderTarget(const Meson::Common::Text::String& p_strName);
		virtual RenderTarget* GetRenderTarget(void);

		virtual void PushRenderTarget(void);
		virtual void PopRenderTarget(void);

		virtual RenderTargetEnumerator GetRenderTargetEnumeration(void);
		virtual void AttachRenderTarget(RenderTarget* p_pRenderTarget);
		virtual void DetachRenderTarget(const Meson::Common::Text::String& p_strRenderTargetName);
		virtual WindowRenderTarget* CreateWindowRenderTarget(const Meson::Common::Text::String& p_strName, 
			uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, 
			Meson::Common::Config::Configuration* p_pConfiguration) = 0;
		virtual TextureRenderTarget* CreateTextureRenderTarget(const Meson::Common::Text::String& p_strName, 
			TextureBuffer* p_pTextureBuffer, bool p_bCreateDepthStencilSurface) = 0;
		virtual MultipleTextureRenderTarget* CreateMultipleTextureRenderTarget(
			const Meson::Common::Text::String& p_strName) = 0;
		virtual void DestroyRenderTarget(const Meson::Common::Text::String& p_strName) = 0;
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual void SetProjectionMatrix(const Meson::Common::Maths::Matrix4f& p_projectionMatrix);
		virtual void SetWorldMatrix(const Meson::Common::Maths::Matrix4f& p_worldMatrix);
		virtual void SetViewMatrix(const Meson::Common::Maths::Matrix4f& p_viewMatrix);
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual void MakePerspectiveProjection(float p_fLeft, float p_fRight, float p_fTop, float p_fBottom, 
			float p_fNear, float p_fFar, Meson::Common::Maths::Matrix4f* p_pMatrixOut ) = 0;
		virtual void MakeOrthographicProjection(float p_fLeft, float p_fRight, float p_fTop, float p_fBottom, 
			float p_fNear, float p_fFar, Meson::Common::Maths::Matrix4f* p_pMatrixOut ) = 0;
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		virtual void SetViewport(Viewport* p_pViewport);
		virtual Viewport* GetViewport(void);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		virtual void SetSamplerState(uint p_uiSamplerIndex, const SamplerState* p_pSamplerState);
		virtual const SamplerState* GetSamplerState(uint p_uiSamplerIndex);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		virtual void SetTexture(uint p_uiTextureIndex, TextureBuffer* p_pTextureBuffer) = 0;
		//virtual TextureBuffer* GetTexture(uint p_uiTextureIndex) = 0;
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		virtual void SetLight(uint p_uiLightIndex, const Light* p_pLight);
		virtual const Light* GetLight(uint p_uiLightIndex) const;
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		virtual void BindShaderProgram(IShaderProgram* p_pShaderProgram);
		virtual void UnbindShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType);
		virtual void LoadShaderParameters(IShaderProgram* p_pShaderProgram);
		virtual IShaderProgram* GetShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType);
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual HardwareResourceManager* GetHardwareResourceManager(void);
		virtual ShaderProgramManager* GetShaderProgramManager(void);
		//----------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------
		RenderConstant* GetRenderConstant( RenderConstantType::RenderConstantType p_eRenderConstantType );
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual const Meson::Common::Maths::Matrix4f& GetProjectionMatrix(void);
		virtual const Meson::Common::Maths::Matrix4f& GetViewMatrix(void);
		virtual const Meson::Common::Maths::Matrix4f& GetWorldMatrix(void);
		//----------------------------------------------------------------------------------------------
		
		//----------------------------------------------------------------------------------------------
		virtual uint GetDeviceCapabilities(IRenderDevice::DeviceCapabilitiesI p_eDeviceCapabilities);
		virtual float GetDeviceCapabilities(IRenderDevice::DeviceCapabilitiesF p_eDeviceCapabilities);
		//----------------------------------------------------------------------------------------------

	public:
		Meson::Common::Config::Configuration m_configuration;
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, RenderTarget*> m_renderTargetMap;

		Meson::Vistas::ShaderProgramManager*	m_pShaderProgramManager;
		Meson::Vistas::HardwareResourceManager* m_pHardwareResourceManager;

		Meson::Vistas::Viewport*		m_pViewport;
		Meson::Vistas::Light*			m_pLights;
		Meson::Vistas::SamplerState*	m_pSamplerStates;
		Meson::Vistas::RenderTarget*	m_pRenderTarget;
		Meson::Vistas::IShaderProgram*	m_pShaderPrograms[IShaderProgram::SPT_COUNT];
		Meson::Vistas::RenderConstant*  m_pRenderConstants[RenderConstantType::RCT_COUNT];

		Meson::Vistas::RenderContextPtr m_pRenderContext,
										m_pGlobalRenderContext;

		Meson::Common::Collections::TStack<RenderStatePtr> m_renderStateStack[IRenderState::RST_COUNT];
		
		// TODO: RenderTarget is abstract - should its name be changed to IRenderTarget?
		Meson::Common::Collections::TStack<RenderTarget*> m_renderTargetStack;

		dword m_dwDeviceCapabilities[IRenderDevice::DCI_COUNT];
		float m_fDeviceCapabilities[IRenderDevice::DCF_COUNT];

		Meson::Common::Maths::Matrix4f	m_projectionMatrix, 
										m_worldMatrix, 
										m_viewMatrix;

		ColourMask m_eColourMask;
	};

Meson_Vistas_END
