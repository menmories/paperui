#ifndef PAPERUI_CWIDGET_H
#define PAPERUI_CWIDGET_H

#include "platformdef.h"
#include "Render/CRect.h"
#include "Render/CPoint.h"
#include "Render/CBrush.h"
#include "Render/CPainter.h"


class PAPERUI_API CWidget
{
public:
    CWidget(CWidget* parent = nullptr);
    virtual ~CWidget();

    inline void SetBackgroundColor(const CColor& color)
    {
        m_backgroundColor = color;
    }

    inline void SetRect(const CRect& rect)
    {
        m_rect = rect;
    }
    inline void SetPos(const CPoint& pos)
    {
        m_rect.Move(pos.X(), pos.Y());
    }
    inline CRect GetRect() const
    {
        return m_rect;
    }
    virtual void Move(int x, int y)
    {
        m_rect.Move(x, y);
    }

    virtual void Resize(int width, int height);

    virtual void OnPaint(CPainter& painter);

    virtual void NcPaint(CPainter& painter);
protected:
    CRect m_rect;
    CColor m_backgroundColor;
    CWidget* m_parent;
};


#endif  // PAPERUI_CWIDGET_H

