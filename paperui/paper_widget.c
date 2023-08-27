#include "paper_widget.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
struct paper_widget* paper_widget_create(struct paper_render* render, paper_widget_paint_cb cb)
{
	struct paper_widget* widget = (struct paper_widget*)malloc(sizeof(struct paper_widget));
	if (!widget)
	{
		return widget;
	}
	widget->parent = NULL;
	widget->child = NULL;
	widget->next = NULL;
	widget->prev = NULL;
	widget->on_paper_widget_paint = cb;
	widget->render = render;
	memset(&widget->rect, 0, sizeof(struct paper_rect));
	return widget;
}

void paper_widget_init(struct paper_widget* widget, struct paper_render* render, paper_widget_paint_cb cb, struct paper_rect* rect)
{
	widget->parent = NULL;
	widget->child = NULL;
	widget->next = NULL;
	widget->prev = NULL;
	widget->on_paper_widget_paint = cb;
	memcpy(&widget->rect, rect, sizeof(struct paper_rect));
	widget->render = render;
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

void paper_widget_paint_all(struct paper_widget_queue* widget_queue)
{
	struct paper_widget* p = widget_queue->head;
	while (p)
	{
		assert(p->on_paper_widget_paint);		//防止渲染回调没有赋值
		p->on_paper_widget_paint(p);
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
