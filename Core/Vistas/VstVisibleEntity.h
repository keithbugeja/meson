//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Queue.h"

#include "VstTypes.h"

Meson_Vistas_BEGIN
	
	class VISTAS_API VisibleEntity : public Meson::Common::TReferenceCounter<VisibleEntity>
	{
	public:
		enum Action 
		{
			VACT_ENTER,
			VACT_LEAVE,
			VACT_PUSH,

			VACT_COUNT
		};

	public:
		VisibleEntity(void);
		VisibleEntity(const VisibleEntity& p_visibleEntity);
		VisibleEntity(ISceneNode* p_pSceneNode, IEffect* p_pEffect, Action m_eAction = VACT_PUSH);

		const VistasType& GetType(void) const;

		ISceneNode* GetEntity(void) const;
		void SetEntity(ISceneNode* p_pEntity);

		IEffect* GetEffect(void) const;
		void SetEffect(IEffect* p_pEffect);

		Action GetAction(void) const;
		void SetAction(Action p_eAction);

		bool IsEffect(void) const;
		bool IsEntity(void) const;
		bool IsSentinel(void) const;

		bool IsMatchingSentinel(const VisibleEntity* p_pVisibleEntity) const;

	protected:
		ISceneNode* m_pEntity;
		IEffect* m_pEffect;
		Action m_eAction;
	};

	typedef Meson::Common::TPointer<VisibleEntity> VisibleEntityPtr;
	typedef Meson::Common::Collections::TEnumerator<VisibleEntityPtr>& VisibleEntityEnumerator;

Meson_Vistas_END