#ifndef __PAPER_EVENT_H__
#define __PAPER_EVENT_H__


#include "paper_define.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define PAPER_EVENT_INIT		WM_CREATE
#define PAPER_EVENT_QUIT		WM_QUIT
#define PAPER_EVENT_MOUSEMOVE	WM_MOUSEMOVE
#define PAPER_EVENT_LBUTTONDOWN WM_LBUTTONDOWN
#define PAPER_EVENT_LBUTTONUP	WM_LBUTTONUP
#define PAPER_EVENT_MOUSELEAVE	WM_MOUSELEAVE
#define PAPER_EVENT_MOUSEENTER	WM_MOUSEHOVER
#define PAPER_EVENT_MOUSEWHEEL	WM_MOUSEWHEEL
#define PAPER_EVENT_MOUSEHWHEEL	WM_MOUSEHWHEEL
#define PAPER_EVENT_KEYDOWN		WM_KEYDOWN
#define PAPER_EVENT_KEYUP		WM_KEYUP
#define PAPER_EVENT_PAINT		WM_PAINT
#define PAPER_EVENT_SIZE		WM_SIZE
#define PAPER_EVENT_DESTROY		WM_DESTROY

	typedef void (*paper_widget_paint_cb)(struct paper_widget* widget);
	typedef void (*paper_event_cb)(struct paper_event* e);
	typedef void (*paper_event_mouse_enter_cb)();
	typedef void (*paper_event_mouse_leave_cb)();
	typedef void (*paper_event_mouse_button_cb)(int mousebutton, int state);
	typedef void (*paper_event_keydown_cb)(int key);
	typedef void (*paper_event_keyup_cb)(int key);

	struct paper_event
	{
		struct paper_window* window;
		uint32 type;
		uint_ptr param1;
		uint_ptr param2;
	};

	struct paper_event_listener
	{
		struct paper_window* window;
		struct paper_event* e;
	};

	void paper_event_listener_set(struct paper_event_listener* listener);


	extern uint8 paper_key[255];

	PAPER_API int32 paper_event_run(struct paper_event* event);

	PAPER_API uint_ptr paper_event_handle(struct paper_event* event);

	PAPER_API int32 paper_event_dispatch();

	PAPER_API void paper_event_add(struct paper_event* event);

	//PAPER_API LRESULT CALLBACK handle_windows_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#ifdef __cplusplus
}
#endif
#endif	//__PAPER_EVENT_H__

