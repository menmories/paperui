#include "paper_event.h"
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "paper_widget.h"
#include "paper_window.h"

static uint8 paper_key[255];

static handle_event_cb handle_window_event;

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

void paper_set_window_event_cb(handle_event_cb cb)
{
	handle_window_event = cb;
}

handle_event_cb paper_get_event_cb(void)
{
	return handle_window_event;
}

void paper_event_handle_windows_key(WPARAM wParam, LPARAM lParam)
{

}

//extern uint_ptr CALLBACK handle_windows_message(struct paper_event* event);
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
	//if (uMsg == PAPER_EVENT_LBUTTONDOWN)
	//{
	//	int32 x = GET_X_LPARAM(lParam);
	//	int32 y = GET_Y_LPARAM(lParam);
	//	static int32 count = 0;
	//	TCHAR szText[1024];
	//	wsprintf(szText, L"鼠标左键按下：%d次", ++count);
	//	SetWindowText(hWnd, szText);
	//	//printf("鼠标左键按下：%d次\n", ++count);
	//	//paper_widget_queue_on_lbutton(window->widget_queue, x, y, 1);
	//	return 0;
	//}
	if (uMsg == WM_ERASEBKGND)
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
		event.source = window;
		event.type = uMsg;
		event.param1 = wParam;
		event.param2 = lParam;
		return handle_window_event(&event);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

