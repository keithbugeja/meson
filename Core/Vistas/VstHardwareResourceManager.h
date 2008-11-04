//----------------------------------------------------------------------------------------------
//	VstHardwareResourceManager.h
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
#include "VstHardwareResourceFactory.h"
#include "VstVertexChannel.h"
#include "VstTextureBuffer.h"
#include "VstIndexBuffer.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	// \class HardwareResourceManager
	//----------------------------------------------------------------------------------------------
	class VISTAS_API HardwareResourceManager
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** On construction, the HardwareResourceManager is provided with a factory for the actual
			creation and destruction of hardware resource objects.

			\param p_pHardwareResourceFactory Pointer to an implementation of IHardwareResourceFactory
		 */
		//----------------------------------------------------------------------------------------------
		HardwareResourceManager(IHardwareResourceFactory* p_pHardwareResourceFactory);
		
		//----------------------------------------------------------------------------------------------
		/** Destructor
		 */
		//----------------------------------------------------------------------------------------------
		~HardwareResourceManager(void);

		//----------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------
		TextureBufferPtr CreateTextureBuffer(TextureBuffer::TextureFormat p_eTextureFormat, TextureBuffer::TextureType p_eTextureType, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage, bool p_bOwner=true);
		TextureBufferPtr CreateTextureBuffer2D(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiWidth, uint p_uiHeight, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage, bool p_bOwner=true);
		TextureBufferPtr CreateTextureBuffer3D(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage, bool p_bOwner=true);
		TextureBufferPtr CreateTextureBufferCube(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiEdgeLength, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage, bool p_bOwner=true);
		TextureBufferPtr CreateTextureBufferFromFile(const Meson::Common::Text::String& p_strFilename, TextureBuffer::TextureType p_eTextureType=TextureBuffer::TT_2D, bool p_bOwner=true);
		//----------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------
		void ReleaseTextureBuffer(TextureBufferPtr p_pTextureBuffer);
		void ReleaseAllTextureBuffers(void);
		
		//----------------------------------------------------------------------------------------------
		/** Creates an index buffer, and returns a pointer to it. 

			\param p_eIndexType		Type of index entities
			\param p_uiIndexCount	Number of indices in buffer
			\param p_dwPool			Memory pool flags
			\param p_dwUsage		Memory usage flags

			\returns A pointer to a managed index buffer object
		 */
		//----------------------------------------------------------------------------------------------
		IndexBufferPtr CreateIndexBuffer(IndexBuffer::IndexType p_eIndexType, uint p_uiIndexCount, dword p_dwPool, dword p_dwUsage, bool p_bOwner=true);

		//----------------------------------------------------------------------------------------------
		/** Forces the manager to relinquish ownership of the index buffer. When release is called,
		    the manager can no longer provide access to the index buffer. Moreover, once the object is
			not externally referenced anymore, it is automatically disposed of.

			\param p_pIndexBuffer Pointer to index buffer object to release.
		 */
		//----------------------------------------------------------------------------------------------
		void ReleaseIndexBuffer(IndexBufferPtr p_pIndexBuffer);
		void ReleaseAllIndexBuffers(void);

		//----------------------------------------------------------------------------------------------
		/** Creates a vertex buffer, and returns a pointer to it. 

			\param p_uiVertexSize	Size in bytes of each vertex entity
			\param p_uiVertexCount	Number of vertices in buffer
			\param p_dwPool			Memory pool flags
			\param p_dwUsage		Memory usage flags

			\returns A pointer to a managed vertex buffer object
		 */
		//----------------------------------------------------------------------------------------------
		VertexBufferPtr CreateVertexBuffer(uint p_uiVertexSize, uint p_uiVertexCount, dword p_dwPool, dword p_dwUsage, bool p_bOwner=true);

		//----------------------------------------------------------------------------------------------
		/** Forces the manager to relinquish ownership of the vertex buffer. When release is called,
		    the manager can no longer provide access to the vertex buffer. Moreover, once the object is
			not externally referenced anymore, it is automatically disposed of.
			
			\param p_pVertexBuffer Pointer to vertex buffer object to release.
		 */
		//----------------------------------------------------------------------------------------------
		void ReleaseVertexBuffer(VertexBufferPtr p_pVertexBuffer);
		void ReleaseAllVertexBuffers(void);

		//----------------------------------------------------------------------------------------------
		/** Creates a managed vertex declaration object.
		 */
		//----------------------------------------------------------------------------------------------
		VertexDeclarationPtr CreateVertexDeclaration(bool p_bOwner=true);

		//----------------------------------------------------------------------------------------------
		/** Forces the manager to relinquish ownership of the vertex declaration.
		 */
		//----------------------------------------------------------------------------------------------
		void ReleaseVertexDeclaration(VertexDeclarationPtr p_pVertexDeclaration);
		void ReleaseAllVertexDeclarations(void);

		//----------------------------------------------------------------------------------------------
		/** Creates a managed vertex to stream binding object.
		 */
		//----------------------------------------------------------------------------------------------
		VertexChannelPtr CreateVertexChannel(bool p_bOwner=true);

		//----------------------------------------------------------------------------------------------
		/** Forces the manager to relinquish ownership of the vertex to stream binding.
		 */
		//----------------------------------------------------------------------------------------------
		void ReleaseVertexChannel(VertexChannelPtr p_pVertexChannel);
		void ReleaseAllVertexChannels(void);
		
	protected:
		/** Vertex buffers, vertex declarations and vertex to stream bindings.
		 */
		Meson::Common::Collections::TArrayList<VertexDeclarationPtr> m_vertexDeclarationList;
		Meson::Common::Collections::TArrayList<VertexBufferPtr> m_vertexBufferList;
		Meson::Common::Collections::TArrayList<VertexChannelPtr> m_vertexChannelList;
		
		/** Index buffers.
		 */ 
		Meson::Common::Collections::TArrayList<IndexBufferPtr> m_indexBufferList;
		
		/** Texture buffers.
		 */ 
		Meson::Common::Collections::TArrayList<TextureBufferPtr> m_textureBufferList;
		
		/** Hardware resource factory
		 */
		IHardwareResourceFactory* m_pHardwareResourceFactory;
	};

Meson_Vistas_END