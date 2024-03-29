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
#define PAPER_EVENT_MBUTTONDOWN WM_MBUTTONDOWN
#define PAPER_EVENT_MBUTTONUP	WM_MBUTTONUP
#define PAPER_EVENT_RBUTTONDOWN WM_RBUTTONDOWN
#define PAPER_EVENT_RBUTTONUP	WM_RBUTTONUP
#define PAPER_EVENT_MOUSELEAVE	WM_MOUSELEAVE
#define PAPER_EVENT_MOUSEENTER	WM_MOUSEHOVER
#define PAPER_EVENT_MOUSEWHEEL	WM_MOUSEWHEEL
#define PAPER_EVENT_MOUSEHWHEEL	WM_MOUSEHWHEEL
#define PAPER_EVENT_KEYDOWN		WM_KEYDOWN
#define PAPER_EVENT_KEYUP		WM_KEYUP
#define PAPER_EVENT_PAINT		WM_PAINT
#define PAPER_EVENT_SIZE		WM_SIZE
#define PAPER_EVENT_DESTROY		WM_DESTROY

	typedef void (*paper_event_cb)(struct paper_event* e);
	typedef void (*paper_event_mouse_enter_cb)(struct paper_widget* widget);
	typedef void (*paper_event_mouse_leave_cb)(struct paper_widget* widget);
	typedef void (*paper_event_resize_cb)(struct paper_widget* widget, int32 width, int32 height);		//窗口大小改变时触发此回调
	typedef void (*paper_event_mousebutton_cb)(struct paper_widget* widget, uint32 button, int32 x, int32 y, int8 state);
	typedef void (*paper_event_keydown_cb)(int key);
	typedef void (*paper_event_keyup_cb)(int key);

	typedef uint_ptr(__stdcall* handle_event_cb)(struct paper_event* event);

	//可监听基础事件列表
#define PAPER_LISTEN_EVENT_LBUTTON		0x01
#define PAPER_LISTEN_EVENT_MOUSEENTER	0x02
#define PAPER_LISTEN_EVENT_MOUSELEAVE	0x04
#define PAPER_LISTEN_EVENT_RESIZE		0x08
	//扩展事件列表。。。暂时不更新


	struct paper_event_callbacks
	{
		paper_event_mouse_enter_cb	on_mouse_enter;
		paper_event_mouse_leave_cb	on_mouse_leave;
		paper_event_resize_cb		on_resize;
		paper_event_mousebutton_cb	on_mousebutton;
	};

	struct paper_event_queue
	{
		struct paper_event* event;
		struct paper_vector* evqueue;
		struct paper_widget* enterwidget;       //进入的widget
		paper_event_mouse_enter_cb on_mouse_enter;
		paper_event_mouse_leave_cb on_mouse_leave;
	};

	struct paper_event
	{
		void* source;			//事件来源句柄
		uint32 type;
		uint_ptr param1;
		uint_ptr param2;
	};

	PAPER_API int32 paper_event_run(struct paper_event* event);

	PAPER_API uint_ptr paper_event_handle(struct paper_event* event);

	PAPER_API void paper_event_enable_main_looper();

	/*分发事件，调用此函数进行阻塞式事件循环*/
	PAPER_API int32 paper_event_dispatch();
	/*设置窗口事件处理回调*/
	PAPER_API void paper_set_window_event_cb(handle_event_cb cb);

	PAPER_API handle_event_cb paper_get_event_cb(void);


	/*循环体，每一个线程只有一个循环*/
	typedef void(*LooperInvokeCb)(void* param);
	struct paper_looper;

	PAPER_API struct paper_looper* paper_looper_new();

	PAPER_API void paper_looper_free(struct paper_looper* looper);

	/*获取主循环体*/
	PAPER_API struct paper_looper* paper_get_main_looper();

	/*使用循环体调用操作，一般用于异步更新UI*/
	PAPER_API int32 paper_looper_invoke(struct paper_looper* looper, LooperInvokeCb cb, void* params);

	//PAPER_API LRESULT CALLBACK handle_windows_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#ifdef __cplusplus
}
#endif
#endif	//__PAPER_EVENT_H__

