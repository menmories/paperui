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

	/*��ֱ����ê��*/
	enum paper_valign_type
	{
		paper_valign_top,
		paper_valign_bottom,
		paper_valign_center,
		paper_valign_fill
	};
	//ˮƽ����ê��
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
		paper_widget_paint_cb			paint;				//��ͼ�ص�
		paper_widget_pt_in_region_cb	pt_in_region;		//�ж����ָ���Ƿ��������ڻص�
		paper_event_mouse_enter_cb		on_mouse_enter;
		paper_event_mouse_leave_cb		on_mouse_leave;
		paper_event_resize_cb			on_resize;
		paper_event_mousebutton_cb		on_mousebutton;
		paper_widget_free_cb			free_widget;

		//struct paper_event_callbacks event_cb;						//�¼��ص��б�
		struct paper_rect rect;
		struct paper_rect rect_global;	//global of window rect.
		uint32 listen_events;			//�������¼�
		uint32 events;
		struct paper_render* render;
		struct paper_widget* parent;
	};

	struct paper_widget_init_struct
	{
		paper_widget_paint_cb			paint;
		paper_widget_pt_in_region_cb	pt_in_region;		//�ж����ָ���Ƿ��������ڻص�
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
		enum paper_valign_type valign;	    //��ֱ����
		enum paper_halign_type halign;	    //ˮƽ����
		struct paper_vector* slots;			//slots vector
		//struct paper_widget* child;			//slot child
	};

	//overlay�Ĳ��
	struct paper_widget_overlay_slot 
	{
		struct paper_widget base;
		enum paper_valign_type valign;	    //��ֱ����
		enum paper_halign_type halign;	    //ˮƽ����
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
		struct paper_rect paint_rect;		//��ǰwidget�ڴ��ڻ��Ƶ�����
		struct paper_widget* enter_widget;	//�������widget
		struct paper_vector* widgets;		//��Ⱦ/�¼���������
	};

	

	/*����widgetʵ��*/
	PAPER_API struct paper_widget* paper_widget_create(struct paper_widget_init_struct* init);
	PAPER_API void paper_widget_init(struct paper_widget* widget, struct paper_widget_init_struct* init);
	PAPER_API void paper_widget_set_size(struct paper_widget* widget, uint32 width, uint32 height);
	PAPER_API void paper_widget_set_pos(struct paper_widget* widget, int32 x, int32 y);
	PAPER_API void paper_widget_paint(struct paper_widget* widget, struct paper_render* render, const struct paper_rect* rcpaint);			//widget ��ͼ����
	PAPER_API void paper_widget_free_components(struct paper_widget* widget);		//�ͷ�widget�µ����ж�̬��������
	PAPER_API void paper_widget_free(struct paper_widget* widget);
	PAPER_API struct paper_render* paper_widget_get_render(struct paper_widget* widget);
	PAPER_API void paper_widget_set_render(struct paper_widget* widget, struct paper_render* render);
	PAPER_API void paper_widget_get_rect(struct paper_widget* widget, struct paper_rect* rect);
	PAPER_API void paper_widget_map_global_point(struct paper_widget* widget, struct paper_point* global_pt);
	PAPER_API struct paper_widget* paper_widget_pt_in_region(struct paper_widget* widget, const struct paper_rect* rcpaint, struct paper_point* pt);

	//����Ҫ����������ؼ���Χ��Ϣʱʹ�ô˺���
	PAPER_API void paper_widget_should_handle_enter(struct paper_widget* widget);
	//����Ҫ��������뿪�ؼ���Χ��Ϣʱʹ�ô˺���
	PAPER_API void paper_widget_should_handle_leave(struct paper_widget* widget);
	//Ϊ�ؼ�����һ���¼�
	PAPER_API void paper_widget_add_event(struct paper_widget* widget, uint32 evtype);
	PAPER_API void paper_widget_set_event(struct paper_widget* widget, uint32 evtype);
	//�����ڴ�С�����ı�ʱ�����еĿؼ��������˺���
	PAPER_API void paper_widget_on_resize(struct paper_widget* widget, int32 width, int32 height);
	//��������Ƴ���Ϣ
	PAPER_API void paper_widget_on_mouseenter(struct paper_widget* widget);
	PAPER_API void paper_widget_on_mouseleave(struct paper_widget* widget);
	//���������µ����¼�,stateΪ1��ʾ���£�0��ʾ����
	//button,�������ͣ�����ɲ鿴����������
	//VK_LBUTTON ������
	//VK_RBUTTON ����Ҽ�
	//VK_MBUTTON ����м�
	PAPER_API void paper_widget_on_mousebutton(struct paper_widget* widget, uint32 button, int32 x, int32 y, int8 state);

	/*image�ؼ�ʵ��*/
	PAPER_API struct paper_widget_image* paper_widget_image_create(struct paper_widget_init_struct* init, struct paper_image* image);
	PAPER_API void paper_widget_image_paint(struct paper_widget_image* widget, struct paper_render* render, const struct paper_rect* rcpaint);
	PAPER_API void paper_widget_image_free(struct paper_widget_image* widget);

	PAPER_API struct paper_widget_text* paper_widget_text_create(struct paper_widget_init_struct* init, struct paper_render* render, const wchar_t* text, uint32 len);
	PAPER_API void paper_widget_text_paint(struct paper_widget_text* text, struct paper_render* render, const struct paper_rect* rcpaint);
	/*button�ؼ�ʵ��*/
	PAPER_API struct paper_widget_button* paper_widget_button_create(struct paper_widget_init_struct* init, struct paper_render* render, const wchar_t* text, uint32 len);
	PAPER_API void paper_widget_button_paint(struct paper_widget_button* button, struct paper_render* render, const struct paper_rect* rcpaint);
	PAPER_API void paper_widget_button_free(struct paper_widget_button* button);
	PAPER_API void paper_widget_button_set_normal_brush(struct paper_widget_button* button, struct paper_brush* brush);
	PAPER_API void paper_widget_button_set_hot_brush(struct paper_widget_button* button, struct paper_brush* brush);
	PAPER_API void paper_widget_button_set_pushed_brush(struct paper_widget_button* button, struct paper_brush* brush);
	/*overlayʵ��*/
	/*
	* overlay ��left top right bottom����λƫ��,�������������㷨
	*/
	PAPER_API struct paper_widget_overlay* paper_overlay_create(struct paper_widget_init_struct* init);
	//rcpaint �ɻ�������
	PAPER_API void paper_overlay_paint(struct paper_widget_overlay* overlay, struct paper_render* render, const struct paper_rect* rcpaint);
	PAPER_API void paper_overlay_free(struct paper_widget_overlay* overlay);
	PAPER_API void paper_overlay_add_slot(struct paper_widget_overlay* overlay, struct paper_widget_overlay_slot* slot);
	PAPER_API struct paper_widget* paper_widget_overlay_pt_in_region(struct paper_widget_overlay* overlay, const struct paper_rect* rcpaint, struct paper_point* pt);		//�������ӿؼ������Ա���ʵ�������жϺ���

	/*overlay slotʵ��*/
	PAPER_API struct paper_widget_overlay_slot* paper_widget_overlay_slot_create();
	PAPER_API void paper_widget_overlay_slot_paint(struct paper_widget_overlay_slot* slot, struct paper_render* render, const struct paper_rect* rcpaint);
	PAPER_API void paper_widget_overlay_slot_free(struct paper_widget_overlay_slot* slot);

	//��Ⱦ����
	PAPER_API void paper_widget_queue_paint_all(struct paper_widget_queue* widget_queue, struct paper_render* render);
	PAPER_API struct paper_widget_queue* paper_widget_queue_create(uint32 width, uint32 height);
	PAPER_API void paper_widget_queue_add(struct paper_widget_queue* widget_queue, struct paper_widget* widget);
	PAPER_API void paper_widget_queue_free(struct paper_widget_queue* widget_queue);
	PAPER_API void paper_widget_queue_clear(struct paper_widget_queue* widget_queue);

	//resize
	PAPER_API void paper_widget_queue_on_resize(struct paper_widget_queue* widget_queue, int32 width, int32 height);
	PAPER_API void paper_widget_queue_on_mousemove(struct paper_widget_queue* widget_queue, int32 x, int32 y);
	PAPER_API void paper_widget_queue_on_mousebutton(struct paper_widget_queue* widget_queue, uint32 button, int32 x, int32 y, int8 state);		//������������ʱ���ô˺���
	PAPER_API void paper_widget_queue_on_mouseleave(struct paper_widget_queue* widget_queue);

	PAPER_API struct paper_widget* paper_widget_queue_findpoint(struct paper_widget_queue* widget_queue, struct paper_point* pt);
	PAPER_API void paper_widget_convert_point(struct paper_widget* widget, const struct paper_point* pt, struct paper_point* outpt);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__PAPER_WIDGET_H__
