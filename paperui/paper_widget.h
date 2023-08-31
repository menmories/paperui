#ifndef __PAPER_WIDGET_H__
#define __PAPER_WIDGET_H__

#include "paper_define.h"
#include "paper_event.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include "paper_elememt.h"
	struct paper_widget;
	struct paper_widget_image;
	struct paper_widget_text;
	struct paper_widget_overlay;
	struct paper_widget_button;
	struct paper_widget_sizebox;
	struct paper_widget_text_editor;

	struct paper_widget_queue;

	typedef void (*paper_widget_paint_cb)(struct paper_widget* widget);
	typedef int8 (*paper_widget_pt_in_region_cb)(struct paper_widget* widget, struct paper_point* pt);

	struct paper_widget
	{
		paper_widget_paint_cb			paint;				//绘图回调
		paper_widget_pt_in_region_cb	pt_in_region;		//判断鼠标指针是否在区域内回调
		paper_event_mouse_enter_cb		on_mouse_enter;
		paper_event_mouse_leave_cb		on_mouse_leave;
		paper_event_resize_cb			on_resize;
		paper_event_lbutton_cb			on_lbutton;

		//struct paper_event_callbacks event_cb;						//事件回调列表
		struct paper_rect rect;
		struct paper_rect rect_global;	//global of window rect.
		uint32 listen_events;			//监听的事件
		uint32 events;
		struct paper_brush* background;
		struct paper_brush* background1;
		struct paper_brush* background2;
		struct paper_render* render;
		struct paper_widget* prev;
		struct paper_widget* next;
		struct paper_widget* child;
		struct paper_widget* parent;
	};

	struct paper_widget_init_struct
	{
		paper_widget_paint_cb			paint;
		paper_widget_pt_in_region_cb	pt_in_region;		//判断鼠标指针是否在区域内回调
		paper_event_mouse_enter_cb		on_mouse_enter;
		paper_event_mouse_leave_cb		on_mouse_leave;
		paper_event_resize_cb			on_resize;
		paper_event_lbutton_cb			on_lbutton;
		struct paper_rect rect;
		struct paper_widget* parent;
		struct paper_render* render;
	};

	struct paper_widget_image
	{
		struct paper_widget base;
	};
	struct paper_widget_text
	{
		struct paper_widget base;
	};

	struct paper_widget_sizebox
	{
		struct paper_widget base;
	};

	struct paper_widget_overlay
	{
		struct paper_widget_sizebox base;
	};
	struct paper_widget_button
	{
		struct paper_widget_overlay base;

	};
	
	struct paper_widget_text_editor
	{
		struct paper_widget base;
	};

	struct paper_widget_queue
	{
		int count;
		struct paper_widget* enter_widget;
		struct paper_widget* head;		//头部
		struct paper_widget* end;		//尾部
	};

	

	/*基础widget实现*/
	struct paper_widget* paper_widget_create(struct paper_widget_init_struct* init);
	void paper_widget_init(struct paper_widget* widget, struct paper_widget_init_struct* init);
	void paper_widget_set_size(struct paper_widget* widget, uint32 width, uint32 height);
	void paper_widget_set_pos(struct paper_widget* widget, int32 x, int32 y);
	void paper_widget_paint(struct paper_widget* widget);			//widget 绘图函数
	void paper_widget_free(struct paper_widget* widget);
	struct paper_render* paper_widget_get_render(struct paper_widget* widget);
	void paper_widget_get_rect(struct paper_widget* widget, struct paper_rect* rect);
	void paper_widget_map_global_point(struct paper_widget* widget, struct paper_point* global_pt);
	int8 paper_widget_pt_in_region(struct paper_widget* widget, struct paper_point* pt);

	//当需要处理鼠标进入控件范围消息时使用此函数
	void paper_widget_should_handle_enter(struct paper_widget* widget);
	//当需要处理鼠标离开控件范围消息时使用此函数
	void paper_widget_should_handle_leave(struct paper_widget* widget);
	//为控件增添一个事件
	void paper_widget_add_event(struct paper_widget* widget, uint32 evtype);
	//当窗口大小发生改变时，所有的控件将触发此函数
	void paper_widget_on_resize(struct paper_widget* widget, int32 width, int32 height);
	//鼠标进入和移出消息
	void paper_widget_on_mouseenter(struct paper_widget* widget);
	void paper_widget_on_mouseleave(struct paper_widget* widget);
	//鼠标左键按下弹起事件,state为1表示按下，0表示弹起
	void paper_widget_on_lbutton(struct paper_widget* widget, int32 x, int32 y, int8 state);

	/*image控件实现*/

	/*overlay实现*/
	struct paper_widget_overlay* paper_overlay_create(struct paper_widget_init_struct* init);
	void paper_overlay_paint(struct paper_widget_overlay* overlay);

	//渲染队列
	void paper_widget_queue_paint_all(struct paper_widget_queue* widget_queue);
	struct paper_widget_queue* paper_widget_queue_create();
	void paper_widget_queue_add(struct paper_widget_queue* widget_queue, struct paper_widget* widget);
	void paper_widget_queue_free(struct paper_widget_queue* widget_queue);
	void paper_widget_queue_clear(struct paper_widget_queue* widget_queue);

	//resize
	void paper_widget_queue_on_resize(struct paper_widget_queue* widget_queue, int32 width, int32 height);
	void paper_widget_queue_on_mousemove(struct paper_widget_queue* widget_queue, int32 x, int32 y);
	void paper_widget_queue_on_lbutton(struct paper_widget_queue* widget_queue, int32 x, int32 y, int8 state);		//当鼠标左键触发时调用此函数


	struct paper_widget* paper_widget_queue_findpoint(struct paper_widget_queue* widget_queue, struct paper_point* pt);
	void paper_widget_convert_point(struct paper_widget* widget, const struct paper_point* pt, struct paper_point* outpt);
	struct paper_widget* paper_widget_findpointinchild(struct paper_widget* widget, struct paper_point* pt);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__PAPER_WIDGET_H__
