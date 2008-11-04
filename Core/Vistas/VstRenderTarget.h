//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::RenderTarget
//	Interface class for render targets.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "TreeMap.h"
#include "VstTypes.h"

Meson_Vistas_BEGIN

	class VISTAS_API RenderTarget : public Meson::Common::TReferenceCounter<RenderTarget>
	{
	public:
		enum RenderTargetType
		{
			RTT_WINDOW,
			RTT_TEXTURE,
			RTT_MULTI,
			
			RTT_COUNT
		};

	public:
		virtual ~RenderTarget(void);

		virtual const RenderTargetType GetType(void) const = 0;
		virtual const Meson::Common::Text::String& GetName(void) const;

		virtual void GetDimensions(uint& p_uiWidth, uint& p_uiHeight, uint& p_uiColourDepth) const;
		virtual uint GetWidth(void) const;
		virtual uint GetHeight(void) const;
		virtual uint GetColourDepth(void) const;
		
		virtual int  GetPriority(void) const;
		virtual void SetPriority(int p_nPriority);

		virtual bool IsActive(void) const;
		virtual void SetActive(bool p_bActive);

		virtual void CreateViewport(Camera* p_pCamera, uint p_uiPriorityIndex = 0, float p_fLeft = 0.0f, float p_fTop = 0.0f, float p_fWidth = 1.0f, float p_fHeight = 1.0f, float p_fNear = 0.0f, float p_fFar = 1.0f);
		virtual void DestroyViewport(uint p_uiPriorityIndex);
		virtual void DestroyAllViewports(void);

		virtual uint GetViewportCount(void) const;
		virtual Viewport* GetViewport(uint p_uiPriorityIndex);
		virtual Meson::Common::Collections::TEnumerator<Viewport*>& GetAllViewports(void);

		virtual void OnCreateViewport(Viewport* p_pViewport);
		virtual void OnUpdateViewport(Viewport* p_pViewport);
		virtual void OnDestroyViewport(Viewport* p_pViewport);

	protected:
		Meson::Common::Text::String m_strName;
		Meson::Common::Collections::TTreeMap<uint, Viewport*> m_viewportMap;

		uint m_uiWidth,
			 m_uiHeight,
			 m_uiColourBufferSize;

		int m_nPriority;

		bool m_bActive;
	};

	typedef Meson::Common::TPointer<RenderTarget> RenderTargetPtr;

Meson_Vistas_END
