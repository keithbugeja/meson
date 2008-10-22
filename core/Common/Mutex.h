#pragma once

#include "Namespaces.h"
#include "Types.h"

Meson_Common_System_BEGIN

struct MutexState;

class Mutex
{
private:
	MutexState *m_pMutexState;

public:
	Mutex(void);
	Mutex(uint *p_punContext);
	~Mutex(void);
	void Lock(void);
	void Unlock(void);
};

Meson_Common_System_END
