#include "paper_widget.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <Windows.h>
#include "paper_render.h"
#include "paper_vector.h"
#include "paper_event.h"
struct paper_widget* paper_widget_create(struct paper_widget_init_struct* init)
{
	struct paper_widget* widget = (struct paper_widget*)malloc(sizeof(struct paper_widget));
	if (!widget)
	{
		return widget;
	}
	memset(widget, 0, sizeof(struct paper_widget));
	if (init)
	{
		paper_widget_init(widget, init);
	}
	else
	{
		struct paper_widget_init_struct init2;
		init2.paint = paper_widget_paint;
		init2.pt_in_region = paper_widget_pt_in_region;
		init2.parent = NULL;
		init2.on_resize = paper_widget_on_resize;
		init2.on_mouse_enter = paper_widget_on_mouseenter;
		init2.on_mouse_leave = paper_widget_on_mouseleave;
		init2.on_lbutton = paper_widget_on_lbutton;
		paper_widget_init(widget, &init2);
	}
	
	return widget;
}

void paper_widget_init(struct paper_widget* widget, struct paper_widget_init_struct* init)
{
	widget->parent = init->parent;
	widget->child = NULL;
	widget->next = NULL;
	widget->prev = NULL;
	widget->paint = init->paint;
	widget->pt_in_region = init->pt_in_region;
	widget->on_mouse_enter = init->on_mouse_enter;
	widget->on_mouse_leave = init->on_mouse_leave;
	widget->on_resize = init->on_resize;
	widget->on_lbutton = init->on_lbutton;
	memcpy(&widget->rect, &init->rect, sizeof(struct paper_rect));
	widget->parent = init->parent;
	widget->render = init->render;
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

void paper_widget_paint(struct paper_widget* widget)
{
	paper_render_fill_rectangle(widget->render, &widget->rect, widget->background);
}

void paper_widget_free(struct paper_widget* widget)
{
	struct paper_widget* temp = widget->prev;
	struct paper_widget* temp_next = widget->next;
	if (temp)
	{
		temp->next = widget->next;
		temp_next->prev = temp;
		free(widget);
		return;
	}
	temp_next->prev = NULL;
	free(widget);
}

void paper_widget_map_global(struct paper_widget* widget, struct paper_rect* rect)
{
	memcpy(rect, &widget->rect_global, sizeof(struct paper_rect));
}

int8 paper_widget_pt_in_region(struct paper_widget* widget, struct paper_point* pt)
{
	//需要将其转化为全局坐标
	return paper_rect_pt_in(&widget->rect, pt->x, pt->y);
}

void paper_widget_add_event(struct paper_widget* widget, uint32 evtype)
{
	widget->listen_events |= evtype;
}

void paper_widget_on_resize(struct paper_widget* widget, int32 width, int32 height)
{

}

void paper_widget_on_mouseenter(struct paper_widget* widget)
{
	//struct paper_color color = {0.0f, 0.8f, 1.0f};
	//paper_brush_solid_setcolor(widget->background, &color);
	widget->background = widget->background2;
}

void paper_widget_on_mouseleave(struct paper_widget* widget)
{
	//struct paper_color color = { 0.1f, 1.8f, 1.0f };
	//paper_brush_solid_setcolor(widget->background, &color);
	widget->background = widget->background1;
}

void paper_widget_on_lbutton(struct paper_widget* widget, int32 x, int32 y, int8 state)
{

}

void paper_widget_queue_paint_all(struct paper_widget_queue* widget_queue)
{
	struct paper_widget* p = widget_queue->head;
	while (p)
	{
		assert(p->paint);		//防止渲染回调没有赋值
		p->paint(p);
		p = p->next;
	}
}

struct paper_widget_queue* paper_widget_queue_create()
{
	struct paper_widget_queue* widget_queue = (struct paper_widget_queue*)malloc(sizeof(struct paper_widget_queue));
	if (!widget_queue)
	{
		return NULL;
	}
	widget_queue->count = 0;
	widget_queue->head = NULL;
	widget_queue->end = NULL;
	widget_queue->enter_widget = NULL;
	return widget_queue;
}

void paper_widget_queue_add(struct paper_widget_queue* widget_queue, struct paper_widget* widget)
{
	if (widget_queue->count > 1)
	{
		widget_queue->end->next = widget;
		widget->prev = widget_queue->end;
		widget_queue->end = widget;
		widget_queue->count++;
		return;
	}
	if (!widget_queue->head)
	{
		widget_queue->head = widget;
		widget_queue->count = 1;
		return;
	}
	if (!widget_queue->end)
	{
		widget_queue->end = widget;
		widget_queue->end->prev = widget_queue->head;
		widget_queue->head->next = widget_queue->end;
		widget_queue->count++;
		return;
	}
}

void paper_widget_queue_free(struct paper_widget_queue* widget_queue)
{
	struct paper_widget* widget = widget_queue->head;
	while (widget)
	{
		struct paper_widget* temp = widget;
		widget = widget->next;
		free(temp);
	}
	free(widget_queue);
}

void paper_widget_queue_clear(struct paper_widget_queue* widget_queue)
{
	struct paper_widget* widget = widget_queue->head;
	while (widget)
	{
		struct paper_widget* temp = widget;
		widget = widget->next;
		free(temp);
	}
	widget_queue->count = 0;
	widget_queue->end = NULL;
	widget_queue->head = NULL;
}

void paper_widget_queue_on_resize(struct paper_widget_queue* widget_queue, int32 width, int32 height)
{
	struct paper_widget* widget = widget_queue->head;
	while (widget)
	{
		if (widget->listen_events & PAPER_LISTEN_EVENT_RESIZE)
		{
			assert(widget->on_resize);		//防止渲染回调没有赋值
			widget->on_resize(widget, width, height);
		}
		widget = widget->next;
	}
}

void paper_widget_queue_on_mousemove(struct paper_widget_queue* widget_queue, int32 x, int32 y)
{
	struct paper_point pt = { x, y };
	struct paper_widget* widget = widget_queue->head;
	if (widget_queue->enter_widget)
	{
		if (!widget->pt_in_region(widget_queue->enter_widget, &pt))
		{
			widget_queue->enter_widget->on_mouse_leave(widget);      //鼠标离开了
			widget_queue->enter_widget = NULL;
		}
		else
		{
			return;
		}
	}
	while (widget)
	{
		struct paper_rect widget_global_rect;
		paper_widget_map_global(widget, &widget_global_rect);   //获取当前widget的坐标在全局坐标的位置
		assert(widget->pt_in_region);
		if (widget->pt_in_region(widget, &pt))
		{
			if (widget->listen_events & PAPER_LISTEN_EVENT_MOUSEENTER)
			{
				assert(widget->on_mouse_enter);
				widget->on_mouse_enter(widget);          //鼠标进入了
			}
			widget_queue->enter_widget = widget;
			return;
		}
		widget = widget->next;
	}
}

void paper_widget_queue_on_lbutton(struct paper_widget_queue* widget_queue, int32 x, int32 y, int8 state)
{
	struct paper_widget* widget = widget_queue->head;
	while (widget)
	{
		if (widget->listen_events & PAPER_LISTEN_EVENT_LBUTTON)
		{
			assert(widget->on_lbutton);		//防止渲染回调没有赋值
			widget->on_lbutton(widget, x, y, state);
		}
		widget = widget->next;
	}
}
