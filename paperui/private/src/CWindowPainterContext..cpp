#include "private/include/CWindowPainterContext.h"
#include <assert.h>
#include <stdio.h>
CWindowPainterContext::CWindowPainterContext(CWindow* window)
	: m_window(window)
{
	AttachWindow(window);
	m_painter = new CPainter();
}

CWindowPainterContext::~CWindowPainterContext()
{
	delete m_painter;
}

void CWindowPainterContext::AttachWindow(CWindow* window)
{
	assert(window);
	HWND hWnd = window->GetWindowId();
	assert(hWnd);
	m_window = window;
	ID2D1HwndRenderTarget* renderTarget = nullptr;
	CRect rcClient = window->GetRect();
	HRESULT hr = CD2D1Context::Get()->Factory()->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rcClient.Width(), rcClient.Height())),
		&renderTarget
	);
	if (FAILED(hr))
	{
		fprintf(stderr, "AttachWindow error,CreateHwndRenderTarget failed.\n");
		return;
	}
	//renderTarget->SetTextRenderingParams(RenderParams);
	//renderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_DEFAULT);
	//renderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	m_renderTarget = renderTarget;
}

CPainter* CWindowPainterContext::BeginPaint()
{
	m_renderTarget->BeginDraw();
	ID2D1BitmapRenderTarget* pCompatibleRenderTarget = nullptr;
	HRESULT hr = m_renderTarget->CreateCompatibleRenderTarget(&pCompatibleRenderTarget);
	if (FAILED(hr))
	{
		fprintf(stderr, "CreateCompatibleRenderTarget failed.\n");
		return nullptr;
	}
	m_renderTarget->Clear(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
	m_painter->m_renderTarget = pCompatibleRenderTarget;
	m_painter->BeginPaint();
	return m_painter;
}

void CWindowPainterContext::EndPaint()
{
    ID2D1Bitmap* bitmap = nullptr;
    m_painter->m_renderTarget->GetBitmap(&bitmap);
    if (bitmap)
    {
        m_renderTarget->DrawBitmap(bitmap);
    }
    else
    {
        fprintf(stderr, "bitmap is null.\n");
    }
	m_painter->EndPaint();
	m_renderTarget->EndDraw();
	bitmap->Release();
	m_painter->m_renderTarget->Release();
}

