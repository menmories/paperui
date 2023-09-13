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

	enum paper_line_type
	{
		paper_line_solid,		//实线
		paper_line_dashed		//虚线
	};

	/*初始化渲染器环境，在使用渲染器环境之前必须调用一次*/
	PAPER_API int paper_render_initenv(void);

	/*当不再使用渲染器时可使用此函数销毁渲染器*/
	PAPER_API void paper_render_destroyenv(void);

	/*从本地HWND窗口句柄创建一个渲染器*/
	PAPER_API struct paper_render* paper_render_create(void* wnd, uint32 width, uint32 height);

	PAPER_API void paper_render_free(struct paper_render* render);

	/*当窗口大小发生改变时必须调用此函数更新绘制区域大小*/
	PAPER_API void paper_render_resize(struct paper_render* render, uint32 width, uint32 height);

	/*使目标窗口区域发生重绘，当需要更新绘制目标区域时使用*/
	PAPER_API void paper_render_invalid(struct paper_render* render);
	/*使目标窗口不再发生重绘，主要目的是节省绘制资源，在需要的时候再进行绘制*/
	PAPER_API void paper_render_valid(struct paper_render* render);

	/*
	* 功能：开始绘图，必须要在绘制之前调用才能开始绘图，绘制结束后调用paper_render_end_draw以结束绘图
	*/
	PAPER_API void paper_render_begin_draw(struct paper_render* render);
	/*当无需绘图过后，调用此函数结束绘图*/
	PAPER_API void paper_render_end_draw(struct paper_render* render);
	/*将绘制目标清理成一种颜色*/
	PAPER_API void paper_render_clear(struct paper_render* render, struct paper_color* color);

	/*绘制一张图片，使用输入的坐标来作为绘制位置*/
	PAPER_API void paper_render_draw_image(struct paper_render* render, struct paper_image* image, int32 x, int32 y, uint32 width, uint32 height);
	/*绘制一张图片，以0作为X和Y坐标*/
	PAPER_API void paper_render_draw_image2(struct paper_render* render, struct paper_image* image, uint32 width, uint32 height);
	/*绘制一张图片，绘制的坐标为0，绘制的区域采用render的大小*/
	PAPER_API void paper_render_draw_image3(struct paper_render* render, struct paper_image* image);

	/*绘制一段字符文本*/
	PAPER_API void paper_render_draw_text(struct paper_render* render, const TCHAR* szText, uint32 len,
		const struct paper_rect* rect, struct paper_font* font, struct paper_brush* brush);

	/*绘制一条直线*/
	PAPER_API void paper_render_draw_line(struct paper_render* render, struct paper_point* start, struct paper_point* end, struct paper_brush* brush, float stroke, enum paper_line_type type);

	PAPER_API void paper_render_draw_rectangle(struct paper_render* render, const struct paper_rect* rect, struct paper_brush* brush);

	/*覆盖窗口一片区域为指定颜色*/
	PAPER_API void paper_render_fill_rectangle(struct paper_render* render, const struct paper_rect* rect, struct paper_brush* brush);

	/*
	* @功能：绘制一个圆角矩形
	* @param render 渲染器句柄
	* @param rect	矩形的区域范围
	* @param radius 圆角半径
	* @param brush	画刷
	* @param stroke 绘制的线粗（默认可填1.0f）
	*/
	PAPER_API void paper_render_draw_roundrectangle(struct paper_render* render, struct paper_rect* rect, struct paper_point* radius, struct paper_brush* brush, float stroke);
	/*覆盖一个圆角矩形*/
	PAPER_API void paper_render_fill_roundrectangle(struct paper_render* render, struct paper_rect* rect, struct paper_point* radius, struct paper_brush* brush);
	/*绘制一个圆*/
	PAPER_API void paper_render_draw_ellipse(struct paper_render* render, struct paper_point* center, float radius_x, float radius_y, struct paper_brush* brush, float stroke);
	PAPER_API void paper_render_fill_ellipse(struct paper_render* render, struct paper_point* center, float radius_x, float radius_y, struct paper_brush* brush);

	/*
	* @功能：创建一个兼容位图
	* @param width 位图的宽度
	* @param height 位图的高度
	* @return 成功返回paper_render指针，失败返回NULL
	*/
	PAPER_API struct paper_render* paper_render_create_compatible(struct paper_render* render, int32 width, int32 height);
	/*创建一个兼容渲染器，宽和高继承自指定的渲染器*/
	PAPER_API struct paper_render* paper_render_create_compatible_extendsize(struct paper_render* render);
	
	/*image load*/
	/*从compatible render获取一个位图*/
	PAPER_API struct paper_image* paper_image_get_from_render(struct paper_render* render);

	/*从文件读取一张图片，支持目前常用的jpg、bmp、png等格式*/
	PAPER_API struct paper_image*  paper_image_load_from_file(struct paper_render* render, const char* filename);
	/*从内存读取一张图片*/
	PAPER_API struct paper_image* paper_image_load_from_memory(struct paper_render* render, void* data, uint32 len);

	PAPER_API void paper_image_free(struct paper_image* image);


	/*brush*/
	PAPER_API struct paper_brush* paper_brush_create_solid(struct paper_render* render, struct paper_color* color);
	/*创建一个线性渐变画刷*/
	PAPER_API struct paper_brush* paper_brush_create_lineargradient(struct paper_render* render, struct paper_gradient_stop* attributes, uint32 count, struct paper_gradient_pos* pos);

	//创建一个位图画刷
	PAPER_API struct paper_brush* paper_brush_create_from_image(struct paper_render* render, struct paper_image* image);

	//只有使用了paper_brush_create_solid创建的颜色画刷才能使用该函数设置颜色
	PAPER_API void paper_brush_set_opacity(struct paper_brush* brush, float opacity);
	PAPER_API void paper_brush_solid_setcolor(struct paper_brush* solidbrush, struct paper_color* color);
	PAPER_API void paper_brush_free(struct paper_brush* brush);

	/*font*/
	PAPER_API struct paper_font* paper_font_create(const wchar_t* family, float size, float weight, const wchar_t* localname/* = L"zh-cn"*/);

	PAPER_API void paper_font_free(struct paper_font* font);
#ifdef __cplusplus
}
#endif

#endif	//__PAPER_RENDER_H__

