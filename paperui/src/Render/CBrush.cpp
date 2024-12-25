#include "Render/CBrush.h"
#include "private/include/CGlobalResource.h"
CBrush::CBrush()
	: m_type(EBrushType::Brush_Solid)
	, m_brush(nullptr)
{
	ID2D1HwndRenderTarget* renderTarget = CGlobalResource::Get()->Pop()->RenderTarget();
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &m_brush);
}

CBrush::CBrush(const CColor& color)
	: m_color(color)
	, m_type(EBrushType::Brush_Solid)
	, m_brush(nullptr)
{
	ID2D1HwndRenderTarget* renderTarget = CGlobalResource::Get()->Pop()->RenderTarget();
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(color.RF(), color.GF(), color.BF(), color.AF()), 
		&m_brush);
}

CBrush::~CBrush()
{
	m_brush->Release();
}

