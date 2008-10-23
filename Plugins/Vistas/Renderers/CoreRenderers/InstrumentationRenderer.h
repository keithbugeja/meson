//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Pointer.h"
#include "Stack.h"

#include "VstTypes.h"
#include "VstLight.h"
#include "VstRenderer.h"
#include "VstBoundingSphere.h"
#include "VstBoundingAxisAlignedBox.h"

#include "VstSceneGeometryNode.h"

Meson_Vistas_BEGIN
	
	class InstrumentationRenderer : public IRenderer
	{
	protected:
		void CreateInstrumentationShaders(void);
		void CreateInstrumentationPrimitives(void);

		void ProcessNode(VisibleEntityPtr& p_pVisibleEntity);
		void DrawBoundingVolume(BoundingVolumePtr& p_pBoundingVolume, Transformation* p_pTransformation);
		void DrawAxisAlignedBoundingBox(BoundingAxisAlignedBox* p_pBoundingBox, Transformation* p_pTransformation);
		void DrawBoundingSphere(BoundingSphere* p_pBoundingSphere, Transformation* p_pTransformation);

	public:
		InstrumentationRenderer(void);
		~InstrumentationRenderer(void);

		const Meson::Common::Text::String& GetName(void) const;

		Meson::Common::TPointer<IRenderer> Clone(bool p_bShallow);
		Meson::Common::TPointer<IRenderer> CreateInstance(void);

		void SetGlobalEffects(bool p_bEnabled) { m_bIsEnabledGlobalEffects = p_bEnabled; }
		void SetLocalEffects(bool p_bEnabled) { m_bIsEnabledLocalEffects = p_bEnabled; }

		bool IsEnabledGlobalEffects(void) const { return m_bIsEnabledGlobalEffects; }
		bool IsEnabledLocalEffects(void) const { return m_bIsEnabledLocalEffects; }

		void SetVisibleSet(VisibleEntitySetPtr p_pVisibleEntitySet);
		
		void Draw(void);
		void Draw(VisibleEntityPtr p_pVisibleEntity);
		void Draw(VisibleEntitySetPtr p_pVisibleEntitySet);

	protected:
		Meson::Common::Text::String m_strName;

		bool m_bIsEnabledGlobalEffects,
			 m_bIsEnabledLocalEffects;

		VisibleEntitySetPtr		m_pVisibleEntitySet;
		IRenderDevice*			m_pRenderDevice;

		MaterialRenderStatePtr	m_pMaterialWireframe;

		CullRenderStatePtr		m_pCullDisabled;

		IShaderProgram			*m_pInstrumentationVS,
								*m_pInstrumentationPS;

		VertexDeclarationPtr	m_pCubeVertexDeclaration;
		VertexBufferPtr			m_pCubeVertexBuffer;
		IndexBufferPtr			m_pCubeIndexBuffer;

		GeometryPtr				m_pSphere;
	};

Meson_Vistas_END