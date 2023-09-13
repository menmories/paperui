#include "paper_vector.h"
#include <malloc.h>
#include <string.h>
#include <assert.h>

#define BASE_ALLOCELEMENT 4
struct paper_vector
{
    uint8  alloc_count;      //当前分配的块数量
    uint32 element_size;    //元素大小
    uint32 element_count;   //存在的元素个数
    uint8* block;
};

struct paper_vector* paper_vector_create(uint32 element_size)
{
    struct paper_vector* vec = (struct paper_vector*)malloc(sizeof(struct paper_vector));
    if (!vec)
    {
        return NULL;
    }
    vec->alloc_count = BASE_ALLOCELEMENT;
    vec->element_count = 0;
    vec->element_size = element_size;
    uint32 alloc_size = vec->alloc_count * vec->element_size;
    vec->block = (uint8_t*)malloc(alloc_size);
    if (!vec->block)
    {
        free(vec);
        return NULL;
    }
    return vec;
}

uint32 paper_vector_get_blocksize(struct paper_vector* vec)
{
    return vec->alloc_count* vec->element_size;
}

void paper_vector_add(struct paper_vector* vec, void* data)
{
    if (vec->element_count >= vec->alloc_count)
    {
        vec->alloc_count *= 2;
        uint32 alloc_size = vec->alloc_count * vec->element_size;
        void* block = realloc(vec->block, alloc_size);
        if (!block)
        {
            return;
        }
        vec->block = block;
    }
    uint32 used_blocksize = vec->element_size * vec->element_count;
    void* addr = vec->block + used_blocksize;
    memcpy(addr, data, vec->element_size);
    vec->element_count++;
}

void paper_vector_add_front(struct paper_vector* vec, void* data)
{
	if (vec->element_count == vec->alloc_count)
	{
		vec->alloc_count *= 2;
		uint32 alloc_size = vec->alloc_count * vec->element_size;
		void* block = realloc(vec->block, alloc_size);
		if (!block)
		{
			return;
		}
		vec->block = block;
	}
    paper_vector_right_move_element(vec);
	memcpy(vec->block, data, vec->element_size);
	vec->element_count++;
}

void paper_vector_right_move_element(struct paper_vector* vec)
{
    uint32 allelement_size = vec->element_count * vec->element_size;
    uint8* last = vec->block + allelement_size;
    uint8* prev = vec->block + allelement_size - vec->element_size;
	for (uint32 i = 0; i < vec->element_count; i++)
	{
        memcpy(last, prev, vec->element_size);
        if (prev == vec->block)
        {
            break;
        }
        prev -= vec->element_size;
        last -= vec->element_size;
	}
}

void paper_vector_remove(struct paper_vector* vec)
{
    assert(vec->element_count > 0);
    vec->element_count--;
}

void paper_vector_remove_front(struct paper_vector* vec)
{
    assert(vec->element_count > 0);
    uint32 copyed_size = vec->element_count - 1;
    memcpy(vec->block, vec->block + vec->element_size, copyed_size * vec->element_size);
	vec->element_count = copyed_size;
}

void paper_vector_get_value(struct paper_vector* vec, uint32 pos, void* data)
{
    uint32 real_pos = vec->element_size * pos;
    memcpy(data, vec->block + real_pos, vec->element_size);
}

void paper_vector_pop_front(struct paper_vector* vec, void* data)
{
	uint32 copyed_size = (vec->element_count - 1) * vec->element_size;
	memcpy(data, vec->block, vec->element_size);
	memcpy(vec->block, vec->block + vec->element_size, copyed_size);
	vec->element_count--;
}

void paper_vector_pop_back(struct paper_vector* vec, void* data)
{
    assert(vec->element_count > 0);
    uint32 cur_size = vec->element_size * vec->element_count;
    memcpy(data, vec->block + (cur_size - vec->element_size), vec->element_size);
    vec->element_count--;
}

void paper_vector_free(struct paper_vector* vec)
{
    free(vec->block);
    free(vec);
}

uint32 paper_vector_get_count(struct paper_vector* vec)
{
    return vec->element_count;
}
