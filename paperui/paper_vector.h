#ifndef __PAPER_VECTOR_H__
#define __PAPER_VECTOR_H__

#include "paper_define.h"

#ifdef __cplusplus
extern "C"
{
#endif
	struct paper_vector;

	struct paper_vector* paper_vector_create(uint32 element_size);

	uint32 paper_vector_get_blocksize(struct paper_vector* vec);

	void paper_vector_add(struct paper_vector* vec, void* data);

	void paper_vector_add_front(struct paper_vector* vec, void* data);

	void paper_vector_right_move_element(struct paper_vector* vec);

	void paper_vector_remove(struct paper_vector* vec);

	void paper_vector_remove_front(struct paper_vector* vec);

	void paper_vector_get_value(struct paper_vector* vec, uint32 pos, void* data);

	void paper_vector_pop_front(struct paper_vector* vec, void* data);

	void paper_vector_pop_back(struct paper_vector* vec, void* data);

	//void paper_vector_pop_back_ptr(struct paper_vector* vec, void** data);

	void paper_vector_free(struct paper_vector* vec);

	uint32 paper_vector_get_count(struct paper_vector* vec);
#ifdef __cplusplus
}
#endif

#endif	//__PAPER_VECTOR_H__
