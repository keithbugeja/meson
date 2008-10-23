#include "File.h"

#include "MesonException.h"
#include "Directory.h"

#ifdef PLATFORM_WIN32
	#include <Windows.h>
#else
	// other platform includes here...
#endif

using namespace Meson::Common;
using namespace Meson::Common::Text;
using namespace Meson::Common::IO;

Meson_Common_IO_BEGIN

#ifdef PLATFORM_WIN32
#pragma warning( disable : 4748 )

	struct FileState
	{
		String m_strPath, m_strName;
		bool m_bDirectory, m_bReadOnly, m_bHidden;
		size_t m_unSize;
		Directory m_directoryParent;

		FileState(void)
			: m_strPath()
			, m_strName()
			, m_bDirectory(false)
			, m_bReadOnly(false)
			, m_bHidden(false)
			, m_unSize(0)
			, m_directoryParent()
		{
		}
	};

	File::File(void)
		: m_pFileState(NULL)
	{
	}

	File::File(const String &p_strPath)
		: m_pFileState(NULL)
	{	
		WIN32_FIND_DATA win32FindData;
		HANDLE hFile = ::FindFirstFile(p_strPath, &win32FindData);

		if (hFile == INVALID_HANDLE_VALUE)
			throw new MesonException(
				"The path '" + p_strPath
				+ "' does not represent a valid file or directory.",
				__FILE__, __LINE__);
		::FindClose(hFile);

		char szFullPath[MAX_PATH + 1];
		::GetFullPathName(p_strPath, MAX_PATH, szFullPath, NULL);
		DWORD dwAttributes = win32FindData.dwFileAttributes;

		String strDirectory = szFullPath;
		for (int nIndex = (int) strDirectory.Length() - 1; nIndex >= 0; nIndex--)
			if (strDirectory[nIndex] == Directory::Delimeter[0])
			{
				strDirectory = strDirectory.Substring(0, nIndex);
				break;
			}

		m_pFileState = new FileState();
		m_pFileState->m_strPath = szFullPath;
		m_pFileState->m_strName = win32FindData.cFileName;
		m_pFileState->m_bDirectory
			= (dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
		m_pFileState->m_bReadOnly
			= (dwAttributes & FILE_ATTRIBUTE_READONLY) != 0;
		m_pFileState->m_bHidden
			= (dwAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;
		m_pFileState->m_unSize = win32FindData.nFileSizeLow;
		m_pFileState->m_directoryParent = Directory(strDirectory);
	}

	File::File(const File &p_file)
		: m_pFileState(NULL)
	{
		if (p_file.m_pFileState == NULL) return;

		m_pFileState = new FileState();
		m_pFileState->m_strPath = p_file.m_pFileState->m_strPath;
		m_pFileState->m_strName = p_file.m_pFileState->m_strName;
		m_pFileState->m_bDirectory = p_file.m_pFileState->m_bDirectory;
		m_pFileState->m_bReadOnly = p_file.m_pFileState->m_bReadOnly;
		m_pFileState->m_bHidden = p_file.m_pFileState->m_bHidden;
		m_pFileState->m_unSize = p_file.m_pFileState->m_unSize;
		m_pFileState->m_directoryParent = p_file.m_pFileState->m_directoryParent;
	}

	File::~File(void)
	{
		if (m_pFileState != NULL)
			delete m_pFileState;
	}

	File &File::operator=(const File &p_file)
	{
		if (p_file.m_pFileState == NULL) return *this;

		if (m_pFileState == NULL)
			m_pFileState = new FileState();
		m_pFileState->m_strPath = p_file.m_pFileState->m_strPath;
		m_pFileState->m_strName = p_file.m_pFileState->m_strName;
		m_pFileState->m_bDirectory = p_file.m_pFileState->m_bDirectory;
		m_pFileState->m_bReadOnly = p_file.m_pFileState->m_bReadOnly;
		m_pFileState->m_bHidden = p_file.m_pFileState->m_bHidden;
		m_pFileState->m_unSize = p_file.m_pFileState->m_unSize;
		m_pFileState->m_directoryParent = p_file.m_pFileState->m_directoryParent;
		return *this;
	}

	bool File::operator==(const File &p_file)
	{
		if (m_pFileState == NULL) return false;
		if (p_file.m_pFileState == NULL) return false;
		return m_pFileState->m_strPath == p_file.m_pFileState->m_strPath;
	}

	const String &File::GetPath() const
	{
		return m_pFileState->m_strPath;
	}

	const String &File::GetName() const
	{
		return m_pFileState->m_strName;
	}

	bool File::IsDirectory(void) const
	{
		return m_pFileState->m_bDirectory;
	}

	bool File::IsReadOnly(void) const
	{
		return m_pFileState->m_bReadOnly;
	}

	bool File::IsHidden(void) const
	{
		return m_pFileState->m_bHidden;
	}

	size_t File::GetSize() const
	{ 
		return m_pFileState->m_unSize;
	}

	const Directory& File::GetParent(void) const
	{
		return m_pFileState->m_directoryParent;
	}

	void File::MoveTo(const Directory &p_directory)
	{
		String strNewPath = p_directory.GetPath()
			+ Directory::Delimeter + m_pFileState->m_strName;

		if (::MoveFile(m_pFileState->m_strPath, strNewPath) == 0)
			throw new MesonException(
				"Unable to move file or directory '"
				+ m_pFileState->m_strName + "' to directory '"
				+ p_directory.GetPath() + "'.",
				__FILE__, __LINE__);

		m_pFileState->m_strPath = strNewPath;
		m_pFileState->m_directoryParent = p_directory;
	}

	void File::RenameTo(const String &p_strNewName)
	{
		String strNewPath
			= m_pFileState->m_directoryParent.GetPath()
				+ Directory::Delimeter + p_strNewName;
		
		if (::MoveFile(m_pFileState->m_strPath, strNewPath) == 0)
			throw new MesonException(
				"Unable to rename file or directory '"
				+ m_pFileState->m_strName + "' to '"
				+ p_strNewName + "'.",
				__FILE__, __LINE__);

		m_pFileState->m_strPath = strNewPath;
		m_pFileState->m_strName = p_strNewName;
	}

	void File::Delete(void)
	{
		BOOL bResult = 0;

		if (m_pFileState->m_bDirectory)
			bResult = ::RemoveDirectory(m_pFileState->m_strPath);
		else
			bResult = ::DeleteFile(m_pFileState->m_strPath);

		if (bResult == 0) throw new MesonException(
			"Unable to delete file '"
			+ m_pFileState->m_strName + "'.",
			__FILE__, __LINE__);
	}

	bool File::Exists(const String &p_strPath)
	{	
		WIN32_FIND_DATA win32FindData;
		HANDLE hFile = ::FindFirstFile(p_strPath, &win32FindData);
		bool bExists = (hFile != INVALID_HANDLE_VALUE);

		::FindClose(hFile);
		return bExists;
	}

#else
	// other platform implementations here...

	const String Directory::Delimeter = "/";
#endif

Meson_Common_IO_END
