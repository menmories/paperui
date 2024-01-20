#include "paper_widget.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <Windows.h>
#include "paper_render.h"
#include "paper_vector.h"
#include "paper_event.h"
#include <stdio.h>





struct paper_widget* paper_widget_new(struct paper_widget_init_struct* init)
{
	struct paper_widget* widget = (struct paper_widget*)malloc(sizeof(struct paper_widget));
	if (!widget)
	{
		return widget;
	}
	memset(widget, 0, sizeof(struct paper_widget));
	paper_widget_init(widget, init);
	/*if (init)
	{
		paper_widget_init(widget, init);
	}
	else
	{
		struct paper_widget_init_struct init2;
		memset(&init2, 0, sizeof(struct paper_widget_init_struct));
		init2.paint = paper_widget_paint;
		init2.pt_in_region = paper_widget_pt_in_region;
		init2.parent = NULL;
		init2.on_resize = paper_widget_on_resize;
		init2.on_mouse_enter = paper_widget_on_mouseenter;
		init2.on_mouse_leave = paper_widget_on_mouseleave;
		init2.on_lbutton = paper_widget_on_lbutton;
		paper_widget_init(widget, &init2);
	}*/
	
	return widget;
}

void paper_widget_init(struct paper_widget* widget, struct paper_widget_init_struct* init)
{
	if (init)
	{
		widget->paint = init->paint;
		widget->pt_in_region = init->pt_in_region;
		widget->on_mouse_enter = init->on_mouse_enter;
		widget->on_mouse_leave = init->on_mouse_leave;
		widget->on_resize = init->on_resize;
		widget->on_mousebutton = init->on_mousebutton;
		memcpy(&widget->rect, &init->rect, sizeof(struct paper_rect));
		widget->parent = init->parent;
		widget->free_widget = init->free_widget;
	}
	else
	{
		struct paper_widget_init_struct init2;
		memset(&init2, 0, sizeof(struct paper_widget_init_struct));
		init2.paint = paper_widget_paint;
		init2.pt_in_region = paper_widget_pt_in_region;
		init2.parent = NULL;
		init2.on_resize = paper_widget_on_resize;
		init2.on_mouse_enter = paper_widget_on_mouseenter;
		init2.on_mouse_leave = paper_widget_on_mouseleave;
		init2.on_mousebutton = paper_widget_on_mousebutton;
		init2.free_widget = paper_widget_free;
		//init2.rect.right = 20;
		//init2.rect.bottom = 20;
		paper_widget_init(widget, &init2);
	}
}

void paper_widget_set_size(struct paper_widget* widget, uint32 width, uint32 height)
{
	widget->rect.right = widget->rect.left + width;
	widget->rect.bottom = widget->rect.top + height;
}

void paper_widget_set_pos(struct paper_widget* widget, int32 x, int32 y)
{
	int width = widget->rect.right - widget->rect.left;
	int height = widget->rect.bottom - widget->rect.top;
	widget->rect.left = x;
	widget->rect.top = y;
	widget->rect.right = x + width;
	widget->rect.bottom = y + height;
}

void paper_widget_paint(struct paper_widget* widget, struct paper_render* render, const struct paper_rect* rcpaint)
{
	//基础widget并不做任何操作，使用此函数的目的是为了widget基类不留空
}

void paper_widget_free_components(struct paper_widget* widget)
{
	//暂时并没有什么组件需要释放的，备用
}

void paper_widget_free(struct paper_widget* widget)
{
	paper_widget_free_components(widget);
	free(widget);
}

void paper_widget_get_rect(struct paper_widget* widget, struct paper_rect* rect)
{
	memcpy(rect, &widget->rect, sizeof(struct paper_rect));
}

void paper_widget_map_global_point(struct paper_widget* widget, struct paper_point* global_pt)
{
	widget = widget->parent;
	while (widget)
	{
		global_pt->x -= widget->rect.left;
		global_pt->y -= widget->rect.top;
		widget = widget->parent;
	}
}

struct paper_widget* paper_widget_pt_in_region(struct paper_widget* widget, const struct paper_rect* rcpaint, struct paper_point* pt)
{
	//需要将其转化为全局坐标
	assert(widget);
	assert(pt);
	return paper_rect_pt_in(rcpaint, pt->x, pt->y) ? widget : NULL;
}

void paper_widget_should_handle_enter(struct paper_widget* widget)
{
	assert(widget);
	widget->events |= PAPER_EVENT_MOUSEENTER;
	if (widget->listen_events & PAPER_LISTEN_EVENT_MOUSEENTER)
	{
		assert(widget->on_mouse_enter);
		widget->on_mouse_enter(widget);
	}
}

void paper_widget_should_handle_leave(struct paper_widget* widget)
{
	widget->events |= PAPER_EVENT_MOUSELEAVE;
	if (widget->listen_events & PAPER_EVENT_MOUSELEAVE)
	{
		assert(widget->on_mouse_leave);
		widget->on_mouse_leave(widget);      //鼠标离开了
	}
}

void paper_widget_add_event(struct paper_widget* widget, uint32 evtype)
{
	widget->listen_events |= evtype;
}

void paper_widget_set_event(struct paper_widget* widget, uint32 evtype)
{
	widget->listen_events = evtype;
}

void paper_widget_on_resize(struct paper_widget* widget, int32 width, int32 height)
{

}

void paper_widget_on_mouseenter(struct paper_widget* widget)
{
	//鼠标进入触发此函数，widget不作任何操作，仅保持函数不留空
	//struct paper_color color = {0.0f, 0.8f, 1.0f};
	//paper_brush_solid_setcolor(widget->background, &color);
	//widget->background = widget->background2;
	//paper_render_invalid(widget->render);
}

void paper_widget_on_mouseleave(struct paper_widget* widget)
{
	//鼠标离开触发此函数，widget不作任何操作，仅保持函数不留空
	//struct paper_color color = { 0.1f, 1.8f, 1.0f };
	//paper_brush_solid_setcolor(widget->background, &color);
	//widget->background = widget->background1;
	//paper_render_invalid(widget->render);
}

void paper_widget_on_mousebutton(struct paper_widget* widget, uint32 button, int32 x, int32 y, int8 state)
{
	//鼠标左键按下触发此函数，widget不作任何操作，仅保持函数不留空
	
}

struct paper_widget_image* paper_widget_image_new(struct paper_widget_init_struct* init, struct paper_image* image)
{
	struct paper_widget_image* image_widget = (struct paper_widget_image*)malloc(sizeof(struct paper_widget_image));
	if (!image_widget)
	{
		return NULL;
	}
	struct paper_widget* base = BASE_WIDGET(image_widget);
	paper_widget_init(base, init);
	base->listen_events = 0;		//不监听任何事件
	base->paint = (paper_widget_paint_cb)paper_widget_image_paint;

	image_widget->image = image;
	return image_widget;
}

void paper_widget_image_paint(struct paper_widget_image* widget, struct paper_render* render, const struct paper_rect* rcpaint)
{
	//rcpaint == 可绘制区域
	assert(widget);
	struct paper_widget* base = (struct paper_widget*)widget;
	/*paper_render_draw_image(render, widget->image,
		rcpaint->left,
		rcpaint->top,
		rcpaint->right - rcpaint->left,
		rcpaint->bottom - rcpaint->top);*/
	paper_render_draw_image_rect(render, widget->image, rcpaint);
	//paper_render_draw_image_flip(render, widget->image, rcpaint);
}

void paper_widget_image_free(struct paper_widget_image* widget)
{
	struct paper_widget* tmpWidget = (struct paper_widget*)widget;
	tmpWidget->free_widget(tmpWidget);
}

struct paper_widget_text* paper_widget_text_new(struct paper_widget_init_struct* init, struct paper_color* textcolor, struct paper_font* font, const wchar_t* text, uint32 len)
{
	assert(text);
	struct paper_widget_text* text_widget = (struct paper_widget_text*)malloc(sizeof(struct paper_widget_text));
	if (text_widget)
	{
		memset(text_widget, 0, sizeof(struct paper_widget_text));
		text_widget->text_font = font;
		text_widget->text = (wchar_t*)malloc(sizeof(wchar_t) * len + 2);
		text_widget->text_len = len;
		memcpy(&text_widget->text_color, textcolor, sizeof(struct paper_color));
		text_widget->text_align = paper_text_align_center;
		if (text_widget->text)
		{
			wcscpy(text_widget->text, text);
		}
		struct paper_widget* base = BASE_WIDGET(text_widget);
		base->paint = (paper_widget_paint_cb)paper_widget_text_paint;
	}
	return text_widget;
}

void paper_widget_text_paint(struct paper_widget_text* textwidget, struct paper_render* render, const struct paper_rect* rcpaint)
{
	//struct paper_rect rc = { 10, 10, 210, 42 };
	struct paper_brush* text_brush = paper_brush_create_solid(render, &textwidget->text_color);
	paper_render_draw_text(render, textwidget->text, textwidget->text_len, rcpaint, textwidget->text_font, text_brush);
	paper_brush_free(text_brush);
}

void paper_widget_text_set(struct paper_widget_text* textwidget, const wchar_t* text)
{
	uint32 len = (uint32)wcslen(text);
	textwidget->text = realloc(textwidget->text, len + 2);
	wmemcpy(textwidget->text, text, len);
	textwidget->text[len] = L'\0';
	textwidget->text_len = len;
}

void paper_widget_text_set_text(struct paper_widget_text* textwidget, const wchar_t* text, uint32 len)
{
	textwidget->text = realloc(textwidget->text, len + 2);
	wmemcpy(textwidget->text, text, len);
	textwidget->text[len] = L'\0';
	textwidget->text_len = len;
}

PAPER_API void paper_widget_text_set_font(struct paper_widget_text* textWidget, struct paper_font* font)
{
	textWidget->text_font = font;
}

void paper_widget_text_free(struct paper_widget_text* textwidget)
{
	free(textwidget->text);
	paper_font_free(textwidget->text_font);
	free(textwidget);
}

struct paper_button* paper_button_new(struct paper_widget_init_struct* init, const wchar_t* text, uint32 len)
{
	struct paper_button* button = (struct paper_button*)malloc(sizeof(struct paper_button));
	if (!button)
	{
		return NULL;
	}
	memset(button, 0, sizeof(struct paper_button));
	paper_widget_init((struct paper_widget*)button, init);
	struct paper_widget* widget = (struct paper_widget*)button;
	paper_widget_add_event(widget, PAPER_LISTEN_EVENT_LBUTTON | 
		PAPER_LISTEN_EVENT_MOUSEENTER | 
		PAPER_LISTEN_EVENT_MOUSELEAVE);
	widget->paint = (paper_widget_paint_cb)paper_button_paint;
	widget->on_mouse_enter = (paper_event_mouse_enter_cb)paper_button_on_mouseenter;
	widget->on_mouse_leave = (paper_event_mouse_leave_cb)paper_button_on_mouseleave;
	widget->on_mousebutton = (paper_event_mousebutton_cb)paper_button_on_mousebutton;
	button->current_brush = button->normal_brush;
	paper_widget_text_set_text((struct paper_widget_text*)button, text, len);
	paper_widget_text_set_font((struct paper_widget_text*)button, paper_get_default_font());
	return button;
}

void paper_button_paint(struct paper_button* button, struct paper_render* render, const struct paper_rect* rcpaint)
{
	struct paper_color clearColor = { 1.0f, 1.0f, 1.0f };
	struct paper_render* comp_render = paper_render_create_compatible(render, paper_rect_get_width(rcpaint), paper_rect_get_height(rcpaint));
	paper_render_begin_draw(comp_render, &clearColor);
	//paint background
	//paper_render_fill_rectangle(render, rcpaint, button->current_brush);
	
	//paint text
	paper_widget_text_paint((struct paper_widget_text*)button, comp_render, rcpaint);

	//end draw
	paper_render_end_draw(comp_render);
	struct paper_image* image = paper_image_get_from_render(comp_render);
	paper_render_draw_image(render, image, rcpaint->left, rcpaint->top, rcpaint->right - rcpaint->left, rcpaint->bottom - rcpaint->top);
	paper_render_free(comp_render);
	paper_image_free(image);
}

void paper_button_free(struct paper_button* button)
{
	free(button);
}

void paper_button_on_mouseenter(struct paper_button* button)
{
	if (button->mouse_button == VK_LBUTTON)
	{
		button->current_brush = button->hot_brush;
	}
	else
	{
		button->current_brush = button->pushed_brush;
	}
}

void paper_button_on_mouseleave(struct paper_button* button)
{
	button->current_brush = button->hot_brush;
}

void paper_button_on_mousebutton(struct paper_button* button, uint32 mouseid, int32 x, int32 y, int8 state)
{
	button->mouse_button = mouseid;
}

void paper_button_set_normal_brush(struct paper_button* button, struct paper_brush* brush)
{
	button->normal_brush = brush;
}

void paper_button_set_hot_brush(struct paper_button* button, struct paper_brush* brush)
{
	button->hot_brush = brush;
}

void paper_button_set_pushed_brush(struct paper_button* button, struct paper_brush* brush)
{
	button->pushed_brush = brush;
}

struct paper_overlay* paper_overlay_new(struct paper_widget_init_struct* init)
{
	struct paper_overlay* overlay = (struct paper_overlay*)malloc(sizeof(struct paper_overlay));
	if (!overlay)
	{
		return NULL;
	}
	memset(overlay, 0, sizeof(struct paper_overlay));
	struct paper_widget* base = (struct paper_widget*)overlay;
	paper_widget_init((struct paper_widget*)overlay, init);
	base->paint = (paper_widget_paint_cb)paper_overlay_paint;
	base->on_resize = (paper_event_resize_cb)paper_overlay_on_resize;
	base->listen_events = PAPER_LISTEN_EVENT_RESIZE;
	return overlay;
}

void paper_overlay_paint(struct paper_overlay* overlay, struct paper_render* render, const struct paper_rect* rcpaint)
{
	struct paper_color clearColor = { 1.0f, 1.0f, 1.0f };
	struct paper_render* comp_render = NULL;
	int32 width = rcpaint->right - rcpaint->left;
	int32 height = rcpaint->bottom - rcpaint->top;
	comp_render = paper_render_create_compatible(render, width, height);
	paper_render_begin_draw(comp_render, &clearColor);
	//在此处绘制overlay及其子控件...
	//uint32 count = paper_vector_get_count(overlay->slots);
	//struct paper_widget* base = (struct paper_widget*)overlay;
	struct paper_widget* slot = (struct paper_widget*)overlay->slot_begin;
	struct paper_rect rc_child = { 0, 0, width, height };
	while (slot)
	{
		slot->paint(slot, comp_render, &rc_child);
		slot = slot->next;
	}
	/*end draw*/
	paper_render_end_draw(comp_render);
	struct paper_image* image = paper_image_get_from_render(comp_render);
	paper_render_draw_image(render, image, rcpaint->left, rcpaint->top, width, height);
	paper_render_free(comp_render);
	paper_image_free(image);
}

void paper_overlay_free(struct paper_overlay* overlay)
{
	struct paper_widget* slot = (struct paper_widget*)overlay->slot_begin;
	while (slot)
	{
		struct paper_widget* tempWidget = slot;
		slot = slot->next;
		tempWidget->free_widget(tempWidget);
	}
	free(overlay);
}

void paper_overlay_add_slot(struct paper_overlay* overlay, struct paper_overlay_slot* slot)
{
	//paper_vector_add(overlay->slots, &slot);
	if (overlay->slot_begin == NULL)
	{
		overlay->slot_begin = slot;
		return;
	}
	if (overlay->slot_end == NULL)
	{
		overlay->slot_end = slot;
		((struct paper_widget*)(overlay->slot_begin))->next = (struct paper_widget*)overlay->slot_end;
		return;
	}
	struct paper_widget* overlay_base = (struct paper_widget*)overlay;
	struct paper_widget* slot_base = (struct paper_widget*)slot;
	struct paper_widget* slot_end_base = (struct paper_widget*)overlay->slot_end;
	slot_end_base->next = slot_base;
	slot_base->prev = slot_end_base;
	slot_end_base = slot_base;
	slot_end_base->next = NULL;
}

void paper_overlay_on_resize(struct paper_overlay* widget, int32 width, int32 height)
{

}

struct paper_widget* paper_overlay_pt_in_region(struct paper_overlay* overlay, const struct paper_rect* rcpaint, struct paper_point* pt)
{
	struct paper_widget* base = (struct paper_widget*)overlay;
	struct paper_rect real_rect;		//获取真实的坐标区域
	real_rect.left = rcpaint->left + base->rect.left;
	real_rect.top = rcpaint->top + base->rect.top;
	real_rect.right = rcpaint->right - base->rect.right;
	real_rect.bottom = rcpaint->bottom - base->rect.bottom;
	if (paper_widget_pt_in_region((struct paper_widget*)overlay, &real_rect, pt))
	{
		struct paper_widget* pWidget = (struct paper_widget*)overlay->slot_begin;
		while (pWidget)
		{
			struct paper_point mapPoint = { pt->x - real_rect.left, pt->y - real_rect.top };
			if (pWidget->pt_in_region(pWidget, &real_rect, &mapPoint))
			{
				return pWidget;
			}
			pWidget = pWidget->next;
		}

		/*for (uint32 i = 0; i < count; i++)
		{
			struct paper_widget* slot = NULL;
			paper_vector_get_value(overlay->slots, i, &slot);
			struct paper_point mapPoint = { pt->x - real_rect.left, pt->y - real_rect.top };
			if (slot->pt_in_region(slot, &real_rect, &mapPoint))
			{
				return slot;
			}
		}*/
		return (struct paper_widget*)overlay;
	}
	return NULL;		//0 == false
}

struct paper_overlay_slot* paper_overlay_slot_new()
{
	struct paper_overlay_slot* slot = (struct paper_overlay_slot*)malloc(sizeof(struct paper_overlay_slot));
	if (!slot)
	{
		return NULL;
	}
	memset(slot, 0, sizeof(struct paper_overlay_slot));
	struct paper_widget* base = (struct paper_widget*)slot;
	paper_widget_init((struct paper_widget*)slot, NULL);
	base->pt_in_region = (paper_widget_pt_in_region_cb)paper_overlay_slot_pt_in_region;
	base->paint = (paper_widget_paint_cb)paper_overlay_slot_paint;
	slot->halign = paper_halign_left;
	slot->valign = paper_valign_top;
	return slot;
}

void paper_overlay_slot_paint(struct paper_overlay_slot* slot, struct paper_render* render, const struct paper_rect* rcpaint)
{
	assert(slot);
	assert(slot->widget);
	assert(slot->widget->paint);
	struct paper_rect real_rect = { 0 };		//获取真实的坐标区域
	//real_rect.left = rcpaint->left;
	//real_rect.top = rcpaint->top;
	memcpy(&real_rect, &slot->widget->rect, sizeof(struct paper_rect));
	if (slot->halign == paper_halign_fill)
	{
		real_rect.left = rcpaint->left;
		real_rect.right = rcpaint->right;
	}
	else if (slot->halign == paper_halign_left)
	{
		real_rect.left = rcpaint->left;
		real_rect.right = real_rect.left + paper_rect_get_width(&slot->widget->rect);
	}
	else if (slot->halign == paper_halign_center)
	{
		paper_rect_hcenter_rect(&real_rect, rcpaint);
	}
	else if (slot->halign == paper_halign_right)
	{
		real_rect.right = rcpaint->right;
		real_rect.left = real_rect.right - paper_rect_get_width(&slot->widget->rect);
	}
	if (slot->valign == paper_valign_fill)
	{
		real_rect.top = rcpaint->top;
		real_rect.bottom = rcpaint->bottom;
	}
	else if(slot->valign == paper_valign_top)
	{
		real_rect.top = rcpaint->top;
		real_rect.bottom = real_rect.top + paper_rect_get_height(&slot->widget->rect);
	}
	else if (slot->valign == paper_valign_center)
	{
		paper_rect_vcenter_rect(&real_rect, rcpaint);
	}
	else if (slot->valign == paper_valign_bottom)
	{
		real_rect.bottom = rcpaint->bottom;
		real_rect.top = real_rect.bottom - paper_rect_get_height(&slot->widget->rect);
	}
	memcpy(&slot->widget->rect, &real_rect, sizeof(struct paper_rect));
	slot->widget->paint(slot->widget, render, &real_rect);
}

void paper_overlay_slot_free(struct paper_overlay_slot* slot)
{
	free(slot);
}

struct paper_widget* paper_overlay_slot_pt_in_region(struct paper_overlay_slot* overlay, const struct paper_rect* rcpaint, struct paper_point* pt)
{
	if (paper_rect_pt_in(rcpaint, pt->x, pt->y))
	{
		return (struct paper_widget*)overlay;
	}
	return NULL;
}

void paper_widget_queue_paint_all(struct paper_widget_queue* widget_queue, struct paper_render* render)
{
	//uint32 count = paper_vector_get_count(widget_queue->widgets);
	//for (uint32 i = 0; i < count; i++)
	//{
	//	struct paper_widget* widget = NULL;
	//	paper_vector_get_value(widget_queue->widgets, i, &widget);
	//	assert(widget);
	//	assert(widget->paint);		//防止渲染回调没有赋值
	//	widget->paint(widget, render, &widget->rect);
	//}
	struct paper_widget* widget = widget_queue->first_widget;
	while (widget)
	{
		widget->paint(widget, render, &widget_queue->paint_rect);
		widget = widget->next;
	}
}

struct paper_widget_queue* paper_widget_queue_new(uint32 width, uint32 height)
{
	struct paper_widget_queue* widget_queue = (struct paper_widget_queue*)malloc(sizeof(struct paper_widget_queue));
	if (!widget_queue)
	{
		return NULL;
	}
	widget_queue->paint_rect.left = 0;
	widget_queue->paint_rect.top = 0;
	widget_queue->paint_rect.right = width;
	widget_queue->paint_rect.bottom = height;
	widget_queue->enter_widget = NULL;
	widget_queue->first_widget = NULL;
	widget_queue->last_widget = NULL;
	widget_queue->widget_count = 0;
	return widget_queue;
}

void paper_widget_queue_add(struct paper_widget_queue* widget_queue, struct paper_widget* widget)
{
	//paper_vector_add(widget_queue->widgets, &widget);
	assert(widget);
	if (widget_queue->widget_count == 0)
	{
		widget_queue->widget_count++;
		widget_queue->first_widget = widget;
		widget_queue->first_widget->prev = NULL;
		widget_queue->first_widget->next = NULL;
		return;
	}
	if (widget_queue->widget_count == 1)
	{
		widget_queue->widget_count++;
		widget_queue->last_widget = widget;
		widget_queue->first_widget->next = widget_queue->last_widget;
		widget_queue->first_widget->prev = NULL;
		widget_queue->last_widget->next = NULL;
		return;
	}
	widget_queue->widget_count++;
	widget_queue->last_widget->next = widget;
	widget->prev = widget_queue->last_widget;
	widget->next = NULL;
	widget_queue->last_widget = widget;
}

void paper_widget_queue_remove(struct paper_widget_queue* widget_queue, struct paper_widget* widget)
{
	assert(widget);
	assert(widget_queue);
	if (widget->next)
	{
		if (widget->prev)
		{
			widget->next->prev = widget->prev;
			widget->prev->next = widget->next;
		}
	}
}

void paper_widget_queue_free(struct paper_widget_queue* widget_queue)
{
	struct paper_widget* widget = widget_queue->first_widget;
	while (widget)
	{
		struct paper_widget* temp = widget;
		widget = widget->next;
		temp->free_widget(temp);
	}
	free(widget_queue);
}

void paper_widget_queue_clear(struct paper_widget_queue* widget_queue)
{
	struct paper_widget* widget = widget_queue->first_widget;
	while (widget)
	{
		struct paper_widget* temp = widget;
		widget = widget->next;
		temp->free_widget(temp);
	}


	/*uint32 count = paper_vector_get_count(widget_queue->widgets);
	for (uint32 i = 0; i < count; i++)
	{
		struct paper_widget* widget = NULL;
		paper_vector_pop_back(widget_queue->widgets, &widget);
		free(widget);
	}*/
}

void paper_widget_queue_on_resize(struct paper_widget_queue* widget_queue, int32 width, int32 height)
{
	paper_rect_set_size(&widget_queue->paint_rect, width, height);
	struct paper_widget* widget = widget_queue->first_widget;
	while (widget)
	{
		if (widget->listen_events & PAPER_LISTEN_EVENT_RESIZE)
		{
			assert(widget->on_resize);		//防止渲染回调没有赋值
			widget->on_resize(widget, width, height);
		}
		widget = widget->next;
	}

	
	//uint32 count = paper_vector_get_count(widget_queue->widgets);
	//for (uint32 i = 0; i < count; i++)
	//{
	//	struct paper_widget* widget = NULL;
	//	paper_vector_get_value(widget_queue->widgets, i, &widget);
	//	if (widget->listen_events & PAPER_LISTEN_EVENT_RESIZE)
	//	{
	//		assert(widget->on_resize);		//防止渲染回调没有赋值
	//		widget->on_resize(widget, width, height);
	//	}
	//}
}

void paper_widget_queue_on_mousemove(struct paper_widget_queue* widget_queue, int32 x, int32 y)
{
	struct paper_point pt = { x, y };
	struct paper_widget* widget = NULL;
	if (widget_queue->enter_widget)
	{
		if (!widget_queue->enter_widget->pt_in_region(widget_queue->enter_widget, &widget_queue->paint_rect, &pt))
		{
			//widget_queue->enter_widget->on_mouse_leave(widget_queue->enter_widget);      
			paper_widget_should_handle_leave(widget_queue->enter_widget);	//鼠标离开了
			widget_queue->enter_widget = NULL;
		}
		else
		{
			return;
		}
	}
	widget = paper_widget_queue_findpoint(widget_queue, &pt);
	if (widget)
	{
		widget_queue->enter_widget = widget;
		paper_widget_should_handle_enter(widget_queue->enter_widget);	//鼠标进入了
	}
}

void paper_widget_queue_on_mousebutton(struct paper_widget_queue* widget_queue, uint32 button, int32 x, int32 y, int8 state)
{
	struct paper_point pt = { x, y };
	if (widget_queue->enter_widget)
	{
		widget_queue->enter_widget->on_mousebutton(widget_queue->enter_widget, button, x, y, state);
		return;
	}
	struct paper_widget* p = paper_widget_queue_findpoint(widget_queue, &pt);
	if (p)
	{
		p->on_mousebutton(p, button, x, y, state);
	}
	//struct paper_widget* widget = widget_queue->head;
	//while (widget)
	//{
	//	if (widget->listen_events & PAPER_LISTEN_EVENT_LBUTTON)
	//	{
	//		assert(widget->on_lbutton);		//防止渲染回调没有赋值
	//		
	//		
	//	}
	//	widget = widget->next;
	//}
}

void paper_widget_queue_on_mouseleave(struct paper_widget_queue* widget_queue)
{
	if (widget_queue->enter_widget)
	{
		paper_widget_should_handle_leave(widget_queue->enter_widget);
		widget_queue->enter_widget = NULL;
	}
}

struct paper_widget* paper_widget_queue_findpoint(struct paper_widget_queue* widget_queue, struct paper_point* pt)
{
	/*uint32 count = paper_vector_get_count(widget_queue->widgets);
	for (uint32 i = 0; i < count; i++)
	{
		struct paper_widget* widget = NULL;
		paper_vector_get_value(widget_queue->widgets, i, &widget);
		if (widget->listen_events & PAPER_LISTEN_EVENT_LBUTTON)
		{
			if (widget->pt_in_region(widget, &widget_queue->paint_rect, pt))
			{
				return widget;
			}
		}
	}
	return NULL;*/

	struct paper_widget* widget = widget_queue->first_widget;
	while (widget)
	{
		if (widget->listen_events & PAPER_LISTEN_EVENT_LBUTTON)
		{
			struct paper_widget* p = widget->pt_in_region(widget, &widget_queue->paint_rect, pt);
			if (p)
			{
				return p;
				//struct paper_point ptchild;
				//paper_widget_convert_point(widget, pt, &ptchild);
				//struct paper_widget* p = paper_widget_findpointinchild(widget->child, &ptchild);
				//if (p)
				//{
				//	return p;
				//}
				//else
				//{
				//	return widget;
				//}
			}
			return widget;
		}
		widget = widget->next;
	}
	return NULL;
}

void paper_widget_convert_point(struct paper_widget* widget, const struct paper_point* pt, struct paper_point* outpt)
{
	outpt->x = pt->x - widget->rect.left;
	outpt->y = pt->y - widget->rect.top;
}
//
//struct paper_widget* paper_widget_findpointinchild(struct paper_widget* widget, struct paper_point* pt)
//{
//	struct paper_widget* p = widget;
//	while (p)
//	{
//		if (p->pt_in_region(p, pt))
//		{
//			struct paper_point ptchild;
//			paper_widget_convert_point(p, pt, &ptchild);
//			widget = paper_widget_findpointinchild(p, &ptchild);
//			if (widget)
//			{
//				return widget;
//			}
//			else
//			{
//				return p;
//			}
//		}
//		p = p->next;
//	}
//
//	return NULL;
//}
