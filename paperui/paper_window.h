#ifndef __PAPER_WINDOW_H__
#define __PAPER_WINDOW_H__
#include "paper_define.h"
#include <windows.h>
//paper����
#ifdef __cplusplus
extern "C"
{
#endif
	struct paper_window;
#define PAPER_CURSOR_ARROW IDC_ARROW
#define PAPER_CURSOR_HAND  IDC_HAND

	//������Ϣѭ��ΪĬ����Ϣѭ��
	PAPER_API void paper_window_set_default_eventcb();

	PAPER_API struct paper_window* paper_window_create(const wchar_t* szTitle, int32 x, int32 y, uint32 width, uint32 height, struct paper_window* parent);

	PAPER_API struct paper_window* paper_window_create2(const wchar_t* szTitle, int32 x, int32 y, uint32 width, uint32 height);

	PAPER_API struct paper_window* paper_window_create3(int32 x, int32 y, uint32 width, uint32 height);

	PAPER_API struct paper_window* paper_window_create_native(const wchar_t* szTitle, WNDPROC proc, int32 x, int32 y, uint32 width, uint32 height, struct paper_window* parent);

	/*�ӱ��ش���HWND��ȡһ��paper_window���*/
	PAPER_API struct paper_window* paper_window_create_from_native_handle(void* handle);
	/*�ͷŴ�paper_window_create_from_native_handle��ȡ�ľ��*/
	PAPER_API void paper_window_free_form_native_handle(struct paper_window* window);
	
	/*���ٴ���*/
	PAPER_API void paper_window_destroy(struct paper_window* window);

	PAPER_API void* paper_window_get_native_id(struct paper_window* window);

	PAPER_API void paper_window_set_native_id(struct paper_window* window, void* winid);

	PAPER_API void paper_window_set_render(struct paper_window* window, struct paper_render* render);

	PAPER_API void paper_window_set_clearcolor(struct paper_window* window, struct paper_color* color);

	PAPER_API void paper_window_free(struct paper_window* window);

	PAPER_API void paper_window_show(struct paper_window* window);

	PAPER_API void paper_window_hide(struct paper_window* window);

	PAPER_API void paper_window_set_size(struct paper_window* window, uint32 width, uint32 height);

	PAPER_API void paper_window_set_pos(struct paper_window* window, int32 x, int32 y);

	/*�������������Ļ������ʾ*/
	PAPER_API void paper_window_center_screen(struct paper_window* window);

	PAPER_API void paper_window_show_maximized(struct paper_window* window);

	PAPER_API void paper_window_show_minimized(struct paper_window* window);

	PAPER_API int32 paper_window_is_maximized(struct paper_window* window);

	PAPER_API int32 paper_window_is_minimized(struct paper_window* window);

	PAPER_API uint_ptr paper_window_default_handle(struct paper_event* event);

	PAPER_API struct paper_render* paper_window_get_render(struct paper_window* window);

	PAPER_API void paper_window_add_widget(struct paper_window* window, struct paper_widget* widget);

	PAPER_API void paper_window_set_root_widget(struct paper_window* window, struct paper_widget* widget);

	PAPER_API struct paper_widget* paper_window_get_root_widget(struct paper_window* window);

	PAPER_API void paer_window_set_cursor(struct paper_window* window, TCHAR* source);

	PAPER_API void paer_window_set_icon(struct paper_window* window, uint32 sourceid);
	
	//windows event dispatcher.
	//void paper_window_add_event(struct paper_window* window, int32 type, struct paper_widget* widget);

#ifdef __cplusplus
}
#endif

#endif	//__PAPER_WINDOW_H__

