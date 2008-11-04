//----------------------------------------------------------------------------------------------
//	Meson::Vistas::VistasEngine
//	Vistas 3D visualisation engine.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
//	Notes:
//		1. To extend lights to natively provide data structures capable of supporting
//		   area and volume lights.
//		2. To provide a tighter integration between the Material\Technique\Pass system
//		   and the lighting system.
//		3. Improve PotentialVisibility and Renderable sets, furnishing them with a query
//		   interface.
//----------------------------------------------------------------------------------------------
#pragma once

#include "Engine.h"
#include "VstTypes.h"

#include "VstLightManager.h"
#include "VstCameraManager.h"
#include "VstCullerManager.h"
#include "VstEffectManager.h"
#include "VstTextureManager.h"
#include "VstMaterialManager.h"
#include "VstRendererManager.h"
#include "VstGeometryManager.h"
#include "VstScenegraphManager.h"
#include "VstRepositoryManager.h"
#include "VstRenderDeviceManager.h"
#include "VstWindowMessageManager.h"
#include "VstVisibleEntitySetManager.h"

Meson_Vistas_BEGIN
	
	struct VistasEngineState;

	typedef Meson::Common::Collections::TEnumerator<IRenderDevice*>& RenderDeviceEnumerator;

	class VISTAS_API VistasEngine : public Meson::Common::TEngine<VistasEngine>
	{
		friend class Meson::Common::TEngine<VistasEngine>;

	private:
		VistasEngineState *m_pVistasEngineState;

	protected:
		/** The engine is in itself a singleton; the constructor and destructor 
		    are protected and may only be accessed through the <i>GetInstance</i> or
			<i>Delete</i> functions.
		 */
		VistasEngine();
		~VistasEngine();

	public:
		/** Methods through which the engine provides identification.
		 */
		const Meson::Common::Text::String& GetName(void);
		const Meson::Common::Version& GetVersion(void);

		/** Managers are retrieved through the interface provided by the engine.
		 */
		LightManager* GetLightManager(void) const;
		CameraManager* GetCameraManager(void) const;
		CullerManager* GetCullerManager(void) const;
		EffectManager* GetEffectManager(void) const;
		TextureManager* GetTextureManager(void) const;
		MaterialManager* GetMaterialManager(void) const;
		RendererManager* GetRendererManager(void) const;
		GeometryManager* GetGeometryManager(void) const;
		ScenegraphManager* GetScenegraphManager(void) const;
		RepositoryManager* GetRepositoryManager(void) const;
		RenderDeviceManager* GetRenderDeviceManager(void) const;
		WindowMessageManager* GetWindowMessageManager(void) const;
		HardwareResourceManager* GetHardwareResourceManager(void) const;
		VisibleEntitySetManager* GetVisibleEntitySetManager(void) const;
	};

Meson_Vistas_END