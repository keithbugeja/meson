#include "Directory.h"

#include "MesonException.h"
#include "Platform.h"

#ifdef PLATFORM_WIN32
	#include <Windows.h>
#else
	// other platform includes here...
#endif

using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Common_IO_BEGIN

#ifdef PLATFORM_WIN32

	struct DirectoryState
	{
		String m_strPath, m_strName;

		DirectoryState()
			: m_strPath()
			, m_strName()
		{
		}
	};

	const String Directory::Delimeter = "\\";

	Directory Directory::GetRoot(void)
	{
		return Directory(Directory::Delimeter);
	}

	Directory Directory::GetCurrent(void)
	{
		return Directory();
	}

#pragma warning( disable : 4748 )

	Directory::Directory(void)
		: m_pDirectoryState(NULL)
	{
		char szPath[MAX_PATH + 1];
		::GetCurrentDirectory(1023, szPath);

		WIN32_FIND_DATA win32FindData;
		HANDLE hFile = ::FindFirstFile(szPath, &win32FindData);

		m_pDirectoryState = new DirectoryState();
		m_pDirectoryState->m_strPath = szPath;
		m_pDirectoryState->m_strName = win32FindData.cFileName;
	}

	Directory::Directory(const String& p_strPath)
		: m_pDirectoryState(NULL)
	{
		WIN32_FIND_DATA win32FindData;
		HANDLE hFile = ::FindFirstFile(p_strPath, &win32FindData);

		if (hFile == INVALID_HANDLE_VALUE)
			throw new MesonException(
				"The path '" + p_strPath
				+ "' does not represent a valid file or directory.",
				__FILE__, __LINE__);
		::FindClose(hFile);

		if ((win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			throw new MesonException(
				"The path '" + p_strPath + "' does not refer to a directory.",
				__FILE__, __LINE__);

		char szFullPath[MAX_PATH + 1];
		::GetFullPathName(p_strPath, MAX_PATH, szFullPath, NULL);

		m_pDirectoryState = new DirectoryState();
		m_pDirectoryState->m_strPath = szFullPath;
		m_pDirectoryState->m_strName = win32FindData.cFileName;
	}

	Directory::~Directory(void)
	{
		delete m_pDirectoryState;
	}

	Directory& Directory::operator=(const Directory& p_directory)
	{
		m_pDirectoryState->m_strPath = p_directory.m_pDirectoryState->m_strPath;
		m_pDirectoryState->m_strName = p_directory.m_pDirectoryState->m_strName;

		return *this;
	}

	const String& Directory::GetPath(void) const
	{
		return m_pDirectoryState->m_strPath;
	}

	const String& Directory::GetName(void) const
	{
		return m_pDirectoryState->m_strName;
	}

	Directory Directory::GetParent(void) const
	{
		Directory directoryParent(m_pDirectoryState->m_strPath
			+ Directory::Delimeter + "..");
		if (directoryParent.GetPath() == m_pDirectoryState->m_strPath)
			throw new MesonException(
				"Cannot get the parent directory of a root directory.",
				__FILE__, __LINE__);

		return directoryParent;
	}

	void Directory::EnumerateFiles(TList<File>& p_listFiles)
	{
		String& strPath = m_pDirectoryState->m_strPath;

		WIN32_FIND_DATA win32FindData;
		String strPattern = m_pDirectoryState->m_strPath + Directory::Delimeter + "*";
		HANDLE hFile = ::FindFirstFile(strPattern, &win32FindData);

		p_listFiles.Clear();

		if (hFile == INVALID_HANDLE_VALUE)
			throw new MesonException(
				"Directory path '" + strPath + "' is no longer valid.",
				__FILE__, __LINE__);

		//NOTE: filename from FindFirstFile ignored as it is current '.' dir

		while (::FindNextFile(hFile, &win32FindData) != 0)
		{
			String strFilename = win32FindData.cFileName;
			if (strFilename == "..") continue;
			File file(strPath + Delimeter + strFilename);
			p_listFiles.Add(file);
		}

		::FindClose(hFile);
	}

	Directory Directory::CreateSubDirectory(const String& p_strName)
	{
		String strFullPath
			= m_pDirectoryState->m_strPath
				+ Directory::Delimeter + p_strName;
		if (::CreateDirectory(strFullPath, NULL) == 0)
			throw new MesonException(
				"Cannot create directory '" + strFullPath + "'.",
				__FILE__, __LINE__);

		return Directory(strFullPath);
	}

#else
	// other platform implementations here...

	const String Directory::Delimeter = "/";
#endif

Meson_Common_IO_END
