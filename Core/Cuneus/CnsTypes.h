#pragma once

#include <Types.h>

#ifdef CUNEUS_EXPORTS
	#define CUNEUS_API __declspec(dllexport)
#else
	#define CUNEUS_API __declspec(dllimport)
#endif

