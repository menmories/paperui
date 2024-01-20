#ifndef __PAPER_THREAD_H__
#define __PAPER_THREAD_H__

#include "paper_define.h"
#include "paper_elememt.h"

#ifdef __cplusplus
extern "C"
{
#endif
	typedef uint32(*threadcb)(void* params);

	struct paper_thread
	{
		uint32 threadId;
		uint_ptr threadHandle;
	};

	PAPER_API struct paper_thread* paper_thread_new();

	/*@brief ���ʹ����paper_thread_new������һ�����̣���ôʹ�øú������ͷ�*/
	PAPER_API void paper_thread_free(struct paper_thread* th);
	
	PAPER_API int32 paper_thread_start(struct paper_thread* thread, threadcb cb, void* data);

	/*
	* @brief ��һ���̹߳���
	*/
	PAPER_API void paper_thread_suspend(struct paper_thread* thread);

	/*
	* @brief ��һ��������ָ̻߳�����
	*/
	PAPER_API void paper_thread_resume(struct paper_thread* thread);

	/*
	* @brief : �ȴ�һ���߳�ִ�����
	* @param thread Ҫ�ȴ����߳�
	* @param wtime �ȴ�ʱ�䣬�Ժ���Ϊ��λ
	* @return
	* �ɹ�����0
	* ��ʱ����1
	* ʧ�ܷ���-1
	*/
	PAPER_API int32 paper_thread_wait(struct paper_thread* thread, uint32 wtime);

	PAPER_API void paper_thread_sleep(uint32 wtime);



	struct paper_mutex;

	PAPER_API struct paper_mutex* paper_mutex_new();

	PAPER_API void paper_mutex_free(struct paper_mutex* mtx);

	PAPER_API void paper_mutex_enter(struct paper_mutex* mtx);

	PAPER_API void paper_mutex_leave(struct paper_mutex* mtx);

	PAPER_API int32 paper_mutex_try_enter(struct paper_mutex* mtx);

#ifdef __cplusplus
}
#endif	//__cplusplus


#endif	//__PAPER_THREAD_H__