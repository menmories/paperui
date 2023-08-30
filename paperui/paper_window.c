#include "paper_window.h"
#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>
#include "paper_vector.h"
#include "paper_memorypool.h"
#include "paper_render.h"
#include "paper_event.h"
#include "paper_widget.h"
#include <assert.h>
#define DEFAULT_WINDOW_CLASSNAME TEXT("PAPER_WINDOW")

struct paper_window
{
	void* winid;				//���ھ��
	struct paper_render* render;
	struct paper_widget_queue* widget_queue;				//�ؼ����У���Ҫ������Ⱦ�ʹ����¼�
};

static uint32 window_count = 0;			//�������ü����ﵽ0������˳�

extern LRESULT CALLBACK paper_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int_ptr CALLBACK handle_windows_message(struct paper_event* event)
{
	struct paper_window* window = event->source;
	if (event->type == PAPER_EVENT_PAINT)
	{
		static struct paper_color color = { 0.1f, 1.0f, 1.0f, 1.0f };
		paper_render_begin_draw(window->render);
		paper_render_clear(window->render, &color);
		paper_widget_queue_paint_all(window->widget_queue);
		paper_render_end_draw(window->render);
		return 0;
	}
	if (event->type == PAPER_EVENT_SIZE)
	{
		int32 width = LOWORD(event->param2);
		int32 height = HIWORD(event->param2);
		paper_render_resize(window->render, width, height);
		paper_widget_queue_on_resize(window->widget_queue, width, height);
		return 0;
	}
	if (event->type == PAPER_EVENT_LBUTTONDOWN)
	{
		int32 x = LOWORD(event->param2);
		int32 y = HIWORD(event->param2);
		paper_widget_queue_on_lbutton(window->widget_queue, x, y, 1);
		return 0;
	}
	if (event->type == PAPER_EVENT_LBUTTONUP)
	{
		int32 x = LOWORD(event->param2);
		int32 y = HIWORD(event->param2);
		paper_widget_queue_on_lbutton(window->widget_queue, x, y, 0);
	}
	if (event->type == PAPER_EVENT_INIT)
	{
		//�ڴ˴���ʼ����Ⱦ����
		RECT rcClient;
		GetClientRect((HWND)window->winid, &rcClient);
		window->render = paper_render_create(window->winid, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
		window->widget_queue = paper_widget_queue_create();
		window_count++;		//���ڴ����ɹ����������ü���+1
		return 0;
	}
	if (event->type == PAPER_EVENT_DESTROY)
	{
		window_count--;
		if (window_count == 0)
		{
			PostQuitMessage(0);
		}
		return 0;
	}
	return DefWindowProc((HWND)window->winid,
		event->type,
		event->param1,
		event->param2);
}

void paper_window_set_default_eventcb()
{
	paper_set_event_cb(handle_windows_message);
}

struct paper_window* paper_window_create(const wchar_t* szTitle, int32 x, int32 y, uint32 width, uint32 height, struct paper_window* parent)
{
	return paper_window_create_native(szTitle, paper_wnd_proc, x, y, width, height, parent);
}

struct paper_window* paper_window_create_native(const wchar_t* szTitle, WNDPROC proc, int32 x, int32 y, uint32 width, uint32 height, struct paper_window* parent)
{
	WNDCLASS wc = { 0 };
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = (WNDPROC)proc;		//���ڻص�����
	wc.hInstance = (HINSTANCE)GetModuleHandle(NULL);		//ʵ�����
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
	struct paper_window* window = (struct paper_window*)malloc(sizeof(struct paper_window));
	if (!window)
	{
		return NULL;		//�ڴ治����
	}
	DWORD dwExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_CLIENTEDGE | WS_EX_APPWINDOW;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	RECT rcWindow;
	rcWindow.left = x;
	rcWindow.top = y;
	rcWindow.right = x + width;
	rcWindow.bottom = y + height;
	HWND hParent = parent ? parent->winid : NULL;
	AdjustWindowRectEx(&rcWindow, dwStyle, FALSE, dwExStyle);		//���ͻ�����Сת��Ϊ����ָ���Ĵ�С
	//12������
	HWND hWnd = CreateWindowEx(0, 
		DEFAULT_WINDOW_CLASSNAME, 
		szTitle,
		dwStyle, 
		rcWindow.left, 
		rcWindow.top, 
		rcWindow.right - rcWindow.left, 
		rcWindow.bottom - rcWindow.top, 
		NULL, 
		NULL, 
		wc.hInstance, 
		window);
	if (!hWnd)
	{
		free(window);
		return NULL;
	}
	return window;
}

struct paper_window* paper_window_create_from_native_handle(void* handle)
{
	struct paper_window* window = (struct paper_window*)malloc(sizeof(struct paper_window));
	if (!window)
	{
		return NULL;		//�ڴ治����
	}
	window->winid = handle;
	return window;
}

void paper_window_free_form_native_handle(struct paper_window* window)
{
	free(window);
}

void paper_window_destroy(struct paper_window* window)
{
	assert(IsWindow(window->winid));
	DestroyWindow(window->winid);
}

void* paper_window_get_native_id(struct paper_window* window)
{
	return window->winid;
}

void paper_window_set_native_id(struct paper_window* window, void* winid)
{
	assert(window);
	assert(winid);
	window->winid = winid;
}

void paper_window_free(struct paper_window* window)
{
	if (window->widget_queue)
	{
		paper_widget_queue_free(window->widget_queue);
		window->widget_queue = NULL;
	}
	if (window->render)
	{
		paper_render_free(window->render);
		window->render = NULL;
	}
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

void paper_window_center_screen(struct paper_window* window)
{
	assert(IsWindow(window->winid));
	assert((GetWindowStyle(window->winid) & WS_CHILD) == 0);
	RECT rcWindow = { 0 };
	RECT rcScreen = { 0 };
	GetWindowRect(window->winid, &rcWindow);
	SystemParametersInfoW(SPI_GETWORKAREA, 0, (void*) & rcScreen, 0);
	int32 screenWidth = rcScreen.right - rcScreen.left;
	int32 screenHeight = rcScreen.bottom - rcScreen.top;
	int32 windowWidth = rcWindow.right - rcWindow.left;
	int32 windowHeight = rcWindow.bottom - rcWindow.top;
	int32 x = (screenWidth - windowWidth) / 2;
	int32 y = (screenHeight - windowHeight) / 2;
	SetWindowPos(window->winid, NULL, x, y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
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

uint_ptr paper_window_default_handle(struct paper_event* event)
{
	struct paper_window* window = event->source;
	return DefWindowProc(window->winid, event->type, event->param1, event->param2);
}

