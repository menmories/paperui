#include "paper_timer.h"
//#include <threadpoollegacyapiset.h>
#include <stdlib.h>
#include <Windows.h>
static HANDLE hTimerQueue = nullptr;

struct paper_timer
{
	uint_ptr handle;
	uint32 time_interval;
	TimerProcCb cb;
	void* data;
};

void NTAPI __TimerQueueProc(PVOID pvContext, BOOLEAN fTimerOrWaitFired)
{
	struct paper_timer* timer = (struct paper_timer*)pvContext;
	timer->cb(timer->data);
}
struct paper_timer* paper_timer_start(uint32 timeInterval, TimerProcCb cb, void* data)
{
	struct paper_timer* timer = (struct paper_timer*)malloc(sizeof(struct paper_timer));
	if (timer)
	{
		HANDLE handle = nullptr;
		if (!hTimerQueue)
		{
			hTimerQueue = CreateTimerQueue();
		}
		timer->cb = cb;
		timer->data = data;
		if (!CreateTimerQueueTimer(&handle, hTimerQueue, __TimerQueueProc, (void*)timer, 0, timeInterval, 0))
		{
			free(timer);
			return nullptr;
		}
		timer->handle = (uint_ptr)handle;
		timer->time_interval = timeInterval;
	}
	return timer;
}

void paper_timer_free(struct paper_timer* timer)
{
	assert(timer);
	if (hTimerQueue)
	{
		if (timer->handle)
		{
			if (DeleteTimerQueueTimer(hTimerQueue, (HANDLE)timer->handle, nullptr))
			{

			}
		}
	}
	free(timer);
}
