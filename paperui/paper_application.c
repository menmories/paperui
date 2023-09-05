#include "paper_application.h"

#include "paper_render.h"
#include "paper_event.h"
#include "paper_window.h"


static int8* szPath = NULL;
static uint32 uPathLen = 0;
#define PAPER_MAX_PATH 4096
int32 paper_application_init()
{
	//初始化paper渲染器环境
	szPath = (int8*)malloc(PAPER_MAX_PATH);
	if (szPath)
	{
		memset(szPath, 0, PAPER_MAX_PATH);
		uPathLen = GetModuleFileNameA(GetModuleHandle(NULL), szPath, PAPER_MAX_PATH - 2);
	}
	if (paper_render_initenv() != 0)
	{
		return -1;
	}
	paper_window_set_default_eventcb();
	return 0;
}

void paper_application_get_path(int8** path, uint32* len)
{
	assert(path);
	assert(len);
	*path = szPath;
	* len = uPathLen;
}

int32 paper_application_run()
{
	int32 code = paper_event_dispatch();
	paper_render_destroyenv();
	free(szPath);
	return code;
}


#define	CRC	0xa001
unsigned int Get_Crc(unsigned char* p, unsigned char len)
{
	unsigned char i = 0;
	unsigned int tmp = 0xffff;
	while (len > 0)
	{
		tmp ^= *p++;
		while (i < 8)
		{
			if (tmp & 0x0001)
			{
				tmp >>= 1;
				tmp ^= CRC;
			}
			else
			{
				tmp >>= 1;
			}
			i++;
		}
		i = 0;
		len--;
	}
	return tmp;
}
