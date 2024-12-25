#include "Render/CRect.h"

CRect::CRect()
    : m_left(0)
    , m_top(0)
    , m_right(0)
    , m_bottom(0)
{
}

CRect::CRect(int left, int top, int right, int bottom)
    : m_left(left)
    , m_top(top)
    , m_right(right)
    , m_bottom(bottom)
{
}

CRect::~CRect()
{
}

void CRect::SetRect(int left, int top, int right, int bottom)
{
	m_left = left;
	m_top = top;
	m_right = right;
	m_bottom = bottom;
}

void CRect::SetRect(const CRect& rect)
{
	m_left = rect.m_left;
	m_top = rect.m_top;
	m_right = rect.m_right;
	m_bottom = rect.m_bottom;
}

void CRect::OffsetRect(int dx, int dy)
{
	m_left += dx;
	m_top += dy;
	m_right += dx;
	m_bottom += dy;
}

void CRect::Move(int x, int y)
{
	int width = m_right - m_left;
	int height = m_bottom - m_top;
	m_left = x;
	m_top = y;
	m_right = x + width;
	m_bottom = y + height;
}

void CRect::Resize(int width, int height)
{
	m_right = m_left + width;
	m_bottom = m_top + height;
}

