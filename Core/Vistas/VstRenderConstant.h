//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstRenderTypes.h"
#include "VstShaderProgramVariable.h"

Meson_Vistas_BEGIN

	class RenderDeviceAdapter;

	class RenderConstant : public ShaderProgramVariable
	{
	public:
		RenderConstant(RenderConstantType::RenderConstantType p_constantType, RenderDeviceAdapter* p_pRenderDeviceAdapter);

		void Update(void);

		void UpdateMatrixWorld(void);
		void UpdateMatrixWorldTranspose(void);
		void UpdateMatrixWorldInverse(void);
		void UpdateMatrixWorldInverseTranspose(void);

		void UpdateMatrixView(void);
		void UpdateMatrixViewTranspose(void);
		void UpdateMatrixViewInverse(void);
		void UpdateMatrixViewInverseTranspose(void);

		void UpdateMatrixProjection(void);
		void UpdateMatrixProjectionTranspose(void);
		void UpdateMatrixProjectionInverse(void);
		void UpdateMatrixProjectionInverseTranspose(void);

		void UpdateMatrixViewProjection(void);
		void UpdateMatrixViewProjectionTranspose(void);
		void UpdateMatrixViewProjectionInverse(void);
		void UpdateMatrixViewProjectionInverseTranspose(void);

		void UpdateMatrixWorldViewProjection(void);
		void UpdateMatrixWorldViewProjectionTranspose(void);
		void UpdateMatrixWorldViewProjectionInverse(void);
		void UpdateMatrixWorldViewProjectionInverseTranspose(void);

		void UpdateVectorMaterialAmbient(void);
		void UpdateVectorMaterialDiffuse(void);
		void UpdateVectorMaterialSpecular(void);
		void UpdateVectorMaterialEmissive(void);

		void UpdateVectorLightPosition(void);
		void UpdateVectorLightDirection(void);
		void UpdateVectorLightAmbient(void);
		void UpdateVectorLightDiffuse(void);
		void UpdateVectorLightSpecular(void);
		void UpdateVectorLightCone(void);
		void UpdateVectorLightAttenuation(void);

		void UpdateVectorArrayLightPosition(void);
		void UpdateVectorArrayLightDirection(void);
		void UpdateVectorArrayLightAmbient(void);
		void UpdateVectorArrayLightDiffuse(void);
		void UpdateVectorArrayLightSpecular(void);
		void UpdateVectorArrayLightCone(void);
		void UpdateVectorArrayLightAttenuation(void);
		void UpdateIntegerLightActiveCount(void);

		void UpdateVectorViewerPosition(void);
		void UpdateVectorViewerDirection(void);

		void UpdateVectorAmbient(void);

	protected:
		RenderDeviceAdapter* m_pRenderDeviceAdapter;
		RenderConstantType::RenderConstantType m_constantType;

		typedef void (RenderConstant::*UpdateConstantFunction)(void);
		static UpdateConstantFunction m_updateConstantFunctions[RenderConstantType::RCT_COUNT];

		static Meson::Common::Text::String m_strConstantNames[RenderConstantType::RCT_COUNT];
	};

Meson_Vistas_END
