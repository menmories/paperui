#ifndef __PAPER_MEMORYPOOL_H__
#define __PAPER_MEMORYPOOL_H__

#include "paper_define.h"

#ifdef __cplusplus
extern "C"
{
#endif
	struct paper_memorypool;

	struct paper_memorypool* paper_memorypool_create(uint32 block_size);

	void* paper_memorypool_alloc(struct paper_memorypool* pool);

	void paper_memorypool_dealloc(struct paper_memorypool* pool, void* block);

	void paper_memorypool_free(struct paper_memorypool* pool);

#ifdef __cplusplus
}
#endif

#endif