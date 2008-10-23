//----------------------------------------------------------------------------------------------
//	Meson::Common::FileResourceDevice
//	Resource device interface.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Colin Vella		15/12/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "FileResourceDevice.h"

#include "Directory.h"
#include "HashMap.h"
#include "LinkedList.h"
#include "FileInputStream.h"
#include "FileOutputStream.h"

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Resources;

Meson_Common_Resources_BEGIN

struct FileResourceDeviceState
{
	String m_strFileRoot;
	Directory m_directoryRoot;
	THashMap<String, InputStreamPtr> m_mapInputStreams;
	THashMap<String, OutputStreamPtr> m_mapOutputStreams;

	FileResourceDeviceState(const String& p_strFileRoot)
		: m_strFileRoot(p_strFileRoot)
		, m_directoryRoot(p_strFileRoot)
		, m_mapInputStreams()
		, m_mapOutputStreams()
	{ }
};

Meson_Common_Resources_END

FileResourceDevice::FileResourceDevice(const String& p_strFileRoot)
	: m_pFileResourceDeviceState(NULL)
{
	m_pFileResourceDeviceState = new FileResourceDeviceState(p_strFileRoot);
}

FileResourceDevice::~FileResourceDevice(void)
{
	delete m_pFileResourceDeviceState;
}

void FileResourceDevice::EnumerateResources(const String& p_strResourcePath,
	TList<String>& p_listResourceNames)
{
	p_listResourceNames.Clear();

	TLinkedList<File> listFiles;
	m_pFileResourceDeviceState->m_directoryRoot.EnumerateFiles(listFiles);
	for (size_t unIndex = 0; unIndex < listFiles.Size(); unIndex++)
	{
		File& file = listFiles(unIndex);
		if (!file.IsDirectory())
		{
			p_listResourceNames.Add(file.GetPath());
			continue;
		}

		// otherwise, recurse into sub-directory
		TLinkedList<String> listSubResourceNames;
		EnumerateResources(file.GetPath(), listSubResourceNames);
		p_listResourceNames.AddList(listSubResourceNames);
	}
}

bool FileResourceDevice::ContainsResource(const String& p_strResourceName)
{
	try
	{
		String strFullPath = m_pFileResourceDeviceState->m_directoryRoot.GetPath()
			+ Directory::Delimeter + p_strResourceName;
		File fileResource(strFullPath);
		return !fileResource.IsDirectory();
	}
	catch (MesonException* pMesonException)
	{
		delete pMesonException;
		return false;
	}
}

InputStreamPtr FileResourceDevice::GetInputStream(const String& p_strResourceName)
{
	InputStreamPtr pInputStream;

	// attempt to re-use cached stream
	if (m_pFileResourceDeviceState->m_mapInputStreams.ContainsKey(
		p_strResourceName, pInputStream))
	{
		if (pInputStream->IsOpen())
			pInputStream->Close();
		pInputStream->Reopen();
		return pInputStream;
	}

	// otherwise, create new stream and cache it
	String strFullPath = m_pFileResourceDeviceState->m_directoryRoot.GetPath()
		+ Directory::Delimeter + p_strResourceName;
	pInputStream = new FileInputStream(strFullPath);
	m_pFileResourceDeviceState->m_mapInputStreams[p_strResourceName]
		= pInputStream;

	return pInputStream;
}

OutputStreamPtr FileResourceDevice::GetOutputStream(const String& p_strResourceName)
{
	OutputStreamPtr pOutputStream;

	// attempt to re-use cached stream
	if (m_pFileResourceDeviceState->m_mapOutputStreams.ContainsKey(
		p_strResourceName, pOutputStream))
	{
		if (pOutputStream->IsOpen())
			pOutputStream->Close();
		pOutputStream->Reopen();
		return pOutputStream;
	}

	// otherwise, create new stream and cache it
	String strFullPath = m_pFileResourceDeviceState->m_directoryRoot.GetPath()
		+ Directory::Delimeter + p_strResourceName;
	pOutputStream = new FileOutputStream(strFullPath);

	m_pFileResourceDeviceState->m_mapOutputStreams[p_strResourceName]
		= pOutputStream;

	return pOutputStream;
}
