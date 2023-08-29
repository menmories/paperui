#include "paper_event.h"
#include <Windows.h>
#include "paper_widget.h"
#include "paper_window.h"

static uint8 paper_key[255];

int32 paper_event_run(struct paper_event* event)
{
	//窗口消息事件循环
	MSG msg = { 0 };
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);      //WM_QUIT
		event->type = msg.message;
		event->param1 = msg.wParam;
		event->param2 = msg.lParam;
		return 1;
	}
	return 0;
}

uint_ptr paper_event_handle(struct paper_event* event)
{
	//在此处处理paperui事件

	return 0;
}


int32 paper_event_dispatch()
{
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);      //WM_QUIT
	}
	return (int32) msg.wParam;
}

void paper_event_add(struct paper_event* event)
{

}

void paper_event_handle_windows_key(WPARAM wParam, LPARAM lParam)
{

}

uint_ptr CALLBACK handle_windows_message(struct paper_event* event);
LRESULT CALLBACK paper_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	struct paper_window* window = NULL;
	if (uMsg == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
		window = (struct paper_window*)lpcs->lpCreateParams;
		paper_window_set_native_id(window, hWnd);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	window = (struct paper_window*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (uMsg == WM_ERASEBKGND)
	{
		return FALSE;
	}
	if (uMsg == WM_QUIT)
	{
		return FALSE;
	}
	if (uMsg == WM_NCDESTROY)
	{
		paper_window_free(window);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
		return FALSE;
	}
	if (window)
	{
		struct paper_event event;
		event.window = window;
		event.type = uMsg;
		event.param1 = wParam;
		event.param2 = lParam;
		return handle_windows_message(&event);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

