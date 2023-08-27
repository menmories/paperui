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

#ifdef __cplusplus
}
#endif

#endif	//__PAPER_ELEMENT_H__
