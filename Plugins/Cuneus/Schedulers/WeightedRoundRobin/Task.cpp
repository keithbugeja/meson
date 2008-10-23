#include "Task.h"

using namespace Meson::Cuneus::VCPU;

Meson_Cuneus_BEGIN

Task::Task( int p_nStackSize ) : m_context( p_nStackSize )
{
	m_nPriority = 1;
	m_nCount = 0;
	m_eState = TaskState::New;
}

Task::~Task( void )
{

}

int Task::GetPriority( void )
{
	return m_nPriority;
}

void Task::SetPriority( int p_nPriority )
{
	m_nPriority = p_nPriority;
}

TaskState::TaskState Task::GetState( void )
{
	return m_eState;
}

void Task::SetState( TaskState::TaskState p_eState )
{
	m_eState = p_eState;
}


Context *Task::GetContext( void )
{
	return &m_context;
}

int Task::Count( void )
{
	if( m_nCount == 0 )
		m_nCount = m_nPriority;

	m_nCount--;
	return m_nCount;
}


Meson_Cuneus_END
