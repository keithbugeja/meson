//----------------------------------------------------------------------------------------------
//	Meson::Common::FileResourceDevice
//	Resource device interface.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		15/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "ResourceDevice.h"

Meson_Common_Resources_BEGIN

struct FileResourceDeviceState;

class FileResourceDevice
	: public Meson::Common::Resources::IResourceDevice
{
private: // private variables
	FileResourceDeviceState* m_pFileResourceDeviceState;

public: // public methods
	FileResourceDevice(const Meson::Common::Text::String& p_strFileRoot);
	~FileResourceDevice(void);

	// IResourceDevice interface
	void EnumerateResources(const Meson::Common::Text::String& p_strResourcePath,
		Meson::Common::Collections::TList<Meson::Common::Text::String>& p_listResourceNames);
	bool ContainsResource(const Meson::Common::Text::String& p_strResourceName);

	Meson::Common::IO::InputStreamPtr GetInputStream(
		const Meson::Common::Text::String& p_strResourceName);
	Meson::Common::IO::OutputStreamPtr GetOutputStream(
		const Meson::Common::Text::String& p_strResourceName);
};

Meson_Common_Resources_END
