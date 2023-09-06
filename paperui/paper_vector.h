#ifndef __PAPER_VECTOR_H__
#define __PAPER_VECTOR_H__

#include "paper_define.h"

#ifdef __cplusplus
extern "C"
{
#endif
	struct paper_vector;

	/*
	* @function：创建一个vector容器
	* @param element_size 单个元素的大小，比如sizeof(int32)
	* @return
	* 成功返回struct paper_vector指针，失败返回NULL
	*/
	PAPER_API struct paper_vector* paper_vector_create(uint32 element_size);

	/*
	* @function：获取当前vector容器分配的内存块大小
	* @param vec 容器句柄
	* @return
	* 返回当前vector容器分配的内存块大小
	*/
	PAPER_API uint32 paper_vector_get_blocksize(struct paper_vector* vec);

	/*
	* @function：从容器末尾添加一个元素
	* @param vec 容器句柄
	* @param data 要添加的数据
	* @return 无
	*/
	PAPER_API void paper_vector_add(struct paper_vector* vec, void* data);

	/*
	* @function：从容器头部添加一个元素
	* @param vec 容器句柄
	* @param data 要添加的数据
	* @return 无
	*/
	PAPER_API void paper_vector_add_front(struct paper_vector* vec, void* data);

	/*
	* @function：将容器所有元素往右移动一个位置
	* @param vec 容器句柄
	* @return 无
	*/
	PAPER_API void paper_vector_right_move_element(struct paper_vector* vec);

	/*
	* @function：移除末尾的一个元素
	* @param vec 容器句柄
	* @return 无
	*/
	PAPER_API void paper_vector_remove(struct paper_vector* vec);

	PAPER_API void paper_vector_remove_front(struct paper_vector* vec);

	PAPER_API void paper_vector_get_value(struct paper_vector* vec, uint32 pos, void* data);

	PAPER_API void paper_vector_pop_front(struct paper_vector* vec, void* data);

	PAPER_API void paper_vector_pop_back(struct paper_vector* vec, void* data);

	PAPER_API void paper_vector_free(struct paper_vector* vec);

	PAPER_API uint32 paper_vector_get_count(struct paper_vector* vec);
#ifdef __cplusplus
}
#endif

#endif	//__PAPER_VECTOR_H__
