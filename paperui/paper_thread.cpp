#include "paper_thread.h"
#include <stdlib.h>
#include <process.h>
#include <Windows.h>
#include <processthreadsapi.h>

struct threaddata
{
	threadcb cb;
	void* data;
};

static uint32 __stdcall __thread_proc(void* param)
{
	struct threaddata* thdata = (struct threaddata*)param;
	uint32 ret = thdata->cb(thdata->data);
	free(thdata);
	_endthread();
	return ret;
}


struct paper_thread* paper_thread_new()
{
	struct paper_thread* thread = (struct paper_thread*)malloc(sizeof(struct paper_thread));
	if (thread)
	{
		memset(thread, 0, sizeof(struct paper_thread));
	}
	return thread;
}

void paper_thread_free(struct paper_thread* th)
{
	free(th);
}

int32 paper_thread_start(struct paper_thread* thread, threadcb cb, void* data)
{
	assert(thread);
	struct threaddata* thdata = (struct threaddata*)malloc(sizeof(struct threaddata));
	if (thdata)
	{
		thdata->cb = cb;
		thdata->data = data;
	}
	thread->threadHandle = (uint_ptr)_beginthreadex(nullptr, 0, __thread_proc, thdata, CREATE_SUSPENDED, &thread->threadId);
	if (!thread->threadHandle)
	{
		return -1;
	}
	ResumeThread((HANDLE)thread->threadHandle);
	return 0;
}

PAPER_API void paper_thread_suspend(struct paper_thread* thread)
{
	SuspendThread((HANDLE)thread->threadHandle);
}

PAPER_API void paper_thread_resume(struct paper_thread* thread)
{
	ResumeThread((HANDLE)thread->threadHandle);
}

PAPER_API int32 paper_thread_wait(struct paper_thread* thread, uint32 wtime)
{
	DWORD dwRet = WaitForSingleObject((HANDLE)thread->threadHandle, wtime);
	if (dwRet == WAIT_OBJECT_0)
	{
		return 0;
	}
	if (dwRet == WAIT_TIMEOUT)
	{
		return 1;
	}
	return -1;
}

void paper_thread_sleep(uint32 wtime)
{
	Sleep(wtime);
}

struct paper_mutex
{
	CRITICAL_SECTION cs;
};

struct paper_mutex* paper_mutex_new()
{
	struct paper_mutex* mtx = (struct paper_mutex*)malloc(sizeof(struct paper_mutex));
	if (mtx)
	{
		memset(mtx, 0, sizeof(struct paper_mutex));
		InitializeCriticalSection(&mtx->cs);
	}
	return mtx;
}

void paper_mutex_free(struct paper_mutex* mtx)
{
	assert(mtx);
	DeleteCriticalSection(&mtx->cs);
	free(mtx);
}

PAPER_API void paper_mutex_enter(struct paper_mutex* mtx)
{
	assert(mtx);
	EnterCriticalSection(&mtx->cs);
}

PAPER_API void paper_mutex_leave(struct paper_mutex* mtx)
{
	assert(mtx);
	LeaveCriticalSection(&mtx->cs);
}

PAPER_API int32 paper_mutex_try_enter(struct paper_mutex* mtx)
{
	assert(mtx);
	return TryEnterCriticalSection(&mtx->cs);
}
