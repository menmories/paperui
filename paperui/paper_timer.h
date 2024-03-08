#ifndef __PAPER_TIMER_H__
#define __PAPER_TIMER_H__

#include "paper_define.h"

#ifdef __cplusplus
extern "C"
{
#endif
	typedef void (*TimerProcCb)(void* data);

	struct paper_timer;


	struct paper_timer* paper_timer_start(uint32 timeInterval, TimerProcCb cb, void* data);

	void paper_timer_free(struct paper_timer* timer);
#ifdef __cplusplus
}
#endif
#endif	//__PAPER_TIMER_H__
