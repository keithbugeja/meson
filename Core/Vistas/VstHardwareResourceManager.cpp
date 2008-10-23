//----------------------------------------------------------------------------------------------
//	Meson::Vistas::
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once
#include "MesonException.h"

#include "VstVistasEngine.h"
#include "VstHardwareResourceManager.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN
	
//----------------------------------------------------------------------------------------------
HardwareResourceManager::HardwareResourceManager(IHardwareResourceFactory* p_pHardwareResourceFactory)
	: m_pHardwareResourceFactory(p_pHardwareResourceFactory)
{
}
//----------------------------------------------------------------------------------------------
HardwareResourceManager::~HardwareResourceManager(void)
{
	ReleaseAllVertexBuffers();
	ReleaseAllVertexDeclarations();
	ReleaseAllVertexChannels();
	ReleaseAllIndexBuffers();
	ReleaseAllTextureBuffers();
}
//----------------------------------------------------------------------------------------------
TextureBufferPtr HardwareResourceManager::CreateTextureBuffer(TextureBuffer::TextureFormat p_eTextureFormat, TextureBuffer::TextureType p_eTextureType, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage, bool p_bOwner)
{
	TextureBufferPtr pTextureBuffer(m_pHardwareResourceFactory->CreateTextureBuffer(p_eTextureFormat, p_eTextureType, p_uiWidth, p_uiHeight, p_uiDepth, p_uiMipmapLevels, p_dwPool, p_dwUsage));
	if (p_bOwner) m_textureBufferList.Add(pTextureBuffer);
	return pTextureBuffer;
}
//----------------------------------------------------------------------------------------------
TextureBufferPtr HardwareResourceManager::CreateTextureBuffer2D(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiWidth, uint p_uiHeight, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage, bool p_bOwner)
{
	TextureBufferPtr pTextureBuffer(m_pHardwareResourceFactory->CreateTextureBuffer2D(p_eTextureFormat, p_uiWidth, p_uiHeight, p_uiMipmapLevels, p_dwPool, p_dwUsage));
	if (p_bOwner) m_textureBufferList.Add(pTextureBuffer);
	return pTextureBuffer;
}
//----------------------------------------------------------------------------------------------
TextureBufferPtr HardwareResourceManager::CreateTextureBuffer3D(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiWidth, uint p_uiHeight, uint p_uiDepth, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage, bool p_bOwner)
{
	TextureBufferPtr pTextureBuffer(m_pHardwareResourceFactory->CreateTextureBuffer3D(p_eTextureFormat, p_uiWidth, p_uiHeight, p_uiDepth, p_uiMipmapLevels, p_dwPool, p_dwUsage));
	if (p_bOwner) m_textureBufferList.Add(pTextureBuffer);
	return pTextureBuffer;
}
//----------------------------------------------------------------------------------------------
TextureBufferPtr HardwareResourceManager::CreateTextureBufferCube(TextureBuffer::TextureFormat p_eTextureFormat, uint p_uiEdgeLength, uint p_uiMipmapLevels, dword p_dwPool, dword p_dwUsage, bool p_bOwner)
{
	TextureBufferPtr pTextureBuffer(m_pHardwareResourceFactory->CreateTextureBufferCube(p_eTextureFormat, p_uiEdgeLength, p_uiMipmapLevels, p_dwPool, p_dwUsage));
	if (p_bOwner) m_textureBufferList.Add(pTextureBuffer);
	return pTextureBuffer;
}
//----------------------------------------------------------------------------------------------
TextureBufferPtr HardwareResourceManager::CreateTextureBufferFromFile(const String& p_strFilename, TextureBuffer::TextureType p_eTextureType, bool p_bOwner)
{
	String strResourcePath;
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strFilename, strResourcePath))
		throw new MesonException("Texture resource not found!", __FILE__, __LINE__);

	TextureBufferPtr pTextureBuffer(m_pHardwareResourceFactory->CreateTextureBufferFromFile(strResourcePath, p_eTextureType));
	if (p_bOwner) m_textureBufferList.Add(pTextureBuffer);
	return pTextureBuffer;
}
//----------------------------------------------------------------------------------------------
void HardwareResourceManager::ReleaseTextureBuffer(TextureBufferPtr p_pTextureBuffer)
{
	if (m_textureBufferList.IndexOf(p_pTextureBuffer) == -1)
		throw new MesonException("Texture buffer is not managed by resource manager.");

	m_textureBufferList.Remove(p_pTextureBuffer);
}
//----------------------------------------------------------------------------------------------
void HardwareResourceManager::ReleaseAllTextureBuffers(void)
{
	TEnumerator<TextureBufferPtr>& textureBufferEnumerator = m_textureBufferList.GetEnumerator();
	
	// Releases all index buffer objects
	while( textureBufferEnumerator.HasMoreElements() )
		textureBufferEnumerator.NextElement() = NULL;

	m_textureBufferList.Clear();
}
//----------------------------------------------------------------------------------------------
IndexBufferPtr HardwareResourceManager::CreateIndexBuffer(IndexBuffer::IndexType p_eIndexType, uint p_uiIndexCount, dword p_dwPool, dword p_dwUsage, bool p_bOwner)
{
	IndexBufferPtr pIndexBuffer(m_pHardwareResourceFactory->CreateIndexBuffer(p_eIndexType, p_uiIndexCount, p_dwPool, p_dwUsage));
	if (p_bOwner) m_indexBufferList.Add(pIndexBuffer);
	return pIndexBuffer;
}
//----------------------------------------------------------------------------------------------
void HardwareResourceManager::ReleaseIndexBuffer(IndexBufferPtr p_pIndexBuffer)
{
	if (m_indexBufferList.IndexOf(p_pIndexBuffer) == -1)
		throw new MesonException("Index buffer is not managed by resource manager.");

	m_indexBufferList.Remove(p_pIndexBuffer);
}
//----------------------------------------------------------------------------------------------
void HardwareResourceManager::ReleaseAllIndexBuffers(void)
{
	TEnumerator<IndexBufferPtr>& indexBufferEnumerator = m_indexBufferList.GetEnumerator();
	
	// Releases all index buffer objects
	while( indexBufferEnumerator.HasMoreElements() )
		indexBufferEnumerator.NextElement() = NULL;

	m_indexBufferList.Clear();
}
//----------------------------------------------------------------------------------------------
VertexBufferPtr HardwareResourceManager::CreateVertexBuffer(uint p_uiVertexSize, uint p_uiVertexCount, dword p_dwPool, dword p_dwUsage, bool p_bOwner)
{
	VertexBufferPtr pVertexBuffer(m_pHardwareResourceFactory->CreateVertexBuffer(p_uiVertexSize, p_uiVertexCount, p_dwPool, p_dwUsage));
	if (p_bOwner) m_vertexBufferList.Add(pVertexBuffer);
	return pVertexBuffer;
}
//----------------------------------------------------------------------------------------------
void HardwareResourceManager::ReleaseVertexBuffer(VertexBufferPtr p_pVertexBuffer)
{
	if (m_vertexBufferList.IndexOf(p_pVertexBuffer) == -1)
		throw new MesonException("Vertex buffer is not managed by resource manager.");

	m_vertexBufferList.Remove(p_pVertexBuffer);
}
//----------------------------------------------------------------------------------------------
void HardwareResourceManager::ReleaseAllVertexBuffers(void)
{
	TEnumerator<VertexBufferPtr>& vertexBufferEnumerator = m_vertexBufferList.GetEnumerator();
	
	// Releases all vertex buffer objects
	while( vertexBufferEnumerator.HasMoreElements() )
		vertexBufferEnumerator.NextElement() = NULL;

	m_vertexBufferList.Clear();
}
//----------------------------------------------------------------------------------------------
VertexDeclarationPtr HardwareResourceManager::CreateVertexDeclaration(bool p_bOwner)
{
	VertexDeclarationPtr pVertexDeclaration(m_pHardwareResourceFactory->CreateVertexDeclaration());
	if (p_bOwner) m_vertexDeclarationList.Add(pVertexDeclaration);
	return pVertexDeclaration;
}
//----------------------------------------------------------------------------------------------
void HardwareResourceManager::ReleaseVertexDeclaration(VertexDeclarationPtr p_pVertexDeclaration)
{
	if (m_vertexDeclarationList.IndexOf(p_pVertexDeclaration) == -1)
		throw new MesonException("Vertex declaration is not managed by resource manager.");

	m_vertexDeclarationList.Remove(p_pVertexDeclaration);
}
//----------------------------------------------------------------------------------------------
void HardwareResourceManager::ReleaseAllVertexDeclarations(void)
{
	TEnumerator<VertexDeclarationPtr>& vertexDeclarationEnumerator = m_vertexDeclarationList.GetEnumerator();
	
	// Releases all vertex declaration objects
	while( vertexDeclarationEnumerator.HasMoreElements() )
		vertexDeclarationEnumerator.NextElement() = NULL;

	m_vertexDeclarationList.Clear();
}
//----------------------------------------------------------------------------------------------
VertexChannelPtr HardwareResourceManager::CreateVertexChannel(bool p_bOwner)
{
	VertexChannelPtr pVertexChannel(new VertexChannel());
	if (p_bOwner) m_vertexChannelList.Add(pVertexChannel);
	return pVertexChannel;
}
//----------------------------------------------------------------------------------------------
void HardwareResourceManager::ReleaseVertexChannel(VertexChannelPtr p_pVertexChannelPtr)
{
	if (m_vertexChannelList.IndexOf(p_pVertexChannelPtr) == -1)
		throw new MesonException("Vertex to stream binding is not managed by resource manager.");

	m_vertexChannelList.Remove(p_pVertexChannelPtr);
}
//----------------------------------------------------------------------------------------------
void HardwareResourceManager::ReleaseAllVertexChannels(void)
{
	TEnumerator<VertexChannelPtr>& vertexChannelEnumerator = m_vertexChannelList.GetEnumerator();
	
	// Releases all vertex to stream binding objects
	while( vertexChannelEnumerator.HasMoreElements() )
		vertexChannelEnumerator.NextElement() = NULL;

	m_vertexChannelList.Clear();
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END