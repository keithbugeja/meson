//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstVistasObject.h"
#include "VstVisibleEntity.h"

Meson_Vistas_BEGIN
	
	class VISTAS_API VisibleEntitySet : public VistasObject
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** Visible entity sets contain a list of objects which are contained or intersect a given view 
			frustum. The scene graph tree structure is flattened and maintained as a queue. Consequently,
			visible entities representing internal nodes of the scene graph which may require some action
			be taken during traversal, both on entering and leaving the node, specify the action property
			accordingly.
		 */
		//----------------------------------------------------------------------------------------------
		VisibleEntitySet(void);
		VisibleEntitySet(const Meson::Common::Text::String& p_strName);
		VisibleEntitySet(const VisibleEntitySet& p_visibleEntitySet);

		~VisibleEntitySet(void);

		//----------------------------------------------------------------------------------------------
		/** Inserts a new VisibleEntity object in the queue. There are three kinds of entity, 
			renderable entities, usually inserted using the push action, effect sentinels, inserted
			using an enter and a leave action, and internal nodes, which are also added using 
			enter and leave actions. 
		 */ 
		//----------------------------------------------------------------------------------------------
		void Insert(ISceneNode* p_pEntity, IEffect* p_pEffect, VisibleEntity::Action p_eAction); 

		//----------------------------------------------------------------------------------------------
		void GetVisibleEntitySubset(Meson::Common::TPointer<VisibleEntitySet> p_pVisibleEntitySet, uint p_uiStartIndex, uint p_uiEndIndex);
		Meson::Common::TPointer<VisibleEntitySet> GetVisibleEntitySubset(uint p_uiStartIndex, uint p_uiEndIndex);

		//----------------------------------------------------------------------------------------------
		void GetVisibleEntitySubset(Meson::Common::TPointer<VisibleEntitySet> p_pVisibleEntitySet, uint p_uiSentinelIndex);
		Meson::Common::TPointer<VisibleEntitySet> GetVisibleEntitySubset(uint p_uiSentinelIndex);

		//----------------------------------------------------------------------------------------------
		void Clear(void);
		size_t Size(void) const;
		VisibleEntityPtr& GetElement(int p_nIndex);
		VisibleEntityPtr& operator[](int p_nIndex);
		VisibleEntityEnumerator GetEnumerator(bool p_bReset=true);

		bool operator==(const VisibleEntitySet& p_visibleEntitySet) const;
		VisibleEntitySet& operator=(const VisibleEntitySet& p_visibleEntitySet);
	
	protected:
		Meson::Common::Collections::TQueue<VisibleEntityPtr> m_visibleEntityQueue;
	};

	typedef Meson::Common::TPointer<VisibleEntitySet> VisibleEntitySetPtr;
	typedef Meson::Common::Collections::TEnumerator<VisibleEntitySetPtr>& VisibleEntitySetEnumerator;

Meson_Vistas_END