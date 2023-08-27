#ifndef __PAPER_WIDGET_H__
#define __PAPER_WIDGET_H__

#include "paper_define.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "paper_elememt.h"
	struct paper_widget;
	struct paper_widget_queue;

	typedef void (*paper_widget_paint_cb)(struct paper_widget* widget);
	typedef void (*paper_event_cb)(struct paper_event* e);
	typedef void (*paper_event_mouse_enter_cb)();
	typedef void (*paper_event_mouse_leave_cb)();
	typedef void (*paper_event_mouse_button_cb)(int mousebutton, int state);
	typedef void (*paper_event_keydown_cb)(int key);
	typedef void (*paper_event_keyup_cb)(int key);

	struct paper_widget
	{
		paper_widget_paint_cb on_paper_widget_paint;
		paper_event_cb on_paper_event;
		struct paper_rect rect;
		struct paper_render* render;
		struct paper_widget* prev;
		struct paper_widget* next;
		struct paper_widget* child;
		struct paper_widget* parent;
	};

	struct paper_widget_queue
	{
		int count;
		struct paper_widget* head;		//头部
		struct paper_widget* end;		//尾部
	};

	/*基础widget实现*/
	struct paper_widget* paper_widget_create(struct paper_render* render, paper_widget_paint_cb cb);
	void paper_widget_init(struct paper_widget* widget, struct paper_render* render, paper_widget_paint_cb cb, struct paper_rect* rect);
	void paper_widget_set_size(struct paper_widget* widget, uint32 width, uint32 height);
	void paper_widget_set_pos(struct paper_widget* widget, int32 x, int32 y);
	void paper_widget_free(struct paper_widget* widget);



	//渲染队列
	void paper_widget_paint_all(struct paper_widget_queue* widget_queue);
	struct paper_widget_queue* paper_widget_queue_create();
	void paper_widget_queue_add(struct paper_widget_queue* widget_queue, struct paper_widget* widget);
	void paper_widget_queue_free(struct paper_widget_queue* widget_queue);
	void paper_widget_queue_clear(struct paper_widget_queue* widget_queue);
#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__PAPER_WIDGET_H__
