//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstVertexChannel.h"
#include "MesonException.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
VertexChannel::VertexChannel(void)
	: m_pVertexDeclaration(NULL),
	  m_pVertexBufferGroup(new VertexBufferGroup())
{
}
//----------------------------------------------------------------------------------------------
VertexChannel::~VertexChannel(void)
{
	ReleaseAllStreams();

	m_pVertexDeclaration = NULL;
	m_pVertexBufferGroup = NULL;
}
//----------------------------------------------------------------------------------------------
void VertexChannel::SetVertexDeclaration(VertexDeclarationPtr p_pVertexDeclaration)
{
	m_pVertexDeclaration = p_pVertexDeclaration;
}
//----------------------------------------------------------------------------------------------
VertexDeclarationPtr VertexChannel::GetVertexDeclaration(void)
{
	return m_pVertexDeclaration;
}
//----------------------------------------------------------------------------------------------
StreamVertexBufferPairEnumerator VertexChannel::GetStreamVertexBufferPairEnumerator(void)
{
	return m_streamVertexBufferMap.GetEnumerator();
}
//----------------------------------------------------------------------------------------------
uint VertexChannel::GetStream(uint p_uiIndex)
{
	if (p_uiIndex < m_streamIndexList.Size())
		return m_streamIndexList.GetElement(p_uiIndex);

	throw new MesonException("Cannot return stream : Specified index exceeds number of streams", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
VertexBufferPtr VertexChannel::GetVertexBuffer(uint p_uiIndex)
{
	if (p_uiIndex < m_streamIndexList.Size())
		return m_pVertexBufferGroup->GetVertexBuffer(p_uiIndex);

	throw new MesonException("Cannot return vertex buffer : Specified index exceeds number of vertex buffers", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
VertexBufferGroupPtr VertexChannel::GetVertexBufferGroup(void)
{
	return m_pVertexBufferGroup;
}
//----------------------------------------------------------------------------------------------
void VertexChannel::SetStreamBinding(uint p_uiStream, VertexBufferPtr p_pVertexBuffer )
{
	MESON_ASSERT(p_pVertexBuffer != NULL, "Null pointer exception on vertex buffer object.");

	// Do we need to replace or insert?
	if (m_streamVertexBufferMap.ContainsKey(p_uiStream))
	{
		uint uiIndex = m_streamIndexList.IndexOf(p_uiStream);
		m_pVertexBufferGroup->SetVertexBuffer(uiIndex, p_pVertexBuffer);
		m_streamIndexList[uiIndex] = p_uiStream;
	}
	else
	{
		uint uiIndex = 0;

		while(uiIndex < m_streamIndexList.Size() && p_uiStream < m_streamIndexList[uiIndex])
			uiIndex++;
		
		m_pVertexBufferGroup->InsertVertexBuffer(uiIndex, p_pVertexBuffer);
		m_streamIndexList.Insert(uiIndex, p_uiStream);
	}

	m_streamVertexBufferMap.Insert(p_uiStream, p_pVertexBuffer);
}
//----------------------------------------------------------------------------------------------
VertexBufferPtr VertexChannel::GetStreamBinding(uint p_uiStream)
{
	if (m_streamVertexBufferMap.ContainsKey(p_uiStream))
		return m_streamVertexBufferMap[p_uiStream];

	throw new MesonException("No vertex buffer binder found for specified stream.");
}
//----------------------------------------------------------------------------------------------
void VertexChannel::ReleaseStream(uint p_uiStream)
{
	uint uiIndex;

	if ((uiIndex = m_streamIndexList.IndexOf(p_uiStream)) == -1)
		throw new MesonException("No vertex buffer binder found for specified stream.");
	
	m_pVertexBufferGroup->RemoveVertexBuffer(uiIndex);
	m_streamIndexList.RemoveAt(uiIndex);
	m_streamVertexBufferMap.RemoveKey(p_uiStream);
}
//----------------------------------------------------------------------------------------------
void VertexChannel::ReleaseAllStreams(void)
{
	TEnumerator<VertexBufferPtr>& vertexBufferEnumerator = m_streamVertexBufferMap.GetValueEnumerator();

	while(vertexBufferEnumerator.HasMoreElements())
		vertexBufferEnumerator.NextElement() = NULL;

	m_pVertexBufferGroup->RemoveAllVertexBuffers();
	m_streamIndexList.Clear();
	m_streamVertexBufferMap.Clear();
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END