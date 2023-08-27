#include "paper_memorypool.h"
#include <stdlib.h>
#include "paper_vector.h"
struct paper_memorypool
{
	struct paper_vector* pool;
	uint32 alloc_count;		//分配过的数量
	uint32 block_size;
};

#define PTR_SIZE sizeof(void*)

struct paper_memorypool* paper_memorypool_create(uint32 block_size)
{
	struct paper_memorypool* pool = (struct paper_memorypool*)malloc(sizeof(struct paper_memorypool));
	if (!pool)
	{
		return NULL;
	}
	pool->pool = paper_vector_create(PTR_SIZE);
	pool->alloc_count = 4;
	pool->block_size = block_size;
	for (uint32 i = 0; i < pool->alloc_count; i++)
	{
		void* block = malloc(pool->block_size);
		paper_vector_add(pool->pool, &block);
	}
	return pool;
}

void* paper_memorypool_alloc(struct paper_memorypool* pool)
{
	pool->alloc_count++;
	void* block = NULL;
	if (paper_vector_get_count(pool->pool) > 0)
	{
		//paper_vector_pop_back_ptr(pool->pool, &block);
		paper_vector_pop_back(pool->pool, &block);
	}
	else
	{
		block = malloc(pool->block_size);
	}
	return block;
}

void paper_memorypool_dealloc(struct paper_memorypool* pool, void* block)
{
	paper_vector_add(pool->pool, &block);
	pool->alloc_count--;
}

void paper_memorypool_free(struct paper_memorypool* pool)
{
	paper_vector_free(pool->pool);
	free(pool);
}
