//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Singleton.h"

#include "VstTypes.h"

Meson_Vistas_BEGIN

	//----------------------------------------------------------------------------------------------
	class RepositoryManager : public Meson::Common::TSingleton<RepositoryManager>
	{
		friend class Meson::Common::TSingleton<RepositoryManager>;

	private:
		RepositoryManager(void);
		~RepositoryManager(void);
	
	public:
		void RegisterPath(const Meson::Common::Text::String& p_strResourcePath, bool p_bRelative=false);
		void UnregisterPath(const Meson::Common::Text::String& p_strResourcePath, bool p_bRelative=false);
		void UnregisterAllPaths(void);

		bool ContainsResource(const Meson::Common::Text::String& p_strResourceName, Meson::Common::Text::String& p_strFullResourcePath);

	protected:
		Meson::Common::Collections::TArrayList<Meson::Common::Text::String> m_repositoryPathList;
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END