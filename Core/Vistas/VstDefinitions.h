#pragma once

#ifndef NULL 
	#define NULL 0
#endif

#ifndef SAFE_DELETE
	#define SAFE_DELETE(p) {if(p) {delete (p); (p)=NULL;}}
#endif

#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p) {if(p) {delete[] (p); (p)=NULL;}}
#endif
