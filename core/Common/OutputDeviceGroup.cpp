
#include "OutputDeviceGroup.h"

Meson_Common_System_LoggingDevice_BEGIN


OutputDeviceGroup::OutputDeviceGroup(void)
{

}

OutputDeviceGroup::~OutputDeviceGroup(void)
{

}

void OutputDeviceGroup::Output( char p_cValue )
{
	size_t unSize = m_lstDevice.Size();
	for( size_t unPos=0; unPos < unSize; unPos++)
	{
		m_lstDevice[ unPos ]->Output( p_cValue );
	}
}

void OutputDeviceGroup::Output( const char *p_cValue, size_t p_unSize )
{
	size_t unSize = m_lstDevice.Size();
	for( size_t unPos=0; unPos < unSize; unPos++)
	{
		m_lstDevice[ unPos ]->Output( p_cValue, p_unSize );
	}
}

void OutputDeviceGroup::AddDevice( IOutputDevice *p_pOutputDevice )
{
	if( m_lstDevice.IndexOf( p_pOutputDevice ) == -1 )
		m_lstDevice.Add( p_pOutputDevice );
}

void OutputDeviceGroup::RemoveDevice( IOutputDevice *p_pOutputDevice )
{
	m_lstDevice.Remove( p_pOutputDevice );
}

Meson_Common_System_LoggingDevice_END
