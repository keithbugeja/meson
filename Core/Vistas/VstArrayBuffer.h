//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "VstTypes.h"

Meson_Vistas_BEGIN

	class ArrayBuffer : public Meson::Common::TReferenceCounter<ArrayBuffer>
	{
	protected:
		void Copy(void* p_pSource, void* p_pDestination, uint p_uiCount = 1);
		void CopyOffset(void* p_pSrc, uint p_uiOffsetSrc, void* p_pDst, uint p_uiOffsetDst, uint p_uiCount = 1);
		void GrowByFactor(float p_fGrowthFactor, uint p_uiMinimum = 0);
		void GrowByCount(uint p_uiCount);

	public:
		ArrayBuffer(uint p_uiElementSize, uint p_uiElementCount, uint p_uiMaxElementCount);
		ArrayBuffer(const ArrayBuffer& p_arrayBuffer);
		~ArrayBuffer(void);

		void Insert(uint p_uiIndex, void* p_pData, uint p_uiCount = 1);
		void Insert(uint p_uiIndex, const ArrayBuffer& p_arrayBuffer);
		void Add(void* p_pData, uint p_uiCount = 1);
		void Add(const ArrayBuffer& p_arrayBuffer);
		void Set(uint p_uiIndex, void* p_pData, uint p_uiCount = 1);
		void Set(uint p_uiIndex, const ArrayBuffer& p_arrayBuffer);
		void RemoveAt(uint p_uiIndex);
		void* ElementAt(uint p_uiIndex);
		int IndexOf(void* p_pData);
		void Swap(uint p_uiIndex1, uint p_uiIndex2);
		uint Size(void) const;
		void Clear(void);
		void SetGrowthFactor(float p_fGrowthFactor);
		void Rebind(uint p_uiElementSize, uint p_uiElementCount, uint p_uiMaxElementCount);

	protected:
		void* m_pArrayBuffer;

		uint m_uiElementSize,
			 m_uiElementCount,
			 m_uiMaximumCount;

		float m_fGrowthFactor;
	};

	typedef Meson::Common::TPointer<ArrayBuffer> ArrayBufferPtr;

Meson_Vistas_END
