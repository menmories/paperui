#ifndef __PAPER_WINDOW_H__
#define __PAPER_WINDOW_H__
#include "paper_define.h"
#include <windows.h>
//paper´°¿Ú
#ifdef __cplusplus
extern "C"
{
#endif
	struct paper_window;

	struct paper_window* paper_window_create(const wchar_t* szTitle, WNDPROC proc, int32 x, int32 y, uint32 width, uint32 height);

	struct paper_window* paper_window_native_fromhandle(void* handle);

	void paper_window_free(struct paper_window* window);

	void paper_window_show(struct paper_window* window);

	void paper_window_hide(struct paper_window* window);

	void paper_window_set_size(struct paper_window* window, uint32 width, uint32 height);

	void paper_window_set_pos(struct paper_window* window, int32 x, int32 y);

	void paper_window_show_maximized(struct paper_window* window);

	void paper_window_show_minimized(struct paper_window* window);

	int32 paper_window_is_maximized(struct paper_window* window);

	int32 paper_window_is_minimized(struct paper_window* window);
#ifdef __cplusplus
}
#endif

#endif	//__PAPER_WINDOW_H__

