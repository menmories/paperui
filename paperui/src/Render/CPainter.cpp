#include "Render/CPainter.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <assert.h>
#include <stdio.h>

#include "private/include/CD2D1Context.h"
#include "Window/CWindow.h"

CPainter::CPainter()
	: m_renderTarget(nullptr)
{
}

CPainter::~CPainter()
{
}

CPainter CPainter::CreateCompatiblePainter(int width, int height)
{
	CPainter painter;
	ID2D1BitmapRenderTarget* renderTarget = nullptr;
	HRESULT hr = m_renderTarget->CreateCompatibleRenderTarget(&renderTarget);
	if (SUCCEEDED(hr))
	{
		painter.m_renderTarget = renderTarget;
	}
	return painter;
}

CBitmap CPainter::GetBitmap()
{
	ID2D1Bitmap* bitmap = nullptr;
	HRESULT hr = m_renderTarget->GetBitmap(&bitmap);
	if (SUCCEEDED(hr))
	{

	}
	CBitmap bmp;
	bmp.m_data = bitmap;
	return bmp;
}

void CPainter::FillRect(const CRect& rect, const CColor& color)
{
	ID2D1SolidColorBrush* brush = nullptr;
	m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(color.RF(), color.GF(), color.BF(), color.AF()),
		&brush
	);
	m_renderTarget->FillRectangle(D2D1::RectF(
		rect.Left(), rect.Top(), rect.Right(), rect.Bottom()
	), brush);
	brush->Release();
}

void CPainter::DrawBitmap(const CBitmap& bitmap, const CRect& rect)
{
	m_renderTarget->DrawBitmap(bitmap.Data(), D2D1::RectF(rect.Left(), rect.Top(), rect.Right(), rect.Bottom()));
}

void CPainter::DrawRect(const CRect& rect, const CBrush& brush)
{
	ID2D1SolidColorBrush* solidBrush = brush.GetBrush();
	m_renderTarget->DrawRectangle(D2D1::RectF(rect.Left(), rect.Right(), rect.Top(), rect.Bottom()),
		solidBrush);
}

void CPainter::DrawText(const CRect& rect, const CString& text, const CFont& font, const CBrush& brush)
{
	int len = text.Length();
	m_renderTarget->DrawTextA(text.Data(), len,
		font.m_pTextFormat, D2D1::RectF(rect.Left(),
			rect.Top(),
			rect.Right(),
			rect.Bottom()
	), brush.GetBrush(), D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL);
}

bool CPainter::InitEnvironment()
{
	return CD2D1Context::Get()->Status();
}

void CPainter::DestroyEnvironment()
{

}

