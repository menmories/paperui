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

	int paper_render_initenv(void);

	void paper_render_destroyenv(void);

	struct paper_render* paper_render_create(void* wnd, uint32 width, uint32 height);

	void paper_render_free(struct paper_render* render);

	void paper_render_resize(struct paper_render* render, uint32 width, uint32 height);

	void paper_render_begin_draw(struct paper_render* render);

	void paper_render_end_draw(struct paper_render* render);

	void paper_render_clear(struct paper_render* render, struct paper_color* color);

	void paper_render_draw_image(struct paper_render* render, struct paper_image* image, int32 x, int32 y, uint32 width, uint32 height);

	struct paper_render* paper_render_create_compatible(struct paper_render* render, uint32 width, uint32 height);


	/*image load*/
	struct paper_image*  paper_image_load_from_file(struct paper_render* render, const char* filename);

	struct paper_image* paper_image_load_from_memory(struct paper_render* render, void* data, uint32 len);

	void paper_image_free(struct paper_image* image);

	/*font*/
	struct paper_font* paper_font_create(const wchar_t* family, float size, float weight, const wchar_t* localname = L"zh-cn");

	void paper_font_free(struct paper_font* font);
#ifdef __cplusplus
}
#endif

#endif	//__PAPER_RENDER_H__

