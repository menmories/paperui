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

	PAPER_API void paper_rect_set_width(struct paper_rect* rect, int32 width);

	PAPER_API int32 paper_rect_get_width(const struct paper_rect* rect);

	PAPER_API int32 paper_rect_get_height(const struct paper_rect* rect);

	PAPER_API void paper_rect_set_height(struct paper_rect* rect, int32 height);

	PAPER_API void paper_rect_set_size(struct paper_rect* rect, int32 width, int32 height);

	PAPER_API void paper_rect_set_pos(struct paper_rect* rect, int32 x, int32 y);

	PAPER_API void paper_rect_center_rect(struct paper_rect* rect, const struct paper_rect* src);

	PAPER_API void paper_rect_hcenter_rect(struct paper_rect* rect, const struct paper_rect* src);

	PAPER_API void paper_rect_vcenter_rect(struct paper_rect* rect, const struct paper_rect* src);

	/*
	* @function:判断一个2D坐标点是否则矩形区域范围内
	* @return 
	* 在区域范围内返回1，否则返回0
	*/
	PAPER_API int8 paper_rect_pt_in(const struct paper_rect* rect, int32 x, int32 y);

#ifdef __cplusplus
}
#endif

#endif	//__PAPER_ELEMENT_H__
