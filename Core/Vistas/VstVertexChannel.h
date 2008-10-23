//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "TreeMap.h"

#include "VstTypes.h"
#include "VstVertexBufferGroup.h"
#include "VstVertexDeclaration.h"

Meson_Vistas_BEGIN
	/**
	 */
	typedef Meson::Common::Collections::TKeyValuePair<uint,VertexBufferPtr> StreamVertexBufferPair;
	typedef Meson::Common::Collections::TEnumerator<StreamVertexBufferPair>& StreamVertexBufferPairEnumerator;

	class VertexChannel : public Meson::Common::TReferenceCounter<VertexChannel>
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** VertexChannel constructor
		 */
		//----------------------------------------------------------------------------------------------
		VertexChannel(void);
		~VertexChannel(void);
	
		//----------------------------------------------------------------------------------------------
		/** Sets the vertex declaration for this vertex channel.
			\param p_pVertexDeclaration Pointer to a VertexDeclaration object.
		 */ 
		//----------------------------------------------------------------------------------------------
		void SetVertexDeclaration(VertexDeclarationPtr p_pVertexDeclaration);

		//----------------------------------------------------------------------------------------------
		/** Returns the vertex declaration for this vertex channel.
			\returns Pointer to a VertexDeclaration object.
		 */ 
		//----------------------------------------------------------------------------------------------
		VertexDeclarationPtr GetVertexDeclaration(void);

		//----------------------------------------------------------------------------------------------
		/** Returns an enumerator over the stream-vertex buffer bindings represented
			by this channel.
			\returns An enumerated pair of stream:vertexbuffer elements.
		 */
		//----------------------------------------------------------------------------------------------
		StreamVertexBufferPairEnumerator GetStreamVertexBufferPairEnumerator(void);

		//----------------------------------------------------------------------------------------------
		/** Returns the stream number at the supplied index.
			\param p_uiIndex Index at which to return stream number.
		 */ 
		//----------------------------------------------------------------------------------------------
		uint GetStream(uint p_uiIndex);
		
		//----------------------------------------------------------------------------------------------
		/** Returns the vertex buffer at the supplied index.
			\param p_uiIndex Index at which to return vertex buffer.
		 */
		//----------------------------------------------------------------------------------------------
		VertexBufferPtr GetVertexBuffer(uint p_uiIndex);

		//----------------------------------------------------------------------------------------------
		/** Returns the vertex buffer group containing all vertex buffers bound
			to a stream within the channel.
			\returns Vertex buffer group
		 */
		//----------------------------------------------------------------------------------------------
		VertexBufferGroupPtr GetVertexBufferGroup(void);

		//----------------------------------------------------------------------------------------------
		/** Returns the vertex buffer bound to the given stream.
			\returns VertexBuffer object
		 */
		//----------------------------------------------------------------------------------------------
		VertexBufferPtr GetStreamBinding(uint p_uiStream);

		//----------------------------------------------------------------------------------------------
		/** Binds a vertex buffer to a given stream.
			\param p_uiStream Stream number to bind with vertex buffer.
			\param p_pVertexBuffer Pointer to a vertex buffer to bind with stream.
		 */
		//----------------------------------------------------------------------------------------------
		void SetStreamBinding(uint p_uiStream, VertexBufferPtr p_pVertexBuffer );

		//----------------------------------------------------------------------------------------------
		/** Unbinds a single stream from the respective vertex buffer.
			\param p_uiStream Stream number to loose from its binding.
		 */
		//----------------------------------------------------------------------------------------------
		void ReleaseStream(uint p_uiStream);

		//----------------------------------------------------------------------------------------------
		/** Clears all the stream bindings.
		 */
		//----------------------------------------------------------------------------------------------
		void ReleaseAllStreams(void);

	protected:
		/** Channel to vertex buffer binding
		 */
		Meson::Common::Collections::TTreeMap<uint, VertexBufferPtr> m_streamVertexBufferMap;
		Meson::Common::Collections::TArrayList<uint> m_streamIndexList;
		
		/** Vertex declaration
		 */
		VertexDeclarationPtr m_pVertexDeclaration;
		VertexBufferGroupPtr m_pVertexBufferGroup;
	};

	typedef Meson::Common::TPointer<VertexChannel> VertexChannelPtr;

Meson_Vistas_END