#pragma once

#include "CnsDataType.h"



Meson_Cuneus_RTS_Typing_BEGIN

class TypeArray : public DataType
{
protected:
	dword m_dwSize;
	dword m_dwElementTypeCode;
	dword m_dwElementSize;
	byte* m_pbyData;
	bool  m_bAutoRelease;

public:
	TypeArray( dword p_dwSize, dword p_dwElementTypeCode );
	TypeArray( dword p_dwSize, dword p_dwElementTypeCode, void* p_pbyData );
	~TypeArray( void );

	dword GetSize( void );
	dword GetElementTypeCode( void );

	Meson::Common::TPointer<DataType> GetElement( dword p_dwIndex );
	void SetElement( dword p_dwIndex, Meson::Common::TPointer<DataType>& p_dataType );
};

Meson_Cuneus_RTS_Typing_END
