//----------------------------------------------------------------------------------------------
//	Meson::Common::TSingleton<T>
//	Templated class for singleton classes.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Namespaces.h"
#include "Definitions.h"
#include "Types.h"

#include "DebugSupport.h"
#include "MesonException.h"

Meson_Common_BEGIN

template <typename TClass> class TSingleton
{
protected:
	static TClass* m_pInstance;

	TSingleton() { }
	virtual ~TSingleton() { };

private:
    TSingleton(const TSingleton& p_tSingleton) { };

public:
	//----------------------------------------------------------------------------------------------
	/// Return a pointer to the singleton instance
	//----------------------------------------------------------------------------------------------
    static TClass* GetInstance(void)
    {
        if (m_pInstance == NULL) 
			m_pInstance = new TClass();

		MESON_ASSERT(m_pInstance != NULL, "Singleton instance is NULL!");
        return m_pInstance;
    }

	//----------------------------------------------------------------------------------------------
	/// Delete singleton instance
	//----------------------------------------------------------------------------------------------
    static void Delete(void)
    {
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
    }
};

template <typename TClass> TClass* TSingleton<TClass>::m_pInstance = NULL;

Meson_Common_END
