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

	enum paper_valign_type
	{
		paper_valign_top,
		paper_valign_bottom,
		paper_valign_center,
		paper_valign_fill
	};

	enum paper_halign_type
	{
		paper_halign_left,
		paper_halign_right,
		paper_halign_center,
		paper_halign_fill
	};


	typedef void (*paper_widget_paint_cb)(struct paper_widget* widget, struct paper_render* render);
	typedef int8 (*paper_widget_pt_in_region_cb)(struct paper_widget* widget, struct paper_point* pt);

	struct paper_widget
	{
		paper_widget_paint_cb			paint;				//��ͼ�ص�
		paper_widget_pt_in_region_cb	pt_in_region;		//�ж����ָ���Ƿ��������ڻص�
		paper_event_mouse_enter_cb		on_mouse_enter;
		paper_event_mouse_leave_cb		on_mouse_leave;
		paper_event_resize_cb			on_resize;
		paper_event_lbutton_cb			on_lbutton;

		//struct paper_event_callbacks event_cb;						//�¼��ص��б�
		struct paper_rect rect;
		struct paper_rect rect_global;	//global of window rect.
		uint32 listen_events;			//�������¼�
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
		paper_widget_pt_in_region_cb	pt_in_region;		//�ж����ָ���Ƿ��������ڻص�
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
		struct paper_image* image;
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
		enum paper_valign_type valign;	    //��ֱ����
		enum paper_halign_type halign;	    //ˮƽ����
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
		struct paper_widget* head;		//ͷ��
		struct paper_widget* end;		//β��
	};

	

	/*����widgetʵ��*/
	PAPER_API struct paper_widget* paper_widget_create(struct paper_widget_init_struct* init);
	PAPER_API void paper_widget_init(struct paper_widget* widget, struct paper_widget_init_struct* init);
	PAPER_API void paper_widget_set_size(struct paper_widget* widget, uint32 width, uint32 height);
	PAPER_API void paper_widget_set_pos(struct paper_widget* widget, int32 x, int32 y);
	PAPER_API void paper_widget_paint(struct paper_widget* widget, struct paper_render* render);			//widget ��ͼ����
	PAPER_API void paper_widget_free(struct paper_widget* widget);
	PAPER_API struct paper_render* paper_widget_get_render(struct paper_widget* widget);
	PAPER_API void paper_widget_set_render(struct paper_widget* widget, struct paper_render* render);
	PAPER_API void paper_widget_get_rect(struct paper_widget* widget, struct paper_rect* rect);
	PAPER_API void paper_widget_map_global_point(struct paper_widget* widget, struct paper_point* global_pt);
	PAPER_API int8 paper_widget_pt_in_region(struct paper_widget* widget, struct paper_point* pt);

	//����Ҫ����������ؼ���Χ��Ϣʱʹ�ô˺���
	PAPER_API void paper_widget_should_handle_enter(struct paper_widget* widget);
	//����Ҫ��������뿪�ؼ���Χ��Ϣʱʹ�ô˺���
	PAPER_API void paper_widget_should_handle_leave(struct paper_widget* widget);
	//Ϊ�ؼ�����һ���¼�
	PAPER_API void paper_widget_add_event(struct paper_widget* widget, uint32 evtype);
	//�����ڴ�С�����ı�ʱ�����еĿؼ��������˺���
	PAPER_API void paper_widget_on_resize(struct paper_widget* widget, int32 width, int32 height);
	//��������Ƴ���Ϣ
	PAPER_API void paper_widget_on_mouseenter(struct paper_widget* widget);
	PAPER_API void paper_widget_on_mouseleave(struct paper_widget* widget);
	//���������µ����¼�,stateΪ1��ʾ���£�0��ʾ����
	PAPER_API void paper_widget_on_lbutton(struct paper_widget* widget, int32 x, int32 y, int8 state);

	/*image�ؼ�ʵ��*/
	PAPER_API struct paper_widget_image* paper_widget_image_create(struct paper_widget_init_struct* init, struct paper_image* image);
	PAPER_API void paper_widget_image_paint(struct paper_widget_image* widget);


	/*overlayʵ��*/
	PAPER_API struct paper_widget_overlay* paper_overlay_create(struct paper_widget_init_struct* init);
	PAPER_API void paper_overlay_paint(struct paper_widget_overlay* overlay, struct paper_render* render);

	//��Ⱦ����
	PAPER_API void paper_widget_queue_paint_all(struct paper_widget_queue* widget_queue, struct paper_render* render);
	PAPER_API struct paper_widget_queue* paper_widget_queue_create();
	PAPER_API void paper_widget_queue_add(struct paper_widget_queue* widget_queue, struct paper_widget* widget);
	PAPER_API void paper_widget_queue_free(struct paper_widget_queue* widget_queue);
	PAPER_API void paper_widget_queue_clear(struct paper_widget_queue* widget_queue);

	//resize
	PAPER_API void paper_widget_queue_on_resize(struct paper_widget_queue* widget_queue, int32 width, int32 height);
	PAPER_API void paper_widget_queue_on_mousemove(struct paper_widget_queue* widget_queue, int32 x, int32 y);
	PAPER_API void paper_widget_queue_on_lbutton(struct paper_widget_queue* widget_queue, int32 x, int32 y, int8 state);		//������������ʱ���ô˺���
	PAPER_API void paper_widget_queue_on_mouseleave(struct paper_widget_queue* widget_queue);

	PAPER_API struct paper_widget* paper_widget_queue_findpoint(struct paper_widget_queue* widget_queue, struct paper_point* pt);
	PAPER_API void paper_widget_convert_point(struct paper_widget* widget, const struct paper_point* pt, struct paper_point* outpt);
	PAPER_API struct paper_widget* paper_widget_findpointinchild(struct paper_widget* widget, struct paper_point* pt);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__PAPER_WIDGET_H__
