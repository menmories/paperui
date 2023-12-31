#ifndef __PAPER_APPLICATION_H__
#define __PAPER_APPLICATION_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "paper_define.h"
	PAPER_API int32 paper_application_init();

	//获取当前应用程序路径
	PAPER_API void paper_application_get_path(int8** path, uint32* len);

	PAPER_API int32 paper_application_run();
#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__PAPER_APPLICATION_H__
