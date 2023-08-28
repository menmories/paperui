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
	Direct2D�����¼���brush��
	Solid color brush����ɫ��ˢ���������������Ĵ�ɫ��ˢ��
	Gradient brush�����仭ˢ�������Դ������Խ��䡢���򽥱䡢�ǶȽ����Ч���Ļ�ˢ��
	Bitmap brush��λͼ��ˢ��������ʹ��λͼ�������Ļ�ˢ��
	Radial gradient brush�����򽥱仭ˢ�������Դ�����һ�����ĵ�������ɢ�ľ��򽥱�Ч����
	LinearGradient brush�����Խ��仭ˢ�������Դ�������һ��ֱ�߷��򽥱�Ļ�ˢ��
	SolidColorBrush��ʵ�Ļ�ˢ�������ڴ�����ɫ��ʵ�Ļ�ˢ��
	Effect brush����Ч��ˢ��������ʹ��ĳ����Ч��任������ͼ��Ļ�ˢ��
	��Щ��ˢ���Ը�����Ҫ������Ϻ�ʹ�ã���ʵ�ַḻ�����Ļ�ͼЧ����
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

	//����һ��λͼ��ˢ
	PAPER_API struct paper_brush* paper_brush_create_from_image(struct paper_render* render, struct paper_image* image);

	//ֻ��ʹ����paper_brush_create_solid��������ɫ��ˢ����ʹ�øú���������ɫ
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

