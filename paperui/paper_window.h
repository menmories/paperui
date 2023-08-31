#ifndef __PAPER_WINDOW_H__
#define __PAPER_WINDOW_H__
#include "paper_define.h"
#include <windows.h>
//paper窗口
#ifdef __cplusplus
extern "C"
{
#endif
	struct paper_window;

	//设置消息循环为默认消息循环
	void paper_window_set_default_eventcb();

	struct paper_window* paper_window_create(const wchar_t* szTitle, int32 x, int32 y, uint32 width, uint32 height, struct paper_window* parent);

	struct paper_window* paper_window_create_native(const wchar_t* szTitle, WNDPROC proc, int32 x, int32 y, uint32 width, uint32 height, struct paper_window* parent);

	struct paper_window* paper_window_create_from_native_handle(void* handle);

	void paper_window_free_form_native_handle(struct paper_window* window);
	
	void paper_window_destroy(struct paper_window* window);

	void* paper_window_get_native_id(struct paper_window* window);

	void paper_window_set_native_id(struct paper_window* window, void* winid);

	void paper_window_free(struct paper_window* window);

	void paper_window_show(struct paper_window* window);

	void paper_window_hide(struct paper_window* window);

	void paper_window_set_size(struct paper_window* window, uint32 width, uint32 height);

	void paper_window_set_pos(struct paper_window* window, int32 x, int32 y);

	/*将窗口相对于屏幕居中显示*/
	void paper_window_center_screen(struct paper_window* window);

	void paper_window_show_maximized(struct paper_window* window);

	void paper_window_show_minimized(struct paper_window* window);

	int32 paper_window_is_maximized(struct paper_window* window);

	int32 paper_window_is_minimized(struct paper_window* window);

	uint_ptr paper_window_default_handle(struct paper_event* event);

	struct paper_render* paper_window_get_render(struct paper_window* window);

	void paper_window_add_widget(struct paper_window* window, struct paper_widget* widget);
	
	//windows event dispatcher.
	//void paper_window_add_event(struct paper_window* window, int32 type, struct paper_widget* widget);

#ifdef __cplusplus
}
#endif

#endif	//__PAPER_WINDOW_H__

