#ifndef __PAPER_RENDER_H__
#define __PAPER_RENDER_H__

#include "paper_define.h"
#include "paper_elememt.h"

#ifdef __cplusplus
extern "C"
{
#endif
	struct paper_render;
	struct paper_image;
	struct paper_font;
	struct paper_brush;

	//paper brush begin
	/*
	Direct2D有以下几种brush：
	Solid color brush（纯色画刷）：用于填充区域的纯色画刷。
	Gradient brush（渐变画刷）：可以创建线性渐变、径向渐变、角度渐变等效果的画刷。
	Bitmap brush（位图画刷）：可以使用位图填充区域的画刷。
	Radial gradient brush（径向渐变画刷）：可以创建从一个中心点向外扩散的径向渐变效果。
	LinearGradient brush（线性渐变画刷）：可以创建沿着一条直线方向渐变的画刷。
	SolidColorBrush（实心画刷）：用于创建纯色的实心画刷。
	Effect brush（特效画刷）：可以使用某种特效或变换来绘制图像的画刷。
	这些画刷可以根据需要进行组合和使用，以实现丰富多样的绘图效果。
	*/
	//D2D1_GRADIENT_STOP;
	struct paper_gradient_stop
	{
		float position;
		struct paper_color color;
	};

	struct paper_gradient_pos
	{
		struct paper_point start_point;
		struct paper_point end_point;
	};

	PAPER_API int paper_render_initenv(void);

	PAPER_API void paper_render_destroyenv(void);

	PAPER_API struct paper_render* paper_render_create(void* wnd, uint32 width, uint32 height);

	PAPER_API void paper_render_free(struct paper_render* render);

	PAPER_API void paper_render_resize(struct paper_render* render, uint32 width, uint32 height);

	PAPER_API void paper_render_begin_draw(struct paper_render* render);

	PAPER_API void paper_render_end_draw(struct paper_render* render);

	PAPER_API void paper_render_clear(struct paper_render* render, struct paper_color* color);

	PAPER_API void paper_render_draw_image(struct paper_render* render, struct paper_image* image, int32 x, int32 y, uint32 width, uint32 height);

	PAPER_API void paper_render_draw_text(struct paper_render* render, const TCHAR* szText, uint32 len,
		struct paper_rect* rect, struct paper_font* font, struct paper_brush* brush);

	PAPER_API void paper_render_draw_rectangle(struct paper_render* render, struct paper_rect* rect, struct paper_brush* brush);

	PAPER_API void paper_render_fill_rectangle(struct paper_render* render, struct paper_rect* rect, struct paper_brush* brush);

	PAPER_API struct paper_render* paper_render_create_compatible(struct paper_render* render);

	

	/*image load*/
	PAPER_API struct paper_image* paper_image_get_from_render(struct paper_render* render);

	PAPER_API struct paper_image*  paper_image_load_from_file(struct paper_render* render, const char* filename);

	PAPER_API struct paper_image* paper_image_load_from_memory(struct paper_render* render, void* data, uint32 len);

	PAPER_API void paper_image_free(struct paper_image* image);


	/*brush*/
	PAPER_API struct paper_brush* paper_brush_create_solid(struct paper_render* render, struct paper_color* color);

	PAPER_API struct paper_brush* paper_brush_create_lineargradient(struct paper_render* render, struct paper_gradient_stop* attributes, uint32 count, struct paper_gradient_pos* pos);

	//创建一个位图画刷
	PAPER_API struct paper_brush* paper_brush_create_from_image(struct paper_render* render, struct paper_image* image);

	//只有使用了paper_brush_create_solid创建的颜色画刷才能使用该函数设置颜色
	PAPER_API void paper_brush_set_opacity(struct paper_brush* brush, float opacity);
	PAPER_API void paper_brush_solid_setcolor(struct paper_brush* solidbrush, struct paper_color* color);
	PAPER_API void paper_brush_free(paper_brush* brush);

	/*font*/
	PAPER_API struct paper_font* paper_font_create(const wchar_t* family, float size, float weight, const wchar_t* localname = L"zh-cn");

	PAPER_API void paper_font_free(struct paper_font* font);
#ifdef __cplusplus
}
#endif

#endif	//__PAPER_RENDER_H__

