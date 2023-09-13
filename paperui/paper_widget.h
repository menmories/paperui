#ifndef __PAPER_WIDGET_H__
#define __PAPER_WIDGET_H__


#ifdef __cplusplus
extern "C"
{
#endif
#include "paper_elememt.h"
#include "paper_define.h"
#include "paper_event.h"
#include "paper_render.h"
	struct paper_widget;
	struct paper_widget_image;
	struct paper_widget_text;
	struct paper_widget_overlay;
	struct paper_widget_button;
	struct paper_widget_sizebox;
	struct paper_widget_text_editor;

	struct paper_widget_queue;

	/*垂直方向锚点*/
	enum paper_valign_type
	{
		paper_valign_top,
		paper_valign_bottom,
		paper_valign_center,
		paper_valign_fill
	};
	//水平方向锚点
	enum paper_halign_type
	{
		paper_halign_left,
		paper_halign_right,
		paper_halign_center,
		paper_halign_fill
	};


	typedef void (*paper_widget_paint_cb)(struct paper_widget* widget, struct paper_render* render, const struct paper_rect* rcpaint);
	typedef struct paper_widget* (*paper_widget_pt_in_region_cb)(struct paper_widget* widget, const struct paper_rect* rcpaint, struct paper_point* pt);
	typedef void (*paper_widget_free_cb)(struct paper_widget* widget);

	struct paper_widget
	{
		paper_widget_paint_cb			paint;				//绘图回调
		paper_widget_pt_in_region_cb	pt_in_region;		//判断鼠标指针是否在区域内回调
		paper_event_mouse_enter_cb		on_mouse_enter;
		paper_event_mouse_leave_cb		on_mouse_leave;
		paper_event_resize_cb			on_resize;
		paper_event_mousebutton_cb		on_mousebutton;
		paper_widget_free_cb			free_widget;

		//struct paper_event_callbacks event_cb;						//事件回调列表
		struct paper_rect rect;
		struct paper_rect rect_global;	//global of window rect.
		uint32 listen_events;			//监听的事件
		uint32 events;
		struct paper_render* render;
		struct paper_widget* parent;
	};

	struct paper_widget_init_struct
	{
		paper_widget_paint_cb			paint;
		paper_widget_pt_in_region_cb	pt_in_region;		//判断鼠标指针是否在区域内回调
		paper_event_mouse_enter_cb		on_mouse_enter;
		paper_event_mouse_leave_cb		on_mouse_leave;
		paper_event_resize_cb			on_resize;
		paper_event_mousebutton_cb		on_mousebutton;
		paper_widget_free_cb			free_widget;
		struct paper_rect rect;
		struct paper_widget* parent;
		struct paper_render* render;
	};

	struct paper_widget_image
	{
		struct paper_widget base;
		struct paper_image* image;
	};
	struct paper_widget_text
	{
		struct paper_widget base;
		wchar_t* text;
		uint32 text_len;
		struct paper_font* text_font;
		struct paper_brush* text_brush;
	};

	struct paper_widget_sizebox
	{
		struct paper_widget base;
	};

	struct paper_widget_overlay
	{
		struct paper_widget base;
		enum paper_valign_type valign;	    //垂直对齐
		enum paper_halign_type halign;	    //水平对齐
		struct paper_vector* slots;			//slots vector
		//struct paper_widget* child;			//slot child
	};

	//overlay的插槽
	struct paper_widget_overlay_slot 
	{
		struct paper_widget base;
		enum paper_valign_type valign;	    //垂直对齐
		enum paper_halign_type halign;	    //水平对齐
		struct paper_widget* widget;
	};

	struct paper_widget_button
	{
		struct paper_widget_overlay base;
		struct paper_brush* normal_brush;
		struct paper_brush* hot_brush;
		struct paper_brush* pushed_brush;
		struct paper_widget_text* text_widget;
	};
	
	struct paper_widget_text_editor
	{
		struct paper_widget base;
	};

	struct paper_widget_queue
	{
		struct paper_rect paint_rect;		//当前widget在窗口绘制的区域
		struct paper_widget* enter_widget;	//鼠标进入的widget
		struct paper_vector* widgets;		//渲染/事件触发队列
	};

	

	/*基础widget实现*/
	PAPER_API struct paper_widget* paper_widget_create(struct paper_widget_init_struct* init);
	PAPER_API void paper_widget_init(struct paper_widget* widget, struct paper_widget_init_struct* init);
	PAPER_API void paper_widget_set_size(struct paper_widget* widget, uint32 width, uint32 height);
	PAPER_API void paper_widget_set_pos(struct paper_widget* widget, int32 x, int32 y);
	PAPER_API void paper_widget_paint(struct paper_widget* widget, struct paper_render* render, const struct paper_rect* rcpaint);			//widget 绘图函数
	PAPER_API void paper_widget_free_components(struct paper_widget* widget);		//释放widget下的所有动态申请的组件
	PAPER_API void paper_widget_free(struct paper_widget* widget);
	PAPER_API struct paper_render* paper_widget_get_render(struct paper_widget* widget);
	PAPER_API void paper_widget_set_render(struct paper_widget* widget, struct paper_render* render);
	PAPER_API void paper_widget_get_rect(struct paper_widget* widget, struct paper_rect* rect);
	PAPER_API void paper_widget_map_global_point(struct paper_widget* widget, struct paper_point* global_pt);
	PAPER_API struct paper_widget* paper_widget_pt_in_region(struct paper_widget* widget, const struct paper_rect* rcpaint, struct paper_point* pt);

	//当需要处理鼠标进入控件范围消息时使用此函数
	PAPER_API void paper_widget_should_handle_enter(struct paper_widget* widget);
	//当需要处理鼠标离开控件范围消息时使用此函数
	PAPER_API void paper_widget_should_handle_leave(struct paper_widget* widget);
	//为控件增添一个事件
	PAPER_API void paper_widget_add_event(struct paper_widget* widget, uint32 evtype);
	PAPER_API void paper_widget_set_event(struct paper_widget* widget, uint32 evtype);
	//当窗口大小发生改变时，所有的控件将触发此函数
	PAPER_API void paper_widget_on_resize(struct paper_widget* widget, int32 width, int32 height);
	//鼠标进入和移出消息
	PAPER_API void paper_widget_on_mouseenter(struct paper_widget* widget);
	PAPER_API void paper_widget_on_mouseleave(struct paper_widget* widget);
	//鼠标左键按下弹起事件,state为1表示按下，0表示弹起
	//button,按键类型，具体可查看鼠标虚拟键码
	//VK_LBUTTON 鼠标左键
	//VK_RBUTTON 鼠标右键
	//VK_MBUTTON 鼠标中键
	PAPER_API void paper_widget_on_mousebutton(struct paper_widget* widget, uint32 button, int32 x, int32 y, int8 state);

	/*image控件实现*/
	PAPER_API struct paper_widget_image* paper_widget_image_create(struct paper_widget_init_struct* init, struct paper_image* image);
	PAPER_API void paper_widget_image_paint(struct paper_widget_image* widget, struct paper_render* render, const struct paper_rect* rcpaint);
	PAPER_API void paper_widget_image_free(struct paper_widget_image* widget);

	PAPER_API struct paper_widget_text* paper_widget_text_create(struct paper_widget_init_struct* init, struct paper_render* render, const wchar_t* text, uint32 len);
	PAPER_API void paper_widget_text_paint(struct paper_widget_text* text, struct paper_render* render, const struct paper_rect* rcpaint);
	/*button控件实现*/
	PAPER_API struct paper_widget_button* paper_widget_button_create(struct paper_widget_init_struct* init, struct paper_render* render, const wchar_t* text, uint32 len);
	PAPER_API void paper_widget_button_paint(struct paper_widget_button* button, struct paper_render* render, const struct paper_rect* rcpaint);
	PAPER_API void paper_widget_button_free(struct paper_widget_button* button);
	PAPER_API void paper_widget_button_set_normal_brush(struct paper_widget_button* button, struct paper_brush* brush);
	PAPER_API void paper_widget_button_set_hot_brush(struct paper_widget_button* button, struct paper_brush* brush);
	PAPER_API void paper_widget_button_set_pushed_brush(struct paper_widget_button* button, struct paper_brush* brush);
	/*overlay实现*/
	/*
	* overlay 的left top right bottom属于位偏移,不是正常矩形算法
	*/
	PAPER_API struct paper_widget_overlay* paper_overlay_create(struct paper_widget_init_struct* init);
	//rcpaint 可绘制区域
	PAPER_API void paper_overlay_paint(struct paper_widget_overlay* overlay, struct paper_render* render, const struct paper_rect* rcpaint);
	PAPER_API void paper_overlay_free(struct paper_widget_overlay* overlay);
	PAPER_API void paper_overlay_add_slot(struct paper_widget_overlay* overlay, struct paper_widget_overlay_slot* slot);
	PAPER_API struct paper_widget* paper_widget_overlay_pt_in_region(struct paper_widget_overlay* overlay, const struct paper_rect* rcpaint, struct paper_point* pt);		//由于有子控件，所以必须实现鼠标点判断函数

	/*overlay slot实现*/
	PAPER_API struct paper_widget_overlay_slot* paper_widget_overlay_slot_create();
	PAPER_API void paper_widget_overlay_slot_paint(struct paper_widget_overlay_slot* slot, struct paper_render* render, const struct paper_rect* rcpaint);
	PAPER_API void paper_widget_overlay_slot_free(struct paper_widget_overlay_slot* slot);

	//渲染队列
	PAPER_API void paper_widget_queue_paint_all(struct paper_widget_queue* widget_queue, struct paper_render* render);
	PAPER_API struct paper_widget_queue* paper_widget_queue_create(uint32 width, uint32 height);
	PAPER_API void paper_widget_queue_add(struct paper_widget_queue* widget_queue, struct paper_widget* widget);
	PAPER_API void paper_widget_queue_free(struct paper_widget_queue* widget_queue);
	PAPER_API void paper_widget_queue_clear(struct paper_widget_queue* widget_queue);

	//resize
	PAPER_API void paper_widget_queue_on_resize(struct paper_widget_queue* widget_queue, int32 width, int32 height);
	PAPER_API void paper_widget_queue_on_mousemove(struct paper_widget_queue* widget_queue, int32 x, int32 y);
	PAPER_API void paper_widget_queue_on_mousebutton(struct paper_widget_queue* widget_queue, uint32 button, int32 x, int32 y, int8 state);		//当鼠标左键触发时调用此函数
	PAPER_API void paper_widget_queue_on_mouseleave(struct paper_widget_queue* widget_queue);

	PAPER_API struct paper_widget* paper_widget_queue_findpoint(struct paper_widget_queue* widget_queue, struct paper_point* pt);
	PAPER_API void paper_widget_convert_point(struct paper_widget* widget, const struct paper_point* pt, struct paper_point* outpt);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__PAPER_WIDGET_H__
