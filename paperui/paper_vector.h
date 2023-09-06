#ifndef __PAPER_VECTOR_H__
#define __PAPER_VECTOR_H__

#include "paper_define.h"

#ifdef __cplusplus
extern "C"
{
#endif
	struct paper_vector;

	/*
	* @function������һ��vector����
	* @param element_size ����Ԫ�صĴ�С������sizeof(int32)
	* @return
	* �ɹ�����struct paper_vectorָ�룬ʧ�ܷ���NULL
	*/
	PAPER_API struct paper_vector* paper_vector_create(uint32 element_size);

	/*
	* @function����ȡ��ǰvector����������ڴ���С
	* @param vec �������
	* @return
	* ���ص�ǰvector����������ڴ���С
	*/
	PAPER_API uint32 paper_vector_get_blocksize(struct paper_vector* vec);

	/*
	* @function��������ĩβ���һ��Ԫ��
	* @param vec �������
	* @param data Ҫ��ӵ�����
	* @return ��
	*/
	PAPER_API void paper_vector_add(struct paper_vector* vec, void* data);

	/*
	* @function��������ͷ�����һ��Ԫ��
	* @param vec �������
	* @param data Ҫ��ӵ�����
	* @return ��
	*/
	PAPER_API void paper_vector_add_front(struct paper_vector* vec, void* data);

	/*
	* @function������������Ԫ�������ƶ�һ��λ��
	* @param vec �������
	* @return ��
	*/
	PAPER_API void paper_vector_right_move_element(struct paper_vector* vec);

	/*
	* @function���Ƴ�ĩβ��һ��Ԫ��
	* @param vec �������
	* @return ��
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
