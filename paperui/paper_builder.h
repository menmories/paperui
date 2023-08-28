#ifndef __PAPER_BUILDER_H__
#define __PAPER_BUILDER_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "paper_define.h"
#include "paper_render.h"
#include "paper_window.h"
#include "paper_widget.h"

	struct paper_builder;

	struct paper_builder
	{
		int32 x;
		int32 y;
		uint32 width;
		uint32 height;
		struct paper_image* image;
		struct paper_rect image_rect;
		struct paper_brush* image_brush;
	};

	enum ELoadType
	{
		Load_XML,
		Load_JSON
	};

	struct paper_builder* paper_builder_load(struct paper_render* render, const char* filename, ELoadType type);

	void paper_builder_free(struct paper_builder* builder);

#ifdef __cplusplus
}
#endif

#endif	//__PAPER_BUILDER_H__