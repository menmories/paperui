#include "paper_application.h"

#include "paper_render.h"
#include "paper_event.h"
#include "paper_window.h"
int32 paper_application_init()
{
	//初始化paper渲染器环境
	if (paper_render_initenv() != 0)
	{
		return -1;
	}
	paper_window_set_default_eventcb();
	return 0;
}

int32 paper_application_run()
{
	int32 code = paper_event_dispatch();
	paper_render_destroyenv();
	return code;
}
