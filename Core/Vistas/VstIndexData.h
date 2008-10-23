//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Pair.h"

#include "VstTypes.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------
	class IndexData : public Meson::Common::TReferenceCounter<IndexData>
	{
	public:
		IndexData(void);
		IndexData(uint p_uiInitialIndexCount);

		void* GetIndexBuffer(uint p_uiIndex);
		uint  GetIndexCount(void) const;
		uint  GetIndex(uint p_uiIndex);
		void  Swap(uint p_uiIndex1, uint p_uiIndex2);

		void AddMulti(uint p_uiCount, ...);
		void Add(uint p_uiValue);
		void Add(uint p_uiValue1, uint p_uiValue2);
		void Add(uint p_uiValue1, uint p_uiValue2, uint p_uiValue3);
		void Add(uint p_uiValue1, uint p_uiValue2, uint p_uiValue3, uint p_uiValue4);

		void InsertMulti(uint p_uiIndex, uint p_uiCount, ...);
		void Insert(uint p_uiIndex, uint p_uiValue);
		void Insert(uint p_uiIndex, uint p_uiValue1, uint p_uiValue2);
		void Insert(uint p_uiIndex, uint p_uiValue1, uint p_uiValue2, uint p_uiValue3);
		void Insert(uint p_uiIndex, uint p_uiValue1, uint p_uiValue2, uint p_uiValue3, uint p_uiValue4);

		void Set(uint p_uiIndex, uint p_uiValue);
		void RemoveAt(uint p_uiIndex);
		void RemoveRange(uint p_uiStart, uint p_uiLength);
		void RemoveAll(void);

		uint operator[] (uint p_uiIndex);

	protected:
		Meson::Common::Collections::TArrayList<uint> m_indexBuffer;
	};
	
	typedef Meson::Common::TPointer<IndexData> IndexDataPtr;

Meson_Vistas_END