#include "Scheduler.h"

#include "CnsTypeDWord.h"

using namespace Meson::Common;
using namespace Meson::Cuneus::VCPU;
using namespace Meson::Cuneus::RTS::Typing;
using namespace Meson::Cuneus::RTS::Reflection;

Meson_Cuneus_BEGIN

Scheduler::Scheduler(void)
{
	m_dwTaskIndex = 0;
	m_pCurrentTask = NULL;
}

Scheduler::~Scheduler(void)
{
	FreePointers( m_lstTasks );
}

void Scheduler::Startup( Meson::Common::Config::Configuration &p_config ) {}
void Scheduler::Shutdown( void ) {}

Context *Scheduler::CreateTask( TypeMethod *p_pMethod, int p_nPriority = 1 )
{
	Task *pTask = new Task();
	pTask->GetContext()->Reset();

	pTask->GetContext()->Registers.PC = new TypeDWord( p_pMethod->GetByteCodeOffset() );
	pTask->GetContext()->Registers.FP = new TypeDWord( 1 );
	pTask->GetContext()->Registers.SP = new TypeDWord( 1 );

	pTask->GetContext()->Stack.Push( TPointer<DataType>( new TypeDWord( 0xFFFFFFFF ) ) );

	pTask->GetContext()->SetIndex( (dword) m_lstTasks.Size() );
	m_lstTasks += pTask;
	return pTask->GetContext();
}

Context *Scheduler::NextTask( void )
{
	size_t dwNum = m_lstTasks.Size();

	if( dwNum == 0 )
		return NULL;

	m_dwTaskIndex++;
	if( m_dwTaskIndex >= dwNum )
		m_dwTaskIndex = 0;

	m_pCurrentTask = m_lstTasks( m_dwTaskIndex );
	return m_pCurrentTask->GetContext();
}

Context *Scheduler::GetCurrentTask( void )
{
	return m_pCurrentTask->GetContext();
}

void Scheduler::TerminateTask( Context *p_pContext )
{
	if( m_pCurrentTask->GetContext() == p_pContext )
	{
		NextTask();
	}

	dword dwIndex = p_pContext->GetIndex();
	
	m_lstTasks.RemoveAt( dwIndex );
	size_t dwNum = m_lstTasks.Size();

	for( dword dw = dwIndex; dw < dwIndex; dw++ )
	{
		m_lstTasks(dw)->GetContext()->SetIndex( dw );
	}
}

Meson_Cuneus_END
