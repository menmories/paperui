#include "Render/CFont.h"
#include "private/include/CD2D1Context.h"
#include <stdio.h>
#include "Utils/CString.h"
CFont::CFont()
{
	Init("微软雅黑", 10);
}

CFont::CFont(const CString& family, float size)
{
	Init(family, size);
}

CFont::~CFont()
{
	if (m_pTextFormat) {
		m_pTextFormat->Release();
		m_pTextFormat = nullptr;
	}
}

CString CFont::GetFontFamily() const
{
	return m_fontFamily;
}

float CFont::GetFontSize() const
{
	return m_pTextFormat->GetFontSize();
}

void CFont::Copy(const CFont& src)
{
	if (m_pTextFormat) {
		m_pTextFormat->Release();
		m_pTextFormat = nullptr;
	}
	Init(src.GetFontFamily(), src.GetFontSize());
}

void CFont::Init(const CString& family, float size)
{
	CD2D1Context* context = CD2D1Context::Get();
	IDWriteFactory* dwriteFactory = context->DWriteFactory();
	m_fontFamily = family;
	HRESULT hr = dwriteFactory->CreateTextFormat(
		m_fontFamily.Data(),    // 字体家族，这里是Arial字体，可替换为其他字体名称
		nullptr,     // 字体集合，这里使用系统默认字体集合
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,       // 字体大小，单位是像素
		L"zh-cn",   // 区域设置，这里是美国英语，可根据需要修改
		&m_pTextFormat
	);
	if (FAILED(hr))
	{
		fprintf(stderr, "CreateTextFormat failed");
		return;
	}
}
