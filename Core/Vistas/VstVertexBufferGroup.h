//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstVertexBuffer.h"

Meson_Vistas_BEGIN
	
	//----------------------------------------------------------------------------------------------
	class VISTAS_API VertexBufferGroup : public Meson::Common::TReferenceCounter<VertexBufferGroup>
	{
	protected:
		void RefreshVertexCounts(void);

	public:
		VertexBufferGroup(void);
		VertexBufferGroup(VertexBufferPtr p_pVertexBuffer);
		VertexBufferGroup(const VertexBufferGroup& p_vertexBufferGroup);
		~VertexBufferGroup(void);

		uint GetMinimumVertexCount(void);
		uint GetMaximumVertexCount(void);
		uint GetVertexBufferCount(void);
		uint GetVertexBufferSize(uint p_uiIndex);

		void AddVertexBuffer(VertexBufferPtr p_pVertexBuffer);
		void SetVertexBuffer(uint p_uiIndex, VertexBufferPtr p_pVertexBuffer);
		void InsertVertexBuffer(uint p_uiIndex, VertexBufferPtr p_pVertexBuffer);
		void RemoveVertexBuffer(uint p_uiIndex);
		void RemoveVertexBuffer(VertexBufferPtr p_pVertexBuffer);
		void RemoveAllVertexBuffers(void);

		void SwapVertexBuffers(uint p_uiIndex1, uint p_uiIndex2);

		VertexBufferPtr GetVertexBuffer(uint p_uiIndex);
		VertexBufferPtr operator[](uint p_uiIndex);
	
	protected:
		Meson::Common::Collections::TArrayList<VertexBufferPtr> m_vertexBufferList;

		bool m_bIsStale;
		uint m_uiMinimumVertexCount,
			 m_uiMaximumVertexCount;
	};

	typedef Meson::Common::TPointer<VertexBufferGroup> VertexBufferGroupPtr;

Meson_Vistas_END