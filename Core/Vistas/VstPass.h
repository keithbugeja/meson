//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"

#include "VstTypes.h"
#include "VstLight.h"
#include "VstRenderState.h"
#include "VstShaderProgram.h"

Meson_Vistas_BEGIN
	//----------------------------------------------------------------------------------------------
	// TODO: Add free shader variables loading to allow per instance variables.
	//----------------------------------------------------------------------------------------------
	class IPass : public Meson::Common::TReferenceCounter<IPass>
	{
		friend class Technique;

	public:
		enum PassType
		{
			PT_AMBIENT_PASS,
			PT_ILLUMINATION_PASS,
			PT_EFFECT_PASS,

			PT_COUNT
		};

	public:
		virtual void BeginPass(bool p_bSaveState=false);
		virtual void EndPass(bool p_bRestoreState=false);
	
	public:
		IPass(void);
		IPass(const Meson::Common::Text::String& p_strName);
		virtual ~IPass(void);
	
		const Meson::Common::Text::String& GetName(void) const;
		virtual PassType GetType(void) const = 0;

		virtual void AddRenderState(IRenderState* p_pRenderState);
		virtual void RemoveRenderState(IRenderState::RenderStateType p_eRenderStateType);
		virtual IRenderState* GetRenderState(IRenderState::RenderStateType p_eRenderStateType);
		virtual void RemoveAllRenderStates(void);

		virtual bool ContainsRenderState(IRenderState::RenderStateType p_eRenderStateType);

		virtual void SetShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType, IShaderProgram* p_pShaderProgram);
		virtual IShaderProgram* GetShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType);
		virtual void RemoveShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType);
		virtual void RemoveAllShaderPrograms(void);

		virtual bool ContainsShaderProgram(IShaderProgram::ShaderProgramType p_eShaderProgramType);

	protected:
		Meson::Common::Collections::THashMap<IRenderState::RenderStateType, RenderStatePtr> m_renderStateMap;
		Meson::Common::Collections::THashMap<IShaderProgram::ShaderProgramType, IShaderProgram*> m_shaderProgramMap;

		Meson::Common::Text::String m_strName;
	};

	typedef  Meson::Common::TPointer<IPass> PassPtr;
	//----------------------------------------------------------------------------------------------
Meson_Vistas_END