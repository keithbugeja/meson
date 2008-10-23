//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "MesonException.h"
#include "VstVistasEngine.h"
#include "VstPass.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
IPass::IPass(void)
	: m_strName()
{
}
//----------------------------------------------------------------------------------------------
IPass::IPass(const String& p_strName)
	: m_strName(p_strName)
{
}
//----------------------------------------------------------------------------------------------
IPass::~IPass(void)
{
	RemoveAllRenderStates();
	RemoveAllShaderPrograms();
}
//----------------------------------------------------------------------------------------------
const String& IPass::GetName(void) const
{
	return m_strName;
}
//----------------------------------------------------------------------------------------------
void IPass::BeginPass(bool p_bSaveState)
{
	IRenderDevice* pRenderDevice = VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive();
	TEnumerator<RenderStatePtr>& renderStateEnumerator = m_renderStateMap.GetValueEnumerator();
	
	if (p_bSaveState)
	{
		while (renderStateEnumerator.HasMoreElements())
		{
			RenderStatePtr pRenderState = renderStateEnumerator.NextElement();
			pRenderDevice->PushRenderState(pRenderState->GetType());
			pRenderDevice->SetRenderState(pRenderState);
		}
	}
	else
	{
		while (renderStateEnumerator.HasMoreElements())
			pRenderDevice->SetRenderState(renderStateEnumerator.NextElement());
	}

	IShaderProgram* pShaderProgram;
	TEnumerator<IShaderProgram*>& shaderProgramEnumerator = m_shaderProgramMap.GetValueEnumerator();

	while(shaderProgramEnumerator.HasMoreElements())
	{
		pShaderProgram = shaderProgramEnumerator.NextElement();
		pRenderDevice->BindShaderProgram(pShaderProgram);
		pShaderProgram->LoadParameters();		
	}
}
//----------------------------------------------------------------------------------------------
void IPass::EndPass(bool p_bRestoreState)
{
	if (p_bRestoreState)
	{
		IRenderDevice* pRenderDevice = VistasEngine::GetInstance()->GetRenderDeviceManager()->GetActive();
		TEnumerator<RenderStatePtr>& renderStateEnumerator = m_renderStateMap.GetValueEnumerator();
	
		while (renderStateEnumerator.HasMoreElements())
		{
			RenderStatePtr pRenderState = renderStateEnumerator.NextElement();
			pRenderDevice->PopRenderState(pRenderState->GetType());
		}
	}
}
//----------------------------------------------------------------------------------------------
void IPass::AddRenderState(IRenderState* p_pRenderState)
{
	m_renderStateMap.Insert(p_pRenderState->GetType(), RenderStatePtr(p_pRenderState));
}
//----------------------------------------------------------------------------------------------
void IPass::RemoveRenderState(IRenderState::RenderStateType p_eRenderStateType)
{
	if (m_renderStateMap.ContainsKey(p_eRenderStateType))
	{
		m_renderStateMap[p_eRenderStateType] = NULL;
		m_renderStateMap.RemoveKey(p_eRenderStateType);
	}
	
	throw new MesonException("Cannot remove render state : Render state does not exist", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
IRenderState* IPass::GetRenderState(IRenderState::RenderStateType p_eRenderStateType)
{
	if (m_renderStateMap.ContainsKey(p_eRenderStateType))
		return m_renderStateMap[p_eRenderStateType];

	throw new MesonException("Cannot return render state : Render state does not exist", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void IPass::RemoveAllRenderStates(void)
{
	TEnumerator<RenderStatePtr>& renderStateEnumerator = m_renderStateMap.GetValueEnumerator();
	
	while (renderStateEnumerator.HasMoreElements())
		renderStateEnumerator.NextElement() = NULL;
	
	m_renderStateMap.Clear();
}
//----------------------------------------------------------------------------------------------
bool IPass::ContainsRenderState(IRenderState::RenderStateType p_eRenderStateType)
{
	return (m_renderStateMap.ContainsKey(p_eRenderStateType));
}
//----------------------------------------------------------------------------------------------
void IPass::SetShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType, IShaderProgram* p_pShaderProgram)
{
	m_shaderProgramMap.Insert(p_eShaderProgramType, p_pShaderProgram);
}
//----------------------------------------------------------------------------------------------
IShaderProgram* IPass::GetShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType)
{
	if (m_shaderProgramMap.ContainsKey(p_eShaderProgramType))
		return m_shaderProgramMap[p_eShaderProgramType];

	throw new MesonException("Cannot return shader program : shader program does not exist", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void IPass::RemoveShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType)
{
	if (m_shaderProgramMap.ContainsKey(p_eShaderProgramType))
	{
		m_shaderProgramMap[p_eShaderProgramType] = NULL;
		m_shaderProgramMap.RemoveKey(p_eShaderProgramType);
	}

	throw new MesonException("Cannot remove shader program : shader program does not exist", __FILE__, __LINE__);
}
//----------------------------------------------------------------------------------------------
void IPass::RemoveAllShaderPrograms(void)
{
	TEnumerator<IShaderProgram*>& shaderProgramEnumerator = m_shaderProgramMap.GetValueEnumerator();
	
	while (shaderProgramEnumerator.HasMoreElements())
		shaderProgramEnumerator.NextElement() = NULL;
	
	m_shaderProgramMap.Clear();
}
//----------------------------------------------------------------------------------------------
bool IPass::ContainsShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType)
{
	return (m_shaderProgramMap.ContainsKey(p_eShaderProgramType));
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END