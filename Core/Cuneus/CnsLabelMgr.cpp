#include "CnsLabelMgr.h"

using namespace Meson::Common::Text;


Meson_Cuneus_Compiler_BEGIN

qword LabelMgr::ms_qwCounter = 0;

LabelMgr::LabelMgr( void )
{

}

LabelMgr::~LabelMgr( void )
{

}

String LabelMgr::CreateLabel( void )
{
	return CreateLabel( "L" );
}

String LabelMgr::CreateLabel( String p_strPrefix )
{
	return p_strPrefix + (ms_qwCounter++);
}

Meson_Cuneus_Compiler_END
