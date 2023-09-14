#include "paper_elememt.h"

void paper_rect_set_width(struct paper_rect* rect, int32 width)
{
	rect->right = rect->left + width;
}

int32 paper_rect_get_width(const struct paper_rect* rect)
{
	return rect->right - rect->left;
}

int32 paper_rect_get_height(const struct paper_rect* rect)
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

void paper_rect_center_rect(struct paper_rect* rect, const struct paper_rect* src)
{
	int32 width = paper_rect_get_width(rect);
	int32 height = paper_rect_get_height(rect);
	int32 x = (paper_rect_get_width(src) - width) / 2;
	int32 y = (paper_rect_get_height(src) - height) / 2;
	rect->left = x;
	rect->top = y;
	rect->right = x + width;
	rect->bottom = y + height;
}

void paper_rect_hcenter_rect(struct paper_rect* rect, const struct paper_rect* src)
{
	int32 width = paper_rect_get_width(rect);
	int32 x = (paper_rect_get_width(src) - width) / 2;
	rect->left = x;
	rect->right = x + width;
}

void paper_rect_vcenter_rect(struct paper_rect* rect, const struct paper_rect* src)
{
	int32 height = paper_rect_get_height(rect);
	int32 y = (paper_rect_get_height(src) - height) / 2;
	rect->top = y;
	rect->bottom = y + height;
}

int8 paper_rect_pt_in(const struct paper_rect* rect, int32 x, int32 y)
{
	if (rect->left < x && rect->right >= x && rect->top < y && rect->bottom >= y)
	{
		return 1;
	}
	return 0;
}
