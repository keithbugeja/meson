#pragma once

#include <CnsNamespaces.h>
#include <CnsContext.h>

#define DEFAULT_STACK_SIZE 65536

Meson_Cuneus_BEGIN

namespace TaskState
{ 
	enum TaskState { New, Running, Interrupted, Terminated }; 
};


class Task
{
private:
	int m_nPriority,
		m_nCount;
	TaskState::TaskState m_eState;
	Meson::Cuneus::VCPU::Context m_context;

public:
	Task( int p_nStackSize = DEFAULT_STACK_SIZE );
	~Task( void );

	int GetPriority( void );
	void SetPriority( int p_nPriority );

	TaskState::TaskState GetState( void );
	void SetState( TaskState::TaskState p_eState );

	Meson::Cuneus::VCPU::Context *GetContext( void );

	int Count( void );

};

Meson_Cuneus_END
