#include "paper_elememt.h"

void paper_rect_set_width(struct paper_rect* rect, int32 width)
{
	rect->right = rect->left + width;
}

int32 paper_rect_get_width(struct paper_rect* rect)
{
	return rect->right - rect->left;
}

int32 paper_rect_get_height(struct paper_rect* rect)
{
	return rect->bottom - rect->top;
}

void paper_rect_set_height(struct paper_rect* rect, int32 height)
{
	rect->bottom = rect->top + height;
}

void paper_rect_set_size(struct paper_rect* rect, int32 width, int32 height)
{
	rect->right = rect->left + width;
	rect->bottom = rect->top + height;
}

void paper_rect_set_pos(struct paper_rect* rect, int32 x, int32 y)
{
	int32 width = rect->right - rect->left;
	int32 height = rect->bottom - rect->top;
	rect->left = x;
	rect->top = y;
	rect->right = rect->left + width;
	rect->bottom = rect->top + height;
}

int8 paper_rect_pt_in(struct paper_rect* rect, int32 x, int32 y)
{
	if (rect->left < x && rect->right >= x && rect->top < y && rect->bottom >= y)
	{
		return 1;
	}
	return 0;
}
