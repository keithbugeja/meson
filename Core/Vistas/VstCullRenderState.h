//----------------------------------------------------------------------------------------------
//	Meson::Vistas::Render::CullRenderState
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	27/09/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"
#include "VstRenderState.h"

Meson_Vistas_BEGIN

	class CullRenderState : public IRenderState
	{
	public:
		/** Vertex winding order for a face
		 */ 
		enum FaceWinding
		{
			CRSFW_CCW,
			CRSFW_CW,
			CRSFW_COUNT
		};

		/** Face culling type: front or back
		 */
		enum FaceCulling
		{
			CRSFC_FRONT,
			CRSFC_BACK,
			CRSFC_COUNT
		};

	public:
		CullRenderState(void);
		RenderStateType GetType(void) const;

		bool IsCullingEnabled(void) const;
		FaceWinding GetFaceWindingMode(void) const;
		FaceCulling GetFaceCullingMode(void) const;

		void SetCulling(bool p_bEnabled);
		void SetFaceWindingMode(FaceWinding p_faceWinding);
		void SetFaceCullingMode(FaceCulling p_faceCulling);

		const CullRenderState& operator= (const CullRenderState& p_cullRenderState);
		bool operator==(const CullRenderState& p_cullRenderState);

	protected:
		FaceWinding m_faceWinding;
		FaceCulling m_faceCulling;

		bool m_bIsCullingEnabled;
	};

	typedef Meson::Common::TPointer<CullRenderState> CullRenderStatePtr;

Meson_Vistas_END
