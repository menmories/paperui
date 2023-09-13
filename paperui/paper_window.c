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
	void* winid;				//窗口句柄
	uint8 bTrackingMouse;
	struct paper_color clear_color;
	struct paper_render* render;
	struct paper_widget_queue* widget_queue;				//控件队列，主要用于渲染和处理事件
};

static uint32 window_count = 0;			//窗口引用计数达到0则程序退出

extern LRESULT CALLBACK paper_wnd_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int_ptr CALLBACK handle_windows_message(struct paper_event* event)
{
	struct paper_window* window = event->source;
	if (event->type == PAPER_EVENT_PAINT)
	{
		//static struct paper_color color = { 1.1f, 1.0f, 1.0f, 1.0f };
		if (paper_window_is_minimized(window))
		{
			paper_render_begin_draw(window->render);
			paper_render_end_draw(window->render);
			ValidateRect(window->winid, NULL);
			return 0;
		}
		paper_render_begin_draw(window->render);
		paper_render_clear(window->render, &window->clear_color);
		struct paper_render* topRender = paper_render_create_compatible_extendsize(window->render);
		paper_render_begin_draw(topRender);
		paper_widget_queue_paint_all(window->widget_queue, topRender);
		paper_render_end_draw(topRender);
		struct paper_image* image = paper_image_get_from_render(topRender);
		paper_render_draw_image3(window->render, image);
		paper_render_end_draw(window->render);
		paper_image_free(image);
		paper_render_free(topRender);
		return 0;
	}
	if (event->type == PAPER_EVENT_MOUSEMOVE)
	{
		int32 x = GET_X_LPARAM(event->param2);
		int32 y = GET_Y_LPARAM(event->param2);
		if (!window->bTrackingMouse)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;			//鼠标离开消息,也可以或上TME_HOVER
			tme.hwndTrack = window->winid;		//按钮句标
			tme.dwHoverTime = 1000;				//鼠标离开窗口后间隔1000毫秒触发消息
			if (TrackMouseEvent(&tme))			//向系统窗口管理器注册鼠标消息
			{
				window->bTrackingMouse = 1;
			}
		}
		paper_widget_queue_on_mousemove(window->widget_queue, x, y);
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
		int32 x = GET_X_LPARAM(event->param2);
		int32 y = GET_Y_LPARAM(event->param2);
		paper_widget_queue_on_mousebutton(window->widget_queue, VK_LBUTTON, x, y, 1);
		return 0;
	}
	if (event->type == PAPER_EVENT_LBUTTONUP)
	{
		int32 x = GET_X_LPARAM(event->param2);
		int32 y = GET_Y_LPARAM(event->param2);
		paper_widget_queue_on_mousebutton(window->widget_queue, VK_LBUTTON, x, y, 0);
		return 0;
	}
	if (event->type == PAPER_EVENT_MBUTTONDOWN)
	{
		int32 x = GET_X_LPARAM(event->param2);
		int32 y = GET_Y_LPARAM(event->param2);
		paper_widget_queue_on_mousebutton(window->widget_queue, VK_MBUTTON, x, y, 1);
		return 0;
	}
	if (event->type == PAPER_EVENT_MBUTTONUP)
	{
		int32 x = GET_X_LPARAM(event->param2);
		int32 y = GET_Y_LPARAM(event->param2);
		paper_widget_queue_on_mousebutton(window->widget_queue, VK_MBUTTON, x, y, 0);
		return 0;
	}
	if (event->type == PAPER_EVENT_RBUTTONDOWN) 
	{
		int32 x = GET_X_LPARAM(event->param2);
		int32 y = GET_Y_LPARAM(event->param2);
		paper_widget_queue_on_mousebutton(window->widget_queue, VK_RBUTTON, x, y, 1);
		return 0;
	}
	if (event->type == PAPER_EVENT_RBUTTONUP)
	{
		int32 x = GET_X_LPARAM(event->param2);
		int32 y = GET_Y_LPARAM(event->param2);
		paper_widget_queue_on_mousebutton(window->widget_queue, VK_RBUTTON, x, y, 0);
		return 0;
	}
	if (event->type == PAPER_EVENT_MOUSELEAVE)
	{
		paper_widget_queue_on_mouseleave(window->widget_queue);
		window->bTrackingMouse = 0;
		return 0;
	}
	if (event->type == PAPER_EVENT_MOUSEENTER)
	{
		window->bTrackingMouse = 0;
		return 0;
	}
	if (event->type == PAPER_EVENT_INIT)
	{
		//在此处初始化渲染队列
		RECT rcClient;
		GetClientRect((HWND)window->winid, &rcClient);
		uint32 width = rcClient.right - rcClient.left;
		uint32 height = rcClient.bottom - rcClient.top;
		window->render = paper_render_create(window->winid, width, height);
		window->widget_queue = paper_widget_queue_create(width, height);
		window_count++;		//窗口创建成功，窗口引用计数+1
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
	paper_set_window_event_cb(handle_windows_message);
}

struct paper_window* paper_window_create(const wchar_t* szTitle, int32 x, int32 y, uint32 width, uint32 height, struct paper_window* parent)
{
	return paper_window_create_native(szTitle, paper_wnd_proc, x, y, width, height, parent);
}

struct paper_window* paper_window_create2(const wchar_t* szTitle, int32 x, int32 y, uint32 width, uint32 height)
{
	return paper_window_create_native(szTitle, paper_wnd_proc, x, y, width, height, NULL);
}

struct paper_window* paper_window_create3(int32 x, int32 y, uint32 width, uint32 height)
{
	return paper_window_create_native(L"", paper_wnd_proc, x, y, width, height, NULL);
}

struct paper_window* paper_window_create_native(const wchar_t* szTitle, WNDPROC proc, int32 x, int32 y, uint32 width, uint32 height, struct paper_window* parent)
{
	WNDCLASS wc = { 0 };
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_CLASSDC;	// | CS_DBLCLKS;
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
	struct paper_window* window = (struct paper_window*)malloc(sizeof(struct paper_window));
	if (!window)
	{
		return NULL;		//内存不足了
	}
	memset(window, 0, sizeof(struct paper_window));
	DWORD dwExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_CLIENTEDGE | WS_EX_APPWINDOW;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	RECT rcWindow;
	rcWindow.left = x;
	rcWindow.top = y;
	rcWindow.right = x + width;
	rcWindow.bottom = y + height;
	HWND hParent = parent ? parent->winid : NULL;
	AdjustWindowRectEx(&rcWindow, dwStyle, FALSE, dwExStyle);		//将客户区大小转化为参数指定的大小
	window->clear_color.r = 1.0f;
	window->clear_color.g = 1.0f;
	window->clear_color.b = 1.0f;
	window->clear_color.a = 1.0f;
	//12个参数
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
		return NULL;		//内存不足了
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
	assert(window);
	return window->winid;
}

void paper_window_set_native_id(struct paper_window* window, void* winid)
{
	assert(window);
	assert(winid);
	window->bTrackingMouse = 0;
	window->winid = winid;
}

void paper_window_set_render(struct paper_window* window, struct paper_render* render)
{
	assert(window);
	assert(render);
	window->render = render;
}

void paper_window_set_clearcolor(struct paper_window* window, struct paper_color* color)
{
	memcpy(&window->clear_color, color, sizeof(struct paper_color));
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

struct paper_render* paper_window_get_render(struct paper_window* window)
{
	return window->render;
}

void paper_window_add_widget(struct paper_window* window, struct paper_widget* widget)
{
	paper_widget_queue_add(window->widget_queue, widget);
}

void paer_window_set_cursor(struct paper_window* window, TCHAR* source)
{
	HCURSOR hCursor = LoadCursor(NULL, source);
	SetCursor(hCursor);
}

void paer_window_set_icon(struct paper_window* window, uint32 sourceid)
{
	HICON hicon = LoadIcon(NULL, MAKEINTRESOURCE(sourceid));
	SendMessage(window->winid, WM_SETICON, ICON_BIG, (LPARAM)hicon);
	SendMessage(window->winid, WM_SETICON, ICON_SMALL, (LPARAM)hicon);
}

