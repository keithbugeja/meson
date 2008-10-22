//----------------------------------------------------------------------------------------------
//	Meson::Common::TEngine<T>
//	Templated class for Meson Engine classes.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Logger.h"
#include "ConsoleOutputDevice.h"

#include "RandomGeneric.h"
#include "PlugInManager.h"
#include "Converter.h"
#include "Singleton.h"
#include "String8.h"

//----------------------------------------------------------------------------------------------
// Namespaces
//----------------------------------------------------------------------------------------------
Meson_Common_BEGIN

//----------------------------------------------------------------------------------------------
// TEngine template
//----------------------------------------------------------------------------------------------
template <typename TClass> class TEngine
{
protected:
    static TClass* m_pInstance;
	Meson::Common::Logging::Logger* m_logger;
	Meson::Common::Random::RandomGeneric* m_pRandom;
	Meson::Common::System::LoggingDevice::ConsoleOutputDevice* m_pConsoleOutputDevice;

protected:
	TEngine() {}
	virtual ~TEngine() {};

private:
    TEngine(const TEngine& p_tEngine) {};

public:
	//----------------------------------------------------------------------------------------------
	/** Set singleton instance. Note that this method should only be employed where linking 
		difficulties prevent correct initialisation of the singleton's static elements.
		\param p_pInstance Instance of the class singleton.
	 */
	//----------------------------------------------------------------------------------------------
	static void SetInstance(TClass* p_pInstance)
	{
		m_pInstance = p_pInstance;
	}

	//----------------------------------------------------------------------------------------------
	/** Returns the instance pointer to the engine singleton class.
	 */
	//----------------------------------------------------------------------------------------------
    static TClass* GetInstance(void)
    {
        if (m_pInstance == NULL) 
		{
			m_pInstance = new TClass();
			Meson::Common::PlugIn::PlugInManager::GetInstance()->RegisterSharedObject(m_pInstance, m_pInstance->GetName());
			
			m_pInstance->m_pConsoleOutputDevice = new Meson::Common::System::LoggingDevice::ConsoleOutputDevice();
			m_pInstance->m_pRandom = new Meson::Common::Random::RandomGeneric();
			m_pInstance->m_logger = new Meson::Common::Logging::Logger( m_pInstance->GetName() );
			m_pInstance->m_logger->Out.SetOutputDevice(m_pInstance->m_pConsoleOutputDevice);
		}

		MESON_ASSERT(m_pInstance != NULL, "Instance pointer is NULL!");
        return m_pInstance;
    };

	//----------------------------------------------------------------------------------------------
	/** Terminates the singleton instance.
	 */
	//----------------------------------------------------------------------------------------------
    static void Delete(void)
    {
		if (m_pInstance != NULL)
		{
			Meson::Common::PlugIn::PlugInManager::GetInstance()->UnregisterSharedObject(m_pInstance->GetName());

			//Meson::Common::Logging::Logger::Erase(m_pInstance->GetName());
			delete m_pInstance->m_logger;
			m_pInstance->m_logger = NULL;

			delete m_pInstance->m_pRandom;
			m_pInstance->m_pRandom = NULL;

			delete m_pInstance->m_pConsoleOutputDevice;
			m_pInstance->m_pConsoleOutputDevice = NULL;

			delete m_pInstance;
			m_pInstance = NULL;
		}
    };

	//----------------------------------------------------------------------------------------------
	/** Returns the name of the singleton engine. Note that derived classes must
		implement this method.
	 */
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Text::String& GetName(void) = 0;

	//----------------------------------------------------------------------------------------------
	/** Returns the version of the singleton engine. Note that derived classes must
		implement this method.
	 */
	//----------------------------------------------------------------------------------------------
	virtual const Meson::Common::Version& GetVersion(void) = 0;
	
	//----------------------------------------------------------------------------------------------
	/** Returns a general purpose random generator.
	 */
	//----------------------------------------------------------------------------------------------
	Meson::Common::Random::RandomGeneric& Random(void)
	{
		return *(m_pInstance->m_pRandom);
	}

	//----------------------------------------------------------------------------------------------
	/** Returns a general purpose logger which outputs to console.
	 */ 
	//----------------------------------------------------------------------------------------------
	Meson::Common::Logging::Logger& Logger(void)
	{
		return *m_logger;
	}

	//----------------------------------------------------------------------------------------------
	/** Generates a unique identifier and returns it encoded as a string. 
	 */
	//----------------------------------------------------------------------------------------------
	Meson::Common::Text::String GenerateGuid(void)
	{
		String strGuid;

		Converter::ToHex(m_pInstance->m_pRandom->Next(), strGuid);
		strGuid += "-";
		Converter::ToHex(m_pInstance->m_pRandom->Next(), strGuid);
		strGuid += "-";
		Converter::ToHex(m_pInstance->m_pRandom->Next(), strGuid);
		strGuid += "-";
		Converter::ToHex(m_pInstance->m_pRandom->Next(), strGuid);

		return strGuid;
	}

	//----------------------------------------------------------------------------------------------
	/** Loads a PlugIn from a dynamic library
	 */
	//----------------------------------------------------------------------------------------------
	Meson::Common::PlugIn::IPlugIn* LoadPlugIn(const Meson::Common::Text::String& p_strPlugIn) 
	{ 
		#ifdef MESON_PLUGIN
			throw new MesonException("Plug-In chaining is not allowed.", __FILE__, __LINE__);
		#else
			return Meson::Common::PlugIn::PlugInManager::GetInstance()->Load(p_strPlugIn); 
		#endif
	}

	//----------------------------------------------------------------------------------------------
	/** Unloads a PlugIn coming from a dynamic library
	 */
	//----------------------------------------------------------------------------------------------
	void UnloadPlugIn(const Meson::Common::Text::String& p_strPlugIn)
	{
		#ifdef MESON_PLUGIN
			throw new MesonException("Plug-In chaining is not allowed.", __FILE__, __LINE__);
		#else
			Meson::Common::PlugIn::PlugInManager::GetInstance()->Unload(p_strPlugIn);
		#endif
	}
};

template <typename TClass> TClass* TEngine<TClass>::m_pInstance = NULL;

Meson_Common_END