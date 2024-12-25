#include "Window/CWindowBase.h"
#include <assert.h>
#include <stdio.h>
#include "Render/CRect.h"

#define DEF_WINDOWCLASSNAME TEXT("PAPERUI_WINDOW")

CWindowBase::CWindowBase()
	: m_hWnd(nullptr)
	, m_nMaxWidth(65535)
	, m_nMaxHeight(65535)
	, m_nMinWidth(150)
	, m_nMinHeight(150)
{
}

CWindowBase::~CWindowBase()
{
}

void CWindowBase::Resize(int width, int height)
{
	SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void CWindowBase::Move(int x, int y)
{
	SetWindowPos(m_hWnd, HWND_NOTOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void CWindowBase::Show()
{
	::ShowWindow(m_hWnd, SW_SHOW);
}

void CWindowBase::ShowNormal()
{
	::ShowWindow(m_hWnd, SW_SHOWNORMAL);
}

void CWindowBase::Hide()
{
	::ShowWindow(m_hWnd, SW_SHOW);
}

void CWindowBase::ShowMaximized()
{
	ShowWindow(m_hWnd, SW_MAXIMIZE);
}

void CWindowBase::ShowMinimized()
{
	ShowWindow(m_hWnd, SW_MINIMIZE);
}

bool CWindowBase::IsMaximized()
{
	return (bool)::IsZoomed(m_hWnd);
}

bool CWindowBase::IsMinimized()
{
	return (bool)::IsIconic(m_hWnd);
}

void CWindowBase::SetMinimizedWidth(int width)
{
	m_nMinWidth = width;
}

void CWindowBase::SetMinimizedHeight(int height)
{
	m_nMinHeight = height;
}

void CWindowBase::SetMinimizedSize(int width, int height)
{
	m_nMaxWidth = width;
	m_nMaxHeight = height;
}

void CWindowBase::SetMaximizedSize(int width, int height)
{
	m_nMinWidth = width;
	m_nMinHeight = height;
}

void CWindowBase::SetMaximizedWidth(int width)
{
	m_nMaxWidth = width;
}

void CWindowBase::SetMaximizedHeight(int height)
{
	m_nMaxHeight = height;
}

void CWindowBase::CenterWindow()
{
	assert(IsWindow(m_hWnd));
	assert((GetWindowStyle(m_hWnd) & WS_CHILD) == 0);
	RECT rcWindow = { 0 };
	RECT rcScreen = { 0 };
	GetWindowRect(m_hWnd, &rcWindow);
	SystemParametersInfoW(SPI_GETWORKAREA, 0, (void*)&rcScreen, 0);
	int screenWidth = rcScreen.right - rcScreen.left;
	int screenHeight = rcScreen.bottom - rcScreen.top;
	int windowWidth = rcWindow.right - rcWindow.left;
	int windowHeight = rcWindow.bottom - rcWindow.top;
	int x = (screenWidth - windowWidth) / 2;
	int y = (screenHeight - windowHeight) / 2;
	SetWindowPos(m_hWnd, NULL, x, y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

const CRect CWindowBase::GetRect() const
{
	RECT rect = { 0 };
	GetClientRect(m_hWnd, &rect);
	return CRect(rect.left, rect.top, rect.right, rect.bottom);
}

const CRect CWindowBase::GetFramelessRect() const
{
	RECT rect = { 0 };
	GetWindowRect(m_hWnd, &rect);
	return CRect(rect.left, rect.top, rect.right, rect.bottom);
}

HWND CWindowBase::CreateNativeWindow(const char* title, int x, int y, int width, int height, HWND hParent)
{
	WNDCLASS wc = { 0 };
	wc.style = CS_VREDRAW | CS_HREDRAW | CS_CLASSDC;	// | CS_DBLCLKS;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = (WNDPROC)CWindowBase::__WndProc;		//窗口回调函数
	wc.hInstance = (HINSTANCE)GetModuleHandle(NULL);		//实例句柄
	wc.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = DEF_WINDOWCLASSNAME;
	if (!RegisterClass(&wc))
	{
		DWORD dwError = GetLastError();
		fprintf(stderr, "RegisterClass error, error code:%u\n", dwError);
		return NULL;
	}
	DWORD dwExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_CLIENTEDGE | WS_EX_APPWINDOW;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	RECT rcWindow;
	rcWindow.left = x;
	rcWindow.top = y;
	rcWindow.right = x + width;
	rcWindow.bottom = y + height;
	//HWND hParent = parent ? parent->winid : NULL;
	AdjustWindowRectEx(&rcWindow, dwStyle, FALSE, dwExStyle);		//将客户区大小转化为参数指定的大小
	//12个参数
	m_hWnd = CreateWindowExA(0,
		DEF_WINDOWCLASSNAME,
		title,
		dwStyle,
		rcWindow.left,
		rcWindow.top,
		rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top,
		NULL,
		NULL,
		wc.hInstance,
		reinterpret_cast<void*>(this));
	return m_hWnd;
}

LRESULT CWindowBase::NativeEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_GETMINMAXINFO)
	{
		LPMINMAXINFO minMaxInfo = (LPMINMAXINFO)lParam;
		/*MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(GetWindowId(), MONITOR_DEFAULTTONEAREST), &oMonitor);
		CRect rcWork(oMonitor.rcWork.left, oMonitor.rcWork.top, oMonitor.rcWork.right, oMonitor.rcWork.bottom);
		CRect rcMonitor(oMonitor.rcMonitor.left, oMonitor.rcMonitor.top, oMonitor.rcMonitor.right, oMonitor.rcMonitor.bottom);
		rcWork.OffsetRect(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);
		minMaxInfo->ptMaxPosition.x = rcWork.Left();
		minMaxInfo->ptMaxPosition.y = rcWork.Top();*/
		minMaxInfo->ptMaxTrackSize.x = m_nMaxWidth;
		minMaxInfo->ptMaxTrackSize.y = m_nMaxHeight;
		minMaxInfo->ptMinTrackSize.x = m_nMinWidth;
		minMaxInfo->ptMinTrackSize.y = m_nMinHeight;
		return 0;
	}
	if (uMsg == WM_ERASEBKGND)
	{
		return 0;
	}
	return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CWindowBase::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindowBase* window = nullptr;
	if (uMsg == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
		window = (CWindowBase*)lpcs->lpCreateParams;
		window->m_hWnd = hWnd;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	window = (CWindowBase*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (window)
	{
		return window->NativeEvent(uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

