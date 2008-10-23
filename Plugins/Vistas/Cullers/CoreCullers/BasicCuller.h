//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Pointer.h"

#include "VstTypes.h"
#include "VstCuller.h"

Meson_Vistas_BEGIN
	
	class BasicCuller : public ICuller
	{
	public:
		BasicCuller(void);
		~BasicCuller(void);

		const Meson::Common::Text::String& GetType(void) const;

		Meson::Common::TPointer<ICuller> Clone(bool p_bShallow);
		Meson::Common::TPointer<ICuller> CreateInstance(void);

		uint GetPlaneCount(void) const;
		void SetPlane(uint p_uiPlaneIndex, bool p_bEnabled);
		bool IsPlaneEnabled(uint p_uiPlaneIndex) const;
		void PushPlane(Meson::Common::Maths::Planef& p_plane);
		void PopPlane(void);

		void SetCamera(Camera* p_pCamera);
		void SetRootNode(ISceneNode* p_pLocalRootNode);

		void Push(ISceneNode* p_pVisibleEntity);
		void Enter(ISceneCompositeNode* p_pVisibleEntity, IEffect* p_pEffect);
		void Leave(ISceneCompositeNode* p_pVisibleEntity, IEffect* p_pEffect);

		bool IsVisible(ISceneNode* p_pEntity) const;
		bool IsVisible(IBoundingVolume* p_pBoundingVolume) const;

		void Rebuild(void);
		void Update(void);

		VisibleEntitySetPtr GetVisibleSet(void);
		void SetVisibleSet(VisibleEntitySetPtr p_pVisibleEntitySet);

		VisibleEntitySetPtr ComputeVisibleSet(bool p_bAccumulate=false);

	protected:
		Meson::Common::Text::String m_strType;
		Meson::Common::Collections::TArrayList<Meson::Common::Maths::Planef> m_planeList;
		Meson::Common::Collections::TArrayList<Meson::Common::Maths::Planef> m_userPlaneList;

		VisibleEntitySetPtr m_pVisibleEntitySet;
		ISceneNode* m_pRootNode;
		Camera* m_pCamera;
	};

Meson_Vistas_END