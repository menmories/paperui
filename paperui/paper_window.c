#include "paper_window.h"
#include <Windows.h>
#include <stdio.h>
#define DEFAULT_WINDOW_CLASSNAME TEXT("PAPER_WINDOW")

struct paper_window
{
	void* winid;			//窗口句柄
};

struct paper_window* paper_window_create(const wchar_t* szTitle, WNDPROC proc, int32 x, int32 y, uint32 width, uint32 height)
{
	WNDCLASS wc = { 0 };
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = (WNDPROC)proc;		//窗口回调函数
	wc.hInstance = (HINSTANCE)GetModuleHandle(NULL);		//实例句柄
	wc.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = DEFAULT_WINDOW_CLASSNAME;
	if (!RegisterClass(&wc))
	{
		DWORD dwError = GetLastError();
		printf("RegisterClass error, error code:%u\n", dwError);
		return NULL;
	}
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	//12个参数！！
	HWND hWnd = CreateWindowEx(0, DEFAULT_WINDOW_CLASSNAME, szTitle,
		dwStyle, x, y, width, height, NULL, NULL, wc.hInstance, NULL);
	if (!hWnd)
	{
		return NULL;
	}
	struct paper_window* window = (struct paper_window*)malloc(sizeof(struct paper_window));
	if (!window)
	{
		return NULL;		//内存不足了
	}
	window->winid = hWnd;
	return window;
}


struct paper_window* paper_window_native_fromhandle(void* handle)
{
	struct paper_window* window = (struct paper_window*)malloc(sizeof(struct paper_window));
	if (!window)
	{
		return NULL;		//内存不足了
	}
	window->winid = handle;
	return window;
}

void paper_window_free(struct paper_window* window)
{
	free(window);
}

void paper_window_show(struct paper_window* window)
{
	ShowWindow((HWND)window->winid, SW_SHOW);
}

void paper_window_hide(struct paper_window* window)
{
	ShowWindow((HWND)window->winid, SW_HIDE);
}

void paper_window_set_size(struct paper_window* window, uint32 width, uint32 height)
{
	SetWindowPos((HWND)window->winid, HWND_NOTOPMOST, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void paper_window_set_pos(struct paper_window* window, int32 x, int32 y)
{
	SetWindowPos(window->winid, HWND_NOTOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void paper_window_show_maximized(struct paper_window* window)
{
	ShowWindow((HWND)window->winid, SW_MAXIMIZE);
}

void paper_window_show_minimized(struct paper_window* window)
{
	ShowWindow((HWND)window->winid, SW_MINIMIZE);
}

int32 paper_window_is_maximized(struct paper_window* window)
{
	return IsIconic((HWND)window->winid);
}

int32 paper_window_is_minimized(struct paper_window* window)
{
	return IsZoomed((HWND)window->winid);
}

