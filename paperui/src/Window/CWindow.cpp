#include "Window/CWindow.h"
#include "private/include/CWindowPainterContext.h"
#include "private/include/CGlobalResource.h"

CWindow::CWindow(CWindow* parent)
	: m_windwPainterContext(nullptr)
	, m_root(nullptr)
	, m_parent(parent)
{
	HWND hParent = m_parent ? m_parent->GetWindowId() : nullptr;
	if (!CreateNativeWindow("window", 100, 100, 1100, 700, hParent))
	{
		fprintf(stderr, "Window Created failed!\n");
		exit(0);
	}
}

CWindow::~CWindow()
{
}

LRESULT CWindow::NativeEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		__NcCreate();
		return 0;
	}
	if (uMsg == WM_PAINT)
	{
		CPainter* painter = m_windwPainterContext->BeginPaint();
		painter->Clear(CColor(255, 255, 255));
		__NcPaint(*painter);
		m_windwPainterContext->EndPaint();
		//ValidateRect(GetWindowId(), nullptr);
		return 0;
	}
	if (uMsg == WM_MOUSEMOVE)
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		OnMouseMove(x, y);
		return 0;
	}
	if (uMsg == WM_SIZE)
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		OnResize(x, y);
		return 0;
	}
	if (uMsg == WM_DESTROY)
	{
		OnDestroy();
		return 0;
	}
	return CWindowBase::NativeEvent(uMsg, wParam, lParam);
}

void CWindow::OnPaint(CPainter& painter)
{
	/*CRect rect(10, 10, 210, 210);
	painter.FillRect(rect, CColor(255, 23, 30));*/
}

void CWindow::OnMouseMove(int x, int y)
{

}

void CWindow::OnResize(int width, int height)
{
	m_windwPainterContext->Resize(width, height);
	m_root->Resize(width, height);
}

void CWindow::OnCreate()
{
}

void CWindow::OnDestroy()
{
	delete m_root;
	delete m_windwPainterContext;
}

void CWindow::__NcCreate()
{
	m_windwPainterContext = new CWindowPainterContext(this);
	CGlobalResource::Get()->Add(m_windwPainterContext);
	OnCreate();
	if (!m_root)
	{
		m_root = new CWidget();
		m_root->Move(0, 0);
		//m_root->SetBackgroundColor(CColor(0, 230, 29));
	}
}

void CWindow::__NcPaint(CPainter& painter)
{
	//先做控件绘制
	m_root->NcPaint(painter);
	OnPaint(painter);
}

