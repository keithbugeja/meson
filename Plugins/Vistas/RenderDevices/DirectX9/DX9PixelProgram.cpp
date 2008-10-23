//----------------------------------------------------------------------------------------------
//  DX9PixelProgram
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
//  TODO: Implement support for trapping compilation errors and possibly correcting them.
//----------------------------------------------------------------------------------------------

#include "DX9RenderDevice.h"
#include "DX9PixelProgram.h"
#include "VstVistasEngine.h"
#include "VstRenderConstant.h"

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
DX9PixelProgram::DX9PixelProgram(LPDIRECT3DDEVICE9 p_pD3DDevice, IShaderProgramFactory* p_pCreator, const Meson::Common::Text::String& p_strProgramName)
	: m_pD3DDevice(p_pD3DDevice),
	  m_pD3DPixelShader(NULL),
	  m_pD3DConstantTable(NULL)
{
	m_strName = p_strProgramName;
	m_pCreator = p_pCreator;
	m_bIsCompiled = false;
	m_bIsBound = false;
}
//----------------------------------------------------------------------------------------------
DX9PixelProgram::DX9PixelProgram(LPDIRECT3DDEVICE9 p_pD3DDevice, IShaderProgramFactory* p_pCreator, const Meson::Common::Text::String& p_strFilename, const Meson::Common::Text::String& p_strProgramName)
	: m_pD3DDevice(p_pD3DDevice),
	  m_pD3DPixelShader(NULL),
	  m_pD3DConstantTable(NULL)
{
	m_strName = p_strProgramName;
	m_pCreator = p_pCreator;
	m_bIsCompiled = false;
	m_bIsBound = false;

	LPD3DXBUFFER pD3DShaderText,
				 pD3DShaderErrors;

	if (D3D_OK != D3DXPreprocessShaderFromFile(p_strFilename, NULL, NULL, &pD3DShaderText, &pD3DShaderErrors))
		throw new MesonException("Unable to open and preprocess shader program.", __FILE__, __LINE__);
	
	m_strSourceCode.Append((char*)pD3DShaderText->GetBufferPointer());

	SAFE_RELEASE(pD3DShaderText);
	SAFE_RELEASE(pD3DShaderErrors);
}
//----------------------------------------------------------------------------------------------
DX9PixelProgram::~DX9PixelProgram(void)
{
	// Delete Constant Table
	SAFE_RELEASE(m_pD3DConstantTable);

	// Delete Vertex Shader
	SAFE_RELEASE(m_pD3DPixelShader);
}
//----------------------------------------------------------------------------------------------
IShaderProgram::ShaderProgramType DX9PixelProgram::GetType(void) const
{
	return IShaderProgram::SPT_PIXEL;
}
//----------------------------------------------------------------------------------------------
void DX9PixelProgram::LoadConstant(ShaderProgramConstant* p_pShaderProgramConstant)
{
	switch(p_pShaderProgramConstant->GetType())
	{
		// Boolean constants
		case ShaderProgramConstant::DT_BOOLEAN:
			if (p_pShaderProgramConstant->IsArray())
			{
				// FIX: As is this function will NOT work. D3D takes an array of BOOL (not bool!) elements which is three bytes per element larger.
				m_pD3DConstantTable->SetBoolArray(m_pD3DDevice, p_pShaderProgramConstant->GetName(), (const BOOL*)p_pShaderProgramConstant->GetBooleanData(), p_pShaderProgramConstant->GetSize());
			}
			else
				m_pD3DConstantTable->SetBool(m_pD3DDevice, p_pShaderProgramConstant->GetName(), p_pShaderProgramConstant->GetBoolean());
			break;

		// Integer constants
		case ShaderProgramConstant::DT_INTEGER:
			if (p_pShaderProgramConstant->IsArray())
				m_pD3DConstantTable->SetIntArray(m_pD3DDevice, p_pShaderProgramConstant->GetName(), p_pShaderProgramConstant->GetIntegerData(), p_pShaderProgramConstant->GetSize());
			else
				m_pD3DConstantTable->SetInt(m_pD3DDevice, p_pShaderProgramConstant->GetName(), p_pShaderProgramConstant->GetInteger());
			break;

		// Float constants
		case ShaderProgramConstant::DT_FLOAT:
			if (p_pShaderProgramConstant->IsArray())
				m_pD3DConstantTable->SetFloatArray(m_pD3DDevice, p_pShaderProgramConstant->GetName(), p_pShaderProgramConstant->GetFloatData(), p_pShaderProgramConstant->GetSize());
			else
				m_pD3DConstantTable->SetFloat(m_pD3DDevice, p_pShaderProgramConstant->GetName(), p_pShaderProgramConstant->GetFloat());
			break;

		// Vector constants
		case ShaderProgramConstant::DT_VECTOR:
			if (p_pShaderProgramConstant->IsArray())
				m_pD3DConstantTable->SetVectorArray(m_pD3DDevice, p_pShaderProgramConstant->GetName(), (D3DXVECTOR4*)p_pShaderProgramConstant->GetFloatData(), p_pShaderProgramConstant->GetSize());
			else
				m_pD3DConstantTable->SetVector(m_pD3DDevice, p_pShaderProgramConstant->GetName(), (D3DXVECTOR4*)p_pShaderProgramConstant->GetFloatData());
			break;

		// Matrix constants
		/** Note that Vistas' matrices are trasposed with respect to Direct3D's. Therefore, 
			the matrices in the constant table are set as transposed matrices.
		 */
		case ShaderProgramConstant::DT_MATRIX:
			if (p_pShaderProgramConstant->IsArray())
				m_pD3DConstantTable->SetMatrixTransposeArray(m_pD3DDevice, p_pShaderProgramConstant->GetName(), (D3DXMATRIX*)p_pShaderProgramConstant->GetFloatData(), p_pShaderProgramConstant->GetSize());
			else
				m_pD3DConstantTable->SetMatrixTranspose(m_pD3DDevice, p_pShaderProgramConstant->GetName(), (D3DXMATRIX*)p_pShaderProgramConstant->GetFloatData());
			break;
	}
}
//----------------------------------------------------------------------------------------------
void DX9PixelProgram::LoadParameters(void)
{
	// Load all user variables. 
	ShaderProgramVariable* pShaderVariable = NULL;

	TEnumerator<ShaderProgramVariable*>& shaderEnumerator = m_shaderVariableMap.GetValueEnumerator();

	while(shaderEnumerator.HasMoreElements())
		LoadConstant(shaderEnumerator.NextElement());

	// Load all render constants
	DX9RenderDevice* pRenderDevice = static_cast<DX9RenderDevice*>(VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive());
	RenderConstant* pRenderConstant = NULL;

	TEnumerator<RenderConstantType::RenderConstantType>& constantEnumerator = m_renderConstantList.GetEnumerator();

	while(constantEnumerator.HasMoreElements())
	{
		pRenderConstant = pRenderDevice->GetRenderConstant(constantEnumerator.NextElement());
		pRenderConstant->Update();

		LoadConstant(pRenderConstant);
	}
}
//----------------------------------------------------------------------------------------------
void DX9PixelProgram::LoadVariable(const Meson::Common::Text::String& p_strVariableName)
{
	ShaderProgramVariable* pShaderVariable = NULL;

	if (m_shaderVariableMap.ContainsKey(p_strVariableName, pShaderVariable))
		LoadConstant(pShaderVariable);
	else
		throw new MesonException("Cannot load variable because no such variable has been found.", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void DX9PixelProgram::LoadRenderConstant(RenderConstantType::RenderConstantType p_renderConstantType)
{
	DX9RenderDevice* pRenderDevice = static_cast<DX9RenderDevice*>(VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive());
	RenderConstant* pRenderConstant = pRenderDevice->GetRenderConstant(p_renderConstantType);
	pRenderConstant->Update();
	LoadConstant(pRenderConstant);
}
//----------------------------------------------------------------------------------------------
void DX9PixelProgram::LoadFreeVariable(ShaderProgramVariable* p_pShaderVariable)
{
	LoadConstant(p_pShaderVariable);
}
//----------------------------------------------------------------------------------------------
void DX9PixelProgram::Unbind(void)
{
	if (m_bIsBound)
	{
		m_pD3DDevice->SetPixelShader(NULL);
		m_bIsBound = false;
	}
}
//----------------------------------------------------------------------------------------------
void DX9PixelProgram::Bind(void) 
{
	m_pD3DDevice->SetPixelShader(m_pD3DPixelShader);
	m_bIsBound = true;
}
//----------------------------------------------------------------------------------------------
// TODO: Tidy this up, adding error handling and recovery from compilation errors.
void DX9PixelProgram::Compile(void) 
{
	if (!m_bIsCompiled)
	{
		LPD3DXBUFFER pD3DShaderBuffer,
					 pD3DErrorMessageBuffer;

		m_bIsCompiled = (D3D_OK == D3DXCompileShader(	m_strSourceCode, 
														(UINT)m_strSourceCode.Size(), 
														NULL, 
														NULL, 
														m_strEntryPoint, 
														m_strProfile, 
														0,
														&pD3DShaderBuffer, 
														&pD3DErrorMessageBuffer, 
														&m_pD3DConstantTable));

		m_pD3DDevice->CreatePixelShader((DWORD*)pD3DShaderBuffer->GetBufferPointer(), &m_pD3DPixelShader);
		
		SAFE_RELEASE(pD3DShaderBuffer);
		SAFE_RELEASE(pD3DErrorMessageBuffer);
	}
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END