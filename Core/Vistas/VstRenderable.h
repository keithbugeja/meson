//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstRenderDevice.h"
#include "VstVertexChannel.h"
#include "VstIndexBuffer.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	/** Renderable objects are almost always manipulated through derived classes. Factoring out
		derived class information, renderable objects contain enough information for the 
		renderer to draw geometry primitives.
	 */
	class VISTAS_API Renderable : public VistasObject
	{
		VISTAS_IMPLEMENT_RTTI(Renderable, VistasObject);

	public:
		/** Constructor for renderable object. Renderable is initialised to indexed type, with 
			vertex channel and index buffer set to NULL. The primitive type is set to triangle
			list.
		 */
		Renderable(bool p_bAutogenerateId = true);
		Renderable(const Meson::Common::Text::String& p_strId);
		Renderable(const Renderable& p_renderable);

		virtual ~Renderable(void);

		bool IsIndexed(void) const;

		VertexChannelPtr GetVertexChannel(void);
		void SetVertexChannel(VertexChannelPtr p_pVertexChannel);

		VertexDeclarationPtr GetVertexDeclaration(void);
		void SetVertexDeclaration(VertexDeclarationPtr p_pVertexDeclaration);

		VertexBufferPtr	GetVertexBuffer(uint p_uiStream);
		void SetVertexBuffer(uint p_uiStream, VertexBufferPtr p_pVertexBuffer);

		IndexBufferPtr GetIndexBuffer(void);
		void SetIndexBuffer(IndexBufferPtr p_pIndexBuffer);

		IRenderDevice::PrimitiveType GetPrimitiveType(void) const;

		virtual uint GetIndexCount(void) const;
		virtual uint GetVertexCount(void) const;

		virtual void Draw(IRenderDevice* p_pRenderDevice);

		Renderable& operator=(const Renderable& p_renderable);

		static uint GetPrimitiveCount(IRenderDevice::PrimitiveType p_ePrimitiveType, uint p_uiElementCount);

	protected:
		bool m_bIsIndexed;

		IRenderDevice::PrimitiveType m_ePrimitiveType;
		VertexChannelPtr m_pVertexChannel;
		IndexBufferPtr m_pIndexBuffer;
	};

	typedef Meson::Common::TPointer<Renderable> RenderablePtr;
	typedef Meson::Common::Collections::TArrayList<RenderablePtr> RenderableList;
	typedef Meson::Common::TPointer<RenderableList> RenderableListPtr;
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END