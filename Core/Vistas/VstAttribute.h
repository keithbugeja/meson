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
	class Attribute
	{
	protected:
		uint m_uiMaterial,
			 m_uiSmoothingGroup;

		bool m_bIsSmoothingEnabled,
			 m_bIsMarked;

	public:
		Attribute(void);
		Attribute(uint p_uiMaterial, uint p_uiSmoothingGroup, bool p_bSmoothingEnabled = true);

		bool IsMarked(void);
		void SetMark(bool p_bEnabled);

		bool IsSmoothingEnabled(void);
		void SetSmoothing(bool p_bEnabled);

		uint GetMaterial(void);
		void SetMaterial(uint p_uiMaterial);

		uint GetSmoothingGroup(void);
		void SetSmoothingGroup(uint p_uiSmoothingGroup);

		bool operator==(const Attribute& p_attribute);
		Attribute& operator=(const Attribute& p_attribute);
	};

Meson_Vistas_END