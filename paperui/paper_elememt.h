#ifndef __PAPER_ELEMENT_H__
#define __PAPER_ELEMENT_H__

#include "paper_define.h"

#ifdef __cplusplus
extern "C"
{
#endif
	struct paper_point {
		int32 x;
		int32 y;
	};

	struct paper_size {
		int32 width;
		int32 height;
	};

	struct paper_rect {
		int32 left;
		int32 top;
		int32 right;
		int32 bottom;
	};

	struct paper_color
	{
		float r;
		float g;
		float b;
		float a;
	};

	void paper_rect_set_width(struct paper_rect* rect, int32 width);

	int32 paper_rect_get_width(struct paper_rect* rect);

	int32 paper_rect_get_height(struct paper_rect* rect);

	void paper_rect_set_height(struct paper_rect* rect, int32 height);

	void paper_rect_set_size(struct paper_rect* rect, int32 width, int32 height);

	void paper_rect_set_pos(struct paper_rect* rect, int32 x, int32 y);

	/*
	* @function:�ж�һ��2D������Ƿ����������Χ��
	* @return 
	* ������Χ�ڷ���1�����򷵻�0
	*/
	int8 paper_rect_pt_in(struct paper_rect* rect, int32 x, int32 y);

#ifdef __cplusplus
}
#endif

#endif	//__PAPER_ELEMENT_H__
