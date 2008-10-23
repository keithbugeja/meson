//----------------------------------------------------------------------------------------------
//	Meson::Vistas::
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "MesonException.h"
#include "VstVistasEngine.h"
#include "VstHardwareResourceManager.h"
#include "DX9VertexDeclaration.h"
#include "DX9VertexBuffer.h"

using namespace Meson::Common;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
DX9VertexBuffer::DX9VertexBuffer(LPDIRECT3DDEVICE9 p_pD3DDevice9, uint p_uiVertexSize, uint p_uiVertexCount, dword p_dwPool, dword p_dwUsage) 
	: VertexBuffer( p_uiVertexSize, p_uiVertexCount, p_dwPool, p_dwUsage ), 
	  m_pD3DDevice9(p_pD3DDevice9)
{
	m_bIsLocked = false;
	
	D3DPOOL d3dPool;
	DWORD dwUsage = 0;

	// TODO: Extend usage
	if (p_dwUsage == HardwareResource::MU_DEFAULT)
		dwUsage = NULL;
	
	// TODO: Extend memory pool
	switch( p_dwPool )
	{
		case HardwareResource::MP_DEFAULT:
		default:
			d3dPool = D3DPOOL_MANAGED;
	}

	if (D3D_OK != p_pD3DDevice9->CreateVertexBuffer( p_uiVertexSize * p_uiVertexCount, dwUsage, 0, d3dPool, &m_pD3DVertexBuffer9, NULL))
		throw new MesonException("Failed creating vertex buffer.", __FILE__, __LINE__ );

	// Create Resource ID
	m_resourceHandle = (void*)m_pD3DVertexBuffer9;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
DX9VertexBuffer::~DX9VertexBuffer(void)
{
	Unlock();

	SAFE_RELEASE(m_pD3DVertexBuffer9);
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void DX9VertexBuffer::Unlock(void) 
{
	if (m_bIsLocked)
	{
		m_pD3DVertexBuffer9->Unlock();
		m_bIsLocked = false;
	}
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void DX9VertexBuffer::Lock(uint p_uiStart, uint p_uiSize, dword p_dwOptions)
{
	if (!m_bIsLocked)
	{
		if (D3D_OK != m_pD3DVertexBuffer9->Lock(p_uiStart, p_uiSize, (void**)&m_pbyData, p_dwOptions))
			throw new MesonException("Unable to lock vertex buffer.", __FILE__, __LINE__ );

		m_bIsLocked = true;
	}
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void DX9VertexBuffer::Lock(dword p_dwOptions) 
{
	Lock(0, m_uiVertexCount * m_uiVertexSize, p_dwOptions);
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void DX9VertexBuffer::Read(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, byte* p_pbyBuffer)
{
	memcpy(p_pbyBuffer + p_nDestinationOffset, m_pbyData + p_nSourceOffset, p_nSize); 
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void DX9VertexBuffer::Write(const uint p_nSourceOffset, const uint p_nDestinationOffset, uint p_nSize, const byte* p_pbyBuffer)
{
	memcpy(m_pbyData + p_nDestinationOffset, p_pbyBuffer + p_nSourceOffset, p_nSize); 
}

Meson_Vistas_END
