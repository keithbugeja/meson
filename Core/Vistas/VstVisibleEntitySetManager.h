//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/11/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Singleton.h"
#include "LookaheadInputStream.h"

#include "VstTypes.h"
#include "VstVisibleEntitySet.h"

#include "VstCamera.h"
#include "VstCuller.h"
#include "VstRenderer.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class Operation : public VistasObject
	{
		VISTAS_IMPLEMENT_RTTI(Operation, VistasObject);

	public:
		enum OperationType
		{
			OT_CULL,
			OT_RENDER,
			OT_MERGE,

			OT_COUNT
		};

		enum RepeatType
		{
			RT_ONCE,
			RT_ALWAYS,

			RT_COUNT
		};

	public:
		Operation(void);
		Operation(const Meson::Common::Text::String& p_strId);

		void SetActive(bool p_bActive);
		bool IsActive(void) const;

		void SetRenderOperation(RepeatType p_eRepeatType, 
			VisibleEntitySetPtr p_pVisibleEntitySet, RendererPtr p_pRenderer);

		void SetCullOperation(RepeatType p_eRepeatType, bool p_bAccumulate,
			VisibleEntitySetPtr p_pVisibleEntitySet, ISceneNode* p_pNode,
			CameraPtr p_pProjector, CullerPtr p_pCuller);

		void SetOperationType(OperationType p_eOperationType);
		OperationType GetOperationType(void) const;
		
		void SetRepeatType(RepeatType p_eRepeatType);
		RepeatType GetRepeatType(void) const;
		
		void SetVisibleEntitySet(VisibleEntitySetPtr p_pVisibleEntitySet);
		VisibleEntitySetPtr GetVisibleEntitySet(void);
		
		void SetCuller(CullerPtr p_pCuller);
		void SetRenderer(RendererPtr p_pRenderer);
		void SetProjector(CameraPtr p_pProjector);
		void SetNode(ISceneNode* p_pNode);
		void SetAccumulate(bool p_bAccumulate);
		void Execute(void);
	
	protected:
		bool m_bActive,
			m_bAccumulate;

		VisibleEntitySetPtr m_pVisibleEntitySet;
		OperationType m_eOperationType;
		RepeatType m_eRepeatType;
		CullerPtr m_pCuller;
		RendererPtr m_pRenderer;
		CameraPtr m_pProjector;
		ISceneNode* m_pNode;
	};
	//----------------------------------------------------------------------------------------------
	typedef Meson::Common::TPointer<Operation> OperationPtr;
	//----------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------
	class VisibleEntitySetManager : public Meson::Common::TSingleton<VisibleEntitySetManager>
	{
		friend class Meson::Common::TSingleton<VisibleEntitySetManager>;

	private: 
		VisibleEntitySetManager(void);
		~VisibleEntitySetManager(void);

	public:
		VisibleEntitySetPtr CreateVisibleEntitySet(const Meson::Common::Text::String& p_strId);

		bool ContainsVisibleEntitySet(const Meson::Common::Text::String& p_strId);
		bool ContainsVisibleEntitySet(const Meson::Common::Text::String& p_strId, VisibleEntitySetPtr& p_pVisibleEntitySet);

		VisibleEntitySetPtr GetVisibleEntitySet(const Meson::Common::Text::String& p_strId);

		void RegisterVisibleEntitySet(VisibleEntitySetPtr p_pVisibleEntitySet, bool p_bReplace=true);

		void UnregisterVisibleEntitySet(VisibleEntitySetPtr p_pVisibleEntitySet);
		void UnregisterVisibleEntitySet(const Meson::Common::Text::String& p_strId);
		void UnregisterAllVisibleEntitySets(void);

		void LoadVisibleEntitySet(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadVisibleEntitySets(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadVisibleEntitySets(const Meson::Common::Text::String& p_strResourceName);

		//----------------------------------------------------------------------------------------------
		void ExecuteOperations(void);

		OperationPtr CreateOperation(const Meson::Common::Text::String& p_strId);

		bool ContainsOperation(const Meson::Common::Text::String& p_strId);
		bool ContainsOperation(const Meson::Common::Text::String& p_strId, OperationPtr& p_pOperation);

		OperationPtr GetOperation(const Meson::Common::Text::String& p_strId);

		void RegisterOperation(OperationPtr p_pOperation, bool p_bReplace=true);

		void UnregisterOperation(OperationPtr p_pOperation);
		void UnregisterOperation(const Meson::Common::Text::String& p_strId);
		void UnregisterAllOperations(void);

		void LoadOperation(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadOperations(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadOperations(const Meson::Common::Text::String& p_strResourceName);
		//----------------------------------------------------------------------------------------------

	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, VisibleEntitySetPtr> m_visibleEntitySetMap;
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, OperationPtr> m_operationMap;
	};

Meson_Vistas_END
