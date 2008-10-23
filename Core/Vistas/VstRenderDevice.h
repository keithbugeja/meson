//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::IRenderDevice
//	Interface class for rendering APIs.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Plane.h"
#include "String8.h"
#include "Matrix4.h"
#include "HashMap.h"
#include "Enumerator.h"
#include "Configuration.h"

#include "VstTypes.h"
#include "VstColour.h"
#include "VstLight.h"
#include "VstCamera.h"
#include "VstViewport.h"
#include "VstSamplerState.h"

#include "VstRenderTypes.h"
#include "VstRenderContext.h"
#include "VstRenderState.h"
#include "VstAlphaRenderState.h"
#include "VstBiasRenderState.h"
#include "VstCullRenderState.h"
#include "VstDepthRenderState.h"
#include "VstLightRenderState.h"
#include "VstMaterialRenderState.h"
#include "VstSamplerRenderState.h"
#include "VstStencilRenderState.h"
#include "VstTargetRenderState.h"
#include "VstWindowRenderTarget.h"
#include "VstTextureRenderTarget.h"
#include "VstShaderProgramManager.h"
#include "VstHardwareResourceManager.h"
#include "VstMultipleTextureRenderTarget.h"

Meson_Vistas_BEGIN

	typedef Meson::Common::Collections::TEnumerator<RenderTarget*>& RenderTargetEnumerator;

	class IRenderDevice
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** The following enumeration represents the list of primitives supported by the atomic
			render oprations DrawPrimitive and DrawIndexedPrimitive.
		 */ 
		//----------------------------------------------------------------------------------------------
		enum PrimitiveType
		{
			PT_POINTLIST,
			PT_LINELIST_SEGMENTS,
			PT_LINELIST_OPEN,
			PT_LINELIST_CLOSED,
			PT_TRIANGLE_LIST,
			PT_TRIANGLE_STRIP,
			PT_TRIANGLE_FAN,

			PT_COUNT
		};

		enum RenderContextType
		{
			RTT_GLOBAL_CONTEXT,
			RTT_ACTIVE_CONTEXT,

			RTT_COUNT
		};

		//----------------------------------------------------------------------------------------------
		/** The colour mask enumeration is used in specifying the active channels of the colour
			buffer. For convenience the enumeration specifies all the colour channel combinations,
			but might as well be represented by intersection of the primary components.
		 */
		//----------------------------------------------------------------------------------------------
		enum ColourMask
		{
			CM_RED						= 1,
			CM_GREEN					= 2,
			CM_BLUE						= 4,
			CM_ALPHA					= 8,
			CM_DISABLED					= 0,

			CM_RED_GREEN_BLUE_ALPHA		= CM_RED | CM_GREEN | CM_BLUE | CM_ALPHA,

			CM_RED_GREEN_ALPHA			= CM_RED | CM_GREEN | CM_ALPHA,
			CM_RED_BLUE_ALPHA			= CM_RED | CM_BLUE | CM_ALPHA,
			CM_GREEN_BLUE_ALPHA			= CM_GREEN | CM_BLUE | CM_ALPHA,

			CM_RED_ALPHA				= CM_RED | CM_ALPHA,
			CM_GREEN_ALPHA				= CM_GREEN | CM_ALPHA,
			CM_BLUE_ALPHA				= CM_BLUE | CM_ALPHA,

			CM_RED_GREEN_BLUE			= CM_RED | CM_GREEN | CM_BLUE,

			CM_RED_GREEN				= CM_RED | CM_GREEN,
			CM_GREEN_BLUE				= CM_GREEN | CM_BLUE,
			CM_RED_BLUE					= CM_RED | CM_BLUE,
			
			CM_COUNT
		};

		//----------------------------------------------------------------------------------------------
		/** DeviceCapabilitiesI enumerates the list of device capabilites that may be queried and 
			return an integer result.
		  */
		//----------------------------------------------------------------------------------------------
		enum DeviceCapabilitiesI
		{
			DCI_MAXIMUM_LIGHTS,
			DCI_MAXIMUM_ANISOTROPY,
			DCI_MAXIMUM_SIMULATANEOUS_TEXTURES,
			DCI_MAXIMUM_STREAMS,
			DCI_MAXIMUM_USER_CLIPPLANES,

			DCI_COUNT
		};

		//----------------------------------------------------------------------------------------------
		/** DeviceCapabilitiesF enumerates the list of device capabilites that may be queried and 
			return a float result.
		  */
		//----------------------------------------------------------------------------------------------
		enum DeviceCapabilitiesF
		{
			DCF_MAXIMUM_POINTSIZE,

			DCF_COUNT
		};

	public:
		//----------------------------------------------------------------------------------------------
		/** Returns the name of the IRenderDevice implementation.
		 */ 
		//----------------------------------------------------------------------------------------------
		virtual const Meson::Common::Text::String& GetName(void) const = 0;

		//----------------------------------------------------------------------------------------------
		/** Starts up the device by performing implementation specific initialisation. Prior to being
			started, the render device supports only configuration-related operations. The initialisation
			sequence creates and returns a window render target which serves as the default canvas for
			all drawing operations.

			\return A WindowRenderTarget* to a newly initialised primary render target
		 */
		//----------------------------------------------------------------------------------------------
		virtual WindowRenderTarget* Startup(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Initiates  render device shutdown process, which forces the device to close
			all owned resources, making it inoperable until it is initialised again.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void Shutdown(void) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Retrieves configuration settings for the render device.

			\return Configuration* to the render device's internal configuraiton object
		 */
		//----------------------------------------------------------------------------------------------
		virtual Meson::Common::Config::Configuration* GetConfiguration(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Marks the beginning of the scene drawing process. All primitive drawing operations
			should follow <b>BeginScene</b> and precede <b>EndScene</b>.
		 */
		//----------------------------------------------------------------------------------------------
		virtual bool BeginScene(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Marks the end of the scene drawing process. All primitive drawing operations should
			lie within a <b>BeginScene</b>/<b>EndScene</b> block.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void EndScene(void) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Clears the depth buffer associated with the active render target.

			\param p_fDepthValue	Value to clear depth buffer with; usually set to 1.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void ClearDepthBuffer(float p_fDepthValue) = 0;

		//----------------------------------------------------------------------------------------------
		/** Clears the stencil buffer associated with the active render target.

			\param p_uiStencilValue	Value to clear stencil buffer with.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void ClearStencilBuffer(uint p_uiStencilValue) = 0;

		//----------------------------------------------------------------------------------------------
		/** Clears the colour buffer associated with the active render target, setting it to the
			specified colour value.

			\param p_colour	Colour to clear buffer with.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void ClearColourBuffer(const Colour& p_colour) = 0;

		//----------------------------------------------------------------------------------------------
		/**	Clears the colour, depth and stencil buffers. Note that the operation will fail if the 
			current render target does not define a depth or a stencil buffer.

			\param p_colour Colour value used to clear colour buffer.
			\param p_fDepthValue Depth value used to clear Z buffer.
			\param p_uiStencilValue Stencil value used to clear stencil buffer; note that this value
									is masked by the size of the stencil buffer, which is usually 
									8 bits wide.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void ClearBuffers(const Colour& p_colour, float p_fDepthValue, uint p_uiStencilValue) = 0;

		//----------------------------------------------------------------------------------------------
		/**	Enables or disables the individual channels of the colour buffer written to during 
			rendering.

			\param p_eColourMask	
		*/
		//----------------------------------------------------------------------------------------------
		virtual void SetColourMask(ColourMask p_eColourMask) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Returns a mask specifying which colour channels are enabled and which are not.

			\return Colour mask.
		 */
		//----------------------------------------------------------------------------------------------
		virtual ColourMask GetColourMask(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Draws a non-indexed batch of primitives.
			
			\param p_ePrimitiveType Determines the kind of primitives drawn. The value is an
									enumerand of PrimitiveType.
			\param p_uiStartVertex	Specifies the starting vertex number in the associated 
									vertex buffer at which drawing is to occur.
			\param p_uiCount		Determines the number of primitives output by the operation.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void DrawPrimitive(PrimitiveType p_ePrimitiveType, uint p_uiStartVertex, uint p_uiCount) = 0;
	
		//----------------------------------------------------------------------------------------------
		/** Draws an indexed batch of primitives.
			
			\param p_ePrimitiveType		Determines the kind of primitives drawn. The value is an
										enumerand of PrimitiveType.
			\param p_uiStartVertex		Specifies the vertex number within the buffer associated 
										with index zero.
			\param p_uiStartIndex		Specifies the index to the first element within the index
										buffer - literally an index to the first index.
			\param p_uiIndexLowerBound	An optimisation parameter which hints at the vertex with 
										the smallest index.
			\param p_uiIndexUpperBound	An optimisation parameter which hints at the vertex with 
										the greatest index.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void DrawIndexedPrimitive(PrimitiveType p_ePrimitiveType, uint p_uiStartVertex, uint p_uiStartIndex, 
			uint p_uiIndexLowerBound, uint p_uiIndexUpperBound, uint p_uiPrimitiveCount ) = 0;

		//----------------------------------------------------------------------------------------------
		/** Binds a vertex buffer to a stream unit. The vertex buffer bound to the given stream is 
			described by the loaded vertex declaration object.

			\param p_uiStream		Stream number, starting from zero.
			\param p_pVertexBuffer	Pointer to a vertex buffer object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetStreamSource(uint p_uiStream, VertexBuffer* p_pVertexBuffer) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Binds a vertex declaration to the current render context. The vertex declaration 
			is used to describe the content of vertex buffers bound to the stream units.

			\param p_pVertexDeclaration	Pointer to a vertex declaration object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetVertexDeclaration(VertexDeclaration* p_pVertexDeclaration) = 0;

		//----------------------------------------------------------------------------------------------
		/** Binds a vertex channel to the current render context. The vertex channel describes
			the binding of vertex buffers to stream units and their respective vertex declaration
			structure.

			\param p_pVertexChannel	Pointer to a vertex channel object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetVertexChannel(VertexChannel* p_pVertexChannel) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Binds an index buffer to the current render context.

			\param p_pIndexBuffer	Pointer to an index buffer object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetIndexBuffer(IndexBuffer* p_pIndexBuffer) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Sets either the active or the global device render context. 

			\param p_eRenderContextType Determines whether to set active or global context.
			\param p_pRenderContext		RenderContext object to use.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetRenderContext(RenderContextType p_eRenderContextType, RenderContextPtr p_pRenderContext) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Retrieve the device render context.

			\param p_eRenderContextType Specifies whether to retrieve active or global render context.
		 */
		//----------------------------------------------------------------------------------------------
		virtual RenderContextPtr GetRenderContext(RenderContextType p_eRenderContextType) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Restores the global render context, replacing the currently active context.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void RestoreRenderContext(void) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Pushes the given render state type on the stack. If the specified render state is not 
			valid, the push does not occur.

			\param The render state type to push onto the stack.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void PushRenderState(IRenderState::RenderStateType p_eRenderStateType) = 0;

		//----------------------------------------------------------------------------------------------
		/** Pops the given render state type off the stack and sets it as current. If no items exist
			on the stack, the function exits.

			\param The render state type to pop from stack.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void PopRenderState(IRenderState::RenderStateType p_eRenderStateType) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Sets a generic renderstate for the current context.
			
			\param IRenderState* p_pRenderState
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetRenderState(IRenderState* p_pRenderState) = 0;

		//----------------------------------------------------------------------------------------------
		/** Returns the requested renderstate.
			
			\param p_eRenderStateType Required renderstate.
		 */
		//----------------------------------------------------------------------------------------------
		virtual IRenderState* GetRenderState(IRenderState::RenderStateType p_eRenderStateType) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Sets the alpha render state object for the current render context.

			\param p_pAlphaRenderState	Pointer to an alpha render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetAlphaRenderState(AlphaRenderState* p_pAlphaRenderState) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the alpha render state object from the current render context.

			\returns Pointer to the current context's alpha render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual AlphaRenderState* GetAlphaRenderState(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Sets the bias render state object for the current render context.

			\param p_pBiasRenderState	Pointer to a bias render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetBiasRenderState(BiasRenderState* p_pBiasRenderState) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the bias render state object from the current render context.

			\returns Pointer to the current context's bias render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual BiasRenderState* GetBiasRenderState(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Sets the target render state object for the current render context.

			\param p_pTargetRenderState	Pointer to a target render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetTargetRenderState(TargetRenderState* p_pTargetRenderState) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the target render state object from the current render context.

			\returns Pointer to the current context's target render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual TargetRenderState* GetTargetRenderState(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the depth render state object from the current render context.

			\returns Pointer to the current context's depth render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetDepthRenderState(DepthRenderState* p_pDepthRenderState) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the depth render state object from the current render context.

			\returns Pointer to the current context's depth render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual DepthRenderState* GetDepthRenderState(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the stencil render state object from the current render context.

			\returns Pointer to the current context's stencil render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetStencilRenderState(StencilRenderState* p_pStencilRenderState) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the stencil render state object from the current render context.

			\returns Pointer to the current context's stencil render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual StencilRenderState* GetStencilRenderState(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the material render state object from the current render context.

			\returns Pointer to the current context's material render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetMaterialRenderState(MaterialRenderState* p_pMaterialRenderState) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the material render state object from the current render context.

			\returns Pointer to the current context's material render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual MaterialRenderState* GetMaterialRenderState(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the sampler render state object from the current render context.

			\returns Pointer to the current context's sampler render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetSamplerRenderState(SamplerRenderState* p_pSamplerRenderState) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the sampler render state object from the current render context.

			\returns Pointer to the current context's sampler render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual SamplerRenderState* GetSamplerRenderState(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the cull render state object from the current render context.

			\returns Pointer to the current context's cull render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetCullRenderState(CullRenderState* p_pCullRenderState) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the cull render state object from the current render context.

			\returns Pointer to the current context's cull render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual CullRenderState* GetCullRenderState(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the light render state object from the current render context.

			\returns Pointer to the current context's light render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetLightRenderState(LightRenderState* p_pLightRenderState) = 0;

		//----------------------------------------------------------------------------------------------
		/** Gets the light render state object from the current render context.

			\returns Pointer to the current context's light render state object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual LightRenderState* GetLightRenderState(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Returns an enumeration of the render targets attached to the device.

			\return Enumerator of render target objects.
		 */
		//----------------------------------------------------------------------------------------------
		virtual RenderTargetEnumerator GetRenderTargetEnumeration(void) = 0;

		//----------------------------------------------------------------------------------------------
		/** Attaches a new render target object to the device. Note that the operation will fail
			if a render target with the same name has already been attached to the device.

			\param p_pRenderTarget Pointer to a render target object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void AttachRenderTarget(RenderTarget* p_pRenderTarget) = 0;

		//----------------------------------------------------------------------------------------------
		/** Detaches a render target object from the device. The operation will fail if the render
			target specified is not attached to the device.

			\param p_pRenderTarget Pointer to a render target object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void DetachRenderTarget(const Meson::Common::Text::String& p_strRenderTargetName) = 0;

		//----------------------------------------------------------------------------------------------
		/** Creates a new window render target object, and automatically attaches the new render 
			target to the device. Additional configuration options specific to the underlying 
			implementation may be specified by the configuration object.

			\param p_strName		Name of new render target.
			\param p_uiWidth		Width
			\param p_uiHeight		Height
			\param p_bFullScreen	Flag specifying whether the render target should be opened as a
									window on the desktop or in full-screen mode.
			\param p_pConfiguration	Additional configuration options.

			\return	Pointer to the newly created window render target.
		 */
		//----------------------------------------------------------------------------------------------
		virtual WindowRenderTarget* CreateWindowRenderTarget(const Meson::Common::Text::String& p_strName, 
			uint p_uiWidth, uint p_uiHeight, bool p_bFullScreen, 
			Meson::Common::Config::Configuration* p_pConfiguration) = 0;

		//----------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------
		virtual TextureRenderTarget* CreateTextureRenderTarget(const Meson::Common::Text::String& p_strName, 
			TextureBuffer* p_pTextureBuffer, bool p_bCreateDepthStencilSurface) = 0;

		//----------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------
		virtual MultipleTextureRenderTarget* CreateMultipleTextureRenderTarget(
			const Meson::Common::Text::String& p_strName) = 0;

		//----------------------------------------------------------------------------------------------
		/** Releases the resources held by the specified render target, detaching it from the 
			render device. The function fails if the render device does not exist.

			\param p_strName	Name of render target object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void DestroyRenderTarget(const Meson::Common::Text::String& p_strName) = 0;

		//----------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------
		virtual void SetRenderTarget(const Meson::Common::Text::String& p_strName) = 0;
		virtual RenderTarget* GetRenderTarget(void) = 0;

		//----------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------
		virtual void PushRenderTarget(void) = 0;
		virtual void PopRenderTarget(void) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Sets the projection matrix for the current render context.

			\param p_projectionMatrix	A 4x4 matrix containing the projection information. 
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetProjectionMatrix(const Meson::Common::Maths::Matrix4f& p_projectionMatrix) = 0;

		//----------------------------------------------------------------------------------------------
		/**	Sets the view matrix for the current render context.

			\param p_viewMatrix	A 4x4 matrix containing the current view transform.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetViewMatrix(const Meson::Common::Maths::Matrix4f& p_viewMatrix) = 0;

		//----------------------------------------------------------------------------------------------
		/** Sets the world matrix for the current render context.

			\param p_worldMatrix	A 4x4 matrix containing the world transform.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetWorldMatrix(const Meson::Common::Maths::Matrix4f& p_worldMatrix) = 0;

		//----------------------------------------------------------------------------------------------
		/** Creates a perspective projection matrix.

			\param p_fLeft		Frustum left	(-x)
			\param p_fRight		Frustum right	(+x)
			\param p_fTop		Frustum top		(+y)
			\param p_fBottom	Frustum bottom	(-y)
			\param p_fNear		Frustum near	(z near)
			\param p_fFar		Frustum far		(z far)
			\param p_pMatrixOut Pointer to a matrix to transform into a perspective projection.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void MakePerspectiveProjection(float p_fLeft, float p_fRight, float p_fTop, float p_fBottom, 
			float p_fNear, float p_fFar, Meson::Common::Maths::Matrix4f* p_pMatrixOut ) = 0;

		//----------------------------------------------------------------------------------------------
		/** Creates an orthographic projection matrix.

			\param p_fLeft		Frustum left	(-x)
			\param p_fRight		Frustum right	(+x)
			\param p_fTop		Frustum top		(+y)
			\param p_fBottom	Frustum bottom	(-y)
			\param p_fNear		Frustum near	(z near)
			\param p_fFar		Frustum far		(z far)
			\param p_pMatrixOut Pointer to a matrix to transform into an orthographic projection.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void MakeOrthographicProjection( float p_fLeft, float p_fRight, float p_fTop, float p_fBottom, 
			float p_fNear, float p_fFar, Meson::Common::Maths::Matrix4f* p_pMatrixOut ) = 0;

		//----------------------------------------------------------------------------------------------
		/** Sets the active viewport to the one specified.

			\param p_pViewport Pointer to a Viewport object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetViewport(Viewport* p_pViewport) = 0;

		//----------------------------------------------------------------------------------------------
		/** Returns the currently active viewport.

			\return Pointer to active Viewport object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual Viewport* GetViewport(void) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Sets the sampler state information for the sampler unit specified by the sampler index.

			\param p_uiSamplerIndex Sampler index.
			\param p_pSamplerState Pointer to a SamplerState object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetSamplerState(uint p_uiSamplerIndex, const SamplerState* p_pSamplerState) = 0;

		//----------------------------------------------------------------------------------------------
		/** Sets the texture data for the texture sampler specified by the index.

			\param p_uiTextureIndex Texture sampler index.
			\param p_pTextureBuffer Pointer to a TextureBuffer object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetTexture(uint p_uiTextureIndex, TextureBuffer* p_pTextureBuffer) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Sets the properties of the hardware light at the given index.

			\param p_uiLightIndex Index of hardware light.
			\param p_pLight Pointer to a Light object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void SetLight(uint p_uiLightIndex, const Light* p_pLight) = 0;

		//----------------------------------------------------------------------------------------------
		/** Binds a shader program to the respective unit. Once bound, a program will execute 
			until unbound or replaced by another program of the same type. Shader program variables
			such as renderer constants or user defined parameters need to be loaded using the
			<i>LoadShaderParameters</i> function. For every specific program type, only one program
			at a time may be bound.

			\param p_pShaderProgram	Pointer to a successfully compiled shader program.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void BindShaderProgram(IShaderProgram* p_pShaderProgram) = 0;

		//----------------------------------------------------------------------------------------------
		/** Releases the shader program binding associated with the specific program type.

			\param p_shaderProgramType Specifies the program type to unbind.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void UnbindShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType) = 0;

		//----------------------------------------------------------------------------------------------
		/** Loads shader parameters such as user variables, renderer constants, etc.
			<b>UPDATE SECTION</b>
		 */
		//----------------------------------------------------------------------------------------------
		virtual void LoadShaderParameters(IShaderProgram* p_pShaderProgram) = 0;

		//----------------------------------------------------------------------------------------------
		/** Returns the shader program currently bound to the specified program type unit.
			
			\param p_shaderProgramType Type of requested active program.
		 */
		//----------------------------------------------------------------------------------------------
		virtual IShaderProgram* GetShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType) = 0;

		//----------------------------------------------------------------------------------------------
		/** Returns the HardwareResourceManager instance bound to this render device. Note that it is
			the responsibility of the implementation to instantiate the HardwareResourceManager and bind 
			it to the appropriate IHardwareResourceFactory implementation.

			\return Pointer to a HardwareResourceManager or NULL if it has not yet been instantiated.
		 */
		//----------------------------------------------------------------------------------------------
		virtual HardwareResourceManager* GetHardwareResourceManager( void ) = 0;

		//----------------------------------------------------------------------------------------------
		/** Returns the ShaderProgramManager instance bound to this render device. ShaderProgramManager
			initialisation should be carried out by the class implementing IRenderDevice.

			\return Pointer to a ShaderProgramManager or NULL if it has not yet been instantiated.
		 */
		//----------------------------------------------------------------------------------------------
		virtual ShaderProgramManager* GetShaderProgramManager(void) = 0;

		//----------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------
		virtual uint GetDeviceCapabilities(IRenderDevice::DeviceCapabilitiesI p_eDeviceCapabilities) = 0;
		virtual float GetDeviceCapabilities(IRenderDevice::DeviceCapabilitiesF p_eDeviceCapabilities) = 0;
	};

Meson_Vistas_END
