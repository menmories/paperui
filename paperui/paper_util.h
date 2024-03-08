#ifndef __PAPER_UTIL_H__
#define __PAPER_UTIL_H__

#include "paper_define.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PAPER_API char* paper_util_new_guid();
	PAPER_API void paper_util_free_guid(char** guid);

#ifdef __cplusplus
}
#endif

#endif	//__PAPER_UTIL_H__
