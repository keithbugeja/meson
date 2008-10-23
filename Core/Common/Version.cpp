//----------------------------------------------------------------------------------------------
//	Meson::Common::Version
//  Implementation for Version class
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

// Core Includes
#include "Version.h"

// namespace Meson.Common
Meson_Common_BEGIN

//----------------------------------------------------------------------------------------------
/// Constructs a new version object.
//----------------------------------------------------------------------------------------------
Version::Version() : m_dwMajor(0), m_dwMinor(0), m_dwBuild(0) 
{
}

//----------------------------------------------------------------------------------------------
/// Constructs a new version object using Major, Minor and Build values.
/// \param p_dwMajor Major version number
/// \param p_dwMinor Minor version number
/// \param p_dwBuild Build number
//----------------------------------------------------------------------------------------------
Version::Version( dword p_dwMajor, dword p_dwMinor, dword p_dwBuild ) : m_dwMajor(p_dwMajor), m_dwMinor(p_dwMinor), m_dwBuild(p_dwBuild) 
{
}

//----------------------------------------------------------------------------------------------
/// Copy constructor for version object.
/// \param p_version Reference to version object
//----------------------------------------------------------------------------------------------
Version::Version( Version &p_version )
{
	*this = p_version;
}
	
//----------------------------------------------------------------------------------------------
/// Sets major, minor and build versions
/// \param p_dwMajor Major version number
/// \param p_dwMinor Minor version number
/// \param p_dwBuild Build number
//----------------------------------------------------------------------------------------------
void Version::Set( dword p_dwMajor, dword p_dwMinor, dword p_dwBuild )
{
	m_dwMajor = p_dwMajor;
	m_dwMinor = p_dwMinor;
	m_dwBuild = p_dwBuild;
}

//----------------------------------------------------------------------------------------------
/// Returns major version number
/// \return Major version number
//----------------------------------------------------------------------------------------------
dword Version::GetMajor(void)
{
	return m_dwMajor;
}

//----------------------------------------------------------------------------------------------
/// Returns minor version number
/// \return Minor version number
//----------------------------------------------------------------------------------------------
dword Version::GetMinor(void)
{
	return m_dwMinor;
}

//----------------------------------------------------------------------------------------------
/// Returns build number
/// \return Build number
//----------------------------------------------------------------------------------------------
dword Version::GetBuild(void)
{
	return m_dwBuild;
}

//----------------------------------------------------------------------------------------------
/// Sets major version number
/// \param p_dwMajor Major version number
//----------------------------------------------------------------------------------------------
void Version::SetMajor(dword p_dwMajor)
{
	m_dwMajor = p_dwMajor;
}

//----------------------------------------------------------------------------------------------
/// Sets minor version number
/// \param p_dwMinor Minor version number
//----------------------------------------------------------------------------------------------
void Version::SetMinor(dword p_dwMinor)
{
	m_dwMinor = p_dwMinor;
}

//----------------------------------------------------------------------------------------------
/// Sets build number
/// \param p_dwBuild Build number
//----------------------------------------------------------------------------------------------
void Version::SetBuild(dword p_dwBuild)
{
	m_dwBuild = p_dwBuild;
}

//----------------------------------------------------------------------------------------------
/// Assignment operator
/// \param p_version Reference to Version object
/// \return Reference to Version object
//----------------------------------------------------------------------------------------------
Version& Version::operator= (Version& p_version)
{
	m_dwMajor = p_version.m_dwMajor;
	m_dwMinor = p_version.m_dwMinor;
	m_dwBuild = p_version.m_dwBuild;

	return p_version;
}

//----------------------------------------------------------------------------------------------
/// Equality operator
/// \param p_version Reference to Version object
/// \return True if equal, false otherwise
//----------------------------------------------------------------------------------------------
bool Version::operator== (Version& p_version)
{
	return m_dwMajor == p_version.m_dwMajor && 
		   m_dwMinor == p_version.m_dwMinor && 
		   m_dwBuild == p_version.m_dwBuild;
}

//----------------------------------------------------------------------------------------------
/// Inequality operator
/// \param p_version Reference to Version object
/// \return False if equal, true otherwise
//----------------------------------------------------------------------------------------------
bool Version::operator!= (Version& p_version)
{
	return !(m_dwMajor == p_version.m_dwMajor && 
		     m_dwMinor == p_version.m_dwMinor && 
		     m_dwBuild == p_version.m_dwBuild);
}

//----------------------------------------------------------------------------------------------
/// Comparison operator
/// \param p_version Reference to Version object
/// \return True if greater, false otherwise
//----------------------------------------------------------------------------------------------
bool Version::operator> (Version& p_version)
{
	if ( m_dwMajor != p_version.m_dwMajor )
		return (m_dwMajor > p_version.m_dwMajor);

	if ( m_dwMinor != p_version.m_dwMinor )
		return (m_dwMinor > p_version.m_dwMinor);

	return (m_dwBuild > p_version.m_dwBuild);
}

//----------------------------------------------------------------------------------------------
/// Comparison operator
/// \param p_version Reference to Version object
/// \return True if smaller, false otherwise
//----------------------------------------------------------------------------------------------
bool Version::operator< (Version& p_version)
{
	if ( m_dwMajor != p_version.m_dwMajor )
		return (m_dwMajor < p_version.m_dwMajor);

	if ( m_dwMinor != p_version.m_dwMinor )
		return (m_dwMinor < p_version.m_dwMinor);

	return (m_dwBuild < p_version.m_dwBuild);
}

//----------------------------------------------------------------------------------------------
/// Comparison operator
/// \param p_version Reference to Version object
/// \return True if less or equal, false otherwise
//----------------------------------------------------------------------------------------------
bool Version::operator<= (Version& p_version)
{
	return (*this==p_version || *this<p_version);
}

//----------------------------------------------------------------------------------------------
/// Comparison operator
/// \param p_version Reference to Version object
/// \return True if greater or equal, false otherwise
//----------------------------------------------------------------------------------------------
bool Version::operator>= (Version& p_version)
{
	return (*this==p_version || *this>p_version);
}

Meson_Common_END