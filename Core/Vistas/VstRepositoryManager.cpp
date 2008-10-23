//----------------------------------------------------------------------------------------------
#include "VstRepositoryManager.h"

#include "File.h"
#include "Directory.h"

using namespace Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN
//----------------------------------------------------------------------------------------------
RepositoryManager::RepositoryManager(void) 
{ 
	m_repositoryPathList.Add(Directory::GetCurrent().GetPath());
}
//----------------------------------------------------------------------------------------------
RepositoryManager::~RepositoryManager(void) 
{ 
}
//----------------------------------------------------------------------------------------------
void RepositoryManager::RegisterPath(const Meson::Common::Text::String& p_strResourcePath, bool p_bRelative)
{
	if (p_bRelative)
		m_repositoryPathList.Add(Directory::GetCurrent().GetPath() + Directory::Delimeter + p_strResourcePath);
	else
		m_repositoryPathList.Add(p_strResourcePath);
}
//----------------------------------------------------------------------------------------------
void RepositoryManager::UnregisterPath(const Meson::Common::Text::String& p_strResourcePath, bool p_bRelative)
{
	if (p_bRelative)
		m_repositoryPathList.Remove(Directory::GetCurrent().GetPath() + Directory::Delimeter + p_strResourcePath);
	else
		m_repositoryPathList.Remove(p_strResourcePath);
}
//----------------------------------------------------------------------------------------------
void RepositoryManager::UnregisterAllPaths(void)
{
	m_repositoryPathList.Clear();
}
//----------------------------------------------------------------------------------------------
bool RepositoryManager::ContainsResource(const Meson::Common::Text::String& p_strResourceName, Meson::Common::Text::String& p_strFullResourcePath)
{
	// If absolute path, return immediately
	if (File::Exists(p_strResourceName))
	{
		p_strFullResourcePath = p_strResourceName;
		return true;
	}

	// Go through path list 
	TEnumerator<String>& pathEnumerator = m_repositoryPathList.GetEnumerator();

	while(pathEnumerator.HasMoreElements())
	{
		p_strFullResourcePath = pathEnumerator.NextElement() + Directory::Delimeter + p_strResourceName;

		if (File::Exists(p_strFullResourcePath))
			return true;
	}

	p_strFullResourcePath.Clear();
	return false;
}
//----------------------------------------------------------------------------------------------
Meson_Vistas_END