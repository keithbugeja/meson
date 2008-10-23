//----------------------------------------------------------------------------------------------
//	Meson::Common::Version
//  Declaration for Version class
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "Types.h"
#include "Namespaces.h"
#include "MesonException.h"
#include "DebugSupport.h"

#include "String8.h"

Meson_Common_BEGIN

class Version
{
private:
	dword	m_dwMajor, 
			m_dwMinor,
			m_dwBuild;
public:
	Version();
	Version( Version &p_version );
	Version( dword p_dwMajor, dword p_dwMinor, dword p_dwBuild );
	
	void Set( dword p_dwMajor, dword p_dwMinor, dword p_dwBuild );

	dword GetMajor(void);
	dword GetMinor(void);
	dword GetBuild(void);

	void SetMajor(dword p_dwMajor);
	void SetMinor(dword p_dwMinor);
	void SetBuild(dword p_dwBuild);

	Version& operator= (Version& p_version);
	bool operator== (Version& p_version);
	bool operator!= (Version& p_version);
	bool operator> (Version& p_version);
	bool operator< (Version& p_version);
	bool operator<= (Version& p_version);
	bool operator>= (Version& p_version);
};

Meson_Common_END