//----------------------------------------------------------------------------------------------
//	VstHardwareResourceFactory.h
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Singleton.h"

#include "VstTypes.h"
#include "VstHardwareResource.h"
#include "VstVertexDeclaration.h"
#include "VstTextureBuffer.h"
#include "VstIndexBuffer.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	// /class IHardwareResourceFactory
	// Interface class for all hardware resource abstract factories. Hardware resource factories 
	// provide a way of allocating resources which are usually allocated in video memory or through 
	// special management by underlying APIs.
	//----------------------------------------------------------------------------------------------
	class IHardwareResourceFactory
	{
	public:
		//----------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------
		virtual TextureBuffer* CreateTextureBuffer(TextureBuffer::TextureFormat p_eTextureFormat, TextureBuffer::TextureType p_eTextureType, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage) = 0;
		virtual TextureBuffer* CreateTextureBuffer2D(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiWidth, uint p_uiHeight, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage) = 0;
		virtual TextureBuffer* CreateTextureBuffer3D(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage) = 0;
		virtual TextureBuffer* CreateTextureBufferCube(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiEdgeLength, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage) = 0;
		virtual TextureBuffer* CreateTextureBufferFromFile(const Meson::Common::Text::String& p_strFilename, TextureBuffer::TextureType p_eTextureType) = 0;
		//----------------------------------------------------------------------------------------------
		/** Destroys a texture buffer object.
			
			\param p_pTextureBuffer	Pointer to a texture buffer to dispose of
		 */
		//----------------------------------------------------------------------------------------------
		virtual void DestroyTextureBuffer(TextureBuffer* p_pTextureBuffer) = 0;

		//----------------------------------------------------------------------------------------------
		/** Creates an index buffer - a buffer for storing vertex indices into vertex buffers.

			\param p_eIndexType		Specifies the type of indices contained in this buffer, 
									whether 16 or 32 bit in size
			\param p_uiIndexCount	Number of indices contained in buffer
			\param p_dwPool			Memory pool allocation flags
			\param p_dwUsage		Memory pool usage

			\return An IndexBuffer* to a specific index buffer implementation
		 */
		//----------------------------------------------------------------------------------------------
		virtual IndexBuffer* CreateIndexBuffer(IndexBuffer::IndexType p_eIndexType, uint p_uiIndexCount, dword p_dwPool, dword p_dwUsage) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Destroys an index buffer object.
			
			\param p_pIndexBuffer	Pointer to an index buffer object
		 */
		//----------------------------------------------------------------------------------------------
		virtual void DestroyIndexBuffer(IndexBuffer* p_pIndexBuffer) = 0;

		//----------------------------------------------------------------------------------------------
		/** Creates a vertex buffer - a buffer for storing a sequence of vertices.

			\param p_uiVertexSize	Specifies the size of each vertex element
			\param p_uiVertexCount	Specifies the number of vertices in the buffer
			\param p_dwPool			Memory pool allocation flags
			\param p_dwUsage		Memory pool usage

			\return A VertexBuffer* to a specific vertex buffer implementation
		 */
		//----------------------------------------------------------------------------------------------
		virtual VertexBuffer* CreateVertexBuffer(uint p_uiVertexSize, uint p_uiVertexCount, dword p_dwPool, dword p_dwUsage) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Destroys a vertex buffer object.
			
			\param p_pVertexBuffer	Pointer to a vertex buffer object.
		 */
		//----------------------------------------------------------------------------------------------
		virtual void DestroyVertexBuffer(VertexBuffer* p_pVertexBuffer) = 0;

		//----------------------------------------------------------------------------------------------
		/** Creates a vertex declaration object. 

			\remark	Vertex declarations are not strictly stored in VRAM. Perhaps
					I should reconsider its position here.

			\return A VertexDeclaration* to a specific vertex declaration implementation
		 */
		//----------------------------------------------------------------------------------------------
		virtual VertexDeclaration* CreateVertexDeclaration(void) = 0;
		
		//----------------------------------------------------------------------------------------------
		/** Destroys a vertex declaration object.

			\param p_pVertexDeclaration Pointer to a vertex declaration object
		 */
		//----------------------------------------------------------------------------------------------
		virtual void DestroyVertexDeclaration(VertexDeclaration* p_pVertexDeclaration) = 0;
	};

Meson_Vistas_END