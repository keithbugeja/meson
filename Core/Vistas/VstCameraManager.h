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
#include "VstCamera.h"

Meson_Vistas_BEGIN
	class CameraManager : public Meson::Common::TSingleton<CameraManager>
	{
		friend class Meson::Common::TSingleton<CameraManager>;

	private: 
		CameraManager(void);
		~CameraManager(void);

	public:
		CameraPtr CreateCamera(const Meson::Common::Text::String& p_strId);

		bool ContainsCamera(const Meson::Common::Text::String& p_strId);
		bool ContainsCamera(const Meson::Common::Text::String& p_strId, CameraPtr& p_pCamera);

		CameraPtr GetCamera(const Meson::Common::Text::String& p_strId);

		void RegisterCamera(CameraPtr p_pCamera, bool p_bReplace=true);

		void UnregisterCamera(CameraPtr p_pCamera);
		void UnregisterCamera(const Meson::Common::Text::String& p_strId);
		void UnregisterAllCameras(void);

		void LoadCamera(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadCameras(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream);
		void LoadCameras(const Meson::Common::Text::String& p_strResourceName);

	private:
		Meson::Common::Collections::THashMap<Meson::Common::Text::String, CameraPtr> m_cameraMap;
	};

Meson_Vistas_END
