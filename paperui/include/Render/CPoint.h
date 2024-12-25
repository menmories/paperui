#ifndef PAPERUI_POINT_H
#define PAPERUI_POINT_H

#include "platformdef.h"

class PAPERUI_API CPoint
{
public:
	CPoint();
    CPoint(int x, int y);
	~CPoint();

    inline void SetPoint(int x, int y)
    {
        m_x = x;
        m_y = y;
    }

	inline int X()const
	{
		return m_x;
	}

	inline int Y()const
	{
		return m_y;
	}

	inline void SetX(int x)
	{
		m_x = x;
	}

	inline void SetY(int y)
	{
		m_y = y;
	}

    inline void Offset(int dx, int dy)
    {
        m_x += dx;
        m_y += dy;
    }
private:
	int m_x;
	int m_y;
};



#endif	//PAPERUI_POINT_H

