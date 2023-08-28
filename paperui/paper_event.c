#include "paper_event.h"
#include <Windows.h>

static uint8 paper_key[255];

int32 paper_event_run(struct paper_event* event)
{
	//窗口消息事件循环
	MSG msg = { 0 };
	if (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);      //WM_QUIT
		if (msg.message == WM_QUIT)
		{
			event->type = paper_event_quit;
			event->code = (int32)msg.wParam;
		}
		return 1;
	}
	return 0;
}

uint_ptr paper_event_handle(struct paper_event* event)
{
	//在此处处理paperui事件

	return 0;
}


void paper_event_handle_windows_key(WPARAM wParam, LPARAM lParam)
{

}

LRESULT CALLBACK handle_windows_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		paper_key[wParam] = 1;
		break;
	}
	case WM_KEYUP:
	{
		break;
	}
	case WM_PAINT:
	{

		break;
	}
	case WM_CLOSE:
	{
		break;
	}
	case WM_DESTROY:
	{
		break;
	}
	case WM_ERASEBKGND: 
	{
		return FALSE;
	}
	default:
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	}
	return FALSE;
}
