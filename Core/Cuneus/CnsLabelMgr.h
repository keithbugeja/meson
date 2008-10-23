#pragma once

//----------------------------------------------------------------------------------------------
//	Meson::Cuneus::Compiler::CuneusCompiler
//	
//  
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion	11/04/2008	1.0.0		First version
//----------------------------------------------------------------------------------------------

#include "CnsNamespaces.h"

#include <String8.h>
#include <Singleton.h>

Meson_Cuneus_Compiler_BEGIN

class LabelMgr : public Meson::Common::TSingleton< LabelMgr >
{
	friend class Meson::Common::TSingleton< LabelMgr >;

	static qword ms_qwCounter;

protected:
	LabelMgr( void );
	virtual ~LabelMgr( void );

public:
	Meson::Common::Text::String CreateLabel( void );
	Meson::Common::Text::String CreateLabel( Meson::Common::Text::String p_strPrefix );
};

Meson_Cuneus_Compiler_END
