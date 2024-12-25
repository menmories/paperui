#ifndef PAPERUI_CRECT_H
#define PAPERUI_CRECT_H

#include "platformdef.h"


class PAPERUI_API CRect
{
public:
	CRect();
    CRect(int left, int top, int right, int bottom);
	~CRect();

	inline int Left()const
	{
		return m_left;
	}

	inline int Top()const
	{
		return m_top;
	}

	inline int Right()const
	{
		return m_right;
	}

	inline int Bottom()const
	{
		return m_bottom;
	}

	inline int Width()const
	{
		return m_right - m_left;
	}

	inline int Height()const
	{
		return m_bottom - m_top;
	}

	inline void SetLeft(int left)
	{
		m_left = left;
	}

	inline void SetTop(int top)
	{
		m_top = top;
	}

	inline void SetRight(int right)
	{
		m_right = right;
	}

	inline void SetBottom(int bottom)
	{
		m_bottom = bottom;
	}

	inline void SetWidth(int width)
	{
		m_right = m_left + width;
	}

	inline void SetHeight(int height)
	{
		m_bottom = m_top + height;
	}

	void SetRect(int left, int top, int right, int bottom);

	void SetRect(const CRect& rect);

	void OffsetRect(int dx, int dy);

	void Move(int x, int y);

	void Resize(int width, int height);
private:
	int m_left;
	int m_top;
	int m_right;
	int m_bottom;
};


#endif	//PAPERUI_CRECT_H

