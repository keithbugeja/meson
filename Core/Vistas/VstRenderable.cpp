//----------------------------------------------------------------------------------------------
#include "VstRenderable.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
Renderable::Renderable(bool p_bAutogenerateId)
	: VistasObject(p_bAutogenerateId)
	, m_ePrimitiveType(IRenderDevice::PT_TRIANGLE_LIST)
	, m_pVertexChannel(NULL)
	, m_pIndexBuffer(NULL)
	, m_bIsIndexed(true)
{
}
//----------------------------------------------------------------------------------------------
Renderable::Renderable(const Meson::Common::Text::String& p_strId)
	: VistasObject(p_strId)
	, m_ePrimitiveType(IRenderDevice::PT_TRIANGLE_LIST)
	, m_pVertexChannel(NULL)
	, m_pIndexBuffer(NULL)
	, m_bIsIndexed(true)
{
}
//----------------------------------------------------------------------------------------------
Renderable::Renderable(const Renderable &p_renderable)
	: VistasObject((VistasObject&)p_renderable) 
	, m_ePrimitiveType(p_renderable.m_ePrimitiveType)
	, m_pVertexChannel(p_renderable.m_pVertexChannel)
	, m_pIndexBuffer(p_renderable.m_pIndexBuffer)
	, m_bIsIndexed(p_renderable.m_bIsIndexed)
{
}
//----------------------------------------------------------------------------------------------
Renderable::~Renderable(void)
{
	m_pIndexBuffer = NULL;
	m_pVertexChannel = NULL;
}
//----------------------------------------------------------------------------------------------
bool Renderable::IsIndexed(void) const
{
	return m_bIsIndexed;
}
//----------------------------------------------------------------------------------------------
VertexChannelPtr Renderable::GetVertexChannel(void)
{
	return m_pVertexChannel;
}
//----------------------------------------------------------------------------------------------
void Renderable::SetVertexChannel(VertexChannelPtr p_pVertexChannel)
{
	MESON_ASSERT(p_pVertexChannel != NULL, "Renderable vertex channel is being set to NULL.");
	m_pVertexChannel = p_pVertexChannel;
}
//----------------------------------------------------------------------------------------------
VertexDeclarationPtr Renderable::GetVertexDeclaration(void)
{
	MESON_ASSERT(m_pVertexChannel != NULL, "Vertex channel is NULL : Cannot return vertex declaration.");
	return m_pVertexChannel->GetVertexDeclaration();
}
//----------------------------------------------------------------------------------------------
void Renderable::SetVertexDeclaration(VertexDeclarationPtr p_pVertexDeclaration)
{
	MESON_ASSERT(m_pVertexChannel != NULL, "Vertex channel is NULL : Cannot set vertex declaration.");
	m_pVertexChannel->SetVertexDeclaration(p_pVertexDeclaration);
}
//----------------------------------------------------------------------------------------------
VertexBufferPtr	Renderable::GetVertexBuffer(uint p_uiStream)
{
	MESON_ASSERT(m_pVertexChannel != NULL, "Vertex channel is NULL : Cannot return vertex buffer.");
	return m_pVertexChannel->GetStreamBinding(p_uiStream);
}
//----------------------------------------------------------------------------------------------
void Renderable::SetVertexBuffer(uint p_uiStream, VertexBufferPtr p_pVertexBuffer)
{
	MESON_ASSERT(m_pVertexChannel != NULL, "Vertex channel is NULL : Cannot set vertex buffer.");
	m_pVertexChannel->SetStreamBinding(p_uiStream, p_pVertexBuffer);
}
//----------------------------------------------------------------------------------------------
IndexBufferPtr Renderable::GetIndexBuffer(void)
{
	return m_pIndexBuffer;
}
//----------------------------------------------------------------------------------------------
void Renderable::SetIndexBuffer(IndexBufferPtr p_pIndexBuffer)
{
	m_pIndexBuffer = p_pIndexBuffer;
}
//----------------------------------------------------------------------------------------------
IRenderDevice::PrimitiveType Renderable::GetPrimitiveType(void) const
{
	return m_ePrimitiveType;
}
//----------------------------------------------------------------------------------------------
uint Renderable::GetIndexCount(void) const
{
	if (m_pIndexBuffer != NULL)
		return m_pIndexBuffer->GetIndexCount();

	return 0;
}
//----------------------------------------------------------------------------------------------
uint Renderable::GetVertexCount(void) const
{
	if (m_pVertexChannel != NULL)
		return m_pVertexChannel->GetVertexBufferGroup()->GetMinimumVertexCount();

	return 0;
}
//----------------------------------------------------------------------------------------------
void Renderable::Draw(IRenderDevice* p_pRenderDevice)
{
	MESON_ASSERT(m_pVertexChannel != NULL, "Cannot draw renderable : Vertex channel is NULL.");
	p_pRenderDevice->SetVertexChannel(m_pVertexChannel);

	if (IsIndexed())
	{
		MESON_ASSERT(m_pIndexBuffer != NULL, "Cannot draw indexed renderable : Index buffer is NULL.");

		p_pRenderDevice->SetIndexBuffer(m_pIndexBuffer);
		p_pRenderDevice->DrawIndexedPrimitive(m_ePrimitiveType, 0, 0, 0, GetVertexCount(), GetPrimitiveCount(m_ePrimitiveType, GetIndexCount()));
	}
	else
	{
		p_pRenderDevice->DrawPrimitive(m_ePrimitiveType, 0, GetPrimitiveCount(m_ePrimitiveType, GetVertexCount()));
	}
}
//----------------------------------------------------------------------------------------------
Renderable& Renderable::operator =(const Renderable& p_renderable)
{
	m_bIsIndexed = p_renderable.m_bIsIndexed;
	m_ePrimitiveType = p_renderable.m_ePrimitiveType;
	m_pVertexChannel = p_renderable.m_pVertexChannel;
	m_pIndexBuffer = p_renderable.m_pIndexBuffer;

	return *this;
}
//----------------------------------------------------------------------------------------------
uint Renderable::GetPrimitiveCount(IRenderDevice::PrimitiveType p_ePrimitiveType, uint p_uiElementCount)
{
	switch(p_ePrimitiveType)
	{
	case IRenderDevice::PT_POINTLIST:
		return p_uiElementCount;
	case IRenderDevice::PT_LINELIST_SEGMENTS:
		return p_uiElementCount / 2;
	case IRenderDevice::PT_LINELIST_OPEN:
		return p_uiElementCount - 1;
	case IRenderDevice::PT_LINELIST_CLOSED:
		return p_uiElementCount;
	case IRenderDevice::PT_TRIANGLE_LIST:
		return p_uiElementCount / 3;
	case IRenderDevice::PT_TRIANGLE_STRIP:
		return p_uiElementCount - 2;
	case IRenderDevice::PT_TRIANGLE_FAN:
		return p_uiElementCount - 2;
	}

	return 0;
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END