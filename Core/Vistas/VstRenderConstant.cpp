//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
//  CLEAN:	Some properties could be packed into single shader constants. For instance, 
//			range could be packed into the attenuation vector to result in something like
//			A = <<C, L, Q>, R>. 
//----------------------------------------------------------------------------------------------
#include "VstRenderConstant.h"
#include "VstRenderDeviceAdapter.h"

using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
String RenderConstant::m_strConstantNames[RenderConstantType::RCT_COUNT] = 
{
	"matWorld",
	"matWorldTrans",
	"matWorldInv",
	"matWorldInvTrans",

	"matView",
	"matViewTrans",
	"matViewInv",
	"matViewInvTrans",

	"matProjection",
	"matProjectionTrans",
	"matProjectionInv",
	"matProjectionInvTrans",

	"matViewProjection",
	"matViewProjectionTrans",
	"matViewProjectionInv",
	"matViewProjectionInvTans",

	"matWorldViewProjection",
	"matWorldViewProjectionTrans",
	"matWorldViewProjectionInv",
	"matWorldViewProjectionInvTrans",

	"materialAmbient",
	"materialDiffuse",
	"materialSpecular",
	"materialEmissive",

	"lightPosition",
	"lightDirection",
	"lightAmbient",
	"lightDiffuse",
	"lightSpecular",
	"lightCone",
	"lightAttenuation",

	"lightPositionArray",
	"lightDirectionArray",
	"lightAmbientArray",
	"lightDiffuseArray",
	"lightSpecularArray",
	"lightConeArray",
	"lightAttenuationArray",

	"lightActiveCount",

	"viewerPosition",
	"viewerDirection",

	"ambient"
};
//----------------------------------------------------------------------------------------------
RenderConstant::UpdateConstantFunction RenderConstant::m_updateConstantFunctions[RenderConstantType::RCT_COUNT] = 
{
	&RenderConstant::UpdateMatrixWorld,
	&RenderConstant::UpdateMatrixWorldTranspose,
	&RenderConstant::UpdateMatrixWorldInverse,
	&RenderConstant::UpdateMatrixWorldInverseTranspose,

	&RenderConstant::UpdateMatrixView,
	&RenderConstant::UpdateMatrixViewTranspose,
	&RenderConstant::UpdateMatrixViewInverse,
	&RenderConstant::UpdateMatrixViewInverseTranspose,

	&RenderConstant::UpdateMatrixProjection,
	&RenderConstant::UpdateMatrixProjectionTranspose,
	&RenderConstant::UpdateMatrixProjectionInverse,
	&RenderConstant::UpdateMatrixProjectionInverseTranspose,

	&RenderConstant::UpdateMatrixViewProjection,
	&RenderConstant::UpdateMatrixViewProjectionTranspose,
	&RenderConstant::UpdateMatrixViewProjectionInverse,
	&RenderConstant::UpdateMatrixViewProjectionInverseTranspose,

	&RenderConstant::UpdateMatrixWorldViewProjection,
	&RenderConstant::UpdateMatrixWorldViewProjectionTranspose,
	&RenderConstant::UpdateMatrixWorldViewProjectionInverse,
	&RenderConstant::UpdateMatrixWorldViewProjectionInverseTranspose,

	&RenderConstant::UpdateVectorMaterialAmbient,
	&RenderConstant::UpdateVectorMaterialDiffuse,
	&RenderConstant::UpdateVectorMaterialSpecular,
	&RenderConstant::UpdateVectorMaterialEmissive,

	&RenderConstant::UpdateVectorLightPosition,
	&RenderConstant::UpdateVectorLightDirection,
	&RenderConstant::UpdateVectorLightAmbient,
	&RenderConstant::UpdateVectorLightDiffuse,
	&RenderConstant::UpdateVectorLightSpecular,
	&RenderConstant::UpdateVectorLightCone,
	&RenderConstant::UpdateVectorLightAttenuation,

	&RenderConstant::UpdateVectorArrayLightPosition,
	&RenderConstant::UpdateVectorArrayLightDirection,
	&RenderConstant::UpdateVectorArrayLightAmbient,
	&RenderConstant::UpdateVectorArrayLightDiffuse,
	&RenderConstant::UpdateVectorArrayLightSpecular,
	&RenderConstant::UpdateVectorArrayLightCone,
	&RenderConstant::UpdateVectorArrayLightAttenuation,

	&RenderConstant::UpdateIntegerLightActiveCount,

	&RenderConstant::UpdateVectorViewerPosition,
	&RenderConstant::UpdateVectorViewerDirection,

	&RenderConstant::UpdateVectorAmbient
};
//----------------------------------------------------------------------------------------------
RenderConstant::RenderConstant(RenderConstantType::RenderConstantType p_constantType, RenderDeviceAdapter* p_pRenderDeviceAdapter)
	: ShaderProgramVariable(m_strConstantNames[p_constantType]),
	  m_pRenderDeviceAdapter(p_pRenderDeviceAdapter),
	  m_constantType(p_constantType)
{
}
//----------------------------------------------------------------------------------------------
void RenderConstant::Update(void)
{
	(this->*m_updateConstantFunctions[m_constantType])();
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixWorld(void)
{
	Set(m_pRenderDeviceAdapter->GetWorldMatrix());
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixWorldTranspose(void)
{
	Matrix4f matWorld(m_pRenderDeviceAdapter->GetWorldMatrix());
	matWorld.Transpose();
	Set(matWorld);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixWorldInverse(void)
{
	Matrix4f matWorld(m_pRenderDeviceAdapter->GetWorldMatrix());
	matWorld.Invert();
	Set(matWorld);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixWorldInverseTranspose(void)
{
	Matrix4f matWorld(m_pRenderDeviceAdapter->GetWorldMatrix());
	matWorld.Invert();
	matWorld.Transpose();
	Set(matWorld);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixView(void) 
{ 
	Set(m_pRenderDeviceAdapter->GetViewMatrix()); 
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixViewTranspose(void)
{
	Matrix4f matView(m_pRenderDeviceAdapter->GetViewMatrix());
	matView.Transpose();
	Set(matView);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixViewInverse(void)
{
	Matrix4f matView(m_pRenderDeviceAdapter->GetViewMatrix());
	matView.Invert();
	Set(matView);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixViewInverseTranspose(void)
{	
	Matrix4f matView(m_pRenderDeviceAdapter->GetViewMatrix());
	matView.Invert();
	matView.Transpose();
	Set(matView);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixProjection(void)
{
	Set(m_pRenderDeviceAdapter->GetProjectionMatrix());
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixProjectionTranspose(void)
{
	Matrix4f matProjection(m_pRenderDeviceAdapter->GetProjectionMatrix());
	matProjection.Transpose();
	Set(matProjection);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixProjectionInverse(void)
{
	Matrix4f matProjection(m_pRenderDeviceAdapter->GetProjectionMatrix());
	matProjection.Invert();
	Set(matProjection);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixProjectionInverseTranspose(void)
{
	Matrix4f matProjection(m_pRenderDeviceAdapter->GetProjectionMatrix());
	matProjection.Invert();
	matProjection.Transpose();
	Set(matProjection);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixViewProjection(void)
{
	Set(m_pRenderDeviceAdapter->GetProjectionMatrix() * m_pRenderDeviceAdapter->GetViewMatrix());
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixViewProjectionTranspose(void)
{
	Matrix4f matViewProjection(m_pRenderDeviceAdapter->GetProjectionMatrix() * m_pRenderDeviceAdapter->GetViewMatrix());
	matViewProjection.Transpose();
	Set(matViewProjection);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixViewProjectionInverse(void)
{
	Matrix4f matViewProjection(m_pRenderDeviceAdapter->GetProjectionMatrix() * m_pRenderDeviceAdapter->GetViewMatrix());
	matViewProjection.Invert();
	Set(matViewProjection);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixViewProjectionInverseTranspose(void)
{
	Matrix4f matViewProjection(m_pRenderDeviceAdapter->GetProjectionMatrix() * m_pRenderDeviceAdapter->GetViewMatrix());
	matViewProjection.Invert();
	matViewProjection.Transpose();
	Set(matViewProjection);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixWorldViewProjection(void)
{
	Set(m_pRenderDeviceAdapter->GetProjectionMatrix() * (m_pRenderDeviceAdapter->GetViewMatrix() * m_pRenderDeviceAdapter->GetWorldMatrix()));
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixWorldViewProjectionTranspose(void)
{
	Matrix4f matWorldViewProjection(m_pRenderDeviceAdapter->GetProjectionMatrix() * (m_pRenderDeviceAdapter->GetViewMatrix() * m_pRenderDeviceAdapter->GetWorldMatrix()));
	matWorldViewProjection.Transpose();
	Set(matWorldViewProjection);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixWorldViewProjectionInverse(void)
{
	Matrix4f matWorldViewProjection(m_pRenderDeviceAdapter->GetProjectionMatrix() * (m_pRenderDeviceAdapter->GetViewMatrix() * m_pRenderDeviceAdapter->GetWorldMatrix()));
	matWorldViewProjection.Invert();
	Set(matWorldViewProjection);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateMatrixWorldViewProjectionInverseTranspose(void)
{
	Matrix4f matWorldViewProjection(m_pRenderDeviceAdapter->GetProjectionMatrix() * (m_pRenderDeviceAdapter->GetViewMatrix() * m_pRenderDeviceAdapter->GetWorldMatrix()));
	matWorldViewProjection.Invert();
	matWorldViewProjection.Transpose();
	Set(matWorldViewProjection);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorMaterialAmbient(void)
{
	Set(m_pRenderDeviceAdapter->GetMaterialRenderState()->GetAmbient());
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorMaterialDiffuse(void)
{
	Set(m_pRenderDeviceAdapter->GetMaterialRenderState()->GetDiffuse());
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorMaterialSpecular(void)
{
	Colour& specularColour = m_pRenderDeviceAdapter->GetMaterialRenderState()->GetSpecular();
	Vector4f specular(specularColour.R, specularColour.G, specularColour.B, m_pRenderDeviceAdapter->GetMaterialRenderState()->GetShininess());
	Set(specular);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorMaterialEmissive(void)
{
	Set(m_pRenderDeviceAdapter->GetMaterialRenderState()->GetEmissive());
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorLightPosition(void)
{
	Set(Vector4f(m_pRenderDeviceAdapter->GetLight(0)->GetPosition()));
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorLightDirection(void)
{
	Set(Vector4f(m_pRenderDeviceAdapter->GetLight(0)->GetDirection()));
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorLightAmbient(void)
{
	Set(m_pRenderDeviceAdapter->GetLight(0)->GetAmbient());
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorLightDiffuse(void)
{
	Set(m_pRenderDeviceAdapter->GetLight(0)->GetDiffuse());
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorLightSpecular(void)
{
	Set(m_pRenderDeviceAdapter->GetLight(0)->GetSpecular());
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorLightCone(void)
{
	const Light* pLight = m_pRenderDeviceAdapter->GetLight(0);
	//Vector3f vec3Cone(pLight->GetCutoffAngle(), pLight->GetFalloffAngle(), pLight->GetFalloffExponent());
	//Set(vec3Cone.Element, 3);

	// NOTE: The function name is not totally descriptive, and besides users might actually need the
	// angles, not the cosines thereof. Therefore, this should be an offshoot of the light cone update.

	// Use the cosines instead of the angles 
	Vector4f vec4Cone(Mathf::Cos(pLight->GetCutoffAngle()), Mathf::Cos(pLight->GetFalloffAngle()), 0 , pLight->GetFalloffExponent());
	vec4Cone.Z = vec4Cone.Y - vec4Cone.X;
	Set(vec4Cone);
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorLightAttenuation(void)
{
	const Light* pLight = m_pRenderDeviceAdapter->GetLight(0);
	Vector3f vec3Attenuation(pLight->GetConstantAttenuation(), pLight->GetLinearAttenuation(), pLight->GetQuadraticAttenuation());
	Set(vec3Attenuation.Element, 3);
}
//----------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorArrayLightPosition(void)
{
	m_uiSize = 0;

	uint uiLightCount = MIN(m_pRenderDeviceAdapter->GetDeviceCapabilities(IRenderDevice::DCI_MAXIMUM_LIGHTS), ShaderProgramConstant::MaxVector);
	
	for (uint uiLightIndex = 0, uiShaderLightIndex = 0; uiLightIndex < uiLightCount; uiLightIndex++)
	{
		if (m_pRenderDeviceAdapter->GetLight(uiLightIndex)->IsLightEnabled()) 
			Set((const float*) m_pRenderDeviceAdapter->GetLight(uiLightIndex)->GetPosition(), uiShaderLightIndex++);
	}
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorArrayLightDirection(void)
{
	m_uiSize = 0;

	uint uiLightCount = MIN(m_pRenderDeviceAdapter->GetDeviceCapabilities(IRenderDevice::DCI_MAXIMUM_LIGHTS), ShaderProgramConstant::MaxVector);
	
	for (uint uiLightIndex = 0, uiShaderLightIndex = 0; uiLightIndex < uiLightCount; uiLightIndex++)
	{
		if (m_pRenderDeviceAdapter->GetLight(uiLightIndex)->IsLightEnabled()) 
			Set((const float*) m_pRenderDeviceAdapter->GetLight(uiLightIndex)->GetDirection(), uiShaderLightIndex++);
	}
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorArrayLightAmbient(void)
{
	m_uiSize = 0;

	uint uiLightCount = MIN(m_pRenderDeviceAdapter->GetDeviceCapabilities(IRenderDevice::DCI_MAXIMUM_LIGHTS), ShaderProgramConstant::MaxVector);
	
	for (uint uiLightIndex = 0, uiShaderLightIndex = 0; uiLightIndex < uiLightCount; uiLightIndex++)
	{
		if (m_pRenderDeviceAdapter->GetLight(uiLightIndex)->IsLightEnabled()) 
			Set(m_pRenderDeviceAdapter->GetLight(uiLightIndex)->GetAmbient(), uiShaderLightIndex++);
	}
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorArrayLightDiffuse(void)
{
	m_uiSize = 0;

	uint uiLightCount = MIN(m_pRenderDeviceAdapter->GetDeviceCapabilities(IRenderDevice::DCI_MAXIMUM_LIGHTS), ShaderProgramConstant::MaxVector);
	
	for (uint uiLightIndex = 0, uiShaderLightIndex = 0; uiLightIndex < uiLightCount; uiLightIndex++)
	{
		if (m_pRenderDeviceAdapter->GetLight(uiLightIndex)->IsLightEnabled()) 
			Set(m_pRenderDeviceAdapter->GetLight(uiLightIndex)->GetDiffuse(), uiShaderLightIndex++);
	}
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorArrayLightSpecular(void)
{
	m_uiSize = 0;

	uint uiLightCount = MIN(m_pRenderDeviceAdapter->GetDeviceCapabilities(IRenderDevice::DCI_MAXIMUM_LIGHTS), ShaderProgramConstant::MaxVector);
	
	for (uint uiLightIndex = 0, uiShaderLightIndex = 0; uiLightIndex < uiLightCount; uiLightIndex++)
	{
		if (m_pRenderDeviceAdapter->GetLight(uiLightIndex)->IsLightEnabled()) 
			Set(m_pRenderDeviceAdapter->GetLight(uiLightIndex)->GetSpecular(), uiShaderLightIndex++);
	}
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorArrayLightCone(void)
{
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorArrayLightAttenuation(void)
{
	m_uiSize = 0;

	Vector4f vec4Attenuation;
	const Light* pLight;

	uint uiLightCount = MIN(m_pRenderDeviceAdapter->GetDeviceCapabilities(IRenderDevice::DCI_MAXIMUM_LIGHTS), ShaderProgramConstant::MaxVector);
	
	for (uint uiLightIndex = 0, uiShaderLightIndex = 0; uiLightIndex < uiLightCount; uiLightIndex++)
	{
		pLight = m_pRenderDeviceAdapter->GetLight(uiLightIndex);
		if (pLight->IsLightEnabled())
		{
			vec4Attenuation.Set(pLight->GetConstantAttenuation(), pLight->GetLinearAttenuation(), pLight->GetQuadraticAttenuation(), 0);
			Set(vec4Attenuation, uiShaderLightIndex++);
		}
	}
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateIntegerLightActiveCount(void)
{
	m_uiSize = 0;

	uint uiLightCount = MIN(m_pRenderDeviceAdapter->GetDeviceCapabilities(IRenderDevice::DCI_MAXIMUM_LIGHTS), ShaderProgramConstant::MaxVector);
	int nCount = 0;

	for (uint uiLightIndex = 0; uiLightIndex < uiLightCount; uiLightIndex++)
	{
		if (m_pRenderDeviceAdapter->GetLight(uiLightIndex)->IsLightEnabled())
			nCount++;
	}
	
	Set(nCount);
}

//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorViewerPosition(void)
{
	Set(Vector4f(m_pRenderDeviceAdapter->GetViewport()->GetCamera()->GetPosition()));
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorViewerDirection(void)
{
	Set(Vector4f(m_pRenderDeviceAdapter->GetViewport()->GetCamera()->GetDirection()));
}
//----------------------------------------------------------------------------------------------
void RenderConstant::UpdateVectorAmbient(void)
{
	Set(m_pRenderDeviceAdapter->GetLightRenderState()->GetAmbient());
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END
